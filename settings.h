// For logs to Serial Monitor
#define DEBUG

// WiFi Settings
#define WIFI_SSID_STRING "your ssid"
#define WIFI_PASSWORD "your pass"

// OTA Settings
#define HOSTNAME "p1meter"
#define OTA_PASSWORD "admin"


// Serial configuration
#define BAUD_RATE 115200
#define RXD 6
#define TXD 7
#define P1_MAXLINELENGTH 4000
#define P1Serial Serial1
#define CRC_LENGTH 10


// MQTT setup
#define MQTT_MAX_RECONNECT_TRIES 100
#define MQTT_ROOT_TOPIC "home/p1meter"
#define NUMBER_OF_READOUTS 8
#define VALUE_CHAR_ARRAY_LENGTH 20

#define MQTT_SERVER_IP "your mqtt server ip"
#define MQTT_PORT_NUM "1883"
#define MQTT_USER_NAME "mqtt_user"
#define MQTT_PASSWORD "mqttpass"


// Time is in microseconds!
#define UPDATE_FULL_INTERVAL 60000000  // 1 minute
