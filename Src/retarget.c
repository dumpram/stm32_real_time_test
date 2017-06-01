#include <stdbool.h>

#include "usart.h"

static bool byte_received;

void retarget_init()
{
    // Initialize UART
}

int _write (int fd, char *ptr, int len)
{

    HAL_UART_Transmit(&huart6, (uint8_t *)ptr, len, 100);
    return len;
}

int _read (int fd, char *ptr, int len)
{
    int rcnt = 0;
    uint8_t byte;
    while (rcnt < len && byte != '\r')
    {
        HAL_UART_Receive_IT(&huart6, &byte, 1);
        while (!byte_received);
        HAL_UART_Transmit(&huart6, &byte, 1, 100);
        byte_received = false;
        ptr[rcnt++] = (char) byte;
    }
    ptr[rcnt] = 0;
    return rcnt;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    byte_received = true;
}

void _ttywrch(int ch) {
    /* Write one char "ch" to the default console
     * Need implementing with UART here. */
}


