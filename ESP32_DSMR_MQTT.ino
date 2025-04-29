#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <HardwareSerial.h>

#include "settings.h"
#include "crc16.h"
#include "SmartMeterSerialReader.h"
#include "TelegramDecodedObject.h"




// Global Variables
uint64_t LAST_RECONNECT_ATTEMPT = 0;
uint64_t LAST_FULL_UPDATE_SENT = 0;

char WIFI_SSID[32] = WIFI_SSID_STRING;
char WIFI_PASS[32] = WIFI_PASSWORD;

char MQTT_HOST[64] = MQTT_SERVER_IP;
char MQTT_PORT[6] = MQTT_PORT_NUM;
char MQTT_USER[32] = MQTT_USER_NAME;
char MQTT_PASS[32] = MQTT_PASSWORD;

uint64_t currentTime = 0;


/***********************************
            Create various Objects
 ***********************************/

WiFiClient espClient;
PubSubClient mqttClient(espClient);

SmartMeterSerialReader p1Telegram;

CRC16 crc;

/***********************************
            Main Setup
 ***********************************/
void setup() {
  // Initialize pins
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.begin(BAUD_RATE);

#ifdef DEBUG
  Serial.println("Booting - DEBUG mode on");
  blinkLed(2, 500);
  delay(500);
  blinkLed(2, 2000);
  // Blinking 2 times fast and two times slower to indicate DEBUG mode
#endif
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
#ifdef DEBUG
    Serial.println("Connection Failed! Rebooting...");
#endif                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
    delay(5000);
    ESP.restart();
  }

  delay(3000);

  setupDataReadout();

  setupOTA();

  mqttClient.setServer(MQTT_HOST, atoi(MQTT_PORT));
  blinkLed(5, 500);  // Blink 5 times to indicate end of setup

#ifdef DEBUG
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
#endif
}

/***********************************
            Main Loop
 ***********************************/
void loop(){

  // Time keeping
  currentTime = esp_timer_get_time();


  // Check WiFi Status
  if (WiFi.status() != WL_CONNECTED) {
    blinkLed(20, 50);  // Blink fast to indicate failed WiFi connection
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      #ifdef DEBUG
        Serial.println("Connection Failed! Rebooting...");
      #endif
      delay(5000);
      ESP.restart();
    }
  }



  // Check if there are new OTA requests
  ArduinoOTA.handle();



  // Check if new MQTT messages are available (does not send data)
    if (!mqttClient.connected()) {
    if (currentTime - LAST_RECONNECT_ATTEMPT > 5000000) {
      LAST_RECONNECT_ATTEMPT = currentTime;

      if (!mqttReconnect()) {
        #ifdef DEBUG
            Serial.println("Connection to MQTT Failed! Rebooting...");
        #endif
        delay(5000);
        ESP.restart();
      } else {
        LAST_RECONNECT_ATTEMPT = 0;
      }
    }
  } else {
    mqttClient.loop();
  }
  




  // Read Data from P1 into buffer
  p1Telegram.read_serial();



  // Check if CRC is correct -> Valid Telegram
  if (p1Telegram.telegramComplete){
  #ifdef DEBUG
  // Prints the entire telegram
    Serial.print(p1Telegram.get_telegram());
    Serial.println(p1Telegram.get_CRC());
    Serial.println();
  #endif

    // Calculate CRC from telegram
    crc.processBuffer(p1Telegram.get_telegram(), p1Telegram.get_curLenTelegram());

  #ifdef DEBUG
    Serial.println("Calculated CRC:");
    Serial.println(crc.getCrc(),HEX);
    
    Serial.println("Actual CRC:");
    Serial.println(p1Telegram.get_CRC());
  #endif

    // Turn CRS string into unsigned long
    int messageCRC_int = strtoul(p1Telegram.get_CRC(),NULL,16);

    // If message CRC is equal to calculated CRC, parse the telegram (update telegram objects)
    if (messageCRC_int == crc.getCrc()){
      #ifdef DEBUG
        Serial.println("Parsing Telegram");
      #endif
      parse_telegram(p1Telegram.get_telegram());  //This updates the telegram objects every time a new p1 message is received
    }

    #ifdef DEBUG
      Serial.println("Resetting Telegram and CRC");
    #endif
    p1Telegram.reset_telegram();
    crc.resetCrc();


    // Force update with every message for these topics
    // If there are certain topics that you want to send every time (this case Power [kW] consumed and produced), add them here
    telegramObjects[4].sendData = true;
    telegramObjects[5].sendData = true;
  }



  // At desired interval, publish data to MQTT broker
  if (currentTime - LAST_FULL_UPDATE_SENT >= UPDATE_FULL_INTERVAL){
    for (int i = 0; i < NUMBER_OF_READOUTS; i++){
      telegramObjects[i].sendData = true;
      LAST_FULL_UPDATE_SENT = esp_timer_get_time(); // Time in microseconds!
    }
  }

  // Sends all data to broker (if sendData == true)
  sendDataToBroker();

}