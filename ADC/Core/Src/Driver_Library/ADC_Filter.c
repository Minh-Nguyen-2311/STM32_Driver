#include "ADC_Filter.h"

uint32_t filterValue = 0;

void kalman_init(kalman_t *k, double init_x, double init_P, double Q, double R)
{
    k->x = init_x;
    k->P = init_P;
    k->Q = Q;
    k->R = R;
}

double kalman_update(kalman_t *k, double z)
{
    // predict
    double x_pred = k->x;
    double P_pred = k->P + k->Q;

    // update
    double K = P_pred / (P_pred + k->R);
    k->x = x_pred + K * (z - x_pred);
    k->P = (1.0 - K) * P_pred;
    return k->x;
}

uint16_t hysteresis_round(uint16_t currentInt, float filtVal, float hyst)
{
	float mid = currentInt +0.5f;
	if(filtVal>= mid + hyst)	currentInt+=(uint16_t)floorf(filtVal-mid);
	else if(filtVal<= mid - hyst)	currentInt-=(uint16_t)floorf(mid-filtVal);
	return currentInt;
}

uint32_t LowPassFilter(uint32_t adc_value)
{
	filterValue = ALPHA * filterValue + (1-ALPHA) * adc_value;
	return adc_value;
}
