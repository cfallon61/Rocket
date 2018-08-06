#include "rocket.h"

#define LAUNCH_THRESHOLD 3
#define FF_THRESHOLD 0.05
#define TILT_THRESHOLD 200
#define TOUCH_DOWN_THRESHOLD 3

#define SETUP_POS 180
#define ARMED_POS 0
#define DEPLOY_POS 65
#define SETUP_WAIT_TIME 300
#define DEPLOY_WAIT_TIME 0

#define SAMPLE_RATE 3

Rocket Bartholomew(ACCEL_16G, GYRO_1000DG, LAUNCH_THRESHOLD, FF_THRESHOLD, TILT_THRESHOLD, TOUCH_DOWN_THRESHOLD,
                   SETUP_POS, ARMED_POS, DEPLOY_POS, SETUP_WAIT_TIME, DEPLOY_WAIT_TIME);

void setup() {
  Wire.begin();    //enable I2C 
  Serial.begin(9600);   //enable serial monitoring for debugging
  Bartholomew.start();
}

void loop() {
  Bartholomew.current_status(SAMPLE_RATE);
//  Bartholomew.debug(ACCEL);
//  Bartholomew.debug(GYRO);
//  Bartholomew.debug(TEMP);
}
