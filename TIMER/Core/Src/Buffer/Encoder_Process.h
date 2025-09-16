/*
 * Encoder_Process.h
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_ENCODER_PROCESS_H_
#define SRC_BUFFER_ENCODER_PROCESS_H_

#include "OOP.h"
#include "main.h"
#include "tim.h"
#include "Encoder_Timer.h"

typedef struct
{
	OOP super;

	uint8_t ID_Signal;

	uint16_t Encoder_data[4];

	uint8_t mode;
	uint8_t num_encoder;

}Encoder_st;

#endif /* SRC_BUFFER_ENCODER_PROCESS_H_ */
