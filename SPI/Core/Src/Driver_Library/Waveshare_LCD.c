/*
 * SPI_LCD.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#include "Waveshare_LCD.h"

static void RST_High(WaveShareLCD_st *mWaveshare){
	HAL_GPIO_WritePin(mWaveshare->RST_Port, mWaveshare->RST_PIN, GPIO_PIN_SET);
}
static void RST_Low(WaveShareLCD_st *mWaveshare){
	HAL_GPIO_WritePin(mWaveshare->RST_Port, mWaveshare->RST_PIN, GPIO_PIN_RESET);
}
static void CS_High(WaveShareLCD_st *mWaveshare){
	HAL_GPIO_WritePin(mWaveshare->CS_Port, mWaveshare->CS_Pin, GPIO_PIN_SET);
}
static void CS_Low(WaveShareLCD_st *mWaveshare){
	HAL_GPIO_WritePin(mWaveshare->CS_Port, mWaveshare->CS_Pin, GPIO_PIN_RESET);
}
static void DC_High(WaveShareLCD_st *mWaveshare){
	HAL_GPIO_WritePin(mWaveshare->DC_Port, mWaveshare->DC_PIN, GPIO_PIN_SET);
}
static void DC_Low(WaveShareLCD_st *mWaveshare){
	HAL_GPIO_WritePin(mWaveshare->DC_Port, mWaveshare->DC_PIN, GPIO_PIN_RESET);
}
static void DEV_SPI_WRITE(WaveShareLCD_st *mWaveshare, uint8_t _dat)
{
	HAL_SPI_Transmit(mWaveshare->hspi, &_dat, 1, HAL_MAX_DELAY);
	//HAL_SPI_Transmit_DMA(mWaveshare->hspi, &_dat, sizeof(_dat));
}
static void DEV_Module_Init(WaveShareLCD_st *mWaveshare){
	DC_High(mWaveshare);
	CS_High(mWaveshare);
	RST_High(mWaveshare);
}
/*-------------------------------------------------------------------*/
volatile PAINT Paint;
void (*DISPLAY)(WaveShareLCD_st *mWaveshare,UWORD,UWORD,UWORD);
void (*CLEAR)(WaveShareLCD_st *mWaveshare,UWORD);
char words[16]; //minhnd47

Area area1 = {80, 43, 93, 90};
Area area2 = {173, 43, 196, 90};
Area area3 = {110, 10, 220, 125};
Area area4 = {173, -15, 196, 90};
Area area5 = {173, 100, 196, 90};
/******************************************************************************
function:	Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    Height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/
void Paint_NewImage(WaveShareLCD_st *mWaveshare, UWORD Width, UWORD Height, UWORD Rotate, UWORD Color)
{
    Paint.WidthMemory = Width;
    Paint.HeightMemory = Height;
    Paint.Color = Color;
    Paint.WidthByte = Width;
    Paint.HeightByte = Height;
    printf("WidthByte = %d, HeightByte = %d\r\n", Paint.WidthByte, Paint.HeightByte);

    Paint.Rotate = Rotate;
    Paint.Mirror = MIRROR_NONE;

    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) {
        Paint.Width = Width;
        Paint.Height = Height;
    } else {
        Paint.Width = Height;
        Paint.Height = Width;
    }
}
/******************************************************************************
function:	Select Image Rotate
parameter:
    Rotate   :   0,90,180,270
******************************************************************************/
void Paint_SetRotate(WaveShareLCD_st *mWaveshare, UWORD Rotate)
{
    if(Rotate == ROTATE_0 || Rotate == ROTATE_90 || Rotate == ROTATE_180 || Rotate == ROTATE_270) {
        Debug("Set image Rotate %d\r\n", Rotate);
        Paint.Rotate = Rotate;
    } else {
        Debug("rotate = 0, 90, 180, 270\r\n");
      //  exit(0);
    }
}

