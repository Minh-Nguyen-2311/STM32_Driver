/*
 * Audio.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#ifndef SRC_DRIVER_LIBRARY_AUDIO_H_
#define SRC_DRIVER_LIBRARY_AUDIO_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include "sdio.h"
#include "fatfs.h"
#include "stdbool.h"
#include "ff.h"

#define AUDIO_BUFFER 	1024 //256 I2S_samples * (L+R) * 2

extern FATFS fatfs;
extern FIL myfile;
extern FRESULT fresult;
extern UINT bytes_read;

typedef enum{
	BUFFER_OFFSET_NONE = 0,
	BUFFER_OFFSET_HALF,
	BUFFER_OFFSET_FULL,
}BUFFER_StateTypeDef;

void Wav_Init(void);
void PlayWavFile(const char* fname);
void StopWavFile(void);
void AudioAmplifier(uint8_t *buffer, uint32_t size, float volume);
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s);
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s);

#endif /* SRC_DRIVER_LIBRARY_AUDIO_H_ */
