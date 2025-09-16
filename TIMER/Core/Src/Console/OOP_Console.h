/*
 * OOP_Console.h
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#ifndef INC_OOP_CONSOLE_H_
#define INC_OOP_CONSOLE_H_

#include "Encoder_Process.h"
#include "LED_Process.h"

void User_Loop(void);

#define ENCODER_MODE

typedef struct infor_console_
{
	uint8_t NumberOfDevices;

	uint8_t num_encoder;
	uint8_t num_led;

	Encoder_st *pECD;
	ChipLed *pLED;
}infor_console;

void User_ctor(infor_console*);

#endif /* INC_OOP_CONSOLE_H_ */
