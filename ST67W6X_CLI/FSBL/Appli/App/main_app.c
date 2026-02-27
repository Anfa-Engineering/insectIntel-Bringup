/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    main_app.c
  * @author  GPM Application Team
  * @brief   main_app program body
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
#include <inttypes.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Application */
#include "main.h"
#include "main_app.h"
#include "app_config.h"

#if (LOW_POWER_MODE > LOW_POWER_DISABLE)
#include "utilities_conf.h"
#include "stm32_lpm.h"
#endif /* LOW_POWER_MODE */

#include "w6x_api.h"
#include "spi_iface.h" /* SPI falling/rising_callback */
#include "logging.h"
#include "shell.h"
#include "logshell_ctrl.h"
#if (LOW_POWER_MODE == LOW_POWER_STDBY_ENABLE)
#error "low power standby mode not supported"
#endif /* LOW_POWER_MODE */

/* USER CODE BEGIN Includes */
#include "common_parser.h" /* Common Parser functions */
#include "https_client.h"

//Free Rtos includes
#include "app_freertos.h"
#include "event_groups.h"

/* USER CODE END Includes */

/* Global variables ----------------------------------------------------------*/
/* USER CODE BEGIN GV */

/* USER CODE END GV */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define EVENT_FLAG_SCAN_DONE   (1<<1)             /*!< Scan done event bitmask */

#define WIFI_SCAN_TIMEOUT      10000              /*!< Delay before to declare the scan in failure */

/** SSID of the local Access Point */
#define WIFI_SSID                   "Kamitek Solar"

/** Password of the local Access Point */
#define WIFI_PASSWORD               "KamAnfa!2025@18"

/** Define the default factor to apply to AP DTIM interval when connected and power save mode is enabled */
#define WIFI_DTIM                   1

/* USER CODE END PD */

/* Private macros ------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
static EventGroupHandle_t scan_event_flags = NULL;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/**
  * @brief  Wi-Fi event callback
  * @param  event_id: Event ID
  * @param  event_args: Event arguments
  */
static void APP_wifi_cb(W6X_event_id_t event_id, void *event_args);

/**
  * @brief  Network event callback
  * @param  event_id: Event ID
  * @param  event_args: Event arguments
  */
static void APP_net_cb(W6X_event_id_t event_id, void *event_args);

/**
  * @brief  MQTT event callback
  * @param  event_id: Event ID
  * @param  event_args: Event arguments
  */
static void APP_mqtt_cb(W6X_event_id_t event_id, void *event_args);

/**
  * @brief  BLE event callback
  * @param  event_id: Event ID
  * @param  event_args: Event arguments
  */
static void APP_ble_cb(W6X_event_id_t event_id, void *event_args);

/**
  * @brief  W6X error callback
  * @param  ret_w6x: W6X status
  * @param  func_name: function name
  */
static void APP_error_cb(W6X_Status_t ret_w6x, char const *func_name);

/* USER CODE BEGIN PFP */
static void APP_wifi_scan_cb(int32_t status, W6X_WiFi_Scan_Result_t *Scan_results);

