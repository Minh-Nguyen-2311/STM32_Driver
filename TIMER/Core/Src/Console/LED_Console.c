/*
 * LED_Console.c
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#include "OOP.h"
#include "OOP_Console.h"
#include "LED_Process.h"

#if defined(ENCODER_MODE)

ChipLed LED[Num_led_Din1+Num_led_Din2];

OOP const *consoles[] =
{
	&LED[0].super,	&LED[1].super,	&LED[2].super,	&LED[3].super
};

void User_ctor(infor_console* console)
{
	ChipLed_ctor(&LED[0], NULL, 0, 0, 1, 0, 0);
	ChipLed_ctor(&LED[0], NULL, 0, 1, 2, 0, 0);
	ChipLed_ctor(&LED[0], NULL, 0, 2, 3, 0, 0);
	ChipLed_ctor(&LED[0], NULL, 0, 3, 4, 0, 0);

	console->NumberOfDevices = sizeof(consoles)/sizeof(OOP*);
	console->num_led = Num_led_Din1 + Num_led_Din2;
	console->pLED = LED;
	InitAll(consoles, console->NumberOfDevices);
}

#endif
