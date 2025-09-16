/*
 * SPI_LCD_Console.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#include "OOP.h"
#include "OOP_Console.h"
#include "SPI_Hall_Sensor.h"
#include "SPI_LCD.h"

#define NUM_LCD	4
#define NUM_ECD	4

LCD_buf_st LCD[NUM_LCD];
ECD_buf_st ECD[NUM_ECD];

OOP const *consoles[] =
{
	&LCD[0].super,	&LCD[1].super,	&LCD[2].super,	&LCD[3].super,

	&ECD[0].super,	&ECD[1].super,	&ECD[2].super,	&ECD[3].super
};

void User_ctor(infor_console* console)
{
	LCD_buf_ctor(&LCD[0], NULL, 1, 0, 0);
	LCD_buf_ctor(&LCD[1], NULL, 2, 0, 1);
	LCD_buf_ctor(&LCD[2], NULL, 3, 0, 2);
	LCD_buf_ctor(&LCD[3], NULL, 4, 0, 3);

	ECD_buf_ctor(&ECD[0], NULL, 1, 5, 0);
	ECD_buf_ctor(&ECD[1], NULL, 1, 6, 1);
	ECD_buf_ctor(&ECD[2], NULL, 1, 7, 2);
	ECD_buf_ctor(&ECD[3], NULL, 1, 8, 3);

	console->NumberOfDevices = sizeof(consoles)/sizeof(OOP*);
	console->num_lcd = NUM_LCD;
	console->num_ecd = NUM_ECD;

	console->pLCD = LCD;
	console->pECD = ECD;
	InitAll(consoles, console->NumberOfDevices);
}
