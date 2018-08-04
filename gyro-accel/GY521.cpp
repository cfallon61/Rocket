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
  if (!detect_mpu())  //if the MPU is not detected, error
  {
    return false;
  }
  else    //initialize the registers
  {
    init_reg(PWR_MGMT_1, 0);
    init_reg(GYRO_CONFIG, gyro_value<<3);
    init_reg(ACCEL_CONFIG, accel_value<<3);   
    return true;
  }
}

void GY521::init_reg(int reg, int value)
{
	Wire.beginTransmission(GY521_ADDR);
	Wire.write(reg);
	Wire.write(value);
	Wire.endTransmission(true);
}


void GY521::read_reg(int16_t *data, int begin_addr)
{
 Wire.beginTransmission(GY521_ADDR);
  Wire.write(begin_addr);
  Wire.endTransmission(false);
  Wire.requestFrom(GY521_ADDR, 2, true);
  
  *data = Wire.read() <<8 | Wire.read();
}

void GY521::read_data()   //default read all data, filter default is 3
{
  read_data(3);
}

void GY521::read_data(int filter_size)   //reads the data from all 3 sensors and filters using up to 10 samples
{
  check_filter(filter_size);
	read_accel(filter_size);
	read_temp(filter_size);
	read_gyro(filter_size);
}

void GY521::print_accel_data()    //print accelerometer values
{
	Serial.print("x_accel = "); Serial.print(x_accel);
	Serial.print(" | y_accel = "); Serial.print(y_accel);
	Serial.print(" | z_accel = "); Serial.println(z_accel);
}

void GY521::print_gyro_data() //print gyro values
{
	Serial.print("x_gyro = "); Serial.print(x_gyro);
	Serial.print(" | y_gyro = "); Serial.print(y_gyro);
	Serial.print(" | z_gyro = "); Serial.println(z_gyro);
}

void GY521::print_temp_data()   //print temp values
{
	Serial.print("temp = "); Serial.println(temp);
}

double GY521::get_x_accel()
{
	return x_accel;
}
double GY521::get_y_accel()
{
	return y_accel;
}

double GY521::get_z_accel()
{
	return z_accel;
}

double GY521::get_x_gyro()
{
	return x_gyro;
}

double GY521::get_y_gyro()
{
	return y_gyro;
}

double GY521::get_z_gyro()
{
	return z_gyro;
}

double GY521::get_temp()
{
	return temp;
}

void GY521::check_filter(int &filter_size)    //checks for valid filter range, 1 - 10
{
	if (filter_size > 10)   //if greater than 10, reset to 10
	{
		filter_size = 10;
    return;
	}
  if (filter_size < 1)  //if less than 1, set to 1
  {
    filter_size = 1; 
  }
}

double GY521::convert_data(int total, int filter_size, double lsb)
{
  return (total / filter_size) / lsb;
}

void GY521::read_accel()  //default sample size of 3, reads accelerometer data
{
	read_accel(3);
}

void GY521::read_accel(int filter_size)   //read accelerometer data and pre filter it using the sample size provided
{
	check_filter(filter_size);
	
	int16_t x_arr[filter_size];
	int16_t y_arr[filter_size];
	int16_t z_arr[filter_size];
	long tot_x, tot_y, tot_z = 0;
 
	for (int i = 0; i < filter_size; i++)   //takes in filter_size amount of samples and adds them to a total value
	{
		read_reg(&x_arr[i], ACCEL_XOUT_H);
		read_reg(&y_arr[i], ACCEL_YOUT_H);
		read_reg(&z_arr[i], ACCEL_ZOUT_H);
		
		tot_x += x_arr[i];
		tot_y += y_arr[i];
		tot_z += z_arr[i];
	}
	
	x_accel = convert_data(tot_x, filter_size, accel_lsb);    //averages the values and then divides by their LSB to get the actual value
	y_accel = convert_data(tot_y, filter_size, accel_lsb);
	z_accel = convert_data(tot_z, filter_size, accel_lsb);
}

void GY521::read_gyro()   //default sample size of 3, reads gyro data
{
	read_gyro(3);
}

void GY521::read_gyro(int filter_size)    //read gyro data and pre filter it using the sample size provided
{
	check_filter(filter_size);

	int16_t x_arr[filter_size];
	int16_t y_arr[filter_size];
	int16_t z_arr[filter_size];
	long tot_x, tot_y, tot_z = 0;
	
	for (int i = 0; i < filter_size; i++)   //takes in filter_size amount of samples and adds them to a total value
	{
		read_reg(&x_arr[i], GYRO_XOUT_H);
		read_reg(&y_arr[i], GYRO_YOUT_H);
		read_reg(&z_arr[i], GYRO_ZOUT_H);
		
		tot_x += x_arr[i];
		tot_y += y_arr[i];
		tot_z += z_arr[i];
	}
	x_gyro = convert_data(tot_x, filter_size, gyro_lsb);   //averages the values and then divides by their LSB to get the actual value
	y_gyro = convert_data(tot_y, filter_size, gyro_lsb);
	z_gyro = convert_data(tot_z, filter_size, gyro_lsb);
}

void GY521::read_temp()   //default sample size of 3, reads temp data
{
	read_temp(3);
}

void GY521::read_temp(int filter_size)        //read temp data and pre filter it using the sample size provided
{
	check_filter(filter_size);

	int16_t temp_arr[filter_size];
	int16_t tot_temp = 0;
 
	for (int i = 0; i < filter_size; i++)   //takes in filter_size amount of samples and adds them to a total value
	{
		read_reg(&temp_arr[i], TEMP_OUT_H);
		tot_temp += temp_arr[i];
	}
	temp = tot_temp / filter_size;    //averages the values
	temp = temp / 340 + 36.53;      //converts to degrees C based on datasheet
  if (temp > 40)
  {
    temp -= 20;
  }
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
 
		if (error == 0)
		{
			return true;
		}   
  }
  return false;
}


bool GY521::check_freefall(double threshold)
{
  if (x_accel < threshold && x_accel > -threshold 
      && y_accel < threshold && y_accel > -threshold 
      && z_accel < threshold && z_accel > -threshold)
  {
    return true;
  }
  return false;
}

bool GY521::check_launch(double threshold)
{
  if ((x_accel > threshold || x_accel < -threshold) 
      || (y_accel > threshold || y_accel < -threshold) 
      || (z_accel > threshold || z_accel < -threshold))
  {
    return true;
  }
  return false;
}

bool GY521::check_tilt(double threshold)
{
  if ((x_gyro > threshold || x_gyro < -threshold) || (y_gyro > threshold || y_gyro < -threshold) || (z_gyro > threshold || z_gyro < -threshold))
  {
    return true;
  }
  return false;
}




