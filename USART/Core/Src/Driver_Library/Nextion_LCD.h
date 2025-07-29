/*
 * Nextion_LCD.h
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#ifndef SRC_DRIVER_LIBRARY_NEXTION_LCD_H_
#define SRC_DRIVER_LIBRARY_NEXTION_LCD_H_

#include "main.h"
#include "stdio.h"

typedef struct
{
	GPIO_TypeDef *Tx_Port;
	uint16_t 	  Tx_pin;
	GPIO_TypeDef *Rx_Port;
	uint16_t 	  Rx_pin;
}LCD_st;

void Send2Gauge(LCD_st *mLcd, uint16_t value, uint8_t lcd_index);
void Send2Num(LCD_st *mLcd, uint16_t value, uint8_t lcd_index);

#endif /* SRC_DRIVER_LIBRARY_NEXTION_LCD_H_ */
