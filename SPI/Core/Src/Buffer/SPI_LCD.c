#include "SPI_LCD.h"

WaveShareLCD_st waveshare1 = {&hspi2, DEV_CS1_PORT, DEV_CS1_PIN, DEV_RST_PORT, DEV_RST_PIN, DEV_DC_PORT, DEV_DC_PIN};
WaveShareLCD_st waveshare2 = {&hspi2, DEV_CS2_PORT, DEV_CS2_PIN, DEV_RST_PORT, DEV_RST_PIN, DEV_DC_PORT, DEV_DC_PIN};
WaveShareLCD_st waveshare3 = {&hspi3, DEV_CS3_PORT, DEV_CS3_PIN, DEV_RST_PORT, DEV_RST_PIN, DEV_DC_PORT, DEV_DC_PIN};
WaveShareLCD_st waveshare4 = {&hspi3, DEV_CS4_PORT, DEV_CS4_PIN, DEV_RST_PORT, DEV_RST_PIN, DEV_DC_PORT, DEV_DC_PIN};

static uint32_t LCD_Init_(OOP const * const me);
static void LCD_Process_(OOP const * const me);

void LCD_buf_ctor(LCD_buf_st * const me, void *data, uint8_t ID_tinhieu, uint8_t mode, uint8_t number_lcd)
{
	static struct VirtualTable const vtbl = {
			/* vtbl of the ADC class */
			&LCD_Init_,
			&LCD_Process_
	};

	OOP_ctor(&me->super, data); /* call superclass' ctor */

	me->super.vptr = &vtbl;  /* override the vptr */
	me->super.data = NULL;

	me->ID_tinhieu = ID_tinhieu;
	me->len = 8;
	me->format_lcd.mode = mode;
	me->num_lcd = number_lcd;

	me->Waveshare_predata = 0;
	me->Waveshare_currdata = 0;

	me->flagWaveShare = 0;
}

static uint32_t LCD_Init_(OOP const * const me) {
	LCD_buf_st * const me_ = (LCD_buf_st *)me;
	if(me_->format_lcd.mode == 0)
	{
		switch(me_->num_lcd)
		{
		case 0:
			LCD_1IN14_InitScreen_2Values(&waveshare1);
			break;
		case 1:
			LCD_1IN14_InitScreen_2Values(&waveshare2);
			break;
		case 2:
			LCD_1IN14_InitScreen_2Values(&waveshare3);
			break;
		case 3:
			LCD_1IN14_InitScreen_2Values(&waveshare4);
			break;
		}
	}
	(void)me_;
	return 0;
}

static void LCD_Process_(OOP const * const me)
{
	LCD_buf_st * const _me = (LCD_buf_st *)me;
	if(_me->format_lcd.mode == 0) //SPI LCD
	{
		switch(_me->num_lcd)
		{
		case 0:
			Display_1IN14_Screen_2Values(&waveshare1, _me->ValueWaveShare[0], _me->ValueWaveShare[1]);
			break;
		case 1:
			Display_1IN14_Screen_2Values(&waveshare2, _me->ValueWaveShare[0], _me->ValueWaveShare[1]);
			break;
		case 2:
			Display_1IN14_Screen_2Values(&waveshare3, _me->ValueWaveShare[0], _me->ValueWaveShare[1]);
			break;
		case 3:
			Display_1IN14_Screen_2Values(&waveshare4, _me->ValueWaveShare[0], _me->ValueWaveShare[1]);
			break;
		}
	}
}
