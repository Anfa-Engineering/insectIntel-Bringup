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
#include "usart.h"

#ifdef LTE_TEST_UART
#define RX_BUFFER_SIZE 128U

uint8_t rx1_byte;
uint8_t rx3_byte;
uint8_t usart1_buffer[RX_BUFFER_SIZE];
uint8_t usart3_buffer[RX_BUFFER_SIZE];

uint8_t usart1_index = 0;
volatile uint16_t usart3Head = 0;
volatile uint16_t usart3Tail = 0;
volatile uint16_t usart3Tail2 = 0;

//Circular buffer implementation function prototypes
static uint8_t  rb_empty(void);
static uint16_t rb_available(void);
static void     rb_put(uint8_t b);


void lte_test(void){
		//
		MX_USART3_UART_Init();

		//Power the LTE
		HAL_GPIO_WritePin(LTE_PWR_EN_GPIO_Port, LTE_PWR_EN_Pin, GPIO_PIN_SET);
		HAL_Delay(50U);
		HAL_GPIO_WritePin(LTE_PWR_KEY_GPIO_Port, LTE_PWR_KEY_Pin, GPIO_PIN_SET);
		HAL_Delay(2000U);
		HAL_GPIO_WritePin(LTE_DTR_GPIO_Port, LTE_DTR_Pin, GPIO_PIN_RESET);

		/* Start reception of 1 byte on each UART */
		HAL_UART_Receive_IT(&huart1, &rx1_byte, 1);
		HAL_UART_Receive_IT(&huart3, &rx3_byte, 1);

		while(1){

		    if (huart1.gState == HAL_UART_STATE_READY && !rb_empty())
		    {
		        uint16_t len = rb_available();

		        if( HAL_UART_Transmit_IT(&huart1,(uint8_t *)&usart3_buffer[usart3Tail],len) != HAL_OK){
		        	Error_Handler();
		        }else {
					usart3Tail2 =  (usart3Tail + len) % RX_BUFFER_SIZE;
				}

		    }
		}

	}

/* Callback when a byte is received */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
//	sign of life
	HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
    if (huart->Instance == USART1)
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

        // Restart reception
        HAL_UART_Receive_IT(&huart1, &rx1_byte, 1U);
    }else if (huart->Instance == USART3){
        rb_put(rx3_byte);  // store byte safely
        HAL_UART_Receive_IT(&huart3, &rx3_byte, 1); // restart RX
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{

	//	Sign of life
	HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
    if (huart->Instance == USART1)
    {
        uint16_t len = rb_available();

        usart3Tail = usart3Tail2;

        // Send next chunk if data wrapped or more arrived
        if (!rb_empty())
        {
            len = rb_available();
            if( HAL_UART_Transmit_IT(&huart1,(uint8_t *)&usart3_buffer[usart3Tail],len) != HAL_OK){
            	Error_Handler();
            }else {
    			usart3Tail2 =  (usart3Tail + len) % RX_BUFFER_SIZE;
    		}
        }
    }
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
