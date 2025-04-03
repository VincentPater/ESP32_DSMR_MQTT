
struct TelegramDecodedObject
{
  String name;
  char code[16];
  char startChar = '(';
  char endChar = ')';
  char value[VALUE_CHAR_ARRAY_LENGTH];
  bool sendData = false;
};


struct TelegramDecodedObject telegramObjects[NUMBER_OF_READOUTS];

void setupDataReadout() {
  // 1-0:1.8.1(000992.992*kWh)
  // 1-0:1.8.1 = Elektra verbruik laag tarief (DSMR v5.0)
  telegramObjects[0].name = "consumption_tarif_1";
  strcpy(telegramObjects[0].code, "1-0:1.8.1");
  telegramObjects[0].endChar = '*';

  // 1-0:1.8.2(000560.157*kWh)
  // 1-0:1.8.2 = Elektra verbruik hoog tarief (DSMR v5.0)
  telegramObjects[1].name = "consumption_tarif_2";
  strcpy(telegramObjects[1].code, "1-0:1.8.2");
  telegramObjects[1].endChar = '*';

  telegramObjects[2].name = "production_tarif_1";
  strcpy(telegramObjects[2].code, "1-0:2.8.1");
  telegramObjects[2].endChar = '*';

  // 1-0:1.8.2(000560.157*kWh)
  // 1-0:1.8.2 = Elektra verbruik hoog tarief (DSMR v5.0)
  telegramObjects[3].name = "production_tarif_2";
  strcpy(telegramObjects[3].code, "1-0:2.8.2");
  telegramObjects[3].endChar = '*';

  // 1-0:1.7.0(00.424*kW) Actueel verbruik
  // 1-0:1.7.x = Electricity consumption actual usage (DSMR v5.0)
  telegramObjects[4].name = "power_used";
  strcpy(telegramObjects[4].code, "1-0:1.7.0");
  telegramObjects[4].endChar = '*';

  // 1-0:2.7.0(00.000*kW) Actuele teruglevering (-P) in 1 Watt resolution
  telegramObjects[5].name = "power_produced";
  strcpy(telegramObjects[5].code, "1-0:2.7.0");
  telegramObjects[5].endChar = '*';

  // 1-0:21.7.0(00.378*kW)
  // 1-0:21.7.0 = Instantaan vermogen Elektriciteit levering L1
  // telegramObjects[4].name = "instant_power_usage_l1";
  // strcpy(telegramObjects[4].code, "1-0:21.7.0");
  // telegramObjects[4].endChar = '*';

  // 1-0:41.7.0(00.378*kW)
  // 1-0:41.7.0 = Instantaan vermogen Elektriciteit levering L2
  // telegramObjects[5].name = "instant_power_usage_l2";
  // strcpy(telegramObjects[5].code, "1-0:41.7.0");
  // telegramObjects[5].endChar = '*';

  // 1-0:61.7.0(00.378*kW)
  // 1-0:61.7.0 = Instantaan vermogen Elektriciteit levering L3
  // telegramObjects[6].name = "instant_power_usage_l3";
  // strcpy(telegramObjects[6].code, "1-0:61.7.0");
  // telegramObjects[6].endChar = '*';

  // 1-0:22.7.0(00.378*kW)
  // 1-0:22.7.0 = Instantaan vermogen Elektriciteit teruglevering L1
  // telegramObjects[4].name = "instant_power_return_l1";
  // strcpy(telegramObjects[4].code, "1-0:22.7.0");
  // telegramObjects[4].endChar = '*';

  // 1-0:42.7.0(00.378*kW)
  // 1-0:42.7.0 = Instantaan vermogen Elektriciteit teruglevering L2
  // telegramObjects[5].name = "instant_power_return_l2";
  // strcpy(telegramObjects[5].code, "1-0:42.7.0");
  // telegramObjects[5].endChar = '*';

  // 1-0:62.7.0(00.378*kW)
  // 1-0:62.7.0 = Instantaan vermogen Elektriciteit teruglevering L3
  // telegramObjects[6].name = "instant_power_return_l3";
  // strcpy(telegramObjects[6].code, "1-0:62.7.0");
  // telegramObjects[6].endChar = '*';

  // 1-0:31.7.0(002*A)
  // 1-0:31.7.0 = Instantane stroom Elektriciteit L1
  // telegramObjects[7].name = "instant_power_current_l1";
  // strcpy(telegramObjects[7].code, "1-0:31.7.0");
  // telegramObjects[7].endChar = '*';

  // 1-0:51.7.0(002*A)
  // 1-0:51.7.0 = Instantane stroom Elektriciteit L2
  // telegramObjects[8].name = "instant_power_current_l2";
  // strcpy(telegramObjects[8].code, "1-0:51.7.0");
  // telegramObjects[8].endChar = '*';

  // 1-0:71.7.0(002*A)
  // 1-0:71.7.0 = Instantane stroom Elektriciteit L3
  // telegramObjects[9].name = "instant_power_current_l3";
  // strcpy(telegramObjects[9].code, "1-0:71.7.0");
  // telegramObjects[9].endChar = '*';

  // // 1-0:32.7.0(232.0*V)
  // // 1-0:32.7.0 = Voltage L1
  // telegramObjects[10].name = "instant_voltage_l1";
  // strcpy(telegramObjects[10].code, "1-0:32.7.0");
  // telegramObjects[10].endChar = '*';

  // // 1-0:52.7.0(232.0*V)
  // // 1-0:52.7.0 = Voltage L2
  // telegramObjects[11].name = "instant_voltage_l2";
  // strcpy(telegramObjects[11].code, "1-0:52.7.0");
  // telegramObjects[11].endChar = '*';

  // // 1-0:72.7.0(232.0*V)
  // // 1-0:72.7.0 = Voltage L3
  // telegramObjects[12].name = "instant_voltage_l3";
  // strcpy(telegramObjects[12].code, "1-0:72.7.0");
  // telegramObjects[12].endChar = '*';

  // // 0-0:96.14.0(0001)
  // // 0-0:96.14.0 = Actual Tarif
  // telegramObjects[13].name = "actual_tarif_group";
  // strcpy(telegramObjects[13].code, "0-0:96.14.0");

  // // 0-1:24.2.3(150531200000S)(00811.923*m3)
  // // 0-1:24.2.3 = Gas (DSMR v5.0) on Belgian meters
  // telegramObjects[18].name = "gas_meter_m3";
  // strcpy(telegramObjects[18].code, "0-1:24.2.3");
  // telegramObjects[12].endChar = '*';

  // // 0-0:96.14.0(0001)
  // // 0-0:96.14.0 = Actual Tarif
  telegramObjects[6].name = "active_tarif";
  strcpy(telegramObjects[6].code, "0-0:96.14.0");

  // 0-2:24.2.1(241229182503W)(00949.162*m3)
  // 0-2:24.2.1 = Gas (DSMR v5.0) on Dutch meters
  telegramObjects[7].name = "gas_meter_m3";
  strcpy(telegramObjects[7].code, "0-1:24.2.1");
  telegramObjects[7].endChar = '*';

#ifdef DEBUG
  Serial.println("MQTT Topics initialized:");
  for (int i = 0; i < NUMBER_OF_READOUTS; i++) {
    Serial.println(String(MQTT_ROOT_TOPIC) + "/" + telegramObjects[i].name);
  }
#endif
}