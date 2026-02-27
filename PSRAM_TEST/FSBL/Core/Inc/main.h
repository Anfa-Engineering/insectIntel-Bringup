/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined ( __ICCARM__ )
#  define CMSE_NS_CALL  __cmse_nonsecure_call
#  define CMSE_NS_ENTRY __cmse_nonsecure_entry
#else
#  define CMSE_NS_CALL  __attribute((cmse_nonsecure_call))
#  define CMSE_NS_ENTRY __attribute((cmse_nonsecure_entry))
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32n6xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* Function pointer declaration in non-secure*/
#if defined ( __ICCARM__ )
typedef void (CMSE_NS_CALL *funcptr)(void);
#else
typedef void CMSE_NS_CALL (*funcptr)(void);
#endif

/* typedef for non-secure callback functions */
typedef funcptr funcptr_NS;

/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */
#define CAMERA_IMX335_ADDRESS 0x34U

#define BUFFER_ADDRESS  0x34200000U
//#define BUFFER_ADDRESS  0x90000000U

//Meduim Sized Image
#define BUFFER_HEIGHT 480U
#define BUFFER_WIDTH 800U

#define BUFFER_SIZE BUFFER_HEIGHT * BUFFER_WIDTH * 2U

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

#ifdef PSRAM_TEST
extern void psram_test(void);
#endif

#ifdef SD_CARD_TEST
extern void sdcard_test(void);
#endif

#ifdef CAM_TEST
extern void cam_test(void);
#endif

#ifdef LTE_TEST_UART
extern void lte_test(void);
#endif

#ifdef LTE_TEST_USB
extern void lte_test_usb(void);
#endif
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define I2C1_SDA_Pin GPIO_PIN_1
#define I2C1_SDA_GPIO_Port GPIOC
#define LTE_RST_Pin GPIO_PIN_15
#define LTE_RST_GPIO_Port GPIOE
#define PWR_EN_Pin GPIO_PIN_10
#define PWR_EN_GPIO_Port GPIOD
#define LTE_DTR_Pin GPIO_PIN_12
#define LTE_DTR_GPIO_Port GPIOE
#define CAM_EN_Pin GPIO_PIN_2
#define CAM_EN_GPIO_Port GPIOD
#define NRST_CAM_Pin GPIO_PIN_8
#define NRST_CAM_GPIO_Port GPIOC
#define I2CA_SCL_Pin GPIO_PIN_9
#define I2CA_SCL_GPIO_Port GPIOH
#define LTE_PWR_EN_Pin GPIO_PIN_6
#define LTE_PWR_EN_GPIO_Port GPIOD
#define TRACED1_Pin GPIO_PIN_0
#define TRACED1_GPIO_Port GPIOB
#define TRACED3_Pin GPIO_PIN_7
#define TRACED3_GPIO_Port GPIOB
#define TRACED2_Pin GPIO_PIN_6
#define TRACED2_GPIO_Port GPIOB
#define USER_BUTTON1_Pin GPIO_PIN_13
#define USER_BUTTON1_GPIO_Port GPIOC
#define TRACECLK_Pin GPIO_PIN_3
#define TRACECLK_GPIO_Port GPIOB
#define TRACED0_Pin GPIO_PIN_3
#define TRACED0_GPIO_Port GPIOE
#define PWR_SD_EN_Pin GPIO_PIN_7
#define PWR_SD_EN_GPIO_Port GPIOQ
#define SD_SEL_Pin GPIO_PIN_5
#define SD_SEL_GPIO_Port GPIOO
#define GREEN_LED_Pin GPIO_PIN_1
#define GREEN_LED_GPIO_Port GPIOO
#define OCTOSPI_IO2_Pin GPIO_PIN_4
#define OCTOSPI_IO2_GPIO_Port GPION
#define I2C2_SDA_Pin GPIO_PIN_11
#define I2C2_SDA_GPIO_Port GPIOB
#define JTDI_Pin GPIO_PIN_15
#define JTDI_GPIO_Port GPIOA
#define SD_DET_Pin GPIO_PIN_12
#define SD_DET_GPIO_Port GPION
#define OCTOSPI_CLK_Pin GPIO_PIN_6
#define OCTOSPI_CLK_GPIO_Port GPION
#define OCTOSPI_IO4_Pin GPIO_PIN_8
#define OCTOSPI_IO4_GPIO_Port GPION
#define OCTOSPI_DQS_Pin GPIO_PIN_0
#define OCTOSPI_DQS_GPIO_Port GPION
#define UCPD1_ISENSE_Pin GPIO_PIN_5
#define UCPD1_ISENSE_GPIO_Port GPIOA
#define LTE_PWR_KEY_Pin GPIO_PIN_1
#define LTE_PWR_KEY_GPIO_Port GPIOG
#define UCPD_PWR_EN_Pin GPIO_PIN_7
#define UCPD_PWR_EN_GPIO_Port GPIOA
#define OCTOSPI_IO1_Pin GPIO_PIN_3
#define OCTOSPI_IO1_GPIO_Port GPION
#define OCTOSPI_IO3_Pin GPIO_PIN_5
#define OCTOSPI_IO3_GPIO_Port GPION
#define OCTOSPI_NCS_Pin GPIO_PIN_1
#define OCTOSPI_NCS_GPIO_Port GPION
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define UCPD1_VSENSE_Pin GPIO_PIN_11
#define UCPD1_VSENSE_GPIO_Port GPIOA
#define RED_LED_Pin GPIO_PIN_10
#define RED_LED_GPIO_Port GPIOG
#define OCTOSPI_IO5_Pin GPIO_PIN_9
#define OCTOSPI_IO5_GPIO_Port GPION
#define OCTOSPI_IO0_Pin GPIO_PIN_2
#define OCTOSPI_IO0_GPIO_Port GPION
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define I2C2_SCL_Pin GPIO_PIN_10
#define I2C2_SCL_GPIO_Port GPIOB
#define SWO_Pin GPIO_PIN_5
#define SWO_GPIO_Port GPIOB
#define OCTOSPI_IO6_Pin GPIO_PIN_10
#define OCTOSPI_IO6_GPIO_Port GPION
#define OCTOSPI_IO7_Pin GPIO_PIN_11
#define OCTOSPI_IO7_GPIO_Port GPION

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
