/*
 * Audio_Console.c
 *
 *  Created on: Aug 17, 2025
 *      Author: Admin
 */

#include "OOP.h"
#include "OOP_Console.h"
#include "Audio_Process.h"

#define NUM_AUDIO 1

Audio_st Audio[NUM_AUDIO];

OOP const *consoles[] =
{
	&Audio[0].super
};

void User_ctor(infor_console* console)
{
	Audio_buf_ctor(&Audio[0], NULL, 1, 0);

	console->NumberOfDevices = sizeof(consoles)/sizeof(OOP*);

	console->num_audio = NUM_AUDIO;
	console->pAudio = Audio;
	InitAll(consoles, console->NumberOfDevices);
}
