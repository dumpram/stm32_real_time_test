#include <stdbool.h>

#include "test.h"
#include "gpio.h"
#include "tim.h"

int delay_cnt = 0;
int delays[SAMPLE_NUM] = {0};

long long g_sample_start_time;
bool g_record_finished;

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
        delays[delay_cnt++] = get_clock_monotonic() - g_sample_start_time;
    } else
    {
        g_record_finished = true;
    }
}

/**
 * Creates input signal for system under test. System under test is probed by
 * output signal on GPIO_PIN_13 on PORTD.
 */
void TIM1_UP_TIM10_IRQHandler()
{
    __HAL_TIM_CLEAR_IT(&htim1, TIM_IT_UPDATE);

    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13) == GPIO_PIN_RESET)
    {
        // at end of this ISR rising edge will occur, therefore record time
        g_sample_start_time = get_clock_monotonic();
    }

    HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
}

void dump_sample_times()
{
    for (int i = 0; i < SAMPLE_NUM; i++)
    {
        printf("delays[%d] = %d\r\n", i, delays[i]);
    }
}