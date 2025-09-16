/*
 * Encoder_Process.c
 *
 *  Created on: Aug 3, 2025
 *      Author: Admin
 */

#include "Encoder_Process.h"

static uint32_t ECD_Init_(OOP const * const me);
static void ECD_Process_(OOP const * const me);

void Encoder_buf_ctor(Encoder_st * const me, void *data, uint8_t ID_Signal,
						uint8_t mode, uint8_t num_encoder)
{
	static struct VirtualTable const vtbl = {
			/* vtbl of the UART class */
			&ECD_Init_,
			&ECD_Process_
	};

	OOP_ctor(&me->super, data); /* call superclass' ctor */

	me->super.vptr = &vtbl;  /* override the vptr */
	me->super.data = NULL;

	me->Encoder_data[4] = 0;

	me->ID_Signal = ID_Signal;
	me->mode = mode;
	me->num_encoder = num_encoder;
}

static uint32_t ECD_Init_(OOP const * const me)
{
	Encoder_st * const me_ = (Encoder_st*)me;
	if(me_->mode == 0)
	{
		Start_ECD_TIM(&htim4);
	}
	(void)me_; //Avoid compiler error
	return 0;
}

static void ECD_Process_(OOP const * const me)
{
	Encoder_st * const _me = (Encoder_st*)me;
	if(_me->mode == 0)
	{
		switch(_me->num_encoder)
		{
		case 0:
			_me->Encoder_data[_me->num_encoder] = Get_ECD_TIM_Data(&htim4);
			break;
		case 1:
			_me->Encoder_data[_me->num_encoder] = Get_ECD_TIM_Data(&htim4);
			break;
		case 2:
			_me->Encoder_data[_me->num_encoder] = Get_ECD_TIM_Data(&htim4);
			break;
		case 3:
			_me->Encoder_data[_me->num_encoder] = Get_ECD_TIM_Data(&htim4);
			break;
		}
	}
}
