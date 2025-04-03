
#include <string.h>

void parse_telegram(char* telegram){

  // make variables
  char * startpointer;
  char * endpointer;
  int valueLength;
  char * valueStart;

  for (int i = 0; i < NUMBER_OF_READOUTS; i++){

    // Find start of substring
    startpointer = strstr(telegram, telegramObjects[i].code);

    // Should never be the case, but if not found it returns NULL, better exit than crash
    // if (startpointer == NULL){
    //   Serial.print("Not Found: ");
    //   Serial.println(telegramObjects[i].code);
    //   return;
    // }

    if (telegramObjects[i].name == "gas_meter_m3"){
      // Exception Case for Gas because it includes datetime
      endpointer = strchr(startpointer+ strlen(telegramObjects[i].code) + 16,  telegramObjects[i].endChar);     // plus 16 for (241229182503W)(
      valueStart = startpointer + strlen(telegramObjects[i].code) + 16;                                         // plus 16 for (241229182503W)(

    } else{
      // Default for the rest
      endpointer = strchr(startpointer,  telegramObjects[i].endChar);
      valueStart = startpointer + strlen(telegramObjects[i].code) + 1;    // plus 1 for "("                                  
    }

    valueLength = endpointer - valueStart;

    memset(telegramObjects[i].value, 0, VALUE_CHAR_ARRAY_LENGTH);     // Set entire char array to '/0' to flush all current data
    memcpy(telegramObjects[i].value, valueStart, valueLength);

    #ifdef DEBUG
      Serial.print(telegramObjects[i].name);
      Serial.print(": ");
      Serial.println(telegramObjects[i].value);
    #endif
  }
}
