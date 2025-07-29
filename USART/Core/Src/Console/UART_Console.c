/*
 * UART_Console.c
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#include "OOP.h"
#include "OOP_Console.h"
#include "USART_LCD.h"

#define NUM_LCD	4

USART_LCD_st LCD[NUM_LCD];

OOP const *consoles[] =
{
	&LCD[0].super,	&LCD[1].super,	&LCD[2].super,	&LCD[3].super
};

void User_ctor(infor_console* console)
{
	LCD_buf_ctor(&LCD[0], NULL, 0, 0, 0);
	LCD_buf_ctor(&LCD[1], NULL, 0, 0, 1);
	LCD_buf_ctor(&LCD[2], NULL, 0, 0, 2);
	LCD_buf_ctor(&LCD[3], NULL, 0, 0, 3);

	console->NumberOfDevices = sizeof(consoles)/sizeof(OOP*);
	console->num_lcd = NUM_LCD;
	console->pLCD = LCD;
	InitAll(consoles, console->NumberOfDevices);
}