/******************************************************************************
function:	Select Image mirror
parameter:
    mirror   :       Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void Paint_SetMirroring(WaveShareLCD_st *mWaveshare, UBYTE mirror)
{
    if(mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL ||
        mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) {
        Debug("mirror image x:%s, y:%s\r\n",(mirror & 0x01)? "mirror":"none", ((mirror >> 1) & 0x01)? "mirror":"none");
        Paint.Mirror = mirror;
    } else {
        Debug("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, \
        MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
//exit(0);
    }
}

/******************************************************************************
function:	Draw Pixels
parameter:
    Xpoint  :   At point X
    Ypoint  :   At point Y
    Color   :   Painted colors
******************************************************************************/
void Paint_SetPixel(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, UWORD Color)
{
    if(Xpoint > Paint.Width || Ypoint > Paint.Height){
        Debug("Exceeding display boundaries\r\n");
        return;
    }
    UWORD X, Y;

    switch(Paint.Rotate) {
    case 0:
        X = Xpoint;
        Y = Ypoint;
        break;
    case 90:
        X = Paint.WidthMemory - Ypoint - 1;
        Y = Xpoint;
        break;
    case 180:
        X = Paint.WidthMemory - Xpoint - 1;
        Y = Paint.HeightMemory - Ypoint - 1;
        break;
    case 270:
        X = Ypoint;
        Y = Paint.HeightMemory - Xpoint - 1;
        break;

    default:
        return;
    }

    switch(Paint.Mirror) {
    case MIRROR_NONE:
        break;
    case MIRROR_HORIZONTAL:
        X = Paint.WidthMemory - X - 1;
        break;
    case MIRROR_VERTICAL:
        Y = Paint.HeightMemory - Y - 1;
        break;
    case MIRROR_ORIGIN:
        X = Paint.WidthMemory - X - 1;
        Y = Paint.HeightMemory - Y - 1;
        break;
    default:
        return;
    }

    if(X > Paint.WidthMemory || Y > Paint.HeightMemory){
        Debug("Exceeding display boundaries\r\n");
        return;
    }
#if defined(AK176_HTDD_02) || defined(AK176_HTDD_04) || defined(KACY)
    LCD_1IN14_DrawPaint(mWaveshare, X, Y, Color);
#endif
#if defined(MAYTT_P774_02)
    LCD_2IN_DrawPaint(mWaveshare, X, Y, Color);
#endif
}
/******************************************************************************
function:	Clear the color of the picture
parameter:
    Color   :   Painted colors
******************************************************************************/
void Paint_Clear(WaveShareLCD_st *mWaveshare, UWORD Color)
{
	CLEAR(mWaveshare,Color);
}

/******************************************************************************
function:	Clear the color of a window
parameter:
    Xstart :   x starting point
    Ystart :   Y starting point
    Xend   :   x end point
    Yend   :   y end point
******************************************************************************/
void Paint_ClearWindows(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, UWORD Color)
{
    UWORD X, Y;
    for (Y = Ystart; Y < Yend; Y++) {
        for (X = Xstart; X < Xend; X++) {//8 pixel =  1 byte
            Paint_SetPixel(mWaveshare, X, Y, Color);
        }
    }
}

/******************************************************************************
function:	Draw Point(Xpoint, Ypoint) Fill the color
parameter:
    Xpoint		:   The Xpoint coordinate of the point
    Ypoint		:   The Ypoint coordinate of the point
    Color		:   Set color
    Dot_Pixel	:	point size
******************************************************************************/
void Paint_DrawPoint(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, UWORD Color,
                      DOT_PIXEL Dot_Pixel,DOT_STYLE Dot_FillWay)
{
    if (Xpoint > Paint.Width || Ypoint > Paint.Height) {
        Debug("Paint_DrawPoint Input exceeds the normal display range\r\n");
        return;
    }

    int16_t XDir_Num , YDir_Num;
    if (Dot_FillWay == DOT_FILL_AROUND) {
        for (XDir_Num = 0; XDir_Num < 2*Dot_Pixel - 1; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num < 2 * Dot_Pixel - 1; YDir_Num++) {
                if(Xpoint + XDir_Num - Dot_Pixel < 0 || Ypoint + YDir_Num - Dot_Pixel < 0)
                    break;
                // printf("x = %d, y = %d\r\n", Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel);
                Paint_SetPixel(mWaveshare, Xpoint + XDir_Num - Dot_Pixel, Ypoint + YDir_Num - Dot_Pixel, Color);
            }
        }
    } else {
        for (XDir_Num = 0; XDir_Num <  Dot_Pixel; XDir_Num++) {
            for (YDir_Num = 0; YDir_Num <  Dot_Pixel; YDir_Num++) {
                Paint_SetPixel(mWaveshare, Xpoint + XDir_Num - 1, Ypoint + YDir_Num - 1, Color);
            }
        }
    }
}

