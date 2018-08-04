#include <Servo.h>
#include "GY521.h"

#ifndef deploy_h
#define deploy_h

#define SERVO 9
#define BUZZER 2

void deploy(int deploy_pos);
void servo_init(int setup_pos, int armed, int wait_time);
bool check_freefall(GY521 mpu);

#endif
