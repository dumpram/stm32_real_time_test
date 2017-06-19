#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx_hal.h"
#include "main.h"

#define CLOCK_MONOTONIC_PERIOD 65500

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim4;


extern void Error_Handler(void);

void MX_TIM1_Init(int freq, int duty);

void MX_TIM4_Init(void);

long long get_clock_monotonic();

                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                


#ifdef __cplusplus
}
#endif
#endif
