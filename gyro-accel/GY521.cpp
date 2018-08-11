#include "GY521.h"
#include "basics.h"

GY521::GY521(gyro_value_t gyro_value, accel_value_t accel_value)    //sets the accelerometer LSB based on accel_value and initializes the gyro and accelerometer based on their desired parameters
{
  this->accel_value = accel_value;
  this->gyro_value = gyro_value;

  switch (accel_value)    //sets the accelerometer LSB for conversion to actual values
  {
    case(ACCEL_2G):
       accel_lsb = 16384;
       break;
    case(ACCEL_4G):
        accel_lsb = 8192;
        break;
    case(ACCEL_8G):
        accel_lsb = 4096;
        break;
    case (ACCEL_16G):
        accel_lsb = 2048;
        break;
  }

  switch(gyro_value)    //sets the gyro lsb for conversion to actual value
  {
    case (GYRO_250DG):
      gyro_lsb = 131;
      break;
    case (GYRO_500DG):
      gyro_lsb = 65.5;
      break;
    case (GYRO_1000DG):
      gyro_lsb = 32.8;
      break;
    case (GYRO_2000DG):
      gyro_lsb = 16.4;
      break;
  }
}

bool GY521::init()
{
  if (!detect_mpu())  //if the MPU is not detected, return false for error
  {
    return false;
  }
  else    //initialize the registers
  {
    init_reg(PWR_MGMT_1, 0);    //wake up the accelerometer from sleep
    init_reg(GYRO_CONFIG, gyro_value<<3);   //initialize the gyro, see header for relevant bits
    init_reg(ACCEL_CONFIG, accel_value<<3); //initialize the accelerometer, see header for relevant bits
    return true;    //return true on success
  }
}

void GY521::init_reg(int reg, int value)    //method used to write value to the relevant registers
{
	Wire.beginTransmission(GY521_ADDR);    //bring the I2C line to the MPU high for transmission
	Wire.write(reg);     //tell the MPU which register to write the value to
	Wire.write(value);   //write the value to the register
	Wire.endTransmission(true);    //bring the I2C line low for end transmission
}


void GY521::read_reg(int16_t *data, int begin_addr)     //reads the begin and end data registers for the specified axes/MEMS device, see header for list of options
{
 Wire.beginTransmission(GY521_ADDR);    //bring high for begin transmission
  Wire.write(begin_addr);     //specify the register to get data from
  Wire.endTransmission(false);
  Wire.requestFrom(GY521_ADDR, 2, true);    //read 2 bytes from the register

  *data = Wire.read() <<8 | Wire.read();    //read the first byte, left shift the data 8 bits, then logical or with the next byte. write that to data
}

void GY521::read_data()   //default read all data, sample data points is 3
{
  read_data(3);
}

void GY521::print_data(data_type data)    //public method for writing the data to serial interface for live debugging
{
  switch (data)
  {
    case (ACCEL):
      print_accel_data();
      break;
    case (GYRO):
      print_gyro_data();
      break;
    case (TEMP):
      print_temp_data();
      break;
    case (ALL_DATA):
      print_accel_data();
      print_gyro_data();
      print_temp_data();
  }
}

void GY521::read_data(int sample_size)   //reads the data from all 3 sensors and filters using up to 10 samples
{
  check_filter(sample_size);
	read_accel(sample_size);
	read_temp(sample_size);
	read_gyro(sample_size);
}

void GY521::print_accel_data()    //print accelerometer values to serial interface for live debugging
{
	Serial.print("x_accel = "); Serial.print(x_accel);
	Serial.print(" | y_accel = "); Serial.print(y_accel);
	Serial.print(" | z_accel = "); Serial.println(z_accel);
}

void GY521::print_gyro_data() //print gyro values to serial interface for live debugging
{
	Serial.print("x_gyro = "); Serial.print(x_gyro);
	Serial.print(" | y_gyro = "); Serial.print(y_gyro);
	Serial.print(" | z_gyro = "); Serial.println(z_gyro);
}

void GY521::print_temp_data()   //print temp values to serial interface for live debugging
{
	Serial.print("temp = "); Serial.println(temp);
}

double GY521::get_x_accel()   //getter method for returning the current x acceleration value
{
	return x_accel;
}
double GY521::get_y_accel()   //getter method for returning the current y acceleration value
{
	return y_accel;
}

double GY521::get_z_accel()   //getter method for returning the current z acceleration value
{
	return z_accel;
}

double GY521::get_x_gyro()   //getter method for returning the current x gyro value
{
	return x_gyro;
}

double GY521::get_y_gyro()   //getter method for returning the current y gyro value
{
	return y_gyro;
}

double GY521::get_z_gyro()   //getter method for returning the current z gyro value
{
	return z_gyro;
}

double GY521::get_temp()   //getter method for returning the current temp value
{
	return temp;
}

void GY521::check_filter(int &sample_size)    //checks for valid filter range, 1 - 10
{
	if (sample_size > 10)   //if greater than 10, reset to 10
	{
		sample_size = 10;
    return;
	}
  if (sample_size < 1)  //if less than 1, set to 1
  {
    sample_size = 1;
  }
}

