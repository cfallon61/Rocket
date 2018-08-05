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

	check_files();

	return true;
}

void Log::check_files()		//if there are already log files
{
	if (SD.exists("datalog.txt"))
	{
    SD.remove("datalog.txt");
	}
 filename = "datalog.txt";
}

bool Log::log_to_file(GY521 mpu)
{
	File log = SD.open(filename, FILE_WRITE);

	if (log)
	{
			log.print(mpu.get_x_accel()); log.print('\t');
			log.print(mpu.get_y_accel()); log.print('\t');
			log.print(mpu.get_z_accel()); log.print('\t');

			log.print(mpu.get_x_gyro()); log.print('\t');
			log.print(mpu.get_y_gyro()); log.print('\t');
			log.print(mpu.get_z_gyro()); log.print('\t');

			log.println(mpu.get_temp());

			log.close();
			return true;
	}
	else
	{
		return false;
	}
}

bool Log::log_event(String event)
{
  File log = SD.open("datalog.txt", FILE_WRITE);
  if (log)
  {
    log.println(event);
    log.close();
    return true;
  }
  return false;
}
