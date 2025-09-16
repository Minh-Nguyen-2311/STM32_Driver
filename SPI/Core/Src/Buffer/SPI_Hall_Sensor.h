#ifndef SRC_BUFFER_ECD_BUF_H_
#define SRC_BUFFER_ECD_BUF_H_

#include "Hall_Sensor.h"
#include "OOP.h"
#include "main.h"
#include <string.h>

typedef struct{
	OOP super;

	uint8_t mode; // 0: ECD Incremental, 1: Hall Sensor, 2: ECD Absolute, 3: CAN ECD
	uint8_t number_ecd; //0: Don't care, 1: Number hall sensor, 2/3: Number encoder
	uint32_t ecd_data;
	uint32_t ecd_predata;
	uint8_t ID_tinhieu;
	uint16_t len;
	uint8_t flag_ecd;
}ECD_buf_st;

void ECD_buf_ctor(ECD_buf_st * const me, void *data, uint8_t mode, uint8_t ID_tinhieu, uint8_t number_ecd);

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

#endif /* SRC_BUFFER_ECD_BUF_H_ */
