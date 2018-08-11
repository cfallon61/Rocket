#include "deploy.h"

Servo servo;

void servo_init(int setup_pos, int armed, int wait_time){


  servo.attach(SERVO);
  tone(BUZZER, 3000, 333);               //alerts that beginning the arming stage

  servo.write(setup_pos);         //moves servo to arming position

  delay(wait_time);                  //waits X seconds to allow for rubber band to be armed
  delay(wait_time);                  //waits X seconds to allow for rubber band to be armed
  delay(wait_time);                  //waits X seconds to allow for rubber band to be armed

   for(int i = 0; i < 4; i++){               //alerts that arming is done and servo is ready to deploy
      tone(BUZZER, 3000, 125);
      delay(250);
   }

   servo.write(armed);
}

void deploy(int deploy_pos)     //overloaded function which defaults the deploy wait time to 0 ms
{
  deploy(deploy_pos, 0);
}

void deploy(int deploy_pos, int wait){                        //moves servo to final position
  for (int i = 0; i < 3; i++){        //beeps the buzzer to alert that the deploy function has been triggered
    tone(BUZZER, 4000, 125);
    delay(250);
  }
  delay(wait);      //delay for wait ms until moving the servo
  servo.write(deploy_pos);    //move the servo to the deployed position
}
