#ifndef SMARTMETERSERIALREADER_H
#define SMARTMETERSERIALREADER_H

#include <Arduino.h>
#include "settings.h"


class SmartMeterSerialReader {
	public:
	   SmartMeterSerialReader();
     void read_serial();
     void reset_telegram();
     int get_curLenTelegram();
     char* get_telegram();
     char* get_CRC();
     bool telegramComplete;

	private:
	  bool startFound = false;
    bool endFound = false;
    bool crcFound = false;
    int curLenTelegram = 0;
    int curLenCRC = 0;

    char buffer[P1_MAXLINELENGTH];
    char CRC[CRC_LENGTH];
};	

#endif