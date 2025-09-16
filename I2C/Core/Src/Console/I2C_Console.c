/*
 * I2C_Console.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#include "OOP.h"
#include "OOP_Console.h"
#include "I2C_DS3231.h"

#define NUM_I2C	4

DS3231_st Time[NUM_I2C];

OOP const *consoles[] =
{
	&Time[0].super,	&Time[1].super,	&Time[2].super,	&Time[3].super
};

void User_ctor(infor_console* console)
{
	I2C_buf_ctor(&Time[0], NULL, 1, 0);
	I2C_buf_ctor(&Time[1], NULL, 2, 1);
	I2C_buf_ctor(&Time[2], NULL, 3, 2);
	I2C_buf_ctor(&Time[3], NULL, 4, 3);

	console->NumberOfDevices = sizeof(consoles)/sizeof(OOP*);
	console->num_i2c = NUM_I2C;
	console->pTime = Time;
	InitAll(consoles, console->NumberOfDevices);
}
