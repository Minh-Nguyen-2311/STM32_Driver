/*
 * DAC_Console.c
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#include "OOP.h"
#include "OOP_Console.h"
#include "DAC_Process.h"

#define NUM_DAC 4

DAC_st Dac[NUM_DAC];

OOP const *consoles[] =
{
	&Dac[0].super,	&Dac[1].super,	&Dac[2].super,	&Dac[3].super
};

void User_ctor(infor_console* console)
{
	DAC_buf_ctor(&Dac[0], NULL, 1, 0);
	DAC_buf_ctor(&Dac[1], NULL, 2, 1);
	DAC_buf_ctor(&Dac[2], NULL, 3, 2);
	DAC_buf_ctor(&Dac[3], NULL, 4, 3);

	console->NumberOfDevices = sizeof(consoles)/sizeof(OOP*);
	console->num_dac = NUM_DAC;
	console->pDAC = Dac;
	InitAll(consoles, console->NumberOfDevices);
}
