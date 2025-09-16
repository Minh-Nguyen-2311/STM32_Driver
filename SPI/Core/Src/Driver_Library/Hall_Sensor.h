/*
 * SPI_Hall_Sensor.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_SPI_HALL_SENSOR_H_
#define SRC_BUFFER_SPI_HALL_SENSOR_H_

#include "main.h"
#include "stm32f4xx_hal.h"
#include "Delay.h"

typedef struct{
	GPIO_TypeDef *csPort;
	uint16_t csPin;
	SPI_HandleTypeDef *hspi;
	float prePulseCycle;
	float prePulseLinear;
	uint8_t u8Data[8];
}hallEncoder_st;

void hallEncoderInit(hallEncoder_st *mHall);
int32_t getPulseLinear(hallEncoder_st *mHall);
void setPulseZero(hallEncoder_st *mHall);
float getPulseCycle(hallEncoder_st *mHall);
float getReversePulseCycle(hallEncoder_st *mHall);

#endif /* SRC_BUFFER_SPI_HALL_SENSOR_H_ */
