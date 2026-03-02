/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_usbx_host.c
  * @author  MCD Application Team
  * @brief   USBX host applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "app_usbx_host.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usb_otg.h"
#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//Connected class CDC instance
UX_HOST_CLASS_CDC_ACM *connectedHClass = NULL;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance);
static VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code);

extern HCD_HandleTypeDef hhcd_USB_OTG_HS1;

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/**
  * @brief  Application USBX Host Initialization.
  * @param  none
  * @retval status
  */
UINT MX_USBX_Host_Init(VOID)
{
  UINT ret = UX_SUCCESS;

  /* USER CODE BEGIN MX_USBX_Host_Init0 */

  /* USER CODE END MX_USBX_Host_Init0 */
  /* Initialize the Stack Host USB*/
  if (MX_USBX_Host_Stack_Init() != UX_SUCCESS)
  {
    /* USER CODE BEGIN MAIN_INITIALIZE_STACK_ERROR */
    return UX_ERROR;
    /* USER CODE END MAIN_INITIALIZE_STACK_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Host_Init 1 */

  //Inititlise PWR_USB1_EN_GPIO_Port
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /*Configure GPIO pin Output Level */
  /*Configure GPIO pin Output Level */

  HAL_GPIO_WritePin(UCPD_PWR_EN_GPIO_Port, UCPD_PWR_EN_Pin, GPIO_PIN_RESET);
  /*Configure GPIO pin : PWR_USB2_EN_Pin */
  GPIO_InitStruct.Pin = UCPD_PWR_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(UCPD_PWR_EN_GPIO_Port, &GPIO_InitStruct);

  //USB1_OTG_initialisation
  MX_USB1_OTG_HS_HCD_Init();

  /* USER CODE END MX_USBX_Host_Init 1 */

  /* USER CODE BEGIN MX_USBX_Host_Init 2 */
  //Start the host controller
  HAL_HCD_Start(&hhcd_USB_OTG_HS1);
  HAL_Delay(10);
  /* USER CODE END MX_USBX_Host_Init 2 */

  return ret;
}

/**
  * @brief  ux_host_event_callback
  *         This callback is invoked to notify application of instance changes.
  * @param  event: event code.
  * @param  current_class: Pointer to class.
  * @param  current_instance: Pointer to class instance.
  * @retval status
  */
UINT ux_host_event_callback(ULONG event, UX_HOST_CLASS *current_class, VOID *current_instance)
{
  UINT status = UX_SUCCESS;

  /* USER CODE BEGIN ux_host_event_callback0 */
	  UX_PARAMETER_NOT_USED(current_class);
	  UX_PARAMETER_NOT_USED(current_instance);
  /* USER CODE END ux_host_event_callback0 */

  switch (event)
  {
    case UX_DEVICE_INSERTION:

      /* USER CODE BEGIN UX_DEVICE_INSERTION */
	    	if (current_class->ux_host_class_entry_function == ux_host_class_cdc_acm_entry){
	        	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
	        	connectedHClass = (UX_HOST_CLASS_CDC_ACM *)current_instance;
	        	printf("cdc_acm_entry\r\n");
	    	}

	    	printf("UX_DEVICE_INSERTION\r\n");
      /* USER CODE END UX_DEVICE_INSERTION */

      break;

    case UX_DEVICE_REMOVAL:

      /* USER CODE BEGIN UX_DEVICE_REMOVAL */
	        if ((VOID*)connectedHClass == current_instance)
	        {
	        	connectedHClass = NULL;
	        	HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
	        	printf("UX_DEVICE_REMOVAL\r\n");
	        }


      /* USER CODE END UX_DEVICE_REMOVAL */

      break;

    case UX_DEVICE_CONNECTION:

      /* USER CODE BEGIN UX_DEVICE_CONNECTION */

      /* USER CODE END UX_DEVICE_CONNECTION */

      break;

    case UX_DEVICE_DISCONNECTION:

      /* USER CODE BEGIN UX_DEVICE_DISCONNECTION */

      /* USER CODE END UX_DEVICE_DISCONNECTION */

      break;

    default:

      /* USER CODE BEGIN EVENT_DEFAULT */
    	if(current_class || current_instance){
    		printf("Default handler\r\n");

    	}
      /* USER CODE END EVENT_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_event_callback1 */

  /* USER CODE END ux_host_event_callback1 */

  return status;
}

/**
  * @brief  ux_host_error_callback
  *         This callback is invoked to notify application of error changes.
  * @param  system_level: system level parameter.
  * @param  system_context: system context code.
  * @param  error_code: error event code.
  * @retval Status
  */
VOID ux_host_error_callback(UINT system_level, UINT system_context, UINT error_code)
{
  /* USER CODE BEGIN ux_host_error_callback0 */
  UX_PARAMETER_NOT_USED(system_level);
  UX_PARAMETER_NOT_USED(system_context);
  /* USER CODE END ux_host_error_callback0 */

  switch (error_code)
  {
    case UX_DEVICE_ENUMERATION_FAILURE:

      /* USER CODE BEGIN UX_DEVICE_ENUMERATION_FAILURE */

      /* USER CODE END UX_DEVICE_ENUMERATION_FAILURE */

      break;

    case  UX_NO_DEVICE_CONNECTED:

      /* USER CODE BEGIN UX_NO_DEVICE_CONNECTED */

      /* USER CODE END UX_NO_DEVICE_CONNECTED */

      break;

    default:

      /* USER CODE BEGIN ERROR_DEFAULT */
    	printf("error_code : 0x%0x\r\n", error_code);

      /* USER CODE END ERROR_DEFAULT */

      break;
  }

  /* USER CODE BEGIN ux_host_error_callback1 */

  /* USER CODE END ux_host_error_callback1 */
}

/**
  * @brief MX_USBX_Host_Stack_Init
  *        Initialization of USB host stack.
  *        Init USB Host stack, add register the host class stack
  * @retval None
  */
UINT MX_USBX_Host_Stack_Init(void)
{
  UINT ret = UX_SUCCESS;
  /* USER CODE BEGIN MX_USBX_Host_Stack_Init_PreTreatment_0 */
  /* USER CODE END MX_USBX_Host_Stack_Init_PreTreatment_0 */

  /* The code below is required for installing the host portion of USBX.  */
  if (ux_host_stack_initialize(ux_host_event_callback) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* Register a callback error function */
  ux_utility_error_callback_register(&ux_host_error_callback);

  /* Initialize the host cdc acm class */
  if ((ux_host_stack_class_register(_ux_system_host_class_cdc_acm_name,
                                    ux_host_class_cdc_acm_entry)) != UX_SUCCESS)
  {
    /* USER CODE BEGIN USBX_HOST_CDC_ACM_REGISTER_ERROR */
    return UX_ERROR;
    /* USER CODE END USBX_HOST_CDC_ACM_REGISTER_ERROR */
  }

  /* USER CODE BEGIN MX_USBX_Host_Stack_Init_PreTreatment_1 */


  /* Register all the USB host controllers available in this system. */
  if (ux_host_stack_hcd_register(_ux_system_host_hcd_stm32_name,
                                 _ux_hcd_stm32_initialize,
								 USB1_OTG_HS_BASE,
                                 (ULONG)&hhcd_USB_OTG_HS1) != UX_SUCCESS)

  {
	  ret = UX_ERROR;
  }

  /* USER CODE END MX_USBX_Host_Stack_Init_PreTreatment_1 */

  /* USER CODE BEGIN MX_USBX_Host_Stack_Init_PostTreatment */
  /* USER CODE END MX_USBX_Host_Stack_Init_PostTreatment */
  return ret ;
}

/**
  * @brief  MX_USBX_Host_Stack_DeInit
  *         Uninitialize of USB Host stack.
  *         Uninitialize the host stack, unregister of host class stack and
  *         unregister of the usb host controllers
  * @retval None
  */
UINT MX_USBX_Host_Stack_DeInit(void)
{
  UINT ret = UX_SUCCESS;

  /* USER CODE BEGIN MX_USBX_Host_Stack_DeInit_PreTreatment_0 */

  /* USER CODE END MX_USBX_Host_Stack_DeInit_PreTreatment_0 */

  /* Unregister all the USB host controllers available in this system. */
  ux_host_stack_hcd_unregister(_ux_system_host_hcd_stm32_name,
                               USB1_OTG_HS_BASE,
                               (ULONG)&hhcd_USB_OTG_HS1);

  /* Unregister the host cdc acm class */
  if (ux_host_stack_class_unregister(ux_host_class_cdc_acm_entry) != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* The code below is required for uninstalling the host portion of USBX.  */
  if (ux_host_stack_uninitialize() != UX_SUCCESS)
  {
    return UX_ERROR;
  }

  /* USER CODE BEGIN MX_USBX_Host_Stack_DeInit_PreTreatment_1 */
  /* USER CODE END MX_USBX_Host_Stack_DeInit_PreTreatment_1 */

  /* USER CODE BEGIN MX_USBX_Host_Stack_DeInit_PostTreatment */
  /* USER CODE END MX_USBX_Host_Stack_DeInit_PostTreatment */
  return ret ;
}
/* USER CODE BEGIN 1 */
uint32_t USBD_CDC_ACM_Transmit(uint8_t* buffer, uint32_t size, uint32_t* sent){
    UINT retVal;
    	if(connectedHClass!=NULL){
		  do
		  {
			retVal = ux_host_class_cdc_acm_write(connectedHClass, buffer, size, sent);
		  }while(UX_STATE_NEXT != retVal);

		  return 0;

		}else{
			(void) retVal;
			return 1;

		}
}

uint32_t USBD_CDC_ACM_Receive(uint8_t* buffer, uint32_t size, uint32_t* received){
	  if(connectedHClass != NULL){
		  ux_host_class_cdc_acm_read(connectedHClass, buffer, size, received);
		return 0;
	  }else{
	   return 1;
	  }
}

VOID USBX_Host_Process(VOID *arg)
{
  ux_host_stack_tasks_run();
}

/* USER CODE END 1 */
