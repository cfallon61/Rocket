#include "GY521.h"
#include "basics.h"
#include "deploy.h"

#define BUZZER 2
#define INTERRUPT 3
#define DEPLOY_POS 180
#define SETUP_POS 0
#define ARMED_POS 90
#define WAIT_TIME 10000

double ff_threshold = 0.1;
bool in_flight = false;

GY521 mpu6050(GYRO_250DG, ACCEL_16G);

void setup() {
  Wire.begin();    //enable I2C 
  Serial.begin(9600);   //enable serial monitoring
  
  pinMode(BUZZER, OUTPUT);    //sound buzzer to make sure it works
  tone(BUZZER, 500, 100);

  pinMode(INTERRUPT, INPUT_PULLUP);   //set pin 3 to interrupt pin
  attachInterrupt(digitalPinToInterrupt(INTERRUPT), flight_status, HIGH);   //when detects change, change flight status
  
  mpu6050.init();
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
    tone(BUZZER, 700, 100);
    delay(900);
  }
  else 
  {
     mpu6050.read_data(10);
     if (mpu6050.x_accel < ff_threshold && mpu6050.y_accel < ff_threshold && mpu6050.z_accel < ff_threshold)    //if the rocket is in free fall, deploy the parachute
     {
      deploy(DEPLOY_POS);
      while(1)
      {
        flash(250);
      }
     }
  }
 

}


