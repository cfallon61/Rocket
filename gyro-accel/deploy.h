#include <Servo.h>

#ifndef deploy_h
#define deploy_h

#define SERVO 9
#define BUZZER 2

void deploy(int deploy_pos);
void servo_init(int armed, int arming, int wait_time);

#endif