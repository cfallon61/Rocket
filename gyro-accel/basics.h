#include "Arduino.h"


#ifndef basics_h
#define basics_h
#define ONBOARD_LED 13

void init_led(int led_pin);
void flash(int duration);
void flash(int duration, int led);
void loop_flash(int loop, int flash_dur);
void loop_flash(int loop, int flash_dur, int led_pin);
#endif