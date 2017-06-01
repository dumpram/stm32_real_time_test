#include "main.h"
#include "stm32f4xx_hal.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "test.h"

void SystemClock_Config(void);
void Error_Handler(void);

static void platform_init()
{
    HAL_Init();
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM4_Init();
    MX_USART6_UART_Init();
}

int main(void)
{
    platform_init();

    printf("SystemCoreClock freq is: %d Hz\r\n", (int)HAL_RCC_GetHCLKFreq());
    printf("GPIO latency test application...\r\n");
    printf("Enter frequency in kHz...\r\n");

    int freq = 300; // default frequency in kHz
    scanf("%d", &freq);

    printf("You entered %d kHz\r\n", freq);
    printf("Press any key to continue...\r\n");
    getchar();

    printf("Starting TIM4 (monotonic clock)...\r\n");
    HAL_TIM_Base_Start_IT(&htim4);

    printf("Starting TIM1 (%d kHz square wave)...\r\n", freq);
    MX_TIM1_Init(freq);
    HAL_TIM_Base_Start_IT(&htim1);

    while(!g_record_finished);

    dump_sample_times();

    while(1);
}

void SystemClock_Config(void)
{

    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 4;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }

    /**Configure the Systick interrupt time 
    */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


void Error_Handler(void)
{
    while(1) 
    {
    
    }

}

#ifdef USE_FULL_ASSERT


void assert_failed(uint8_t* file, uint32_t line)
{
    
}

#endif
