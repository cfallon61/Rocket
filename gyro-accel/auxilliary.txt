

pinMode(INTERRUPT, INPUT_PULLUP);   //set pin 3 to interrupt pin
attachInterrupt(digitalPinToInterrupt(INTERRUPT), flight_status, HIGH);   //when detects change, change flight stat



  if (!in_flight)
  {
   tone(BUZZER, 2000, 100);
    delay(900);
  }
  else 
  {
       mpu6050.read_data(10);
       mpu6050.print_accel_data();
       mpu6050.print_gyro_data();
     mpu6050.print_temp_data();
     logger.log_to_file(mpu6050);
     if (mpu6050.check_freefall() || mpu6050.check_tilt())   //if the rocket is in free fall, deploy the parachute
delay(5000);
     {
      deploy(DEPLOY_POS);
      while(1)
      {
          tone(BUZZER, 4000, 250);
          delay(1250);
      }
     }

void flight_status()    //if the launch detect switch has been pulled, change status to in_flight = true
{
  if (!in_flight)
  {
    in_flight = true;
  }
  else
  {
    in_flight = false;
  }
}