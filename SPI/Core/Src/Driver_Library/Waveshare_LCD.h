/*
 * SPI_LCD.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_SPI_LCD_H_
#define SRC_BUFFER_SPI_LCD_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include "main.h"
#include "gpio.h"
#include "fonts.h"
#include "Delay.h"

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

#define LCD_1IN14_HEIGHT 	240
#define LCD_1IN14_WIDTH 	135

#define LCD_2IN_WIDTH   	240
#define LCD_2IN_HEIGHT  	320

#define HORIZONTAL 0
#define VERTICAL   1

#define Debug(__info,...) printf("Debug : " __info,##__VA_ARGS__)

typedef struct{
	uint16_t WIDTH;
	uint16_t HEIGHT;
	uint8_t SCAN_DIR;
}LCD_1IN14_ATTRIBUTES;
extern LCD_1IN14_ATTRIBUTES LCD_1IN14;

typedef struct{
	uint16_t WIDTH;
	uint16_t HEIGHT;
	uint8_t SCAN_DIR;
}LCD_2IN_ATTRIBUTES;
extern LCD_2IN_ATTRIBUTES LCD_2IN;

typedef struct{
	uint16_t x_start;
	uint16_t y_start;
	uint16_t x_stop;
	uint16_t y_stop;
}Area;

typedef struct{
	SPI_HandleTypeDef *hspi;

	GPIO_TypeDef *CS_Port;
	uint16_t 	  CS_Pin;

	GPIO_TypeDef *RST_Port;
	uint16_t 	  RST_PIN;

	GPIO_TypeDef *DC_Port;
	uint16_t 	  DC_PIN;
}WaveShareLCD_st;

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

typedef struct {
    volatile UBYTE *Image;
    volatile UWORD Width;
    volatile UWORD Height;
    volatile UWORD WidthMemory;
    volatile UWORD HeightMemory;
    volatile UWORD Color;
    volatile UWORD Rotate;
    volatile UWORD Mirror;
    volatile UWORD WidthByte;
    volatile UWORD HeightByte;
} PAINT;
extern volatile PAINT Paint;

/**
 * Display rotate
**/
#define ROTATE_0            0
#define ROTATE_90           90
#define ROTATE_180          180
#define ROTATE_270          270

