/*
 * WS2812.h
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#ifndef SRC_DRIVER_LIBRARY_WS2812_H_
#define SRC_DRIVER_LIBRARY_WS2812_H_

#include "stm32f4xx_hal.h"
#include "main.h"
#include <math.h>
#include <stdbool.h>
/* USER CODE END Includes */
/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Use_Brightness 1
#define Pi 3.14159265

#define Time_TH_TL 	1250 	// ns
#define Time_1H		800	// ns
#define Time_0H		400		// ns
#define Time_reset	240	// =300us/1.25us (1.25us period PWM)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/


/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void Set_pwm_duty(TIM_HandleTypeDef *htim, uint32_t Channel, uint32_t *duty, uint16_t Length);
void Set_Led(int Din, int LedNum, int Red, int Green, int Blue);
void Set_Brightness(int Din, uint8_t ID_din, int Brightness);
void ICled_Send(TIM_HandleTypeDef *htim, uint32_t Channel);
void Reset_LED (void);
void ENABLE_LEDRGB(bool state);

#endif /* SRC_DRIVER_LIBRARY_WS2812_H_ */
