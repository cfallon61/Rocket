#include <Servo.h>

#ifndef deploy_h
#define deploy_h

#define SERVO 9
#define BUZZER 2

void deploy(int deploy_pos);
void deploy(int deploy_pos, int wait);
void servo_init(int setup_pos, int armed, int wait_time);

#endif