/******************************************************************************
function:	Draw a line of arbitrary slope
parameter:
    Xstart ：Starting Xpoint point coordinates
    Ystart ：Starting Xpoint point coordinates
    Xend   ：End point Xpoint coordinate
    Yend   ：End point Ypoint coordinate
    Color  ：The color of the line segment
******************************************************************************/
void Paint_DrawLine(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                    UWORD Color, DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
    if (Xstart > Paint.Width || Ystart > Paint.Height ||
        Xend > Paint.Width || Yend > Paint.Height) {
        Debug("Paint_DrawLine Input exceeds the normal display range\r\n");
        return;
    }

    UWORD Xpoint = Xstart;
    UWORD Ypoint = Ystart;
    int dx = (int)Xend - (int)Xstart >= 0 ? Xend - Xstart : Xstart - Xend;
    int dy = (int)Yend - (int)Ystart <= 0 ? Yend - Ystart : Ystart - Yend;

    // Increment direction, 1 is positive, -1 is counter;
    int XAddway = Xstart < Xend ? 1 : -1;
    int YAddway = Ystart < Yend ? 1 : -1;

    //Cumulative error
    int Esp = dx + dy;
    char Dotted_Len = 0;

    for (;;) {
        Dotted_Len++;
        //Painted dotted line, 2 point is really virtual
        if (Line_Style == LINE_STYLE_DOTTED && Dotted_Len % 3 == 0) {
            //Debug("LINE_DOTTED\r\n");
            Paint_DrawPoint(mWaveshare, Xpoint, Ypoint, IMAGE_BACKGROUND, Line_width, DOT_STYLE_DFT);
            Dotted_Len = 0;
        } else {
            Paint_DrawPoint(mWaveshare, Xpoint, Ypoint, Color, Line_width, DOT_STYLE_DFT);
        }
        if (2 * Esp >= dy) {
            if (Xpoint == Xend)
                break;
            Esp += dy;
            Xpoint += XAddway;
        }
        if (2 * Esp <= dx) {
            if (Ypoint == Yend)
                break;
            Esp += dx;
            Ypoint += YAddway;
        }
    }
}