/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void main_app(void)
{
  int32_t ret = 0;

  /* USER CODE BEGIN main_app_1 */

  /* Wi-Fi variables */
  W6X_WiFi_Scan_Opts_t Opts = {0};
  W6X_WiFi_Connect_Opts_t ConnectOpts = {0};
  W6X_WiFi_Connect_t connectData = {0};
  W6X_WiFi_StaStateType_e state = W6X_WIFI_STATE_STA_OFF;

  /* USER CODE END main_app_1 */

  /* Initialize the logging utilities */
  LoggingInit();

  /* Initialize the shell utilities on UART instance */
  ShellInit();

  /* USER CODE BEGIN main_app_2 */

  /* USER CODE END main_app_2 */

  /* Register the application callback to received events from ST67W6X Driver */
  W6X_App_Cb_t App_cb = {0};
  App_cb.APP_wifi_cb = APP_wifi_cb;
  App_cb.APP_net_cb = APP_net_cb;
  App_cb.APP_ble_cb = APP_ble_cb;
  App_cb.APP_mqtt_cb = APP_mqtt_cb;
  App_cb.APP_error_cb = APP_error_cb;
  W6X_RegisterAppCb(&App_cb);

  /* Initialize the ST67W6X Driver */
  ret = W6X_Init();
  if (ret)
  {
    LogError("Failed to initialize ST67W6X Driver, %" PRIi32 "\n", ret);
    goto _err;
  }




  /****************************************************************************************************/
  /****************************************************************************************************/
    while(1){// the program is halted here for testing purposes
  	  osDelay(1000U);

    }





  /* Initialize the ST67W6X Wi-Fi module */
  ret = W6X_WiFi_Init();
  if (ret)
  {
    LogError("Failed to initialize ST67W6X Wi-Fi component, %" PRIi32 "\n", ret);
    goto _err;
  }
  LogInfo("Wi-Fi init is done\n");

  /* Initialize the ST67W6X Network module */
  ret = W6X_Net_Init();
  if (ret)
  {
    LogError("Failed to initialize ST67W6X Net component, %" PRIi32 "\n", ret);
    goto _err;
  }
  LogInfo("Net init is done\n");

  /* USER CODE BEGIN main_app_3 */

  /* Run a Wi-Fi scan to retrieve the list of all nearby Access Points */
  scan_event_flags = xEventGroupCreate();
  W6X_WiFi_Scan(&Opts, &APP_wifi_scan_cb);

  /* Wait to receive the EVENT_FLAG_SCAN_DONE event. The scan is declared as failed after 'ScanTimeout' delay */
  if ((int32_t)xEventGroupWaitBits(scan_event_flags, EVENT_FLAG_SCAN_DONE, pdTRUE, pdFALSE,
                                   pdMS_TO_TICKS(WIFI_SCAN_TIMEOUT)) != EVENT_FLAG_SCAN_DONE)
  {
    LogError("Scan Failed\n");
    goto _err;
  }

  /* Connect the device to the pre-defined Access Point */
  LogInfo("\nConnecting to Local Access Point\n");
  strncpy((char *)ConnectOpts.SSID, WIFI_SSID, W6X_WIFI_MAX_SSID_SIZE);
  strncpy((char *)ConnectOpts.Password, WIFI_PASSWORD, W6X_WIFI_MAX_PASSWORD_SIZE);
  ret = W6X_WiFi_Connect(&ConnectOpts);
  if (ret)
  {
    LogError("Failed to connect, %" PRIi32 "\n", ret);
    goto _err;
  }

  LogInfo("App connected\n");
  if (W6X_WiFi_Station_GetState(&state, &connectData) != W6X_STATUS_OK)
  {
    LogError("Failed to get Station state\n");
    goto _err;
  }

  LogInfo("Connected to following Access Point :\n");

  LogInfo("[" MACSTR "] Channel: %" PRIu32 " | RSSI: %" PRIi32 " | SSID: %s\n",
          MAC2STR(connectData.MAC),
          connectData.Channel,
          connectData.Rssi,
          connectData.SSID);

  /* Define the default factor to apply to AP DTIM interval when connected */
  ret = W6X_WiFi_SetDTIM(WIFI_DTIM);
  if (ret)
  {
    LogError("Failed to initialize the DTIM, %" PRIi32 "\n", ret);
    goto _err;
  }

//  Post something binary
  int err = https_client_post();
  if (err){
	  LogInfo("##### Post Error\n");
	  Error_Handler();
  }
//The application should hand here
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  while(1);

  /* USER CODE END main_app_3 */

  LogInfo("##### Quitting the application\n");

  /* USER CODE BEGIN main_app_Last */

  /* USER CODE END main_app_Last */

_err:
  /* USER CODE BEGIN main_app_Err_1 */

  /* USER CODE END main_app_Err_1 */
  /* De-initialize the ST67W6X Network module */
  W6X_Net_DeInit();

  /* De-initialize the ST67W6X Wi-Fi module */
  W6X_WiFi_DeInit();

  /* De-initialize the ST67W6X Driver */
  W6X_DeInit();

  shell_freertos_deinit();

  /* USER CODE BEGIN main_app_Err_2 */

  /* USER CODE END main_app_Err_2 */
  LogInfo("##### Application end\n");
}

