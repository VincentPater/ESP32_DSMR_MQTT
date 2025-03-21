# ESP32_DSMR_MQTT
Software for the ESP32 (I use an official ESP32C6 DevKit.) that decodes and sends P1 smart meter (DSMR) data to a MQTT broker, with the possibility for Over The Air (OTA) firmware updates.

## About this fork
This fork was based on the work done by [Bartwo](https://github.com/bartwo/esp32_p1meter/tree/master).
My ESP32 C6 kept crashing/having panics and I tracked it down to the decodeTelegram function, specifically the CRC calculation.
Try as I might, I could not get it to work.

In this fork, I've reused some parts of the original code and I have adjusted or rewritten others.
- For the serial communication, I've written a class to handle incomming Serial data. This simplifies the main script.
- For CRC checking, I've made a combination of the [CRC Library by jpralves](https://github.com/jpralves/crc16/tree/master) and a [Python script on Tweakers by Kristofferson](https://gathering.tweakers.net/forum/list_messages/2250716)

At this point in time, I have yet to:
- Read the desired data from the telegram into the MQTT topics
- Implement various timing for sending updates to the MQTT broker (1 sec. for power consumption/production and 1 min. for the meter values).

## Setup
This setup requires:
- An ESP32 C6 DevkitC-1 (though other board might work as well)
- Small breadboard
- A 1k ohm resistor (10k does not work for me!)
- A 6-pin RJ12 connector and cable (you can do a 4-pin Telephone cable and power it seperately, but the power from the p1 port is not included in your energy bill -> free electricity)
  
  
Setting up your Arduino IDE:
- Ensure you have selected the right board (you might need to install your esp32board in the Arduino IDE).
- 160 MHz CPU frequency mode
- Using the Tools->Manage Libraries... install `PubSubClient` by Nick O'Leary.
- In the file `Settings.h` change all values accordingly
- Set "USB CDC On Boot" to Enabled to see Serial messages
- Write to your device via USB the first time, you can do it OTA all times thereafter.

### Circuit diagram
_Note: I have only tested this on the `landis+Gyr e350`._

Contrary to other ESPs the C6 does not have a UART2/Serial2. Uart1 can however be assigned most GPIO pins. 
I used GPIO 6 for RXD and GPIO 7 (in software only) for TXD.

Connect the ESP32 to an RJ12 cable/connector following the diagram.
  
| P1 pin   | ESP32 Pin |
| ----     | ---- |
| 1 - 5v out | 5v |
| 2 - RTS  | 3.3v |
| 3 - GND  | GND  |
| 4 -      |      |
| 5 - RXD (data) | gpio6|
| 6 - GND  | GND  |



### Data Sent

All metrics are send to their own MQTT topic.
The software generates all the topic through the Serial monitor when starting up
The ones I use are:

```
sensors/power/p1meter/consumption_low_tarif
sensors/power/p1meter/consumption_high_tarif
sensors/power/p1meter/production_tarif_1
sensors/power/p1meter/production_tarif_2
sensors/power/p1meter/power_used
sensors/power/p1meter/power_produced
sensors/power/p1meter/active_tarif
sensors/power/p1meter/gas_meter_m3
```

But all the metrics you need are easily added using the `setupDataReadout()` method. With the DEBUG mode it is easy to see all the topics you add/create by the serial monitor. To see what your telegram is outputting in the Netherlands see: https://www.netbeheernederland.nl/_upload/Files/Slimme_meter_15_a727fce1f1.pdf for the dutch codes pag. 19 -23


### Home Assistant Configuration

Use this [example](https://raw.githubusercontent.com/daniel-jong/esp8266_p1meter/master/assets/p1_sensors.yaml) for home assistant's `sensor.yaml`


## Thanks to previous people who worked on this project
- https://github.com/bartwo/esp32_p1meter/tree/master
- https://github.com/fliphess/esp8266_p1meter
- https://github.com/jantenhove/P1-Meter-ESP8266
- https://github.com/neographikal/P1-Meter-ESP8266-MQTT
- http://gejanssen.com/howto/Slimme-meter-uitlezen/
- https://github.com/rroethof/p1reader/
- http://romix.macuser.nl/software.html
- http://blog.regout.info/category/slimmeter/
- http://domoticx.com/p1-poort-slimme-meter-hardware/

In addition, I'd like thank and refer to the following projects which served as a source of information:
- [https://github.com/daniel-jong/esp8266_p1meter](https://github.com/daniel-jong/esp8266_p1meter)
- [https://github.com/WhoSayIn/esp8266_dsmr2mqtt](https://github.com/WhoSayIn/esp8266_dsmr2mqtt)
- [https://github.com/jhockx/esp8266_p1meter](https://github.com/jhockx/esp8266_p1meter)

Other sources:
- [DSMR 5.0 documentation](https://www.netbeheernederland.nl/_upload/Files/Slimme_meter_15_a727fce1f1.pdf)
