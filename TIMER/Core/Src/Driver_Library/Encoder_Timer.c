/*
 * Encoder_Timer.c
 *
 *  Created on: Aug 5, 2025
 *      Author: Admin
 */

#include "Encoder_Timer.h"

void Start_ECD_TIM(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
}
/**
  * @brief  The application entry point.
  * @retval int
  */
void Stop_ECD_TIM(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Encoder_Stop(htim, TIM_CHANNEL_ALL);
}
/**
  * @brief
  * @retval
  */
int16_t Get_ECD_TIM_Data(TIM_HandleTypeDef *htim)
{
	uint32_t pulse = 0;
	pulse = htim->Instance->CNT;
	return (int32_t) pulse;
}
