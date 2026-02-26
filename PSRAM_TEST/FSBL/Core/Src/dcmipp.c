/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dcmipp.c
  * @brief   This file provides code for the configuration
  *          of the DCMIPP instances.
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
#include "dcmipp.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

DCMIPP_HandleTypeDef hdcmipp;

/* DCMIPP init function */
void MX_DCMIPP_Init(void)
{

  /* USER CODE BEGIN DCMIPP_Init 0 */

  /* USER CODE END DCMIPP_Init 0 */

  DCMIPP_CSI_PIPE_ConfTypeDef pCSI_PipeConfig = {0};
  DCMIPP_CSI_ConfTypeDef pCSI_Config = {0};
  DCMIPP_PipeConfTypeDef pPipeConfig = {0};

  /* USER CODE BEGIN DCMIPP_Init 1 */
  DCMIPP_DownsizeTypeDef DonwsizeConf = {0};

  /* USER CODE END DCMIPP_Init 1 */
  hdcmipp.Instance = DCMIPP;
  if (HAL_DCMIPP_Init(&hdcmipp) != HAL_OK)
  {
    Error_Handler();
  }

  /** Pipe 1 Config
  */
  pCSI_PipeConfig.DataTypeMode = DCMIPP_DTMODE_DTIDA;
  pCSI_PipeConfig.DataTypeIDA = DCMIPP_DT_RAW10;
  pCSI_PipeConfig.DataTypeIDB = DCMIPP_DT_RAW12;
  if (HAL_DCMIPP_CSI_PIPE_SetConfig(&hdcmipp, DCMIPP_PIPE1, &pCSI_PipeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  pCSI_Config.PHYBitrate = DCMIPP_CSI_PHY_BT_1600;
  pCSI_Config.DataLaneMapping = DCMIPP_CSI_PHYSICAL_DATA_LANES;
  pCSI_Config.NumberOfLanes = DCMIPP_CSI_TWO_DATA_LANES;
  HAL_DCMIPP_CSI_SetConfig(&hdcmipp, &pCSI_Config);
  pPipeConfig.FrameRate = DCMIPP_FRAME_RATE_ALL;
  pPipeConfig.PixelPipePitch = 1600;
  pPipeConfig.PixelPackerFormat = DCMIPP_PIXEL_PACKER_FORMAT_RGB565_1;
  if (HAL_DCMIPP_PIPE_SetConfig(&hdcmipp, DCMIPP_PIPE1, &pPipeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DCMIPP_CSI_SetVCConfig(&hdcmipp, 0U, DCMIPP_CSI_DT_BPP10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCMIPP_Init 2 */
  DonwsizeConf.HRatio      = 25656U;
  DonwsizeConf.VRatio      = 33161U;
  DonwsizeConf.HSize       = 800U;
  DonwsizeConf.VSize       = 480U;
  DonwsizeConf.HDivFactor  = 316U;
  DonwsizeConf.VDivFactor  = 253U;
  if(HAL_DCMIPP_PIPE_SetDownsizeConfig(&hdcmipp, DCMIPP_PIPE1, &DonwsizeConf) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE END DCMIPP_Init 2 */

}

void HAL_DCMIPP_MspInit(DCMIPP_HandleTypeDef* dcmippHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(dcmippHandle->Instance==DCMIPP)
  {
  /* USER CODE BEGIN DCMIPP_MspInit 0 */

  /* USER CODE END DCMIPP_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_DCMIPP|RCC_PERIPHCLK_CSI;
    PeriphClkInitStruct.DcmippClockSelection = RCC_DCMIPPCLKSOURCE_IC17;
    PeriphClkInitStruct.ICSelection[RCC_IC17].ClockSelection = RCC_ICCLKSOURCE_PLL1;
    PeriphClkInitStruct.ICSelection[RCC_IC17].ClockDivider = 4;
    PeriphClkInitStruct.ICSelection[RCC_IC18].ClockSelection = RCC_ICCLKSOURCE_PLL1;
    PeriphClkInitStruct.ICSelection[RCC_IC18].ClockDivider = 60;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* DCMIPP clock enable */
    __HAL_RCC_DCMIPP_CLK_ENABLE();
    __HAL_RCC_CSI_CLK_ENABLE();
    __HAL_RCC_CSI_FORCE_RESET();
    __HAL_RCC_CSI_RELEASE_RESET();

    /* DCMIPP interrupt Init */
    HAL_NVIC_SetPriority(DCMIPP_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DCMIPP_IRQn);
    HAL_NVIC_SetPriority(CSI_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(CSI_IRQn);
  /* USER CODE BEGIN DCMIPP_MspInit 1 */

	/* Camera sensor Power-On sequence */
	/* Assert the camera Enable and NRST pins */
	HAL_GPIO_WritePin(NRST_CAM_GPIO_Port, NRST_CAM_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(CAM_EN_GPIO_Port, CAM_EN_Pin, GPIO_PIN_RESET);

	HAL_Delay(200);   /* NRST and Enable signals asserted during 200ms */

	/* De-assert the camera STANDBY pin (active high) */
	HAL_GPIO_WritePin(CAM_EN_GPIO_Port, CAM_EN_Pin, GPIO_PIN_SET);
	HAL_Delay(5);

  /* USER CODE END DCMIPP_MspInit 1 */
  }
}

void HAL_DCMIPP_MspDeInit(DCMIPP_HandleTypeDef* dcmippHandle)
{

  if(dcmippHandle->Instance==DCMIPP)
  {
  /* USER CODE BEGIN DCMIPP_MspDeInit 0 */

  /* USER CODE END DCMIPP_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CSI_CLK_DISABLE();
    __HAL_RCC_CSI_FORCE_RESET();
    __HAL_RCC_CSI_RELEASE_RESET();

    /* DCMIPP interrupt Deinit */
    HAL_NVIC_DisableIRQ(DCMIPP_IRQn);
    HAL_NVIC_DisableIRQ(CSI_IRQn);
  /* USER CODE BEGIN DCMIPP_MspDeInit 1 */

  /* USER CODE END DCMIPP_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
