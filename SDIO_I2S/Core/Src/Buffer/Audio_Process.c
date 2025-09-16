/*
 * Audio_Process.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#include "Audio_Process.h"

static uint32_t Audio_Init_(OOP const * const me);
static void Audio_Process_(OOP const * const me);

void Audio_buf_ctor(Audio_st * const me, void *data, uint8_t ID_Signal,
		uint8_t mode)
{
	static struct VirtualTable const vtbl = {
				/* vtbl of the UART class */
				&Audio_Init_,
				&Audio_Process_
		};

		OOP_ctor(&me->super, data); /* call superclass' ctor */

		me->super.vptr = &vtbl;  /* override the vptr */
		me->super.data = NULL;

		me->ID_Signal = ID_Signal;
		me->mode = mode;
}

static uint32_t Audio_Init_(OOP const * const me)
{
	Audio_st * const me_ = (Audio_st *)me;
	Wav_Init();
	(void)me_;
	return 0;
}

static void Audio_Process_(OOP const * const me)
{
	Audio_st * const _me = (Audio_st *)me;
	switch(_me->mode)
	{
	case 0:
		StopWavFile();
		break;
	case 1:
		PlayWavFile("0:\Audio1.wav");
		break;
	case 2:
		PlayWavFile("0:\Audio2.wav");
		break;
	}
}
