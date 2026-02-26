/*
 * sd-card-test.c
 *
 *  Created on: 25 Feb 2026
 *      Author: jorda
 */


#include "main.h"
#include  <stdio.h>
#include "app_filex.h"
#include "sdmmc.h"
#include "dcmipp.h"
#include "jpeg.h"
#include "jpeg_utils_conf.h"
#include "i2c.h"
#include "isp_api.h"
#include "isp_conf.h"
#include "imx335.h"
#include "isp_param_conf.h"
#include "encode_dma.h"

//Eported varibles
ISP_HandleTypeDef hIsp;
uint32_t NbMainFrames;
uint32_t takeSnap = 0U;
uint8_t create_file = 0U;

#ifdef CAM_TEST

static IMX335_Object_t   IMX335Obj;
static int32_t isp_gain;
static int32_t isp_exposure;

//ISP Fucntion prototypes
static void IMX335_Probe(uint32_t Resolution, uint32_t PixelFormat);
static ISP_StatusTypeDef GetSensorInfoHelper(uint32_t Instance, ISP_SensorInfoTypeDef *SensorInfo);
static ISP_StatusTypeDef SetSensorGainHelper(uint32_t Instance, int32_t Gain);
static ISP_StatusTypeDef GetSensorGainHelper(uint32_t Instance, int32_t *Gain);
static ISP_StatusTypeDef SetSensorExposureHelper(uint32_t Instance, int32_t Exposure);
static ISP_StatusTypeDef GetSensorExposureHelper(uint32_t Instance, int32_t *Exposure);


// SD-card File variables
extern FX_FILE         fx_file;
extern FX_MEDIA        sdio_disk;
extern SD_ConnectionStateTypeDef cardstaus;
extern ALIGN_32BYTES (uint32_t fx_sd_media_memory[FX_STM32_SD_DEFAULT_SECTOR_SIZE / sizeof(uint32_t)]);
//extern uint32_t encoded_size;

CHAR filename[20];
uint8_t fileindex = 0;

UINT fxsd_status = FX_SUCCESS;
uint8_t create_file = 0U;