/******************************************************************************
function:	Draw a rectangle
parameter:
    Xstart ：Rectangular  Starting Xpoint point coordinates
    Ystart ：Rectangular  Starting Xpoint point coordinates
    Xend   ：Rectangular  End point Xpoint coordinate
    Yend   ：Rectangular  End point Ypoint coordinate
    Color  ：The color of the Rectangular segment
    Filled : Whether it is filled--- 1 solid 0：empty
******************************************************************************/
void Paint_DrawRectangle(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
                          UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Filled )
{
    if (Xstart > Paint.Width || Ystart > Paint.Height ||
        Xend > Paint.Width || Yend > Paint.Height) {
        Debug("Input exceeds the normal display range\r\n");
        return;
    }

    if (Filled) {
        UWORD Ypoint;
        for(Ypoint = Ystart; Ypoint < Yend; Ypoint++){
        	ST77xx_DrawHorizontalLine(mWaveshare, Xstart, Xend, Ypoint, Color, Line_width, Filled);
        }
    } else {
        Paint_DrawLine(mWaveshare, Xstart, Ystart, Xend, Ystart, Color ,Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(mWaveshare, Xstart, Ystart, Xstart, Yend, Color ,Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(mWaveshare, Xend, Yend, Xend, Ystart, Color ,Line_width, LINE_STYLE_SOLID);
        Paint_DrawLine(mWaveshare, Xend, Yend, Xstart, Yend, Color ,Line_width, LINE_STYLE_SOLID);
    }
}

/******************************************************************************
function:	Use the 8-point method to draw a circle of the
            specified size at the specified position->
parameter:
    X_Center  ：Center X coordinate
    Y_Center  ：Center Y coordinate
    Radius    ：circle Radius
    Color     ：The color of the ：circle segment
    Filled    : Whether it is filled: 1 filling 0：Do not
******************************************************************************/
void Paint_DrawCircle(WaveShareLCD_st *mWaveshare, UWORD X_Center, UWORD Y_Center, UWORD Radius,
                        UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Draw_Fill )
{
    if (X_Center > Paint.Width || Y_Center >= Paint.Height) {
        Debug("Paint_DrawCircle Input exceeds the normal display range\r\n");
        return;
    }

    //Draw a circle from(0, R) as a starting point
    int16_t XCurrent, YCurrent;
    XCurrent = 0;
    YCurrent = Radius;

    //Cumulative error,judge the next point of the logo
    int16_t Esp = 3 - (Radius << 1 );

    int16_t sCountY;
    if (Draw_Fill == DRAW_FILL_FULL) {
        while (XCurrent <= YCurrent ) { //Realistic circles
            for (sCountY = XCurrent; sCountY <= YCurrent; sCountY ++ ) {
                Paint_DrawPoint(mWaveshare, X_Center + XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//1
                Paint_DrawPoint(mWaveshare, X_Center - XCurrent, Y_Center + sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//2
                Paint_DrawPoint(mWaveshare, X_Center - sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//3
                Paint_DrawPoint(mWaveshare, X_Center - sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//4
                Paint_DrawPoint(mWaveshare, X_Center - XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//5
                Paint_DrawPoint(mWaveshare, X_Center + XCurrent, Y_Center - sCountY, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//6
                Paint_DrawPoint(mWaveshare, X_Center + sCountY, Y_Center - XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);//7
                Paint_DrawPoint(mWaveshare, X_Center + sCountY, Y_Center + XCurrent, Color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
            }
            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    } else { //Draw a hollow circle
        while (XCurrent <= YCurrent ) {
            Paint_DrawPoint(mWaveshare, X_Center + XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//1
            Paint_DrawPoint(mWaveshare, X_Center - XCurrent, Y_Center + YCurrent, Color, Line_width, DOT_STYLE_DFT);//2
            Paint_DrawPoint(mWaveshare, X_Center - YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//3
            Paint_DrawPoint(mWaveshare, X_Center - YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//4
            Paint_DrawPoint(mWaveshare, X_Center - XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//5
            Paint_DrawPoint(mWaveshare, X_Center + XCurrent, Y_Center - YCurrent, Color, Line_width, DOT_STYLE_DFT);//6
            Paint_DrawPoint(mWaveshare, X_Center + YCurrent, Y_Center - XCurrent, Color, Line_width, DOT_STYLE_DFT);//7
            Paint_DrawPoint(mWaveshare, X_Center + YCurrent, Y_Center + XCurrent, Color, Line_width, DOT_STYLE_DFT);//0

            if (Esp < 0 )
                Esp += 4 * XCurrent + 6;
            else {
                Esp += 10 + 4 * (XCurrent - YCurrent );
                YCurrent --;
            }
            XCurrent ++;
        }
    }
}

/******************************************************************************
function:	Display image
parameter:
    image            ：Image start address
    xStart           : X starting coordinates
    yStart           : Y starting coordinates
    xEnd             ：Image width
    yEnd             : Image height
******************************************************************************/
void Paint_DrawImage(WaveShareLCD_st *mWaveshare, const unsigned char *image, UWORD xStart, UWORD yStart, UWORD W_Image, UWORD H_Image)
{
    int i,j;
		for(j = 0; j < H_Image; j++){
			for(i = 0; i < W_Image; i++){
				if(xStart+i < Paint.WidthMemory  &&  yStart+j < Paint.HeightMemory)//Exceeded part does not display
					Paint_SetPixel(mWaveshare, xStart + i, yStart + j, (*(image + j*W_Image*2 + i*2+1))<<8 | (*(image + j*W_Image*2 + i*2)));
				//Using arrays is a property of sequential storage, accessing the original array by algorithm
				//j*W_Image*2 			   Y offset
				//i*2              	   X offset
			}
		}

}

void Paint_SetClearFuntion(void (*Clear)(WaveShareLCD_st *mWaveshare,UWORD))
{
  CLEAR=Clear;
}

void Paint_SetDisplayFuntion(void (*Display)(WaveShareLCD_st *mWaveshare,UWORD,UWORD,UWORD))
{
  DISPLAY=Display;
}

void ST77xx_WriteChar(WaveShareLCD_st *mWaveshare, UWORD x, UWORD y, const char Acsii_Char,
		FontDef* Font, UWORD Color_Background, UWORD Color_Foreground)
{
	uint32_t i, b, j;
	for (i = 0; i < Font->height; i++) {
		b = Font->data[(Acsii_Char - 48) * Font->height + i];
		for (j = 0; j < Font->width; j++) {
			if ((b << j) & 0x80000000) {
				Paint_SetPixel(mWaveshare, x + Font->width - j, y + i, Color_Foreground);
			} else {
				if (Color_Background) {
					Paint_SetPixel(mWaveshare, x + Font->width - j, y + i, Color_Foreground);
				}
			}
		}
	}
}

void __ST77xx_WriteChar(WaveShareLCD_st *mWaveshare, UWORD x, UWORD y, const char Acsii_Char,
		FontDef* Font, UWORD Color_Background, UWORD Color_Foreground)
{
	uint32_t i, b, j;
	for (i = 0; i < Font->height; i++) {
	   b = Font->data[(Acsii_Char - 32) * Font->height + i];
		for (j = 0; j < Font->width; j++) {
			if ((b << j) & 0x8000) {
				Paint_SetPixel(mWaveshare, x + j, y + i, Color_Foreground);
			} else {
				if (Color_Background) {
					Paint_SetPixel(mWaveshare, x + j, y + i, Color_Foreground);
				}
			}
		}
	}
}

void ST77xx_WriteString(WaveShareLCD_st *mWaveshare, UWORD x, UWORD y, const char * pString,
		FontDef* Font, UWORD Color_Background, UWORD Color_Foreground)
{
	while(*pString) {
		if(*pString == ' ') {
			// skip spaces in the beginning of the new line
			pString++;
			continue;
		}
		if(*pString == '\n'){
			pString++;
			x=0;
			y+= Font->height;
		}
		if(*pString == '\r'){
			pString++;
			y+= Font->height;
			x-=Font->width;
		}
		if (Font->width == 32) {
			ST77xx_WriteChar(mWaveshare, x, y, *pString, Font, Color_Background, Color_Foreground);
		} else {
			__ST77xx_WriteChar(mWaveshare, x, y, *pString, Font, Color_Background, Color_Foreground);
		}
		x += Font->width;
		pString++;
	}
}

void ST77xx_WriteInteger_1Value(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, int32_t Number, FontDef* Font,
		UWORD Color_Background, UWORD Color_Foreground)
{
	sprintf(words, "%d", (int)Number);
	ST77xx_WriteString(mWaveshare, Xpoint, Ypoint, words, Font, Color_Background, Color_Foreground);
}

void ST77xx_WriteInteger_2Values(WaveShareLCD_st *mWaveshare, UWORD Xpoint, UWORD Ypoint, int32_t Number,
		FontDef* Font, UWORD Color_Background, UWORD Color_Foreground)
{
	sprintf(words, "%d", (int)Number);
	if(Number < 10){
		ST77xx_WriteString(mWaveshare, Xpoint, Ypoint, (const char*)("0"), Font, Color_Background, Color_Foreground);
		ST77xx_WriteString(mWaveshare, Xpoint+30, Ypoint, words, Font, Color_Background, Color_Foreground);
	}
	else{
		ST77xx_WriteString(mWaveshare, Xpoint, Ypoint, words, Font, Color_Background, Color_Foreground);
	}
}

void ST77xx_DrawHorizontalLine(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Xend, UWORD Y, UWORD Color,
		DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
	for(UWORD X=Xstart; X<Xend; X++){
		//Paint_SetPixel(X, Y, Color);
		Paint_DrawPoint(mWaveshare, X, Y, Color, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
	}
}

void ST77xx_DrawVerticalLine(WaveShareLCD_st *mWaveshare, UWORD Ystart, UWORD Yend, UWORD X, UWORD Color,
		DOT_PIXEL Line_width, LINE_STYLE Line_Style)
{
	for(UWORD Y=Ystart; Y<Yend; Y++){
		//Paint_SetPixel(X, Y, Color);
		Paint_DrawPoint(mWaveshare, X, Y, Color, DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);
	}
}

void ST77xx_DeleteNumber(WaveShareLCD_st *mWaveshare, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend,
		UWORD Color, DOT_PIXEL Line_width, DRAW_FILL Filled)
{
	if (Xstart > Paint.Width || Ystart > Paint.Height ||
			Xend > Paint.Width || Yend > Paint.Height) {
		Debug("Input exceeds the normal display range\r\n");
		return;
	}

	if (Filled) {
		UWORD Xpoint;
		UWORD Ypoint;
		for(Xpoint = Xstart; Xpoint < Xend; Xpoint++){
			for(Ypoint = Ystart; Ypoint < Yend; Ypoint++){
				Paint_SetPixel(mWaveshare, Xpoint, Ypoint, Color);
			}
		}
	} else {
		Paint_DrawLine(mWaveshare, Xstart, Ystart, Xend, Ystart, Color ,Line_width, LINE_STYLE_SOLID);
		Paint_DrawLine(mWaveshare, Xstart, Ystart, Xstart, Yend, Color ,Line_width, LINE_STYLE_SOLID);
		Paint_DrawLine(mWaveshare, Xend, Yend, Xend, Ystart, Color ,Line_width, LINE_STYLE_SOLID);
		Paint_DrawLine(mWaveshare, Xend, Yend, Xstart, Yend, Color ,Line_width, LINE_STYLE_SOLID);
	}
}

LCD_1IN14_ATTRIBUTES LCD_1IN14;

void LCD_1IN14_Reset(WaveShareLCD_st *mWaveshare)
{
	RST_High(mWaveshare);
	HAL_Delay(100);
	RST_Low(mWaveshare);
	HAL_Delay(100);
	RST_High(mWaveshare);
	HAL_Delay(150);
}
void LCD_1IN14_SendCommand(WaveShareLCD_st *mWaveshare, uint8_t reg)
{
	DC_Low(mWaveshare);
	CS_Low(mWaveshare);
	DEV_SPI_WRITE(mWaveshare,reg);
}
void LCD_1IN14_SendData_8Bit(WaveShareLCD_st *mWaveshare, uint8_t dat)
{
	DC_High(mWaveshare);
	CS_Low(mWaveshare);
	DEV_SPI_WRITE(mWaveshare,dat);
	CS_High(mWaveshare);
}
void LCD_1IN14_SendData_16Bit(WaveShareLCD_st *mWaveshare, uint16_t dat)
{
	DC_High(mWaveshare);
	CS_Low(mWaveshare);
	DEV_SPI_WRITE(mWaveshare,(dat >> 8) & 0xFF);
	DEV_SPI_WRITE(mWaveshare,dat & 0xFF);
	CS_High(mWaveshare);
}
void LCD_1IN14_SetAttributes(WaveShareLCD_st *mWaveshare, uint8_t Scan_dir)
{
    //Get the screen scan direction
    LCD_1IN14.SCAN_DIR = Scan_dir;
    UBYTE MemoryAccessReg = 0x00;

    //Get GRAM and LCD width and height
    if(Scan_dir == HORIZONTAL) {
        LCD_1IN14.HEIGHT	= LCD_1IN14_WIDTH;
        LCD_1IN14.WIDTH   = LCD_1IN14_HEIGHT;
        MemoryAccessReg = 0X70;
    } else {
        LCD_1IN14.HEIGHT	= LCD_1IN14_HEIGHT;
        LCD_1IN14.WIDTH   = LCD_1IN14_WIDTH;
        MemoryAccessReg = 0X00;
    }

    // Set the read / write scan direction of the frame memory
    LCD_1IN14_SendCommand(mWaveshare, 0x36); //MX, MY, RGB mode
    LCD_1IN14_SendData_8Bit(mWaveshare, MemoryAccessReg);	//0x08 set RGB
}
void LCD_1IN14_InitReg(WaveShareLCD_st *mWaveshare)
{
    LCD_1IN14_SendCommand(mWaveshare, 0x3A);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x05);

    LCD_1IN14_SendCommand(mWaveshare, 0xB2);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x0C);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x0C);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x00);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x33);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x33);

    LCD_1IN14_SendCommand(mWaveshare, 0xB7);  //Gate Control
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x35);

    LCD_1IN14_SendCommand(mWaveshare, 0xBB);  //VCOM Setting
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x19);

    LCD_1IN14_SendCommand(mWaveshare, 0xC0); //LCM Control
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x2C);

    LCD_1IN14_SendCommand(mWaveshare, 0xC2);  //VDV and VRH Command Enable
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x01);
    LCD_1IN14_SendCommand(mWaveshare, 0xC3);  //VRH Set
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x12);
    LCD_1IN14_SendCommand(mWaveshare, 0xC4);  //VDV Set
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x20);

    LCD_1IN14_SendCommand(mWaveshare, 0xC6);  //Frame Rate Control in Normal Mode
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x0F);

    LCD_1IN14_SendCommand(mWaveshare, 0xD0);  // Power Control 1
    LCD_1IN14_SendData_8Bit(mWaveshare, 0xA4);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0xA1);

    LCD_1IN14_SendCommand(mWaveshare, 0xE0);  //Positive Voltage Gamma Control
    LCD_1IN14_SendData_8Bit(mWaveshare, 0xD0);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x04);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x0D);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x11);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x13);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x2B);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x3F);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x54);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x4C);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x18);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x0D);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x0B);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x1F);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x23);

    LCD_1IN14_SendCommand(mWaveshare, 0xE1);  //Negative Voltage Gamma Control
    LCD_1IN14_SendData_8Bit(mWaveshare, 0xD0);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x04);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x0C);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x11);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x13);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x2C);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x3F);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x44);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x51);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x2F);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x1F);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x1F);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x20);
    LCD_1IN14_SendData_8Bit(mWaveshare, 0x23);

    LCD_1IN14_SendCommand(mWaveshare, 0x21);  //Display Inversion On

    LCD_1IN14_SendCommand(mWaveshare, 0x11);  //Sleep Out
    Delay_ms(100);

    LCD_1IN14_SendCommand(mWaveshare, 0x29);  //Display On

    //Set the resolution and scanning method of the screen
}
void LCD_1IN14_Init(WaveShareLCD_st *mWaveshare, uint8_t Scan_dir)
{
    //Hardware reset
    LCD_1IN14_Reset(mWaveshare);
//    HAL_Delay(1000);

    //Set the resolution and scanning method of the screen
    LCD_1IN14_SetAttributes(mWaveshare, Scan_dir);
    //Set the initialization register
    LCD_1IN14_InitReg(mWaveshare);
}
void LCD_1IN14_SetWindows(WaveShareLCD_st *mWaveshare, uint16_t Xstart,
		uint16_t Ystart, uint16_t Xend, uint16_t Yend)
{
	uint8_t x,y;
    if(LCD_1IN14.SCAN_DIR == HORIZONTAL){x=40;y=53;}
    else{ x=52; y=40; }
    //set the X coordinates
    LCD_1IN14_SendCommand(mWaveshare, 0x2A);
    LCD_1IN14_SendData_16Bit(mWaveshare, Xstart	+ x);
    LCD_1IN14_SendData_16Bit(mWaveshare, Xend + x);
    //set the Y coordinates
    LCD_1IN14_SendCommand(mWaveshare, 0x2B);
    LCD_1IN14_SendData_16Bit(mWaveshare, Ystart + y);
    LCD_1IN14_SendData_16Bit(mWaveshare, Yend + y);

    LCD_1IN14_SendCommand(mWaveshare, 0X2C);
}
void LCD_1IN14_Clear(WaveShareLCD_st *mWaveshare, uint16_t Color)
{
	uint16_t i,j;
    LCD_1IN14_SetWindows(mWaveshare, 0, 0, LCD_1IN14.WIDTH-1, LCD_1IN14.HEIGHT-1);

    DC_High(mWaveshare);
	for(i = 0; i < LCD_1IN14_WIDTH; i++){
		for(j = 0; j < LCD_1IN14_HEIGHT; j++){
			DEV_SPI_WRITE(mWaveshare,(Color>>8)&0xff);
			DEV_SPI_WRITE(mWaveshare,Color);
		}
	 }
}
void LCD_1IN14_Display(WaveShareLCD_st *mWaveshare, uint16_t *Image)
{
    UWORD i,j;
    LCD_1IN14_SetWindows(mWaveshare, 0, 0, LCD_1IN14.WIDTH-1, LCD_1IN14.HEIGHT-1);
    DC_High(mWaveshare);
    for(i = 0; i < LCD_1IN14_WIDTH; i++){
      for(j = 0; j < LCD_1IN14_HEIGHT; j++){
        DEV_SPI_WRITE(mWaveshare,(*(Image+i*LCD_1IN14_HEIGHT+j)>>8)&0xff);
        DEV_SPI_WRITE(mWaveshare,*(Image+i*LCD_1IN14_WIDTH+j));
      }
    }
}
void LCD_1IN14_DisplayWindows(WaveShareLCD_st *mWaveshare, uint16_t Xstart,
		uint16_t Ystart, uint16_t Xend, uint16_t Yend, uint16_t *Image)
{
    // display
    uint32_t Addr = 0;

    uint16_t i,j;
    LCD_1IN14_SetWindows(mWaveshare, Xstart, Ystart, Xend-1 , Yend-1);
    DC_High(mWaveshare);
    for (i = Ystart; i < Yend - 1; i++) {
        Addr = Xstart + i * LCD_1IN14_WIDTH ;
        for(j=Xstart;j<Xend-1;j++){
          DEV_SPI_WRITE(mWaveshare,(*(Image+Addr+j)>>8)&0xff);
          DEV_SPI_WRITE(mWaveshare,*(Image+Addr+j));
        }
    }
}

