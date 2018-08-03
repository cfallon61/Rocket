#include "basics.h"

void init_led(int led_pin)
{
	pinMode(led_pin, OUTPUT);
	flash(50);
}

void flash(int duration)
{
	flash(duration, ONBOARD_LED);
}
void flash(int duration, int led)
{
	digitalWrite(led, HIGH);
	delay(duration);
	
	digitalWrite(led, LOW); 
	delay(duration);
}

void loop_flash(int loop, int flash_dur)
{
	loop_flash(loop, flash_dur, ONBOARD_LED);
}

void loop_flash(int loop, int flash_dur, int led_pin)
{
	for (int i = 0; i < loop; i++)
	{
		flash(flash_dur);
	}
}



