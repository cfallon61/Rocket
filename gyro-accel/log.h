#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include "GY521.h"


#ifndef log_h
#define log_h

#define CHIP_SELECT 10


class Log
{
	public:
		Log() {}		//empty constructor
		bool init();		//initialize the logger and try to detect if the SD card is present
		bool log_to_file(GY521 mpu);		//log all data values to the SD card
    bool log_event(String event);		//logs an event string to the data file
	private:
		String filename;			//filename to be written to
		void check_files();		//checks to see if there are already files on the device
};


#endif
