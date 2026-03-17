/*
 * hexa.h
 *
 *  Created on: Mar 16, 2026
 *      Author: Admin
 */

#ifndef INC_HEXA_H_
#define INC_HEXA_H_

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Command defines -----------------------------------------------------------*/
#define WRITE_REG_CMD           0xC0
#define READ_REG_CMD            0x40
#define GLOBAL_RESET_CMD        0xFF
#define WRITE_CMD               0x80
#define READ_CMD                0x00

/* Register address defines --------------------------------------------------*/
#define MR0                     0x00000000
#define MR1                     0x00000001
#define MR2                     0x00000002
#define MR4                     0x00000004
#define MR6                     0x00000006
#define MR8                     0x00000008

/* Register value defines ----------------------------------------------------*/
#define APM_VENDOR_ID  0x0D
#define MR0_CONFIG     0x30
#define MR4_CONFIG     0x20
#define MR8_CONFIG     0x4B

/* Latency defines -----------------------------------------------------------*/
#define APS256_READ_LATENCY  6   // or 14 if you want Fixed LC=14 (safer at 200 MHz)
#define DUMMY_CLOCK_CYCLES_READ  (APS256_READ_LATENCY - 1U) */
#define DUMMY_CLOCK_CYCLES_WRITE    4

/* Function prototypes -------------------------------------------------------*/
HAL_StatusTypeDef APS256_WriteReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value);
HAL_StatusTypeDef APS256_ReadReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value, uint32_t LatencyCode);
HAL_StatusTypeDef APS256_GlobalReset(XSPI_HandleTypeDef *Ctx);
HAL_StatusTypeDef APS256_ReadVendorID(XSPI_HandleTypeDef *Ctx);
HAL_StatusTypeDef APS256_Configure(XSPI_HandleTypeDef *Ctx);

#endif /* INC_HEXA_H_ */
