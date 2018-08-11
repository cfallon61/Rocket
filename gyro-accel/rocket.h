#include "Arduino.h"
#include "deploy.h"
#include "log.h"
#include "GY521.h"

#ifndef rocket_h
#define rocket_h

class Rocket{
public:
  Rocket(accel_value_t accel, gyro_value_t gyro, double launch_threshold, double ff_threshold, double tilt_threshold, double touch_down_threshold,
  int setup_pos, int armed_pos, int deploy_pos, int setup_wait_time,
  int deploy_wait_time);    //constructor to instantiate a rocket object and assign the default values
  void start();   //begin the rocket's data logging and processing
  void debug(data_type type);     //method used for printing data values to serial bus
  void current_status();      //get the current status of the rocket, default sample size of 3
  void current_status(int sample_rate);   //get current status of rocket using sample_rate amount of data points

private:
  int deploy_pos;
  int setup_pos;
  int armed_pos;
  int setup_wait_time;
  int deploy_wait_time;

  double tilt_threshold;
  double ff_threshold;
  double launch_threshold;
  double touch_down_threshold;

  int buzzer = 2;

  bool in_flight;
  bool parachute_deployed;

  void landed_event();    //determine if the rocket has landed
  void launch_event();    //determine if the rocket has launched
  void freefall_event();   //determine if the rocket is in freefall
  void logger_init_error();   //error that SD card reader is not found or damaged
  void mpu_init_error();    //MPU is not found or is damaged

  Log logger;   //make a new logger object
  GY521 mpu6050;    //make a new MPU object
};
#endif
