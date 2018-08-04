#include "Arduino.h"
#include <SD.h>
#include <SPI.h>
#include "GY521.h"


#ifndef log_h
#define log_h

#define CHIP_SELECT 10

enum data_type
{
	accel_data,
	gyro_data,
	temp_data,
};

class Log
{
	public:
		Log();
		bool init();
		bool log_to_file(GY521 mpu);
};


#endif
