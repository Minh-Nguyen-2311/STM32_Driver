/*
 * ADC_Filter.h
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#ifndef SRC_DRIVER_LIBRARY_ADC_FILTER_H_
#define SRC_DRIVER_LIBRARY_ADC_FILTER_H_

#include "main.h"
#include "math.h"
#include "stm32f4xx_hal.h"

//Low-pass filter
#define ALPHA 0.9
//Kalman filter
typedef struct {
    double x; // estimate
    double P; // error covariance
    double Q; // process noise
    double R; // measurement noise
} kalman_t;
//Hysteresis
#define EMA_ALPHA_NUM	1	//a=1/1000
#define EMA_ALPHA_DEN	100
#define HYST			1.5f
#define SCALE			10000

uint16_t hysteresis_round(uint16_t currentInt, float filtVal, float hyst);
extern ADC_HandleTypeDef hadc1;

void MX_ADC1_Init(void);

void Start_ADC_DMA(ADC_HandleTypeDef *hadc, uint8_t);

#endif /* SRC_DRIVER_LIBRARY_ADC_FILTER_H_ */
