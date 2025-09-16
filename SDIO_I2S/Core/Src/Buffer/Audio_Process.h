/*
 * Audio_Process.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_AUDIO_PROCESS_H_
#define SRC_BUFFER_AUDIO_PROCESS_H_

#include "OOP.h"
#include "main.h"
#include "Audio.h"

typedef struct
{
	OOP super;

	uint8_t ID_Signal;
	uint8_t mode; //0: OFF audio; 1: audio no.1; 2: audio no.2
}Audio_st;

void Audio_buf_ctor(Audio_st * const me, void *data, uint8_t ID_Signal,
		uint8_t mode);

#endif /* SRC_BUFFER_AUDIO_PROCESS_H_ */
