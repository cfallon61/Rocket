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
		Log();
		bool init();
		bool log_to_file(GY521 mpu);
    bool log_event(String event);
	private:
		String filename;
		void check_files();
};


#endif
