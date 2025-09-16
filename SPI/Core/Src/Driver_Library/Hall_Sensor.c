/*
 * SPI_Hall_Sensor.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#include "Hall_Sensor.h"

uint8_t u8_spi_read_buffer[8];
uint8_t u8_spi_write_buffer[8];
float G_in = 0 , delta = 0, pre_G_out = 0, pre_G_in = 0;
uint8_t loop = 0;

void csHigh(hallEncoder_st *mHall)
{
	Delay_us(100);
	HAL_GPIO_WritePin(mHall->csPort, mHall->csPin, GPIO_PIN_SET);
	Delay_us(100);

}
/*---------- enable spi3 */
void csLow(hallEncoder_st *mHall)
{
	Delay_us(100);
	HAL_GPIO_WritePin(mHall->csPort, mHall->csPin, GPIO_PIN_RESET);
	Delay_us(100);
}

/**
  * @brief
  * @retval
  */
/*---------- send 8 byte data spi */
HAL_StatusTypeDef spiSend8Bytes(hallEncoder_st *mHall)
{
	HAL_StatusTypeDef res;
	csLow(mHall);
	res = HAL_SPI_TransmitReceive(mHall->hspi, u8_spi_write_buffer, u8_spi_read_buffer, 8, HAL_MAX_DELAY);
	csHigh(mHall);
	return res;
}
/**
  * @brief
  * @retval
  */
HAL_StatusTypeDef sendGet1(hallEncoder_st *mHall)
{
	u8_spi_write_buffer[0] = 0x00;
	u8_spi_write_buffer[1] = 0x00;
	u8_spi_write_buffer[2] = 0xFF;
	u8_spi_write_buffer[3] = 0xFF;
	u8_spi_write_buffer[4] = 0x00;
	u8_spi_write_buffer[5] = 0x00;
	u8_spi_write_buffer[6] = 0x13;
	u8_spi_write_buffer[7] = 0xEA;
	return (spiSend8Bytes( mHall));
}

HAL_StatusTypeDef sendNode(hallEncoder_st *mHall)
{
	u8_spi_write_buffer[0] = 0x00;
	u8_spi_write_buffer[1] = 0x00;
	u8_spi_write_buffer[2] = 0xAA;
	u8_spi_write_buffer[3] = 0xAA;
	u8_spi_write_buffer[4] = 0x00;
	u8_spi_write_buffer[5] = 0x00;
	u8_spi_write_buffer[6] = 0xD0;
	u8_spi_write_buffer[7] = 0xAB;

	return (spiSend8Bytes(mHall));
}

void hallEncoderInit(hallEncoder_st *mHall)
{
	mHall->prePulseCycle = 0;
	mHall->prePulseLinear = 0;
	for(int i = 0; i<8; i++)
	{
		mHall->u8Data[i] = 0;
	}
	csLow(mHall);
}

int32_t covertCycle2Liner(hallEncoder_st *mHall)
{
	uint16_t u16AngleLsb;
	float pulseCycle = 0, pulseLinear = 0, delta = 0;
	const float lsb2DecDegrees = 0.02197;

	u16AngleLsb = (mHall->u8Data[1] & 0x3F) << 8;
	u16AngleLsb = u16AngleLsb + mHall->u8Data[0];
	pulseCycle = u16AngleLsb * lsb2DecDegrees;
	delta = pulseCycle - mHall->prePulseCycle;
	if (delta < -200)
	{
		delta += 360;
	}else if (delta > 200)
	{
		delta -= 360;
	 }
	pulseLinear = mHall->prePulseLinear + delta;
	mHall->prePulseLinear = pulseLinear;
	mHall->prePulseCycle = pulseCycle;
	return (int32_t) pulseLinear;
}

int32_t getPulseLinear(hallEncoder_st *mHall)
{
	int32_t pulse;
	sendGet1(mHall);
	Delay_us(1000);
	sendNode(mHall);
	Delay_us(1000);
	for(int i = 0; i<8; i++)
	{
		mHall->u8Data[i] = u8_spi_read_buffer[i];
	}
	pulse = covertCycle2Liner(mHall);
	return pulse;
}

float getPulseCycle(hallEncoder_st *mHall)
{
	float pulseCycle = 0;
	const float lsb2DecDegrees = 0.02197; //360 / 2^14 (14 bit output resolution)
	uint16_t u16AngleLsb;
	sendGet1(mHall);
	Delay_us(1000);
	sendNode(mHall);
	Delay_us(1000);
	for(int i = 0; i<8; i++)
	{
		mHall->u8Data[i] = u8_spi_read_buffer[i];
	}
	/*Extract and convert angle to degrees*/
	u16AngleLsb = (mHall->u8Data[1] & 0x3F) << 8;
	u16AngleLsb = u16AngleLsb + mHall->u8Data[0];
	/*Convert to decimal degrees*/
	pulseCycle = u16AngleLsb * lsb2DecDegrees;
	return pulseCycle;
}

float getReversePulseCycle(hallEncoder_st *mHall)
{
	float pulseCycle1 = 0;
	const float lsb2DecDegrees = 0.02197; //360 / 2^14 (14 bit output resolution)
	uint16_t u16AngleLsb;
	sendGet1(mHall);
	Delay_us(1000);
	sendNode(mHall);
	Delay_us(1000);
	for(int i = 0; i<8; i++)
	{
		mHall->u8Data[i] = u8_spi_read_buffer[i];
	}
	/*Extract and convert angle to degrees*/
	u16AngleLsb = (mHall->u8Data[1] & 0x3F) << 8;
	u16AngleLsb = u16AngleLsb + mHall->u8Data[0];
	/*Convert to decimal degrees*/
	pulseCycle1 = u16AngleLsb * lsb2DecDegrees;
	pulseCycle1 = 360 - pulseCycle1;
	return pulseCycle1;
}

void setPulseZero(hallEncoder_st *mHall)
{
	pre_G_out = 0;
	pre_G_in = 0;

	sendGet1(mHall);
	Delay_us(1000);
	sendNode(mHall);
	Delay_us(1000);

	csHigh(mHall);
}

