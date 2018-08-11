#include "log.h"

bool Log::init()		//initiliaze the logger
{
	pinMode(CHIP_SELECT, OUTPUT);		//initialize the CHIP_SELECT pin
	Serial.print("Initializing SD card...");
	if (!SD.begin(CHIP_SELECT))		//if CHIP_SELECT pin does not return true, print an error and return false
	{
		Serial.println("Card error, card not detected or card is damaged.");
		return false;
	}
	Serial.println("Card initialized");		//return true and print that the card was initialized to serial bus

	check_files();		//checks for already present files

	return true;
}

bool Log::init(bool verbose)
{
	if (verbose)
	{}
}

void Log::check_files()		//if there are already log files
{
	if (SD.exists("datalog.txt"))		//if the dafault file name exists, then start making a new file
	{
    int i = 1;
    while (1)
    {
      String file = "datalog_";    //create a string with base name datalog
      file += i;                  //append the next value in the sequence to the new file name
      file += ".txt";
      if (!SD.exists(file))     //check to see if this file already exists, if not make that the filename and break the loop
      {
        filename = file;		//if the file does not exist, set this to the filename and break out of the loop
        break;
      }
      i++;		//else increment the value again and loop
    }
   }
   else
   {
     filename = "datalog.txt";    //otherwise no data files present so just make a new one
   }
   Serial.print("Filename: "); Serial.println(filename);		//print to serial bus for debugging
}

bool Log::log_to_file(GY521 mpu)		//using the MPU object, log all the data from all the sensors to the SD card
{
	File log = SD.open(filename, FILE_WRITE);

	if (log)
	{
		//prints the data in the format of "X acceleration	Y Acceleration	Z acceleration	X GYRO	Y GYRO	Z GYRO	temp\n"
			log.print(mpu.get_x_accel()); log.print('\t');
			log.print(mpu.get_y_accel()); log.print('\t');
			log.print(mpu.get_z_accel()); log.print('\t');

			log.print(mpu.get_x_gyro()); log.print('\t');
			log.print(mpu.get_y_gyro()); log.print('\t');
			log.print(mpu.get_z_gyro()); log.print('\t');

			log.println(mpu.get_temp());

			log.close();
			return true;		//if the file was able to be opened and written to, return true
	}
	else
	{
		return false; //if file is not found or unable to be written to return false
	}
}

bool Log::log_event(String event)			//logs a string specified by event, example : "Parachute_Deployed"
{
  File log = SD.open(filename, FILE_WRITE);
  if (log)
  {
    log.println(event);
    log.close();
    return true;
  }
  return false;
}
