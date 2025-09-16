/*
 * I2C_DS3231.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_I2C_DS3231_H_
#define SRC_BUFFER_I2C_DS3231_H_

#include "OOP.h"
#include "main.h"
#include "DS3231.h"

typedef struct
{
	OOP super;
	uint8_t ID_Signal;
	uint8_t num_i2c;
	TIME *mTime;
}DS3231_st;

void I2C_buf_ctor(DS3231_st * const me, void *data, uint8_t ID_Signal, uint8_t num_i2c);

#endif /* SRC_BUFFER_I2C_DS3231_H_ */