void cam_test(void){
	    ISP_AppliHelpersTypeDef appliHelpers = {0};

		/* Initialize the IMX335 Sensor ----------------------------- */
		IMX335_Probe(IMX335_R2592_1944, IMX335_RAW_RGGB10);

		/* Fill init struct with Camera driver helpers */
		appliHelpers.GetSensorInfo = GetSensorInfoHelper;
		appliHelpers.SetSensorGain = SetSensorGainHelper;
		appliHelpers.GetSensorGain = GetSensorGainHelper;
		appliHelpers.SetSensorExposure = SetSensorExposureHelper;
		appliHelpers.GetSensorExposure = GetSensorExposureHelper;

		/* Initialize the Image Signal Processing middleware */
		if(ISP_Init(&hIsp, &hdcmipp, 0, &appliHelpers, ISP_IQParamCacheInit[0]) != ISP_OK)
		{
			Error_Handler();
		}

//		if (HAL_DCMIPP_CSI_PIPE_Start(&hdcmipp, DCMIPP_PIPE1, DCMIPP_VIRTUAL_CHANNEL0 , BUFFER_ADDRESS, DCMIPP_MODE_CONTINUOUS) != HAL_OK)
//		{
//			Error_Handler();
//		}
//
//		/* Start the Image Signal Processing */
//		if (ISP_Start(&hIsp) != ISP_OK)
//		{
//			Error_Handler();
//		}
//		while(1);
		//Initial detection of card
		if(!HAL_GPIO_ReadPin(SD_DET_GPIO_Port, SD_DET_Pin)){

			//Open the file filesystem for this media
			fxsd_status =  fx_media_open(&sdio_disk, FX_SD_VOLUME_NAME, fx_stm32_sd_driver, (VOID *)FX_NULL, (VOID *) fx_sd_media_memory, sizeof(fx_sd_media_memory));

			// Check the media open sd_status
			if (fxsd_status != FX_SUCCESS){
			  Error_Handler();
			}
			//Set green led one as high for success indication on boot up
			HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);
			cardstaus = CARD_STATUS_CONNECTED;
			printf("Card Present\r\n");

		}else {
			cardstaus = CARD_STATUS_DISCONNECTED;
			printf("\r\nCard Absent\r\n");

		}

		while(1){

			// Background processing
			if(ISP_BackgroundProcess(&hIsp) != ISP_OK){
				HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);

			}

			*((uint8_t *)BUFFER_ADDRESS) = 0xFFU;
			printf("\r%lX", *((uint32_t *)BUFFER_ADDRESS));

			//SD-CARD logic
		do{
				if(cardstaus == CARD_STATUS_CHANGED){
					  if(!HAL_GPIO_ReadPin(SD_DET_GPIO_Port, SD_DET_Pin)){

						  //Open the file filesystem for this media
						  fxsd_status =  fx_media_open(&sdio_disk, FX_SD_VOLUME_NAME, fx_stm32_sd_driver, (VOID *)FX_NULL, (VOID *) fx_sd_media_memory, sizeof(fx_sd_media_memory));
						  // Check the media open sd_status
						  if (fxsd_status != FX_SUCCESS){
							  Error_Handler();
						  }

						  printf("Card Connected\r\n");

						  cardstaus = CARD_STATUS_CONNECTED;
						  //Set green led one as flagh for success
						  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_SET);

					  }else{
						  fxsd_status =  fx_media_abort(&sdio_disk);
						  // Check the media open sd_status
						  if (fxsd_status != FX_SUCCESS){
							  Error_Handler();
						  }
						//Deinitialise the sd peripheral
						  if(HAL_SD_Abort(&hsd2) != HAL_OK){
							  Error_Handler();
						  }

						//Memset(&sd_media, 0, sizeof(FX_MEDIA));
						  if(HAL_SD_DeInit(&hsd2) != HAL_OK){
							  Error_Handler();
						  }

						  printf("Card Disconnected\r\n");

						  cardstaus = CARD_STATUS_DISCONNECTED;
						  HAL_GPIO_WritePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_PIN_RESET);
					  }
				}
			}while(0);

			if(create_file && cardstaus == CARD_STATUS_CONNECTED){
			//	  //reset flag
			//	  create_file = 0U;
			//	  uint32_t convertedDataCount;
			  uint32_t jpeg_encode_processing_end = 0;
			  printf("File creation\r\n");

			  fileindex++;
			  sprintf(filename, "IMAGE-%d.jpg", fileindex);

			  //Create of overwrite the current file
			  HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
			//	  HAL_Delay(1000U);
			  // first attempt file creation
			  fxsd_status =  fx_file_create(&sdio_disk,filename);

			  if (fxsd_status == FX_SUCCESS || fxsd_status == FX_ALREADY_CREATED)
			  {
				 // Either crested of already present i.e good for opening.
				fxsd_status =  fx_file_open(&sdio_disk, &fx_file, filename, FX_OPEN_FOR_WRITE);
			  }else{

				  /* Error performing file creation excluding FX_ALREADY_CREATED, call error handler.  */
				  Error_Handler();
			  }

			  /* Check the file open status.  */
			  if (fxsd_status != FX_SUCCESS)
			  {
				/* Error opening file, call error handler.  */
				Error_Handler();

			  }

			  /* Seek to the beginning of the test file.  */
			  fxsd_status =  fx_file_seek(&fx_file, 0);

			  /* Check the file seek status.  */
			  if (fxsd_status != FX_SUCCESS)
			  {
				/* Error performing file seek, call error handler.  */
				Error_Handler();
			  }


			//	  Create our MCUS
			//	  RGB2YCbCr((uint8_t *)BUFFER_ADDRESS, (uint8_t *)(BUFFER_ADDRESS - BUFFER_SIZE + 0x400U), 0U,
			//				  BUFFER_SIZE, &convertedDataCount);
			//	  RGB2YCbCr((uint8_t *)BUFFER_ADDRESS, (uint8_t *)(BUFFER_ADDRESS - 0x800U /*+ BUFFER_SIZE*/), 0U,
			//				  BUFFER_SIZE, &convertedDataCount);

			//	  Check if we were succefull
			//	  if(convertedDataCount != BUFFER_HEIGHT * BUFFER_WIDTH * 3){
			//		  Error_Handler();
			//	  }
			//	  For matters of Contingency
			//	  *((uint32_t *)(BUFFER_ADDRESS + BUFFER_SIZE + convertedDataCount)) = 0xFFFFFFFF;

			  /*JPEG Encoding with DMA (Not Blocking ) Method */
			  JPEG_Encode_DMA_IT(&hjpeg, BUFFER_ADDRESS, BUFFER_SIZE, (void *)0U);

			//	  hal_status = HAL_JPEG_Encode_IT(&hjpeg, (uint8_t *)(BUFFER_ADDRESS - 0x400U),
			//			  convertedDataCount, (uint8_t *)(BUFFER_ADDRESS - 0xF00U),
			//			  HAL_MAX_DELAY);

			//	  hal_status = HAL_JPEG_Encode_IT(&hjpeg, (uint8_t *)(BUFFER_ADDRESS - BUFFER_SIZE + 0x400U),
			//			  convertedDataCount, (uint8_t *)(BUFFER_ADDRESS - BUFFER_SIZE),
			//			  HAL_MAX_DELAY);

			//	  if(hal_status != HAL_OK){
			//		  Error_Handler();
			//	  }

			//	  wait for encoding to finish
			  do{
				JPEG_EncodeInputHandler(&hjpeg);
				jpeg_encode_processing_end = JPEG_EncodeOutputHandler(&hjpeg);
			  }while(jpeg_encode_processing_end == 0);

			//	  if(encoded_size > BUFFER_SIZE){
			//		  Error_Handler();
			//	  }

			  /* Write a string to the test file.  */
			//	  fxsd_status =  fx_file_write(&fx_file, data, sizeof(data));
			//	  fxsd_status =  fx_file_write(&fx_file, (uint8_t *)(BUFFER_ADDRESS + BUFFER_SIZE + convertedDataCount ), encoded_size);
			//	  fxsd_status =  fx_file_write(&fx_file, (uint8_t *)(BUFFER_ADDRESS - 0xF00U), encoded_size);

			  /* Check the file write status.  */
			  if (fxsd_status != FX_SUCCESS)
			  {
				/* Error writing to a file, call error handler.  */
				Error_Handler();
			  }

			  /* Close the test file.  */
			  fxsd_status =  fx_file_close(&fx_file);

			  /* Check the file close status.  */
			  if (fxsd_status != FX_SUCCESS)
			  {
				/* Error closing the file, call error handler.  */
				Error_Handler();
			  }


			  fxsd_status = fx_media_flush(&sdio_disk);

			  /* Check the media flush  status.  */
			  if (fxsd_status != FX_SUCCESS)
			  {
				/* Error closing the file, call error handler.  */
				Error_Handler();
			  }


			//	  fxsd_status = fx_media_close(&sdio_disk);
			//
			//	  if (fxsd_status != FX_SUCCESS)
			//	  {
			//		/* Error closing the file, call error handler.  */
			//		Error_Handler();
			//	  }
			//	  HAL_Delay(1000);
			  //reset flag
			  create_file = 0U;
			//	  encoded_size = 0U;
			if (HAL_DCMIPP_CSI_PIPE_Start(&hdcmipp, DCMIPP_PIPE1, DCMIPP_VIRTUAL_CHANNEL0 , BUFFER_ADDRESS, DCMIPP_MODE_CONTINUOUS) != HAL_OK)
			{
				Error_Handler();
			}
			printf("File Creation Completed.\r\n");

			  HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
			}

		}
	}


