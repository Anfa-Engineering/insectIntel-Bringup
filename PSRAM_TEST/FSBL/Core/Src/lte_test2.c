/*
 * lte_test.c
 *
 *  Created on: 26 Feb 2026
 *      Author: jorda
 */


/*
 * sd-card-test.c
 *
 *  Created on: 25 Feb 2026
 *      Author: jorda
 */


#include "main.h"
#include  <stdio.h>
#include <string.h>
#include "usart.h"



#ifdef LTE_TEST_UART3
#define RX_BUFFER_SIZE 128U

uint8_t rx1_byte;
uint8_t rx3_byte;
uint8_t usart1_buffer[RX_BUFFER_SIZE];
uint8_t usart3_buffer[RX_BUFFER_SIZE];

uint8_t usart1_index = 0;
volatile uint16_t usart3Head = 0;
volatile uint16_t usart3Tail = 0;
volatile uint16_t usart3Tail2 = 0;

extern uint8_t changebauderate;

//Circular buffer implementation function prototypes
static uint8_t  rb_empty(void);
static uint16_t rb_available(void);
static void     rb_put(uint8_t b);

void lte_test3(void){
		//
		MX_USART3_UART_Init();


		/* Start reception of 1 byte on each UART */
//		HAL_UART_Receive_IT(&huart1, &rx1_byte, 1);
//		HAL_UART_Receive_IT(&huart4, &rx1_byte, 1);
		HAL_UART_Receive_IT(&huart3, &rx1_byte, 1);
//		HAL_UART_Receive_IT(&huart4, &rx3_byte, 1);

		while(1){


		    if (changebauderate){
		    	HAL_StatusTypeDef hal_status;

		    	changebauderate = 0;

		    	char * atMainBaudrate_cfg = "ATI\r";
		    	hal_status = HAL_UART_Transmit(&huart3, (uint8_t *)atMainBaudrate_cfg, strlen(atMainBaudrate_cfg), 100U);
		    	if (hal_status != HAL_OK) {
		    	    Error_Handler();
		    	}

//		    	//Wait for the okay signal
		    	HAL_Delay(1000U);


		    	char * atMainBaudrate_cfg1 = "AT\r";
		    	hal_status = HAL_UART_Transmit(&huart3, (uint8_t *)atMainBaudrate_cfg1, strlen(atMainBaudrate_cfg1), 100U);
		    	if (hal_status != HAL_OK) {
		    	    Error_Handler();
		    	}

//		    	hal_status = HAL_UART_DeInit(&huart3);
//		    	if (hal_status  != HAL_OK) {
//		    	    Error_Handler();
//		    	}
//
//		    	//Change the baudrate
//		    	huart3.Init.BaudRate = 921600U;
//
//		    	//Reinitilize
//		    	hal_status = HAL_UART_Init(&huart3);
//		    	if (hal_status != HAL_OK) {
//		    	    Error_Handler();
//		    	}
//
//		    	HAL_NVIC_EnableIRQ(USART3_IRQn);
//		    	HAL_Delay(1000U);
//
//		    	hal_status = HAL_UART_Receive_IT(&huart3, &rx3_byte, 1U);
//
//		    	if (hal_status != HAL_OK) {
//		    	    Error_Handler();
//		    	}

		    }
		}

	}

/* Callback when a byte is received */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if (huart->Instance == USART3)
    {
        // Store byte
        usart1_buffer[usart1_index++] = rx1_byte;

        // Check for carriage return
        if (rx1_byte == '\r' || usart1_index == RX_BUFFER_SIZE - 1U)
        {
            // Transmit buffer to USART3
            HAL_UART_Transmit_IT(&huart3, usart1_buffer, usart1_index);

            // Reset buffer index
            usart1_index = 0U;
        }

    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{


    // Restart reception
    HAL_UART_Receive_IT(&huart3, &rx1_byte, 1U);
}

static inline uint8_t rb_empty(void)
{
    return usart3Head == usart3Tail;
}

static inline uint16_t rb_available(void)
{
    if (usart3Head >= usart3Tail)
        return usart3Head - usart3Tail;
    else
        return RX_BUFFER_SIZE - usart3Tail;
}

static inline void rb_put(uint8_t b)
{
    uint16_t next = (usart3Head + 1) % RX_BUFFER_SIZE;
    if (next != usart3Tail) {
        usart3_buffer[usart3Head] = b;
        usart3Head = next;
    }else{
        // else overflow → drop byte or set flag
    	Error_Handler();
    }
}


#endif
