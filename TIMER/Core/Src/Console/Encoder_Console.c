/*
 * Encoder_Console.c
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#include "OOP.h"
#include "OOP_Console.h"
#include "Encoder_Process.h"

#if defined(LED_MODE)

Encoder_st Encoder[4];

OOP const *consoles[] =
{
	&Encoder[0].super,	&Encoder[1].super,	&Encoder[2].super,	&Encoder[3].super
};

void User_ctor(infor_console* console)
{
	Encoder_buf_ctor(&Encoder[0], NULL, 1, 0, 0);
	Encoder_buf_ctor(&Encoder[1], NULL, 2, 0, 1);
	Encoder_buf_ctor(&Encoder[2], NULL, 3, 0, 2);
	Encoder_buf_ctor(&Encoder[3], NULL, 4, 0, 3);

	console->NumberOfDevices = sizeof(consoles)/sizeof(OOP*);
	console->num_encoder = sizeof(Encoder)/sizeof(Encoder[0]);
	console->pECD = Encoder;
	InitAll(consoles, console->NumberOfDevices);
}


#endif
