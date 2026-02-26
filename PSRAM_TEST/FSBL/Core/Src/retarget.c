/*
 * retarget.c
 *
 *  Created on: Oct 28, 2025
 *      Author: jorda
 */
#include "main.h"
#include "usart.h"
//#include "usbd_cdc_if.h"
#include <stdio.h>

//extern UX_SLAVE_CLASS_CDC_ACM  *cdc_acm;
////////////////////////////////////////////////
//int __io_putchar(int ch)
//{
//  if(HAL_UART_Transmit_IT(&huart1, (uint8_t *)&ch, 1) != HAL_OK){
//	  Error_Handler();
//  }
//  return ch;
//}
/////////////////////////////////////////////////

int _write(int file, char *ptr, int len)
{

	(void)file;
//  if(HAL_UART_Transmit_IT(&huart1, (uint8_t *)ptr, len) != HAL_OK){
//
	if(HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY) != HAL_OK){
	  Error_Handler();
	}

//	CDC_Transmit((uint8_t* )ptr, (uint32_t) len);

//	ULONG sent;
//	if(cdc_acm != NULL){
//		ux_device_class_cdc_acm_write_run(
//				cdc_acm,
//				(uint8_t *)ptr,
//				len,
//				&sent);
////		do
////		{
////			retVal = ux_device_class_cdc_acm_write_run(
////					cdc_acm,
////					(uint8_t *)ptr,
////					len,
////					sent
////					);
////
////		}while(UX_STATE_NEXT != retVal);
//
//	return (int)sent;
//
//	}else{
//
//		return (int)0;
//	}

//  for (DataIdx = 0; DataIdx < len; DataIdx++)
//  {
//    __io_putchar(*ptr++);
//  }
	return len;
}