void LCD_1IN14_DrawPaint(WaveShareLCD_st *mWaveshare, uint16_t x, uint16_t y, uint16_t Color)
{
	LCD_1IN14_SetWindows(mWaveshare,x,y,x,y);
	LCD_1IN14_SendData_16Bit(mWaveshare,Color);
}

void LCD_1IN14_SetScrollArea(WaveShareLCD_st *mWaveshare, uint8_t tfa, uint8_t bfa)
{
	uint16_t vsa = Paint.Height - tfa - bfa;
	LCD_1IN14_SendCommand(mWaveshare, 0x33);
	LCD_1IN14_SendData_16Bit(mWaveshare, tfa);
	LCD_1IN14_SendData_16Bit(mWaveshare, vsa);
	LCD_1IN14_SendData_16Bit(mWaveshare, bfa);
}

void LCD_1IN14_SetScroll(WaveShareLCD_st *mWaveshare, uint16_t vsp)
{
	LCD_1IN14_SendCommand(mWaveshare, 0x37);
	LCD_1IN14_SendData_16Bit(mWaveshare, vsp);
}

void LCD_1IN14_InitScreen_2Values(WaveShareLCD_st *mWaveshare)
{
	DEV_Module_Init(mWaveshare);
	LCD_1IN14_Init(mWaveshare, HORIZONTAL);
	Paint_NewImage(mWaveshare, LCD_1IN14.WIDTH, LCD_1IN14.HEIGHT, ROTATE_0, WHITE);
	Display_1IN14_Screen_2Values(mWaveshare, 0, 0);
}

