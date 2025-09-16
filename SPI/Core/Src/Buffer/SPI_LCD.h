#ifndef SRC_BUFFER_LCD_BUF_H_
#define SRC_BUFFER_LCD_BUF_H_

#include "Waveshare_LCD.h"
#include "OOP.h"
#include "main.h"
#include <string.h>
#include "Delay.h"

#define DEV_RST_PORT     	GPIOA
#define DEV_RST_PIN		 	GPIO_PIN_0

#define DEV_DC_PORT      	GPIOA
#define DEV_DC_PIN		 	GPIO_PIN_1

#define DEV_CS1_PORT		GPIOB
#define DEV_CS1_PIN			GPIO_PIN_11

#define DEV_CS2_PORT		GPIOB
#define DEV_CS2_PIN			GPIO_PIN_12

#define DEV_CS3_PORT		GPIOA
#define DEV_CS3_PIN			GPIO_PIN_11

#define DEV_CS4_PORT		GPIOA
#define DEV_CS4_PIN			GPIO_PIN_12

enum{
	CHANNEL_1 = 0,
	CHANNEL_2,
	CHANNEL_3,
	CHANNEL_4,
};

typedef struct{
	OOP super;

	uint8_t ID_tinhieu;
	struct{
		uint8_t mode;
		union{
			uint8_t byte[4];
			float inputValue;
		}float_value;
	}format_lcd;
	uint8_t num_lcd;
	uint8_t len;

	uint32_t Waveshare_predata;
	uint32_t Waveshare_currdata;

	uint8_t ValueWaveShare[6];

	WaveShareLCD_st mWaveShare;
	uint8_t flagWaveShare;
}LCD_buf_st;

void LCD_buf_ctor(LCD_buf_st * const me, void *data, uint8_t ID_tinhieu, uint8_t mode, uint8_t number_lcd);
#endif
