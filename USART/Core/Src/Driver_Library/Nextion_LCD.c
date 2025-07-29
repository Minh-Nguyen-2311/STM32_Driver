/*
 * Nextion_LCD.c
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#include "Nextion_LCD.h"
#include "usart.h"

uint8_t cmd_end[3] = {0xFF,0xFF,0xFF};

void Send2Gauge(LCD_st *mLcd, uint16_t value, uint8_t lcd_index)
{
	char buf[100];
	value += 321;
	if(value >= 360) value -= 360;
	int len = sprintf(buf, "z%d.val=%u", lcd_index, value);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, cmd_end, 3, HAL_MAX_DELAY);
}

void Send2Num(LCD_st *mLcd, uint16_t value, uint8_t lcd_index)
{
	char buf[30];
	int len = sprintf(buf, "z%d.val=%u", lcd_index, value);
	HAL_UART_Transmit(&huart1, (uint8_t*)buf, len, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, cmd_end, 3, HAL_MAX_DELAY);
}

