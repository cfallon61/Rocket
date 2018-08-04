#include "GY521.h"
#include "basics.h"
#include "deploy.h"
#include "log.h"

#define BUZZER 2
#define INTERRUPT 3
#define DEPLOY_POS 65
#define SETUP_POS 180
#define ARMED_POS 0
#define WAIT_TIME 30000

double ff_threshold = 0.1;
bool in_flight = false;

GY521 mpu6050(GYRO_250DG, ACCEL_16G);
Log logger;

void setup() {
  Wire.begin();    //enable I2C 
  Serial.begin(9600);   //enable serial monitoring for debugging
  
  pinMode(BUZZER, OUTPUT);    //sound buzzer to make sure it works
  tone(BUZZER, 3000, 100);

   pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  
  pinMode(INTERRUPT, INPUT_PULLUP);   //set pin 3 to interrupt pin
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), flight_status, HIGH);   //when detects change, change flight status

  if (!logger.init())   //if the logger was not able to init, stop the setup and error
  {
    while(1)
    {
      delay(750);
      tone(BUZZER, 500, 250);
    }
  }
  
  if (!mpu6050.init()){
    while (1)
    {
      tone(BUZZER, 500, 500);
      delay(1500);
    }
  }
  servo_init(SETUP_POS, ARMED_POS, WAIT_TIME);
}

void flight_status()    //if the launch detect switch has been pulled, change status to in_flight = true
{
  if (!in_flight)
  {
    in_flight = true;
  }
  else
  {
    in_flight = false;
  }
}

void loop() {
  if (!in_flight)
  {
    tone(BUZZER, 2000, 100);
    delay(900);
  }
  else 
  {
       mpu6050.read_data(10);
//       mpu6050.print_accel_data();
//       mpu6050.print_gyro_data();
//     mpu6050.print_temp_data();
     logger.log_to_file(mpu6050);
     if (mpu6050.check_freefall() || mpu6050.check_tilt())   //if the rocket is in free fall, deploy the parachute
//delay(5000);
     {
      deploy(DEPLOY_POS);
      while(1)
      {
          tone(BUZZER, 4000, 250);
          delay(1250);
      }
     }
  }
 

}