void LCD_1IN14_InitScreen_6Values(WaveShareLCD_st *mWaveshare)
{
	DEV_Module_Init(mWaveshare);
	LCD_1IN14_Init(mWaveshare, HORIZONTAL);
	Paint_NewImage(mWaveshare, LCD_1IN14.WIDTH, LCD_1IN14.HEIGHT, ROTATE_0, WHITE);
	ST77xx_DeleteNumber(mWaveshare, 0, 0, LCD_1IN14.WIDTH,
			LCD_1IN14.HEIGHT, WHITE, DOT_PIXEL_2X2, DRAW_FILL_FULL);
	Display_1IN14_Screen_6Values(mWaveshare, 0, 0, 0, 0, 0, 0);
}

void Paint_ClearDMA(WaveShareLCD_st *mWaveshare, uint16_t color)
{
	uint16_t line_buffer[LCD_1IN14.WIDTH];

	for(int i=0; i<LCD_1IN14.WIDTH; i++){
		line_buffer[i] = color;
	}

	LCD_1IN14_SetWindows(mWaveshare, 0, 0, LCD_1IN14.WIDTH-1, LCD_1IN14.HEIGHT-1);
	DC_High(mWaveshare);

	for(int j=0; j<LCD_1IN14.HEIGHT+140; j++){
		HAL_SPI_Transmit_DMA(mWaveshare->hspi, (uint8_t*)line_buffer, LCD_1IN14.WIDTH);
		while(HAL_SPI_GetState(mWaveshare->hspi) != HAL_SPI_STATE_READY); //wait until DMA is finished
	}
}

