void sendMQTTMessage(const char *topic, char *payload) {
  bool result = mqttClient.publish(topic, payload, false);
}

bool mqttReconnect() {
  int MQTT_RECONNECT_RETRIES = 0;

  while (!mqttClient.connected() && MQTT_RECONNECT_RETRIES < MQTT_MAX_RECONNECT_TRIES) {
    MQTT_RECONNECT_RETRIES++;

    #ifdef STATUSLED
    rgbLedWrite(RGB_BUILTIN, RGB_BRIGHTNESS, 0, 0);  // Red
    #endif
    #ifdef DEBUG
    Serial.println("Connection Lost to MQTT server");
    #endif

    if (mqttClient.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      // char *message = new char[16 + strlen(HOSTNAME) + 1];
      // strcpy(message, "p1 meter alive: ");
      // strcat(message, HOSTNAME);
      // mqttClient.publish("hass/status", message);
    } else {
      delay(1000);
    }
  }

  if (MQTT_RECONNECT_RETRIES >= MQTT_MAX_RECONNECT_TRIES) {
    return false;
  }

  // Connection is re-established
  #ifdef DEBUG
  Serial.println("Reconnected to MQTT server");
  #endif
  #ifdef STATUSLED
  rgbLedWrite(RGB_BUILTIN, 0, RGB_BRIGHTNESS, 0);  // Green
  #endif
  return true;
}



// void sendMetric(String name, long metric)
void sendMetric(String name, char *metric) {
  char output[20];
  memset(output, 0, 20); //need to reset to 0 or somehow eol '/0' is not present.
  memcpy(output, metric, strlen(metric));

  String topic = String(MQTT_ROOT_TOPIC) + "/" + name;
  sendMQTTMessage(topic.c_str(), output);
  //}
}



void sendDataToBroker() {
  for (int i = 0; i < NUMBER_OF_READOUTS; i++) {
    if (telegramObjects[i].sendData) {
      #ifdef DEBUG
        Serial.println((String) "Sending: " + telegramObjects[i].name + " value: " + telegramObjects[i].value);
      #endif
      sendMetric(telegramObjects[i].name, telegramObjects[i].value);
      telegramObjects[i].sendData = false;
    }
  }
}
