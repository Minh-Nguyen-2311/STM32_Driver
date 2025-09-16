#include "SPI_Hall_Sensor.h"
#include "math.h"

static uint32_t ECD_Init_(OOP const * const me);
static void ECD_Process_(OOP const * const me);

hallEncoder_st hall1 = {GPIOB, GPIO_PIN_11, &hspi2};
hallEncoder_st hall2 = {GPIOB, GPIO_PIN_12, &hspi2};
hallEncoder_st hall3 = {GPIOA, GPIO_PIN_11, &hspi3};
hallEncoder_st hall4 = {GPIOA, GPIO_PIN_12, &hspi3};

float TempVal[4] = {0.0f};
float FilteredVal[4] = {0.0f};
uint32_t FilteredValUint32[4] = {0};

float ECD_Value_Linearized[4] = {0.0f}; //Linearized encoder value for absolute | increment encoder.
uint32_t ECD_Value[4] = {0}; //Absolute encoder value
float ECD_Value_Filters[4] = {0.0f};
float Temp_ECD_Data[4] = {0.0f};
float ECD_Value_Offset[4] = {0.0f};

extern uint32_t ecd_pos;
extern uint32_t flagPubECD;

void ECD_buf_ctor(ECD_buf_st * const me, void *data, uint8_t mode, uint8_t ID_tinhieu, uint8_t number_ecd)
{
	static struct VirtualTable const vtbl = {
			/* vtbl of the ECD class */
			&ECD_Init_,
			&ECD_Process_
	};

	OOP_ctor(&me->super, data); /* call superclass' ctor */

	me->super.vptr = &vtbl;  /* override the vptr */
	me->super.data = NULL;

	me->mode = mode;
	me->number_ecd = number_ecd;
	me->ID_tinhieu = ID_tinhieu;
	me->ecd_predata = 0;
	me->ecd_data = 0;
	me->len = 8;
	me->flag_ecd = 0;
}

static uint32_t ECD_Init_(OOP const * const me) {
	ECD_buf_st * const me_ = (ECD_buf_st *)me;
	if(me_->mode) //Hall Sensor
	{

		switch (me_->number_ecd)
		{
		case 0:
			hallEncoderInit(&hall1);
			setPulseZero(&hall1);
			break;
		case 1:
			hallEncoderInit(&hall2);
			setPulseZero(&hall2);
			break;
		case 2:
			hallEncoderInit(&hall3);
			setPulseZero(&hall3);
			break;
		case 3:
			hallEncoderInit(&hall4);
			setPulseZero(&hall4);
			break;
		}
	}
	(void)me_;  // avoid complier warning!
	return 0;
}

float unwrapped_angle(float new_angle, float prev_unwrapped){
    float ref_angle = fmodf(prev_unwrapped, 360.0f);
    if (ref_angle < 0) ref_angle += 360.0f;

    float delta = new_angle - ref_angle;

    if(delta >= 180.0f) delta -= 360.0f;
    if(delta <= -180.0f) delta += 360.0f;

    prev_unwrapped += delta;
    return prev_unwrapped;
}

float filtered_angle[4] = {0.0f};
float unwrapped[4] = {0.0f};
float unwrapped_angle_value=0;

float FirstOrderFilter(float raw_angle, float alpha, uint8_t ecdIndex)
{
	if(ecdIndex>3)	ecdIndex=3;
	float res = unwrapped_angle(raw_angle, unwrapped[ecdIndex]);
	unwrapped_angle_value=res;
	filtered_angle[ecdIndex] = alpha * res + (1.0f - alpha) * filtered_angle[ecdIndex];

	return fmodf(filtered_angle[ecdIndex],360.0f);

}

static void ECD_Process_(OOP const * const me){
	ECD_buf_st * const _me = (ECD_buf_st *)me;
	if(_me->mode) //Hall Sensor
	{
		switch(_me->number_ecd)
		{
		case 0:
			TempVal[_me->number_ecd] = getPulseCycle(&hall1);

			FilteredVal[_me->number_ecd] = FirstOrderFilter(TempVal[_me->number_ecd], 0.1f, _me->number_ecd);
			if(FilteredVal[_me->number_ecd] < 0)
			{
				FilteredVal[_me->number_ecd] += 360;
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			}
			else
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			_me->ecd_data = FilteredValUint32[_me->number_ecd];
			break;

		case 1:
			TempVal[_me->number_ecd] = getPulseCycle(&hall2);
			FilteredVal[_me->number_ecd] = FirstOrderFilter(TempVal[_me->number_ecd], 0.1f, _me->number_ecd);
			if(FilteredVal[_me->number_ecd] < 0)
			{
				FilteredVal[_me->number_ecd] += 360;
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			}
			else
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			_me->ecd_data = FilteredValUint32[_me->number_ecd];
			break;
		case 2:
			TempVal[_me->number_ecd] = getPulseCycle(&hall3);
			FilteredVal[_me->number_ecd] = FirstOrderFilter(TempVal[_me->number_ecd], 0.1f, _me->number_ecd);

			if(FilteredVal[_me->number_ecd] < 0)
			{
				FilteredVal[_me->number_ecd] += 360;
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			}
			else
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			_me->ecd_data = FilteredValUint32[_me->number_ecd];
			break;
		case 3:
			TempVal[_me->number_ecd] = getPulseCycle(&hall4);
			FilteredVal[_me->number_ecd] = FirstOrderFilter(TempVal[_me->number_ecd], 0.1f, _me->number_ecd);

			if(FilteredVal[_me->number_ecd] < 0)
			{
				FilteredVal[_me->number_ecd] += 360;
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			}
			else
				FilteredValUint32[_me->number_ecd] = (uint32_t)(FilteredVal[_me->number_ecd]);
			_me->ecd_data = FilteredValUint32[_me->number_ecd];
			break;
		}
	}
}
