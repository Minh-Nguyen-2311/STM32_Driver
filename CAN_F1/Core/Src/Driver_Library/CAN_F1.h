/*
 * CAN_F1.h
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#ifndef SRC_DRIVER_LIBRARY_CAN_F1_H_
#define SRC_DRIVER_LIBRARY_CAN_F1_H_

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_hal.h"
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SUM_ID 			0x05
#define INTERFACE_ID	0x01
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/


/* USER CODE BEGIN PV */
extern struct Queue* Can_Queue;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

void Can_enable(CAN_HandleTypeDef hcan);
void Can_disable(CAN_HandleTypeDef hcan);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan);
HAL_StatusTypeDef Can_filter(CAN_HandleTypeDef hcan);
HAL_StatusTypeDef CAN_Transmit(CAN_HandleTypeDef hcan,uint8_t *data, uint8_t len);

#define CAN_RXQUEUE_LEN 32

typedef struct {
	CAN_RxHeaderTypeDef header;
	uint8_t data[8];
}CANFrame;

/* USER CODE END PFP */

#endif /* SRC_DRIVER_LIBRARY_CAN_F1_H_ */
