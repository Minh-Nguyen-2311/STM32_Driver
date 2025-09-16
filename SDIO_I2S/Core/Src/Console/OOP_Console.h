/*
 * OOP_Console.h
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#ifndef INC_OOP_CONSOLE_H_
#define INC_OOP_CONSOLE_H_

#include "Audio_Process.h"

void User_Loop(void);

typedef struct infor_console_
{
	uint8_t NumberOfDevices;
	uint8_t num_audio;
	Audio_st *pAudio;
}infor_console;

void User_ctor(infor_console*);

#endif /* INC_OOP_CONSOLE_H_ */
