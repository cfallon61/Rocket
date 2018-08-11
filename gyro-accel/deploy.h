#include <Servo.h>

#ifndef deploy_h
#define deploy_h

#define SERVO 9
#define BUZZER 2

void deploy(int deploy_pos);    //deploy method with defaults the deploy delay time to 0 ms
void deploy(int deploy_pos, int wait);    //method which deploys the servo and waits "wait" ms to do so
void servo_init(int setup_pos, int armed, int wait_time);   //instantiate the servo object and move it to the setup position
                                                            //to allow for loading into the payload bay

#endif
