const int analogIn = A0;
int mVperAmp = 100;
int RawValue = 0;
int ACSoffset = 2500;
double voltage = 0;
double amps = 0;
int BUZZER = 4;


void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(A0, INPUT);
Serial.begin(9600);
tone(BUZZER, 3000, 1000);
}

void loop() {
  // put your main code here, to run repeatedly:
RawValue - analogRead(A0);
voltage = (RawValue / 1024.0) * 5000;
amps = ((voltage - ACSoffset) / mVperAmp);
 
digitalWrite(13, HIGH);
Serial.print("Raw Value = ");
Serial.print(RawValue);
Serial.print("\t mV = ");
Serial.print(voltage, 3);
Serial.print("\t Amps = ");
Serial.print(amps, 3);
Serial.print("\n");
digitalWrite(13, LOW);
delay(100);



}
