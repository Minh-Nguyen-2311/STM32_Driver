/*
 * USART_LCD.c
 *
 *  Created on: Jul 28, 2025
 *      Author: Admin
 */

#include "USART_LCD.h"

static uint32_t LCD_Init_(OOP const * const me);
static void LCD_Process_(OOP const * const me);

void LCD_buf_ctor(USART_LCD_st * const me, void *data, uint8_t ID_Signal,
		uint8_t mode, uint8_t num_lcd)
{
	static struct VirtualTable const vtbl = {
			/* vtbl of the UART class */
			&LCD_Init_,
			&LCD_Process_
	};

	OOP_ctor(&me->super, data); /* call superclass' ctor */

	me->super.vptr = &vtbl;  /* override the vptr */
	me->super.data = NULL;

	me->mLcd->Tx_Port = GPIOA;
	me->mLcd->Tx_pin = GPIO_PIN_9;
	me->mLcd->Rx_Port = GPIOA;
	me->mLcd->Rx_pin = GPIO_PIN_10;

	me->Nextion_predata[4] = 0;
	me->Nextion_currdata[4] = 0;

	me->ID_Signal = ID_Signal;
	me->mode = mode;
	me->num_lcd = num_lcd;
}

static uint32_t LCD_Init_(OOP const * const me)
{
	USART_LCD_st * const me_ = (USART_LCD_st *)me;
	(void)me_; //Avoid compiler error
	return 0;
}

static void LCD_Process_(OOP const * const me)
{
	USART_LCD_st * const _me = (USART_LCD_st *)me;
	if(_me->mode == 0)
	{
		switch(_me->num_lcd)
		{
		case 0:
			if(_me->Nextion_predata[_me->num_lcd] != _me->Nextion_currdata[_me->num_lcd])
			{
				Send2Gauge(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				Send2Num(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				_me->Nextion_predata[_me->num_lcd] = _me->Nextion_currdata[_me->num_lcd];
			}
			break;
		case 1:
			if(_me->Nextion_predata[_me->num_lcd] != _me->Nextion_currdata[_me->num_lcd])
			{
				Send2Gauge(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				Send2Num(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				_me->Nextion_predata[_me->num_lcd] = _me->Nextion_currdata[_me->num_lcd];
			}
			break;
		case 2:
			if(_me->Nextion_predata[_me->num_lcd] != _me->Nextion_currdata[_me->num_lcd])
			{
				Send2Gauge(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				Send2Num(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				_me->Nextion_predata[_me->num_lcd] = _me->Nextion_currdata[_me->num_lcd];
			}
			break;
		case 3:
			if(_me->Nextion_predata[_me->num_lcd] != _me->Nextion_currdata[_me->num_lcd])
			{
				Send2Gauge(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				Send2Num(&_me->mLcd, _me->Nextion_currdata[_me->num_lcd], _me->num_lcd);
				_me->Nextion_predata[_me->num_lcd] = _me->Nextion_currdata[_me->num_lcd];
			}
			break;
		}
	}
}
