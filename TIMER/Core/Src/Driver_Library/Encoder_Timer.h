/*
 * Encoder_Timer.h
 *
 *  Created on: Aug 5, 2025
 *      Author: Admin
 */

#ifndef SRC_DRIVER_LIBRARY_ENCODER_TIMER_H_
#define SRC_DRIVER_LIBRARY_ENCODER_TIMER_H_

#include "main.h"
#include "stm32f4xx_hal.h"

void Start_ECD_TIM(TIM_HandleTypeDef *htim);

void Stop_ECD_TIM(TIM_HandleTypeDef *htim);

int16_t Get_ECD_TIM_Data(TIM_HandleTypeDef *htim);

#endif /* SRC_DRIVER_LIBRARY_ENCODER_TIMER_H_ */
