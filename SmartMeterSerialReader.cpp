#include "SmartMeterSerialReader.h"
    

// Initialisation method for serial reader object
SmartMeterSerialReader::SmartMeterSerialReader() {
	P1Serial.begin(BAUD_RATE, SERIAL_8N1, RXD, TXD, true);
  bool startFound = false;
  bool endFound = false;
  bool crcFound = false;
  bool telegramComplete = false;
  int curLenTelegram = 0;
  int curLenCRC = 0;
}

void SmartMeterSerialReader::reset_telegram(){
  startFound = false;
  endFound = false;
  crcFound = false;
  telegramComplete = false;
  curLenTelegram = 0;
  curLenCRC = 0;
  memset(buffer, 0, P1_MAXLINELENGTH);
  memset(buffer, 0, CRC_LENGTH);
}

int SmartMeterSerialReader::get_curLenTelegram(){
  return curLenTelegram;
}

char* SmartMeterSerialReader::get_telegram(){
  return buffer;
}

char* SmartMeterSerialReader::get_CRC(){
  return CRC;
}

void SmartMeterSerialReader::read_serial(){

  char character;
  // Check if telegram is already read, should never be the case though -> safety precaution
  if(startFound && endFound && crcFound){
    telegramComplete = true;
    return;
  }

  // Read a byte into the character viariable, if serial is available
  if(P1Serial.available()){
    character = P1Serial.read();
  }else{
    return;
  }


  // Checks if character is '/' which is the start of a new telegram. It's possible that we start ESP32 halfway telegram
  if (!startFound) {
    if (character == '/'){
      startFound = true;
    }
  }

  // If the start is found (but not yet writen to buffer) place it into the buffer and increment counter
  if (startFound && !endFound){
      buffer[curLenTelegram] = character;
      curLenTelegram++;
  }

  // We can also now check if the last writen char was the end of the main telegram, if so, set endFound to TRUE
  if (startFound && character == '!'){
    endFound = true;
    return;
  }

  // If the end is found, all that remains is the CRC, we want to store this in the CRC array
  if (endFound){
      if (character == '\r'){
          telegramComplete = true;
          crcFound = true;
      }else{
        // Serial.print(character);
        CRC[curLenCRC] = character;
        curLenCRC++;
      }
  }

  return;
}

