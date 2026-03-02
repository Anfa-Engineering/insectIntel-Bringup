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
#include "usart.h"

#include  <stdio.h>
#include "app_usbx_host.h"
#include "usbxh_platform.h"
#include "app_usbx.h"

#ifdef LTE_TEST_USB

#define RX_BUFFER_SIZE 128U

uint32_t rx1_byte;
uint32_t rx3_byte;
uint8_t usart1_buffer[RX_BUFFER_SIZE];

uint8_t usart1_index = 0;





void lte_test_usb(void){

	  	MX_USBX_Init();

	//Inititlise PWR_USB1_EN_GPIO_Port
//	  GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//	  /*Configure GPIO pin Output Level */
//	  HAL_GPIO_WritePin(UCPD_PWR_EN_GPIO_Port, UCPD_PWR_EN_Pin, GPIO_PIN_RESET);
//
//	  /*Configure GPIO pin : PWR_USB2_EN_Pin */
//	  GPIO_InitStruct.Pin = UCPD_PWR_EN_Pin;
//	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//	  GPIO_InitStruct.Pull = GPIO_NOPULL;
//	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//	  HAL_GPIO_Init(UCPD_PWR_EN_GPIO_Port, &GPIO_InitStruct);
//	   HAL_GPIO_WritePin(UCPD_PWR_EN_GPIO_Port, UCPD_PWR_EN_Pin, GPIO_PIN_SET);
		//Drive the vbus pin high
		USBH_DriverVBUS(GPIO_PIN_SET);

		//Power the LTE
		HAL_GPIO_WritePin(LTE_PWR_EN_GPIO_Port, LTE_PWR_EN_Pin, GPIO_PIN_SET);
		HAL_Delay(50U);
		HAL_GPIO_WritePin(LTE_PWR_KEY_GPIO_Port, LTE_PWR_KEY_Pin, GPIO_PIN_SET);
		HAL_Delay(50U);
		HAL_GPIO_WritePin(LTE_DTR_GPIO_Port, LTE_DTR_Pin, GPIO_PIN_RESET);

		/* Start reception of 1 byte on each UART */
//		HAL_UART_Receive_IT(&huart1, &rx1_byte, 1);

		while(1){
			//USB DEVICE BACKGROUND STACK PROCESS
			USBX_Host_Process(NULL);

			USBD_CDC_ACM_Receive(&usart1_buffer[rx1_byte], 5U, &rx1_byte);
			if(usart1_buffer[rx1_byte] == '\r'){

				if( HAL_UART_Transmit(&huart1, usart1_buffer, rx1_byte + 1U,  HAL_MAX_DELAY) != HAL_OK){
					Error_Handler();
				}

				if(USBD_CDC_ACM_Transmit((uint8_t  *)"AT\n", 3U, &rx3_byte) == 0U){

					  USBD_CDC_ACM_Receive(&usart1_buffer[rx1_byte], 4U, &rx1_byte);
					  if(usart1_buffer[rx1_byte] == '\r'){

							if( HAL_UART_Transmit(&huart1, usart1_buffer, rx1_byte + 1U,  HAL_MAX_DELAY) != HAL_OK){
								Error_Handler();
							}
					  }
				}

			}

		}

	}



#endif
