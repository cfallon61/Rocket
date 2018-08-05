#include "rocket.h"

Rocket::Rocket(accel_value_t accel, gyro_value_t gyro, double launch_threshold, double ff_threshold, double tilt_threshold, double touch_down_threshold,
int setup_pos, int armed_pos, int deploy_pos, int setup_wait_time,
int deploy_wait_time)
{
  pinMode(buzzer, OUTPUT);
  tone(buzzer, 4000, 100);

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  this->launch_threshold = launch_threshold;
  this->ff_threshold = ff_threshold;
  this->tilt_threshold = tilt_threshold;
  this->touch_down_threshold = touch_down_threshold;
  this->setup_pos = setup_pos;
  this->armed_pos = armed_pos;
  this->deploy_pos = deploy_pos;
  this->setup_wait_time = setup_wait_time;
  this->deploy_wait_time = deploy_wait_time;
  in_flight = false;
  parachute_deployed = false;
  mpu6050 = GY521(gyro, accel);
}

void Rocket::start()
{
  if (!logger.init())   //if the logger was not able to init, stop the setup and error
  {
    logger_init_error();
  }

  if (!mpu6050.init()){   //if the mpu was not able to init, stop and error, and also log to data file
    logger.log_event("MPU_Failure");
    mpu_init_error();
  }
  servo_init(setup_pos, armed_pos, setup_wait_time);
}



void Rocket::debug(data_type type)    //used for writing the data to serial bus for ground debugging
{
  mpu6050.print_data(type);
}

void Rocket::landed_event()   //function for when the rocket has detected having landed
{
  logger.log_event("Landed");   //log event

  tone(buzzer, 3000, 5000);
  delay(5500);

  while(1)    //sound buzzer forever
  {
    for (int i = 0; i < 5; i++)
    {
      tone(buzzer, 3000, 250);
      delay(750);
    }
    tone(buzzer, 4000, 500);
    delay(1000);
  }
}

void Rocket::launch_event()   //fucntion for detection of launch
{
  tone(buzzer, 4000, 250);    //alert of status chance
  in_flight = true;   //toggle flight status
  logger.log_event("Launch_Detect");    //log event
  logger.log_to_file(mpu6050);
}

void Rocket::freefall_event()
{
  deploy(deploy_pos, deploy_wait_time);
  parachute_deployed = true;
  logger.log_event("Parachute_Deployed");
}

void Rocket::current_status()
{
  mpu6050.read_data();

  if (in_flight)
  {
    logger.log_to_file(mpu6050);
    if (mpu6050.check_freefall(ff_threshold))     //if the rocket is in freefall deploy the parachute
    {
      freefall_event();
    }
    else if (mpu6050.check_tilt(tilt_threshold) && parachute_deployed && mpu6050.is_touched_down(touch_down_threshold))  //if the tilt of the rocket has exceeded threshold landing value, stop logging and sound buzzer forever
    {
      landed_event();
    }
  }
  else if (!in_flight && mpu6050.check_launch(launch_threshold))    //if not in flight but spike in G force is noticed, set status in_flight to true and begin logging data
  {
    launch_event();
  }
}

void Rocket::logger_init_error()
{
  while(1)
    {
      tone(buzzer, 500, 500);
      delay(2000);
    }
}

void Rocket::mpu_init_error()
{
  while (1)
{
  for (int i = 0; i < 3; i++)
  {
    tone(buzzer, 300, 500);
    delay(750);
  }
  tone(buzzer, 300, 750);
  delay(1750);
}
}