void Display_1IN14_Screen_2Values(WaveShareLCD_st *mWaveshare, uint8_t val1, uint8_t val2)
{
	Paint_ClearDMA(mWaveshare, WHITE);
	ST77xx_WriteInteger_2Values(mWaveshare, area1.x_start, area1.y_start,
			val1, &Font_32x55, BLACK, BLACK);

	if(val2 == 95)
	{
		ST77xx_WriteInteger_2Values(mWaveshare, area4.x_start, area4.y_start,
				0, &Font_32x55, BLACK, BLACK);

		ST77xx_WriteInteger_2Values(mWaveshare, area2.x_start, area2.y_start,
				val2, &Font_32x55, BLACK, BLACK);

		ST77xx_WriteInteger_2Values(mWaveshare, area5.x_start, area5.y_start,
				val2-5, &Font_32x55, BLACK, BLACK);
	}
	else if(val2 == 0)
	{
		ST77xx_WriteInteger_2Values(mWaveshare, area4.x_start, area4.y_start,
				val2+5, &Font_32x55, BLACK, BLACK);

		ST77xx_WriteInteger_2Values(mWaveshare, area2.x_start, area2.y_start,
				val2, &Font_32x55, BLACK, BLACK);

		ST77xx_WriteInteger_2Values(mWaveshare, area5.x_start, area5.y_start,
				95, &Font_32x55, BLACK, BLACK);
	}
	else
	{
		ST77xx_WriteInteger_2Values(mWaveshare, area4.x_start, area4.y_start,
				val2+5, &Font_32x55, BLACK, BLACK);

		ST77xx_WriteInteger_2Values(mWaveshare, area2.x_start, area2.y_start,
				val2, &Font_32x55, BLACK, BLACK);

		ST77xx_WriteInteger_2Values(mWaveshare, area5.x_start, area5.y_start,
				val2-5, &Font_32x55, BLACK, BLACK);
	}
}