/**
 * Display Flip
**/
typedef enum {
    MIRROR_NONE  = 0x00,
    MIRROR_HORIZONTAL = 0x01,
    MIRROR_VERTICAL = 0x02,
    MIRROR_ORIGIN = 0x03,
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT MIRROR_NONE

/**
 * image color
**/

#define WHITE					0xFFFF
#define BLACK					0x0000
#define BLUE 					0x001F
#define BRED 					0XF81F
#define GRED 					0XFFE0
#define GBLUE					0X07FF
#define RED  					0xF800
#define MAGENTA					0xF81F
#define GREEN					0x07E0
#define CYAN 					0x7FFF
#define YELLOW					0xFFE0
#define BROWN					0XBC40
#define DARKBROWN				0XC443 //Correct color
#define BRRED					0XFC07
#define GRAY 					0X8430
#define DARKBLUE				0X01CF
#define LIGHTBLUE				0X7D7C
#define GRAYBLUE      			0X5458
#define LIGHTGREEN    			0X841F
#define LGRAY 			  		0XC618
#define LGRAYBLUE     			0XA651
#define LBBLUE        			0X2B12

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

#define ST7789_VCSRADD 		0x37
#define ST7789_VSCRDEF		0x33
/**
 * The size of the point
**/
typedef enum {
    DOT_PIXEL_1X1  = 1,		// 1 x 1
    DOT_PIXEL_2X2  , 		// 2 X 2
    DOT_PIXEL_3X3  ,		// 3 X 3
    DOT_PIXEL_4X4  ,		// 4 X 4
    DOT_PIXEL_5X5  , 		// 5 X 5
    DOT_PIXEL_6X6  , 		// 6 X 6
    DOT_PIXEL_7X7  , 		// 7 X 7
    DOT_PIXEL_8X8  , 		// 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  DOT_PIXEL_1X1  //Default dot pilex

/**
 * Point size fill style
**/
typedef enum {
    DOT_FILL_AROUND  = 1,		// dot pixel 1 x 1
    DOT_FILL_RIGHTUP  , 		// dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  DOT_FILL_AROUND  //Default dot pilex

/**
 * Line style, solid or dashed
**/
typedef enum {
    LINE_STYLE_SOLID = 0,
    LINE_STYLE_DOTTED,
} LINE_STYLE;

/**
 * Whether the graphic is filled
**/
typedef enum {
    DRAW_FILL_EMPTY = 0,
    DRAW_FILL_FULL,
} DRAW_FILL;

/**
 * Custom structure of a time attribute
**/
typedef struct {
    UWORD Year;  //0000
    UBYTE  Month; //1 - 12
    UBYTE  Day;   //1 - 30
    UBYTE  Hour;  //0 - 23
    UBYTE  Min;   //0 - 59
    UBYTE  Sec;   //0 - 59
} PAINT_TIME;
extern PAINT_TIME sPaint_time;

//init and Clear
void Paint_NewImage(WaveShareLCD_st *mWaveshare, UWORD Width,
		UWORD Height, UWORD Rotate, UWORD Color);
void Paint_SetRotate(WaveShareLCD_st *mWaveshare, UWORD Rotate);
void Paint_SetMirroring(WaveShareLCD_st *mWaveshare, UBYTE mirror);
void Paint_SetPixel(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, UWORD Color);
void Paint_ClearWindows(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color);

//Drawing
void Paint_DrawPoint(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, UWORD Color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_FillWay);
void Paint_DrawLine(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style);
void Paint_DrawRectangle(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Filled);
void Paint_DrawCircle(WaveShareLCD_st *mWaveshare, UWORD X_Center, UWORD Y_Center, UWORD Radius, UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill);

//pic
void Paint_DrawImage(WaveShareLCD_st *mWaveshare, const unsigned char *image,UWORD Startx, UWORD Starty,UWORD Endx, UWORD Endy);

//minhnd47
void ST77xx_WriteInteger_1Value(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, int32_t Number, FontDef* Font,
		UWORD Color_Background, UWORD Color_Foreground);
void ST77xx_WriteInteger_2Values(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, int32_t Number, FontDef* Font,
		UWORD Color_Background, UWORD Color_Foreground);
void ST77xx_WriteString(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, const char * pString, FontDef* Font,
		UWORD Color_Background, UWORD Color_Foreground);
void ST77xx_WriteChar(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, const char Acsii_Char, FontDef* Font,
		UWORD Color_Background, UWORD Color_Foreground);
void ST77xx_DrawHorizontalLine(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Xend, UWORD Y, UWORD Color,
		DOT_PIXEL Line_width, LINE_STYLE Line_Style);
void ST77xx_DrawVerticalLine(WaveShareLCD_st *mWaveshare, UWORD Ystart, UWORD Yend, UWORD X, UWORD Color,
		DOT_PIXEL Line_width, LINE_STYLE Line_Style);
void ST77xx_DeleteNumber(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
		UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Filled);

void LCD_1IN14_Reset(WaveShareLCD_st *mWaveshare);
void LCD_1IN14_SendCommand(WaveShareLCD_st *mWaveshare, uint8_t reg);
void LCD_1IN14_SendData_8Bit(WaveShareLCD_st *mWaveshare, uint8_t dat);
void LCD_1IN14_SendData_16Bit(WaveShareLCD_st *mWaveshare, uint16_t dat);
void LCD_1IN14_SetAttributes(WaveShareLCD_st *mWaveshare, uint8_t Scan_dir);
void LCD_1IN14_InitReg(WaveShareLCD_st *mWaveshare);
void LCD_1IN14_Init(WaveShareLCD_st *mWaveshare, uint8_t Scan_dir);
void LCD_1IN14_SetWindows(WaveShareLCD_st *mWaveshare, uint16_t Xstart,
				uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void LCD_1IN14_Clear(WaveShareLCD_st *mWaveshare, uint16_t Color);
void LCD_1IN14_Display(WaveShareLCD_st *mWaveshare, uint16_t *Image);
void LCD_1IN14_DisplayWindows(WaveShareLCD_st *mWaveshare, uint16_t Xstart,
		uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image);
void LCD_1IN14_DrawPaint(WaveShareLCD_st *mWaveshare, uint16_t x, uint16_t y, uint16_t Color);
void LCD_1IN14_SetScrollArea(WaveShareLCD_st *mWaveshare, uint8_t tfa, uint8_t bfa);
void LCD_1IN14_SetScroll(WaveShareLCD_st *mWaveshare, uint16_t vsp);
void LCD_1IN14_InitScreen_2Values(WaveShareLCD_st *mWaveshare);
void LCD_1IN14_InitScreen_6Values(WaveShareLCD_st *mWaveshare);
void Display_1IN14_Screen_2Values(WaveShareLCD_st *mWaveshare, uint8_t val1, uint8_t val2);
void Display_1IN14_Screen_6Values(WaveShareLCD_st *mWaveshare, uint8_t val1, uint8_t val2,
									uint8_t val3, uint8_t val4, uint8_t val5, uint8_t val6);

#endif /* SRC_BUFFER_SPI_LCD_H_ */
