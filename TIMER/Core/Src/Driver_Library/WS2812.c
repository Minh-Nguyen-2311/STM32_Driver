/*
 * WS2812.c
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "WS2812.h"
#include "LED_Process.h"
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
#if Num_led_Din1 != 0
uint8_t Led_Data_1[Num_led_Din1][4];
uint8_t Led_Mod_1[Num_led_Din1][4];
#endif

#if Num_led_Din2 != 0
uint8_t Led_Data_2[Num_led_Din2][4];
uint8_t Led_Mod_2[Num_led_Din2][4];
#endif
uint16_t pwmData[(24 * (Num_led_Din1 + Num_led_Din2)) + Time_reset*2]; // 24bit mỗi led và 50 time off reset
volatile int datasentflag = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* Constructor */

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_ALL);
	datasentflag = 1;
}

void Set_pwm_duty(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *duty,
		uint16_t Length) {
	HAL_TIM_PWM_Start_DMA(htim, Channel, duty, Length);
}

void Set_Led(int Din, int LedNum, int Red, int Green, int Blue) {
#if Num_led_Din1 != 0
	if (Din == 1) {
		Led_Data_1[LedNum][0] = LedNum;
		Led_Data_1[LedNum][1] = Green;
		Led_Data_1[LedNum][2] = Red;
		Led_Data_1[LedNum][3] = Blue;
	}
#endif

#if Num_led_Din2 != 0
	if (Din == 2) {
		Led_Data_2[LedNum][0] = LedNum;
		Led_Data_2[LedNum][1] = Green;
		Led_Data_2[LedNum][2] = Red;
		Led_Data_2[LedNum][3] = Blue;
	}
#endif
}

void Set_Brightness(int Din, uint8_t ID_din, int Brightness) {
#if Use_Brightness
	if (Brightness > 20)
		Brightness = 20;

#if Num_led_Din1 != 0
	if (Din == 1)
		Led_Mod_1[ID_din][0] = Led_Data_1[ID_din][0];
#endif

#if Num_led_Din2 != 0
	if (Din == 2)
		Led_Mod_2[ID_din][0] = Led_Data_2[ID_din][0];
#endif

	for (int j = 1; j < 4; j++) {
		float angle = 90 - Brightness;
		angle = (angle * Pi) / 180.0;

#if Num_led_Din1 != 0
		if (Din == 1)
			Led_Mod_1[ID_din][j] = (Led_Data_1[ID_din][j]) / (tan(angle));
#endif

#if Num_led_Din2 != 0
		if (Din == 2)
			Led_Mod_2[ID_din][j] = (Led_Data_2[ID_din][j]) / (tan(angle));
#endif
	}
#endif
}

void ICled_Send(TIM_HandleTypeDef *htim, uint32_t Channel) {
	uint32_t indx = 0;
	uint32_t color = 0;
	uint32_t maxduty = htim->Init.Period + 1;
//	int count_reset = (Time_reset / Time_TH_TL) + 10;
	int num_led = (Channel == TIM_CHANNEL_1) ? Num_led_Din1 : Num_led_Din2;

	for (int i = 0; i < Time_reset; i++) // reset time count_reset
	{
		pwmData[indx] = 0;
		indx++;
	}

	for (int i = 0; i < num_led; i++) {
#if Use_Brightness

#if Num_led_Din1 != 0
		if (Channel == TIM_CHANNEL_1)
			color = ((Led_Mod_1[i][1] << 16) | (Led_Mod_1[i][2] << 8) | (Led_Mod_1[i][3])); // r g b
#endif

#if Num_led_Din2 != 0
		if (Channel == TIM_CHANNEL_2)
			color = ((Led_Mod_2[i][1] << 16) | (Led_Mod_2[i][2] << 8) | (Led_Mod_2[i][3])); // r g b
#endif

#else
#if Num_led_Din1 != 0
		if (Channel == TIM_CHANNEL_1)
			color = ((Led_Data_1[i][1]<<16) | (Led_Data_1[i][2]<<8) | (Led_Data_1[i][3]));
#endif

#if Num_led_Din2 != 0
		if (Channel == TIM_CHANNEL_2)
			color = ((Led_Data_2[i][1]<<16) | (Led_Data_2[i][2]<<8) | (Led_Data_2[i][3]));
#endif
#endif
		for (int i = 23; i >= 0; i--) // tinh thoi gian high voltage
		{
			if (color & (1 << i)) 	// bit 1
				pwmData[indx] = (maxduty * Time_1H) / Time_TH_TL;
			else 				// bit 0
				pwmData[indx] = (maxduty * Time_0H) / Time_TH_TL;

			indx++;
		}
	}
	for (int i = 0; i < Time_reset; i++) // reset time count_reset
	{
		pwmData[indx] = 0;
		indx++;
	}
	Set_pwm_duty(htim, Channel, (uint32_t*) pwmData, indx);
	while (!datasentflag) {
	};
	datasentflag = 0;
}

void Reset_LED(void) {
#if Num_led_Din1 != 0
	for (int i = 0; i < Num_led_Din1; i++) {
		Led_Data_1[i][0] = i;
		Led_Data_1[i][1] = 0;
		Led_Data_1[i][2] = 0;
		Led_Data_1[i][3] = 0;
	}
#endif

#if Num_led_Din2 != 0
	for (int i = 0; i < Num_led_Din2; i++) {
		Led_Data_2[i][0] = i;
		Led_Data_2[i][1] = 0;
		Led_Data_2[i][2] = 0;
		Led_Data_2[i][3] = 0;
	}
#endif
}

/* USER CODE END PFP */
