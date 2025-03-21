#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <HardwareSerial.h>

#include "settings.h"
#include "crc16.h"
#include "SmartMeterSerialReader.h"




// Global Variables
long LAST_RECONNECT_ATTEMPT = 0;
long LAST_UPDATE_SENT = 0;
long LAST_FULL_UPDATE_SENT = 0;

char WIFI_SSID[32] = WIFI_SSID_STRING;
char WIFI_PASS[32] = WIFI_PASSWORD;

char MQTT_HOST[64] = MQTT_SERVER_IP;
char MQTT_PORT[6] = MQTT_PORT_NUM;
char MQTT_USER[32] = MQTT_USER_NAME;
char MQTT_PASS[32] = MQTT_PASSWORD;


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
  long now = millis();


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
    if (now - LAST_RECONNECT_ATTEMPT > 5000) {
      LAST_RECONNECT_ATTEMPT = now;

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
    Serial.print(p1Telegram.get_telegram());
    Serial.println();
  #endif

    crc.processBuffer(p1Telegram.get_telegram(), p1Telegram.get_curLenTelegram()); //strlen(buffer)
    Serial.println("Calculated CRC:");
    Serial.println(crc.getCrc(),HEX);
    
    Serial.println("Actual CRC:");
    Serial.println(p1Telegram.get_CRC());

    int endCRC_int = strtoul(p1Telegram.get_CRC(),NULL,16);
    if (endCRC_int == crc.getCrc()){
      Serial.println("Valid CRC");
    }else{
      Serial.println("Invalid CRC");
    }

    // Temporary clear
    Serial.println("Resetting Telegram");
    p1Telegram.reset_telegram();

    Serial.println("Resetting CRC");
    crc.resetCrc();
  }

  // Extract wanted data from Telegram into struct


  // Flush telegram
  if (false){
    p1Telegram.reset_telegram();
  }


  // At desired interval, publish data to MQTT broker






}





/**
   Over the Air update setup
*/
void setupOTA() {
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else  // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)
        Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR)
        Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR)
        Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}
