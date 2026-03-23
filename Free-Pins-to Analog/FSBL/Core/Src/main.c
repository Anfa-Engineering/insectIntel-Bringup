/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
/* USER CODE BEGIN CLK 1 */
/* USER CODE END CLK 1 */

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the System Power Supply
  */
  if (HAL_PWREx_ConfigSupply(PWR_EXTERNAL_SOURCE_SUPPLY) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /* Enable HSI */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Get current CPU/System buses clocks configuration and if necessary switch
 to intermediate HSI clock to ensure target clock can be set
  */
  HAL_RCC_GetClockConfig(&RCC_ClkInitStruct);
  if ((RCC_ClkInitStruct.CPUCLKSource == RCC_CPUCLKSOURCE_IC1) ||
     (RCC_ClkInitStruct.SYSCLKSource == RCC_SYSCLKSOURCE_IC2_IC6_IC11))
  {
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_CPUCLK | RCC_CLOCKTYPE_SYSCLK);
    RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_NONE;
  RCC_OscInitStruct.PLL1.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL2.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL3.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL4.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_CPUCLK|RCC_CLOCKTYPE_HCLK
                              |RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1
                              |RCC_CLOCKTYPE_PCLK2|RCC_CLOCKTYPE_PCLK5
                              |RCC_CLOCKTYPE_PCLK4;
  RCC_ClkInitStruct.CPUCLKSource = RCC_CPUCLKSOURCE_HSI;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;
  RCC_ClkInitStruct.APB5CLKDivider = RCC_APB5_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOQ_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOP_CLK_ENABLE();
  __HAL_RCC_GPIOO_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPION_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : PQ6 PQ3 PQ4 PQ5
                           PQ2 PQ1 PQ0 PQ7 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_2|GPIO_PIN_1|GPIO_PIN_0|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOQ, &GPIO_InitStruct);

  /*Configure GPIO pins : PH0 PH3 PH6 PH1
                           PH8 PH7 PH9 PH2
                           PH4 PH5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_1
                          |GPIO_PIN_8|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_2
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pins : PC10 PC1 PC4 PC11
                           PC6 PC5 PC15 PC9
                           PC12 PC7 PC0 PC14
                           PC8 PC2 PC3 PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_11
                          |GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_15|GPIO_PIN_9
                          |GPIO_PIN_12|GPIO_PIN_7|GPIO_PIN_0|GPIO_PIN_14
                          |GPIO_PIN_8|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PE13 PE15 PE8 PE9
                           PE14 PE12 PE7 PE10
                           PE5 PE4 PE6 PE11
                           PE3 PE2 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_14|GPIO_PIN_12|GPIO_PIN_7|GPIO_PIN_10
                          |GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_11
                          |GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_0|GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PD10 PD1 PD15 PD4
                           PD2 PD12 PD14 PD13
                           PD6 PD0 PD5 PD7
                           PD3 PD8 PD11 PD9 */
  GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_1|GPIO_PIN_15|GPIO_PIN_4
                          |GPIO_PIN_2|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_13
                          |GPIO_PIN_6|GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_7
                          |GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_11|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : PB14 PB13 PB2 PB9
                           PB8 PB15 PB0 PB7
                           PB6 PB3 PB11 PB12
                           PB1 PB4 PB10 PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_14|GPIO_PIN_13|GPIO_PIN_2|GPIO_PIN_9
                          |GPIO_PIN_8|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_7
                          |GPIO_PIN_6|GPIO_PIN_3|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_1|GPIO_PIN_4|GPIO_PIN_10|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PP7 PP6 PP0 PP4
                           PP1 PP15 PP5 PP12
                           PP3 PP2 PP13 PP11
                           PP8 PP14 PP9 PP10 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_0|GPIO_PIN_4
                          |GPIO_PIN_1|GPIO_PIN_15|GPIO_PIN_5|GPIO_PIN_12
                          |GPIO_PIN_3|GPIO_PIN_2|GPIO_PIN_13|GPIO_PIN_11
                          |GPIO_PIN_8|GPIO_PIN_14|GPIO_PIN_9|GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOP, &GPIO_InitStruct);

  /*Configure GPIO pins : PO5 PO1 PO2 PO3
                           PO0 PO4 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_0|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOO, &GPIO_InitStruct);

  /*Configure GPIO pins : PG7 PG6 PG5 PG4
                           PG14 PG3 PG15 PG1
                           PG0 PG12 PG10 PG2
                           PG9 PG8 PG13 PG11 */
  GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6|GPIO_PIN_5|GPIO_PIN_4
                          |GPIO_PIN_14|GPIO_PIN_3|GPIO_PIN_15|GPIO_PIN_1
                          |GPIO_PIN_0|GPIO_PIN_12|GPIO_PIN_10|GPIO_PIN_2
                          |GPIO_PIN_9|GPIO_PIN_8|GPIO_PIN_13|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

  /*Configure GPIO pins : PF4 PF10 PF6 PF7
                           PF3 PF5 PF15 PF14
                           PF8 PF2 PF9 PF11
                           PF1 PF13 PF0 PF12 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_10|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_15|GPIO_PIN_14
                          |GPIO_PIN_8|GPIO_PIN_2|GPIO_PIN_9|GPIO_PIN_11
                          |GPIO_PIN_1|GPIO_PIN_13|GPIO_PIN_0|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PN4 PN12 PN6 PN8
                           PN0 PN3 PN5 PN1
                           PN9 PN7 PN2 PN10
                           PN11 */
  GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_12|GPIO_PIN_6|GPIO_PIN_8
                          |GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_1
                          |GPIO_PIN_9|GPIO_PIN_7|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPION, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA6 PA1 PA15
                           PA10 PA5 PA7 PA3
                           PA14 PA11 PA2 PA13
                           PA12 PA8 PA0 PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_6|GPIO_PIN_1|GPIO_PIN_15
                          |GPIO_PIN_10|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_3
                          |GPIO_PIN_14|GPIO_PIN_11|GPIO_PIN_2|GPIO_PIN_13
                          |GPIO_PIN_12|GPIO_PIN_8|GPIO_PIN_0|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
