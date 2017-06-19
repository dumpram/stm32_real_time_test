#include <stdbool.h>

#include "test.h"
#include "gpio.h"
#include "tim.h"

int delay_cnt = 0;
int delays[SAMPLE_NUM] = {0};

long long g_sample_start_time;
bool g_record_finished;
bool sample_locked = false;

/**
 * Interrupt occurs on rising edge on GPIO_PIN_14 on PORTD.
 */
void EXTI15_10_IRQHandler(void)
{
    // clear interrupt
    __HAL_GPIO_EXTI_CLEAR_IT(GPIO_PIN_14);

    if (delay_cnt < SAMPLE_NUM)
    {
        // calculate sample delay
        //if (!sample_locked)
        //{
            delays[delay_cnt++] = get_clock_monotonic() - g_sample_start_time;
          //  sample_locked = true;   
        //}
    } else
    {
        g_record_finished = true;
    }
}

/**
 * Creates input signal for system under test. System under test is probed by
 * output signal on GPIO_PIN_13 on PORTD.
//  */
// void TIM1_UP_TIM10_IRQHandler()
// {
//     //__HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);
    
//     __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC1);

//     if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13) == GPIO_PIN_RESET)
//     {
//         // at end of this ISR rising edge will occur, therefore record time
//         if (!sample_locked)
//         {
//             // sample is missed
//         }
//         else
//         {
//             // new sample ready, record time
//             g_sample_start_time = get_clock_monotonic();
//             sample_locked = false;
//         }
//     }
//     GPIOD->ODR ^= (1 << 13);
//     //HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
// }

void TIM1_CC_IRQHandler()
{
    __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_CC1);

    if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_9) == GPIO_PIN_RESET)
    {
        // new sample ready, record time
        g_sample_start_time = get_clock_monotonic();
        sample_locked = false;
        GPIOD->ODR ^= (1 << 13);
    }
    
}

void dump_sample_times()
{
    for (int i = 0; i < SAMPLE_NUM; i++)
    {
        printf("delays[%d] = %d\r\n", i, delays[i]);
    }
}