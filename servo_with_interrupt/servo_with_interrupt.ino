#include<Servo.h>

int interrupt = 3;
int buzzer = 4;
int servoPin = 9;
int led = 13;

Servo servo;

int setupPos = 90;
int startPos = 1;
int endPos = 180;
int setupWaitTime = 10000;


int count = 0;
int i;
boolean launch = false;


void setup() {
  
  pinMode(led, OUTPUT);                 //initializes led 
  pinMode(interrupt, INPUT_PULLUP);     //initializes pin 3 as switch interrupt/launch detect

  attachInterrupt(digitalPinToInterrupt(interrupt), startCount, HIGH);    //initializes interrupt conditions

  servoInit(startPos, setupPos);        //initializes servo. setupPos is the angle to use when loading rubber band, startPos is angle at launch

}

void startCount(){                      //ISR
  if (!launch){
    launch = true;
    digitalWrite(led, HIGH);            //shows that interrupt has been triggered
  }
}

void incCount(){                        //adds 1 second (approx) to count
  if(launch){
   count++;
  tone(buzzer, 1000, 250);              //beeps every second
  delay(1000);
  }
}

void servoInit(int start, int setup){
  servo.attach(servoPin);
  tone(buzzer, 3000, 333);    //alerts that beginning the arming stage              
  
  servo.write(setup);         //moves servo to arming position
  
  delay(setupWaitTime);               //waits X seconds to allow for rubber band to be armed
  
  servo.write(setup);         //finished arming, go to being position
  
   for(i = 0; i < 4; i++){    //alerts that arming is done and servo is ready to deploy
      tone(buzzer, 3000, 125);
      delay(250);
   }

   servo.write(start);
}

void deploy(){                        //moves servo to final position
  for (i = 0; i < 3; i++){
    tone(buzzer, 4000, 125);
    delay(250);
  }

  servo.write(endPos);
  while(1){
     delay(1000);
     digitalWrite(led, HIGH);
     delay(1000);
     digitalWrite(led, LOW);
  }
}

void loop() {
 if(launch){
  incCount();
 }
  if(launch && count > 29){
    deploy();
  }
}
