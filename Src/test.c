#include <stdbool.h>
#include <stdlib.h>

#include "test.h"
#include "gpio.h"
#include "tim.h"

int delay_cnt = 0;
unsigned int delays[SAMPLE_NUM] = {0};

bool g_record_finished;
int g_num_samples;

static bool sample_locked = false;
static unsigned int overflow_cnt = 0;

/**
 * Interrupt occurs on rising edge on GPIO_PIN_14 on PORTD.
 */
void EXTI15_10_IRQHandler(void)
{
    // clear interrupt
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);

    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_14) == GPIO_PIN_RESET)
    {
        if (delay_cnt < g_num_samples)
        {
            // calculate sample delay
            if (!sample_locked)
            {
                delays[delay_cnt++] = __HAL_TIM_GET_COUNTER(&htim4);
                __HAL_TIM_SET_COUNTER(&htim4, 0);
                HAL_TIM_Base_Stop(&htim4);
                sample_locked = true; 
            }
        } else
        {
            g_record_finished = true;
        }
    }
}

void TIM1_CC_IRQHandler()
{
    __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC1);

    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_RESET) 
    {
        if (sample_locked)
        {
            HAL_TIM_Base_Start(&htim4);
            sample_locked = false;
        }
    }

}

void TIM4_IRQHandler()
{
    __HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);
    overflow_cnt++;
}



void dump_sample_times()
{
    int mean = 0;
    int dev = 0;
    int max_dev = 0;

    if (DEBUG_MODE == 1)
    {
        for (int i = 0; i < g_num_samples; i++)
        {
            if (delays[i] > 10000 || delays[i] < 0)
            {
                printf("Big deviation on %d: %u\r\n", i,  delays[i]);
                delays[i] = 10000;
            } else if (delays[i] > max_dev) {
                max_dev = delays[i];
            }
            mean += delays[i];
        }

        mean /= g_num_samples;

        for (int i = 0; i < g_num_samples; i++)
        {
            delays[i] = abs(mean - delays[i]);
        }
        for (int i = 0; i < g_num_samples; i++)
        {
            dev += delays[i];
        }
        
        dev /= g_num_samples;

        dprintf("mean = %u, dev = %u, max_dev = %u\r\n", mean, dev, max_dev);
    }
    else 
    {
        for (int i = 0; i < g_num_samples; i++)
        {
            // print to result script 
            printf("%u\r\n", delays[i]);
        }
    }

}