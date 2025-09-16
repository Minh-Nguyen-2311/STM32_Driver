/*
 * Delay.c
 *
 *  Created on: Sep 7, 2025
 *      Author: Admin
 */

#include "Delay.h"

void Delay_us(__IO uint32_t nCount)
{
  volatile int32_t i = 7.8f*nCount;
  while(i--){};
}
void Delay_ms(__IO uint32_t nCount)
{
  volatile int32_t i = 8000*nCount;
  while(i--){};
}