void Display_1IN14_Screen_6Values(WaveShareLCD_st *mWaveshare, uint8_t val1, uint8_t val2,
									uint8_t val3, uint8_t val4, uint8_t val5, uint8_t val6)
{
	ST77xx_DeleteNumber(mWaveshare, area1.x_start, area1.y_start, area1.x_start + 180,
			area1.y_stop, WHITE, DOT_PIXEL_2X2, DRAW_FILL_FULL);
	ST77xx_WriteInteger_1Value(mWaveshare, area1.x_start, area1.y_start,
					val1, &Font_32x55, BLACK, BLACK);
	ST77xx_WriteInteger_1Value(mWaveshare, area1.x_start + 30, area1.y_start,
					val2, &Font_32x55, BLACK, BLACK);
	ST77xx_WriteInteger_1Value(mWaveshare, area1.x_start + 60, area1.y_start,
					val3, &Font_32x55, BLACK, BLACK);
	ST77xx_WriteInteger_1Value(mWaveshare, area1.x_start + 90, area1.y_start,
					val4, &Font_32x55, BLACK, BLACK);
	ST77xx_WriteInteger_1Value(mWaveshare, area1.x_start + 120, area1.y_start,
					val5, &Font_32x55, BLACK, BLACK);
	ST77xx_WriteInteger_1Value(mWaveshare, area1.x_start + 150, area1.y_start,
					val6, &Font_32x55, BLACK, BLACK);
}