void HAL_GPIO_EXTI_Callback(uint16_t pin);
void HAL_GPIO_EXTI_Rising_Callback(uint16_t pin);
void HAL_GPIO_EXTI_Falling_Callback(uint16_t pin);

void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
  /* USER CODE BEGIN HAL_GPIO_EXTI_Callback_1 */

  /* USER CODE END HAL_GPIO_EXTI_Callback_1 */
  /* Callback when data is available in Network CoProcessor to enable SPI Clock */
  if (pin == SPI_RDY_Pin)
  {
    if (HAL_GPIO_ReadPin(SPI_RDY_GPIO_Port, SPI_RDY_Pin) == GPIO_PIN_SET)
    {
      HAL_GPIO_EXTI_Rising_Callback(pin);
    }
    else
    {
      HAL_GPIO_EXTI_Falling_Callback(pin);
    }
  }
  /* USER CODE BEGIN HAL_GPIO_EXTI_Callback_End */

  /* USER CODE END HAL_GPIO_EXTI_Callback_End */
}

void HAL_GPIO_EXTI_Rising_Callback(uint16_t pin)
{
  /* USER CODE BEGIN EXTI_Rising_Callback_1 */

  /* USER CODE END EXTI_Rising_Callback_1 */
  /* Callback when data is available in Network CoProcessor to enable SPI Clock */
  if (pin == SPI_RDY_Pin)
  {
    spi_on_txn_data_ready();
  }
  /* USER CODE BEGIN EXTI_Rising_Callback_End */

  /* USER CODE END EXTI_Rising_Callback_End */
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t pin)
{
  /* USER CODE BEGIN EXTI_Falling_Callback_1 */

  /* USER CODE END EXTI_Falling_Callback_1 */
  /* Callback when data is available in Network CoProcessor to enable SPI Clock */
  if (pin == SPI_RDY_Pin)
  {
    spi_on_header_ack();
  }
  /* USER CODE BEGIN EXTI_Falling_Callback_End */

  /* USER CODE END EXTI_Falling_Callback_End */
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/* Private Functions Definition ----------------------------------------------*/
static void APP_wifi_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_wifi_cb_1 */
	switch (event_id)
	{
	case W6X_WIFI_EVT_CONNECTED_ID:
	  break;

	case W6X_WIFI_EVT_DISCONNECTED_ID:
	  LogInfo("Station disconnected from Access Point\n");
	  break;

	case W6X_WIFI_EVT_REASON_ID:
	  LogInfo("Reason: %s\n", W6X_WiFi_ReasonToStr(event_args));
	  break;

	default:
	  break;
	}
  /* USER CODE END APP_wifi_cb_1 */
}

static void APP_net_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_net_cb_1 */

  /* USER CODE END APP_net_cb_1 */
}

static void APP_mqtt_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_mqtt_cb_1 */

  /* USER CODE END APP_mqtt_cb_1 */
}

static void APP_ble_cb(W6X_event_id_t event_id, void *event_args)
{
  /* USER CODE BEGIN APP_ble_cb_1 */

  /* USER CODE END APP_ble_cb_1 */
}

static void APP_error_cb(W6X_Status_t ret_w6x, char const *func_name)
{
  /* USER CODE BEGIN APP_error_cb_1 */

  /* USER CODE END APP_error_cb_1 */
  LogError("[%s] in %s API\n", W6X_StatusToStr(ret_w6x), func_name);
  /* USER CODE BEGIN APP_error_cb_2 */

  /* USER CODE END APP_error_cb_2 */
}

/* USER CODE BEGIN PFD */

static void APP_wifi_scan_cb(int32_t status, W6X_WiFi_Scan_Result_t *Scan_results){
  LogInfo("SCAN DONE\n");
  LogInfo(" Cb informed APP that WIFI SCAN DONE.\n");
  W6X_WiFi_PrintScan(Scan_results);
  xEventGroupSetBits(scan_event_flags, EVENT_FLAG_SCAN_DONE);

}
/* USER CODE END PFD */
