#include "log.h"

Log::Log(){}

bool Log::init()
{
	pinMode(10, OUTPUT);
	Serial.print("Initializing SD card...");
	if (!SD.begin(CHIP_SELECT))
	{
		Serial.println("Card error, card not detected or card is damaged.");
		return false;
	}
	Serial.println("Card initialized");
  if (SD.exists("datalog.txt"))
  {
    SD.remove("datalog.txt");
  }
	return true;
}

bool Log::log_to_file(GY521 mpu)
{
	File log = SD.open("datalog.txt", FILE_WRITE);
	
	if (log)
	{
			log.print(mpu.get_x_accel()); log.print(" | ");
			log.print(mpu.get_y_accel()); log.print(" | ");
			log.print(mpu.get_z_accel()); log.println();
			
			log.print(mpu.get_x_gyro()); log.print(" | ");
			log.print(mpu.get_y_gyro()); log.print(" | ");
			log.print(mpu.get_z_gyro()); log.println();
			
			log.println(mpu.get_temp());
			
			log.close();
			return true;
	}
	else
	{
		return false;
	}
}
