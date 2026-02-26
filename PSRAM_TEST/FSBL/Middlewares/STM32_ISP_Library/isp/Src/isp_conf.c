/**
 ******************************************************************************
 * @file    isp_conf_template.c
 * @author  AIS Application Team
 * @brief   DCMIPP event callback implementation
  *         This template should be copied to the user folder,
  *         renamed and customized following user needs.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "isp_api.h"
#include "main.h"
#include "isp_conf.h"

/* Private types -------------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern ISP_HandleTypeDef hIsp;
extern uint32_t NbMainFrames;
extern uint8_t create_file;
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Vsync Event callback on pipe
 * @param  hdcmipp DCMIPP device handle
 *         Pipe    Pipe receiving the callback
 * @retval None
 */

void HAL_DCMIPP_PIPE_VsyncEventCallback(DCMIPP_HandleTypeDef *hdcmipp, uint32_t Pipe)
{
//  UNUSED(hdcmipp);
  /* Update the frame counter and call the ISP statistics handler */
  if (Pipe == DCMIPP_PIPE1 )
  {

    /* Call the IPS statistics handler */
    ISP_IncMainFrameId(&hIsp);
    ISP_GatherStatistics(&hIsp);
    ISP_OutputMeta(&hIsp);
    NbMainFrames++;

//	if(ISP_GetMainFrameId(&hIsp) == (uint32_t) 2U){
//		HAL_GPIO_TogglePin(Monitor_Pin_GPIO_Port, Monitor_Pin_Pin);

//	}

  }
  /* Additional applicative processing can be implemented from here */
}


/**
 * @brief  Frame Event callback on pipe
 * @param  Pipe  Pipe receiving the callback
 * @retval None
 */
void HAL_DCMIPP_PIPE_FrameEventCallback(DCMIPP_HandleTypeDef *hdcmipp, uint32_t Pipe)
{
  UNUSED(hdcmipp);
  /* Update the frame counter for the pipes used to dump the frame */


  switch (Pipe)
  {
    case DCMIPP_PIPE0 :
      ISP_IncDumpFrameId(&hIsp);
      break;
    case DCMIPP_PIPE1:
//   	 HAL_GPIO_TogglePin(Monitor_Pin_GPIO_Port, Monitor_Pin_Pin);
 	if(create_file){
// 	if(ISP_GetMainFrameId(&hIsp) == (uint32_t) 32U){
//		stop the pipe
		HAL_DCMIPP_CSI_PIPE_Stop(hdcmipp, DCMIPP_PIPE1, DCMIPP_VIRTUAL_CHANNEL0);
//		Also stop the camera
//		TODO; do we really need to at this point
//		HAL_GPIO_WritePin(CAM_EN_GPIO_Port, CAM_EN_Pin, GPIO_PIN_RESET);

	}

    case DCMIPP_PIPE2 :
      ISP_IncAncillaryFrameId(&hIsp);

      break;
  }
  /* Additional applicative processing can be implemented from here */
}

/**
  * @brief  Line Event callback on the pipe
  * @param  hdcmipp  Pointer to DCMIPP handle
  * @param  Pipe     Specifies the DCMIPP pipe, can be a value from @ref DCMIPP_Pipes
  * @retval None
  */
void HAL_DCMIPP_PIPE_LineEventCallback(DCMIPP_HandleTypeDef *hdcmipp, uint32_t Pipe)
{
  UNUSED(Pipe);
  UNUSED(hdcmipp);

}


/**
  * @brief  Error callback on the pipe
  * @param  hdcmipp  Pointer to DCMIPP handle
  * @param  Pipe     Specifies the DCMIPP pipe, can be a value from @ref DCMIPP_Pipes
  * @retval None
  */
void HAL_DCMIPP_PIPE_ErrorCallback(DCMIPP_HandleTypeDef *hdcmipp, uint32_t Pipe)
{

  UNUSED(hdcmipp);
//  printf("\n\n"
//		  "HAL_DCMIPP_PIPE_ErrorCallback\r\n"
//		  " Pipe	 : %lu\r\n"
//		  " ErrorCode: 0x%08lX\r\n"
//		  " PipeState: %u\r\n"
//		  " State 	 : %u\r\n"
//		  " Frame No_: %lu\r\n",
//		  Pipe,
//		  hdcmipp->ErrorCode,
//		  hdcmipp->PipeState[Pipe],
//		  hdcmipp->State,
//		  ISP_GetMainFrameId(&hIsp));
  //Error_Handler();

}


/**
  * @brief  Error callback on DCMIPP
  * @param  hdcmipp  Pointer to DCMIPP handle
  * @retval None
  */
void HAL_DCMIPP_ErrorCallback(DCMIPP_HandleTypeDef *hdcmipp)
{
  UNUSED(hdcmipp);
//  printf("DCMIPP_ErrorCallback\r\n"
//		  " ErrorCode : 0x%08lX\r\n"
//		  " Pipe1State : %u\r\n"
//		  " Pipe2State : %u\r\n"
//		  " Pipe3State : %u\r\n"
//		  " State 	  : %u\r\n",
//		  hdcmipp->ErrorCode,
//		  hdcmipp->PipeState[DCMIPP_PIPE0],
//		  hdcmipp->PipeState[DCMIPP_PIPE1],
//		  hdcmipp->PipeState[DCMIPP_PIPE2],
//		  hdcmipp->State);
  }
