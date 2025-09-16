/*
 * Delay.h
 *
 *  Created on: Sep 7, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_DELAY_H_
#define SRC_BUFFER_DELAY_H_

#include "stm32f4xx_hal.h"
#include "main.h"

void Delay_us(__IO uint32_t nCount);
void Delay_ms(__IO uint32_t nCount);

#endif /* SRC_BUFFER_DELAY_H_ */