static void IMX335_Probe(uint32_t Resolution, uint32_t PixelFormat)
{
  IMX335_IO_t              IOCtx;
  uint32_t                 id;

  /* Configure the camera driver */
  IOCtx.Address     = CAMERA_IMX335_ADDRESS;
  IOCtx.Init        = I2C1_Init;
  IOCtx.DeInit      = I2C1_DeInit;
  IOCtx.ReadReg     = I2C1_ReadReg_Func;
  IOCtx.WriteReg    = I2C1_WriteReg_Func;
  IOCtx.GetTick     = (long int (*)(void))HAL_GetTick;

  if (IMX335_RegisterBusIO(&IMX335Obj, &IOCtx) != IMX335_OK)
  {
    Error_Handler();
  }
  else if (IMX335_ReadID(&IMX335Obj, &id) != IMX335_OK)
  {
    Error_Handler();
  }
  else
  {
    if (id != (uint32_t) IMX335_CHIP_ID)
    {
      Error_Handler();
    }
    else
    {
      if (IMX335_Init(&IMX335Obj, Resolution, PixelFormat) != IMX335_OK)
      {
        Error_Handler();
      }
      else if(IMX335_SetFrequency(&IMX335Obj, IMX335_INCK_24MHZ)!= IMX335_OK)
      {
        Error_Handler();
      }
//		else if(IMX335_SetFramerate(&IMX335Obj, 15U)){
//		  Error_Handler();
//		}
      else
      {
        return;
      }
    }
  }
}

/**
  * @brief  ISP Middleware helper. Camera sensor info getter
  * @retval ISP Status
  */
static ISP_StatusTypeDef GetSensorInfoHelper(uint32_t Instance, ISP_SensorInfoTypeDef *SensorInfo)
{
  UNUSED(Instance);
  return (ISP_StatusTypeDef) IMX335_GetSensorInfo(&IMX335Obj, (IMX335_SensorInfo_t *) SensorInfo);
}

/**
  * @brief  ISP Middleware helper. Camera gain setter
  * @retval ISP Status
  */
static ISP_StatusTypeDef SetSensorGainHelper(uint32_t Instance, int32_t Gain)
{
  UNUSED(Instance);
  isp_gain = Gain;
  return (ISP_StatusTypeDef) IMX335_SetGain(&IMX335Obj, Gain);
}

/**
  * @brief  ISP Middleware helper. Camera gain getter
  * @retval ISP Status
  */
static ISP_StatusTypeDef GetSensorGainHelper(uint32_t Instance, int32_t *Gain)
{
  UNUSED(Instance);
  *Gain = isp_gain;
  return ISP_OK;
}

/**
  * @brief  ISP Middleware helper. Camera exposure setter
  * @retval ISP Status
  */
static ISP_StatusTypeDef SetSensorExposureHelper(uint32_t Instance, int32_t Exposure)
{
  UNUSED(Instance);
  isp_exposure = Exposure;
  return (ISP_StatusTypeDef) IMX335_SetExposure(&IMX335Obj, Exposure);
}

/**
  * @brief  ISP Middleware helper. Camera exposure getter
  * @retval ISP Status
  */
static ISP_StatusTypeDef GetSensorExposureHelper(uint32_t Instance, int32_t *Exposure)
{
  UNUSED(Instance);
  *Exposure = isp_exposure;
  return ISP_OK;
}

#endif
