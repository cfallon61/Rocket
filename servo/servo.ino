#include<Servo.h>

Servo servo;
int servoPin = 9;
int buzzer = 4;
int led = 13;
int setupPos = 90;
int startPos = 1;
int endPos = 180;
int count;
int i;

void setup() {
pinMode(led, OUTPUT);
servoInit(startPos, setupPos);
Serial.begin(9600);
}

void servoInit(int start, int setup){
  servo.attach(servoPin);
  tone(buzzer, 3000, 333);    //alerts that beginning the arming stage              
  
  servo.write(setup);         //moves servo to arming position
  
  delay(30500);               //waits 30 seconds to allow for rubber band to be armed
  
  servo.write(setup);         //finished arming, go to being position
  
   for(i = 0; i < 4; i++){ //alerts that arming is done and servo is ready to deploy
      tone(buzzer, 3000, 125);
      delay(250);
   }

   servo.write(start);        //start == 1
}

void deploy(){          //moves servo to final position
  for (i = 0; i < 3; i++){
    tone(buzzer, 4000, 125);
    delay(250);
  }

  servo.write(endPos);    //endPos == 180
  while(1){
                          //keeps arduino busy
  }
}

void loop() {
  tone(buzzer, 1000, 100);    //counts to 30 
  delay(1100);
  count++;
  if(count == 30){
    deploy();         //calls deploy function
  }
}
