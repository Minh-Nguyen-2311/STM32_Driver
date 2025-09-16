/*
 * LED_Process.h
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#ifndef SRC_BUFFER_LED_PROCESS_H_
#define SRC_BUFFER_LED_PROCESS_H_

#include "OOP.h"
#include "main.h"
#include "WS2812.h"
#include "tim.h"

#define Num_led_Din1	2
#define Num_led_Din2	2

#define ON_LED		1
#define OFF_LED 	0
#define BLINK_LED 	2

typedef struct{
	OOP super;
	uint8_t Din; // day led (din = 1 day din 1(chanel 1), din = 2 day din 2(chanel 2))
	uint8_t ID_Din;//stt led trong mảng led
	uint8_t ID_tinhieu;// ID của tín hiệu theo bảng tín hiệu (file word)

	// bản tin từ interface xuống sum: id mạch | id tín hiệu | status | freq_time | red | green | blue | brightness
	struct {
		uint8_t Status;// trạng thái led(0 OFF, 1 ON, 2 NHAP NHAY)
		uint8_t freq_time; // tần số nhấp nháy được cài trên pc
		uint8_t Red;// màu đỏ
		uint8_t Green;// màu xanh
		uint8_t Blue;// màu xanh dương
		uint8_t Brightness;// độ sáng của đèn
	}format_msg;

	uint8_t led_state; // trạng thái on off của led hiện tại
	uint8_t fist_time; // trạng thái nhảy led lần đầu (bằng 1 là chưa nháy, bằng 0 là đã nháy)
	uint16_t freq_count; // đếm tần số nhấp nháy trên pc
	uint32_t Current_Color;  // trạng thái trước đó
	uint8_t IsChange; 	// thay đổi trạng thái: 0-không | 1-có
	uint8_t len;
	uint8_t flag_led;
}ChipLed;

void ChipLed_ctor(ChipLed * const me, void *data,
	uint8_t Din,
	uint8_t Id_Din,
	uint8_t ID_tinhieu,
	uint8_t Status,
	uint8_t led_state);

#endif /* SRC_BUFFER_LED_PROCESS_H_ */
