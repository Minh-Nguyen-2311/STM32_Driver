/*
 * Audio.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#include "Audio.h"
#include "string.h"
#include "stdio.h"

static uint32_t WavDataLength = 0;
static volatile uint32_t WavRemSize = 0;

int16_t AudioBuff[4096];

volatile BUFFER_StateTypeDef buffer_offset = BUFFER_OFFSET_NONE;

extern I2S_HandleTypeDef hi2s3;

int samples = sizeof(AudioBuff) / sizeof(AudioBuff[0]);

uint8_t header[144];

void PlayWavFile(const char* fname)
{
	fresult = f_open(&myfile, fname, FA_OPEN_EXISTING|FA_READ);
	if(fresult != FR_OK)
	{
		printf("File open error\r\n");
		return;
	}
	printf("File opening....\r\n");
	fresult = f_read(&myfile, header, sizeof(header), &bytes_read);
	if(fresult != FR_OK)
	{
		printf("File read error\r\n");
		f_close(&myfile);
		return;
	}
	if(memcmp((const char*)header,"RIFF",4) != 0)
	{
		printf("File's header no 'RIFF' at offset 0\r\n");
		f_close(&myfile);
		return;
	}
	if(memcmp((const char*)header+8,"WAVEfmt ",8) != 0)
	{
		printf("File's header no 'WAVEfmt ' at offset 8\r\n");
		f_close(&myfile);
		return;
	}
	if(memcmp((const char*)header+36,"data",4 != 0)) //(memcmp((const char*)header+36,"LIST",4)
	{
		printf("File's header no 'data' at offset 36\r\n"); //LOG_I("File's header no 'LIST' at offset 36\r\n")
		f_close(&myfile);
		return;
	}
	//uint32_t fileSize = 8 + (header[4] | (header[5] << 8) | (header[6] << 16) | (header[7] << 24));
	uint32_t headerSize = header[16] | (header[17] << 8) | (header[18] << 16) | (header[19] << 24);
	uint16_t compress = header[20] | (header[21] << 8);
	uint16_t channelNum = header[22] | (header[23] << 8);
	uint16_t sampleRate = header[24] | (header[25] << 8) | (header[26] << 16) | (header[27] << 24);
	uint32_t bytePerSec = header[28] | (header[29] << 8) | (header[30] << 16) | (header[31] << 24);
	uint16_t bytePerSamp = header[32] | (header[33] << 8);
	uint16_t bitPerSampPerChannel = header[34] | (header[35] << 8);
	uint32_t dataSize = header[40] | (header[41] << 8) | (header[42] << 16) | (header[43] << 24); //Data for sample
	//uint32_t dataSize = header[138] | (header[139] << 8) | (header[140] << 16) | (header[141] << 24); //Data for song

	if(headerSize != 16)
	{
		printf("Header's size error \r\n");
		f_close(&myfile);
		return;
	}

	if(compress != 1)
	{
		printf("Compression error \r\n");
		f_close(&myfile);
		return;
	}

	if(channelNum != 2)
	{
		printf("Channel num error \r\n");
		f_close(&myfile);
		return;
	}

	if((sampleRate != 44100) || (bytePerSamp != 4) || (bitPerSampPerChannel != 16) || (bytePerSec != 44100*4)
			|| (dataSize < sizeof(AudioBuff))) //(dataSize < sizeof(buff1) + sizeof(buff2))
	{
		printf("Format error, 16-bit file with sample rate of 44100 Hz expected\r\n");
		f_close(&myfile);
		return;
	}

	WavDataLength = dataSize;
	WavRemSize = WavDataLength;

	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)&AudioBuff[0], samples); /*Transmit I2S for whole buffer*/

	while(WavRemSize > 0)
	{
		HAL_I2S_DMAResume(&hi2s3);
		bytes_read = 0;
		if(buffer_offset == BUFFER_OFFSET_HALF) /*1st half already transmitted*/
		{
			f_read(&myfile, &AudioBuff[0], AUDIO_BUFFER/2 * sizeof(int16_t), (void*)&bytes_read); /*Read File into 1st half buffer*/
			buffer_offset = BUFFER_OFFSET_NONE;
			WavRemSize -= bytes_read;
		}
		if(buffer_offset == BUFFER_OFFSET_FULL) /*2nd half already transmitted*/
		{
			f_read(&myfile, &AudioBuff[AUDIO_BUFFER/2], AUDIO_BUFFER/2 * sizeof(int16_t), (void*)&bytes_read); /*Read File into 2nd half buffer*/
			buffer_offset = BUFFER_OFFSET_NONE;
			WavRemSize -= bytes_read;
		}
		if(WavRemSize < AUDIO_BUFFER/2 * sizeof(int16_t))
		{
			WavRemSize -= WavRemSize;
			HAL_I2S_DMAPause(&hi2s3);
			f_close(&myfile);
		}
	}
}

void StopWavFile(void)
{
	WavDataLength = 0;
	HAL_I2S_DMAStop(&hi2s3);
}

void Wav_Init(void)
{
	fresult = f_mount(&fatfs, "", 0);
	if(fresult != FR_OK)
	{
		printf("Mount unsuccessfully\r\n");
		return;
	}
	printf("Mount successfully\r\n");
}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	buffer_offset = BUFFER_OFFSET_HALF;
}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	buffer_offset = BUFFER_OFFSET_FULL;
	HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)&AudioBuff[0], 2048); /*Change back to 1st buffer*/
}
