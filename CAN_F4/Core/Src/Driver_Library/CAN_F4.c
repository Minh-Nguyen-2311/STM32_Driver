/*
 * CAN_F4.c
 *
 *  Created on: Jul 29, 2025
 *      Author: Admin
 */

#include "CAN_F4.h"
#include "Queue_LL.h"

CAN_TxHeaderTypeDef   	TxHeader;
CAN_RxHeaderTypeDef   	RxHeader;

uint8_t  TxData[8], RxData[8];

uint32_t count_Tx_Can=0, count_Rx_Can=0;

struct Queue* Can_Queue;

CAN_FilterTypeDef sFilterConfig1;

CANFrame can_Rx_queue[CAN_RXQUEUE_LEN];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan)
{
	if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
	{
		enqueue(Can_Queue, RxData, RxHeader.DLC);
		count_Rx_Can++;
	}
}

HAL_StatusTypeDef Can_filter(CAN_HandleTypeDef hcan)
{

	Can_Queue = createQueue();

	TxHeader.DLC = 8;//data length code
	TxHeader.IDE = CAN_ID_STD;// cau hinh la Standard
	TxHeader.RTR = CAN_RTR_DATA;
	TxHeader.StdId = INTERFACE_ID;// StdID de xac dinh device gui data

	sFilterConfig1.FilterFIFOAssignment = CAN_FILTER_FIFO0;
	sFilterConfig1.FilterIdHigh = 0;
	sFilterConfig1.FilterIdLow = 0;
	sFilterConfig1.FilterMaskIdHigh = 0;
	sFilterConfig1.FilterMaskIdLow = 0;
	sFilterConfig1.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig1.FilterActivation = ENABLE;
	//phinh4
	sFilterConfig1.FilterBank =0;
	sFilterConfig1.FilterMode =CAN_FILTERMODE_IDMASK;
	//end phinh4
	if(HAL_CAN_ConfigFilter(&hcan,&sFilterConfig1) != HAL_OK)
	{
		return HAL_ERROR;
	}
	if(HAL_CAN_Start(&hcan) != HAL_OK)
	{
		return HAL_ERROR;
	}
	return HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}

HAL_StatusTypeDef CAN_Transmit(CAN_HandleTypeDef hcan, uint8_t *data, uint8_t len)
{
	uint32_t timeout = HAL_GetTick();
	HAL_StatusTypeDef result = HAL_OK;
	uint32_t TxMailbox;
	if(len>8) return result;
	TxHeader.DLC = len;

	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan) == 0)
	{
		if(HAL_GetTick()-timeout > 200) break;
	}
	count_Tx_Can++;
	HAL_CAN_AddTxMessage(&hcan, &TxHeader, data, &TxMailbox);
	HAL_Delay(1);
	return result;
}