double GY521::convert_data(int total, int sample_size, double lsb)    //takes the raw data and averages the values by the sample size and then divides by the LSB of the corresponding sensor
{
  return (total / sample_size) / lsb;
}

void GY521::read_accel()  //default sample size of 3, reads accelerometer data
{
	read_accel(3);
}

void GY521::read_accel(int sample_size)   //read accelerometer data and pre filter it using the sample size provided
{
	check_filter(sample_size);

	int16_t x_arr[sample_size];    //make 3 new arrays to store the list of values in to be averaged later
	int16_t y_arr[sample_size];
	int16_t z_arr[sample_size];
	long tot_x, tot_y, tot_z = 0;

	for (int i = 0; i < sample_size; i++)   //takes in sample_size amount of samples and adds them to a total value
	{
		read_reg(&x_arr[i], ACCEL_XOUT_H);    //read the accelerometer registers
		read_reg(&y_arr[i], ACCEL_YOUT_H);
		read_reg(&z_arr[i], ACCEL_ZOUT_H);

		tot_x += x_arr[i];    //add all the values for averaging later
		tot_y += y_arr[i];
		tot_z += z_arr[i];
	}

	x_accel = convert_data(tot_x, sample_size, accel_lsb);    //averages the raw values and then divides by their LSB to get the actual value
	y_accel = convert_data(tot_y, sample_size, accel_lsb);
	z_accel = convert_data(tot_z, sample_size, accel_lsb);
}

void GY521::read_gyro()   //default sample size of 3, reads gyro data
{
	read_gyro(3);
}

void GY521::read_gyro(int sample_size)    //read gyro data and pre filter it using the sample size provided
{
	check_filter(sample_size);

	int16_t x_arr[sample_size];    //make 3 new arrays to store the list of values in to be averaged later
	int16_t y_arr[sample_size];
	int16_t z_arr[sample_size];
	long tot_x, tot_y, tot_z = 0;

	for (int i = 0; i < sample_size; i++)   //takes in sample_size amount of samples and adds them to a total value
	{
		read_reg(&x_arr[i], GYRO_XOUT_H);
		read_reg(&y_arr[i], GYRO_YOUT_H);
		read_reg(&z_arr[i], GYRO_ZOUT_H);

		tot_x += x_arr[i];
		tot_y += y_arr[i];
		tot_z += z_arr[i];
	}
	x_gyro = convert_data(tot_x, sample_size, gyro_lsb);   //averages the values and then divides by their LSB to get the actual value
	y_gyro = convert_data(tot_y, sample_size, gyro_lsb);
	z_gyro = convert_data(tot_z, sample_size, gyro_lsb);
  x_gyro += 5;
}

void GY521::read_temp()   //default sample size of 3, reads temp data
{
	read_temp(3);
}

void GY521::read_temp(int sample_size)        //read temp data and filter it using the sample size provided
{
	check_filter(sample_size);

	int16_t temp_arr[sample_size];
	int16_t tot_temp = 0;

	for (int i = 0; i < sample_size; i++)   //takes in sample_size amount of samples and adds them to a total value
	{
		read_reg(&temp_arr[i], TEMP_OUT_H);
		tot_temp += temp_arr[i];
	}
	temp = tot_temp / sample_size;    //averages the values
	temp = (temp / 340) + 35.53;      //converts to degrees C based on datasheet
}


bool GY521::detect_mpu()		//return true if the MPU is detected, false otherwise
{
	int address;
	int error;

	for(address = 1; address < 127; address++ )
	{
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0) //if the error is not 1, return true because there is a device detected at this address
		{
			return true;
		}
  }
  return false;   //no devices detected, return false
}


bool GY521::check_freefall(double threshold)    //checks to see if the rocket is in a state of 0 g / freefall
{
  if (x_accel < threshold && x_accel > -threshold
      && y_accel < threshold && y_accel > -threshold
      && z_accel < threshold && z_accel > -threshold)
  {
    return true;
  }
  return false;
}

bool GY521::check_launch(double threshold)    //if the g forces have spiked beyond the specified threshold, then the rocket is assumed to have launched
{
  if ((x_accel > threshold || x_accel < -threshold)
      || (y_accel > threshold || y_accel < -threshold)
      || (z_accel > threshold || z_accel < -threshold))
  {
    return true;
  }
  return false;
}

bool GY521::check_tilt(double threshold)    //check the rotational speed of the rocket
{
  if ((x_gyro > threshold || x_gyro < -threshold)
  || (y_gyro > threshold || y_gyro < -threshold)
  || (z_gyro > threshold || z_gyro < -threshold))
  {
    return true;
  }
  return false;
}

bool GY521::is_touched_down(double threshold)   //if there is another spike in g forces after the rocket has been detected to launch, the rocket has landed
{
  if (x_accel < threshold && x_accel > -threshold
      && y_accel < threshold && y_accel > -threshold
      && z_accel < threshold && z_accel > -threshold)
   {
      return true;
   }
   return false;
}
