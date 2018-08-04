#include<Servo.h>

#define INTERRUPT 3
#define BUZZER 4
#define SERVO 9
#define LED 13

Servo parachute_servo;

int setupPos = 90;
int startPos = 1;
int endPos = 180;
int setupWaitTime = 10000;

int flight_time = 0;
int i;

boolean launch_detect = false;
boolean init_state = true;


void setup() {
  
  pinMode(LED, OUTPUT);                 //initializes LED 
  pinMode(INTERRUPT, INPUT_PULLUP);     //initializes pin 3 as switch interrupt/launch_detect

  attachInterrupt(digitalPinToInterrupt(INTERRUPT), startCount, HIGH);    //initializes interrupt conditions

  servoInit(startPos, setupPos);        //initializes parachute_servo. setupPos is the angle to use when loading rubber band, startPos is angle at launch
  init_state = false;

}

void startCount(){                      //ISR
  if (!launch_detect && !init_state){   //if the interrupt has been triggered but the mechanism is still in init state, do nothing, else set launch flag true and begin the parachute deploy sequence
    launch = true;
    digitalWrite(LED, HIGH);            //shows that interrupt has been triggered
  }
}

void inc_time(){                        //adds 1 second (approx) to count
  flight_time++;
  tone(BUZZER, 1000, 250);              //beeps every second
  delay(1000);
}

void servoInit(int armed, int arming){
  parachute_servo.attach(SERVO);
  tone(BUZZER, 3000, 333);               //alerts that beginning the arming stage              
  
  parachute_servo.write(arming);         //moves servo to arming position
  
  delay(setupWaitTime);                  //waits X seconds to allow for rubber band to be armed
  
  parachute_servo.write(arming);         //finished arming, go to begin position
  
   for(i = 0; i < 4; i++){               //alerts that arming is done and servo is ready to deploy
      tone(BUZZER, 3000, 125);
      delay(250);
   }

   parachute_servo.write(armed);
}

void deploy(){                        //moves servo to final position
  for (i = 0; i < 3; i++){
    tone(BUZZER, 4000, 125);
    delay(250);
  }

  parachute_servo.write(endPos);
  while(1){
     delay(1000);
     digitalWrite(LED, HIGH);
     delay(1000);
     digitalWrite(LED, LOW);
  }
}

void loop() {
 if(launch_detect){
  inc_time();
 }
  if(launch_detect && flight_time > 4)
  {
    deploy();
    while(1)
    {
     flash(250);
    }
  }
}
