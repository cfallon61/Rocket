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
  int deploy_wait_time);
  void start();
  void debug(data_type type);
  void current_status();

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

  void landed_event();
  void launch_event();
  void freefall_event();
  void logger_init_error();
  void mpu_init_error();

  Log logger;
  GY521 mpu6050;
};
#endif
