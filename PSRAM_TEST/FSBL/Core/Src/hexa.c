/*
 * hexa.c
 *
 *  Created on: Mar 16, 2026
 *      Author: Admin
 */


/* Includes ------------------------------------------------------------------*/
#include "hexa.h"
#include <stdio.h>

/**
  * @brief  Write mode register
  * @param  Ctx    XSPI handle pointer
  * @param  Address Register address
  * @param  Value  Register value pointer (2 bytes)
  * @retval HAL status
  */
HAL_StatusTypeDef APS256_WriteReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value)
{
    XSPI_RegularCmdTypeDef sCommand = {0};

    sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
    sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Instruction        = WRITE_REG_CMD;
    sCommand.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
    sCommand.AddressWidth       = HAL_XSPI_ADDRESS_32_BITS;
    sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
    sCommand.Address            = Address;
    sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
    sCommand.DataMode           = HAL_XSPI_DATA_8_LINES;
    sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
    sCommand.DataLength         = 2;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        printf("APS256_WriteReg: Command failed at address 0x%08lX\r\n", Address);
        return HAL_ERROR;
    }

    if (HAL_XSPI_Transmit(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        printf("APS256_WriteReg: Transmit failed at address 0x%08lX\r\n", Address);
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  Read mode register
  * @param  Ctx         XSPI handle pointer
  * @param  Address     Register address
  * @param  Value       Buffer to store register value (2 bytes)
  * @param  LatencyCode Read latency code
  * @retval HAL status
  */
HAL_StatusTypeDef APS256_ReadReg(XSPI_HandleTypeDef *Ctx, uint32_t Address, uint8_t *Value, uint32_t LatencyCode)
{
    XSPI_RegularCmdTypeDef sCommand = {0};

    sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
    sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Instruction        = READ_REG_CMD;
    sCommand.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
    sCommand.AddressWidth       = HAL_XSPI_ADDRESS_32_BITS;
    sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_ENABLE;
    sCommand.Address            = Address;
    sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
    sCommand.DataMode           = HAL_XSPI_DATA_8_LINES;
    sCommand.DataDTRMode        = HAL_XSPI_DATA_DTR_ENABLE;
    sCommand.DataLength         = 2;
    sCommand.DummyCycles        = (LatencyCode - 1U);
    sCommand.DQSMode            = HAL_XSPI_DQS_ENABLE;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        printf("APS256_ReadReg: Command failed at address 0x%08lX\r\n", Address);
        return HAL_ERROR;
    }

    if (HAL_XSPI_Receive(Ctx, Value, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        printf("APS256_ReadReg: Receive failed at address 0x%08lX\r\n", Address);
        return HAL_ERROR;
    }

    return HAL_OK;
}

/**
  * @brief  Send Global Reset command to the PSRAM
  * @note   Resets all register contents. Memory content not guaranteed.
  *         Must re-run APS256_Configure after calling this.
  * @param  Ctx  XSPI handle pointer
  * @retval HAL status
  */
HAL_StatusTypeDef APS256_GlobalReset(XSPI_HandleTypeDef *Ctx)
{
    XSPI_RegularCmdTypeDef sCommand = {0};

    /* Initialize the command */
    sCommand.OperationType      = HAL_XSPI_OPTYPE_COMMON_CFG;
    sCommand.InstructionMode    = HAL_XSPI_INSTRUCTION_8_LINES;
    sCommand.InstructionWidth   = HAL_XSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDTRMode = HAL_XSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Instruction        = GLOBAL_RESET_CMD;
    sCommand.AddressMode        = HAL_XSPI_ADDRESS_8_LINES;
    sCommand.AddressWidth       = HAL_XSPI_ADDRESS_24_BITS;
    sCommand.AddressDTRMode     = HAL_XSPI_ADDRESS_DTR_DISABLE;
    sCommand.Address            = 0;
    sCommand.AlternateBytesMode = HAL_XSPI_ALT_BYTES_NONE;
    sCommand.DataMode           = HAL_XSPI_DATA_NONE;
    sCommand.DataLength         = 0;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_XSPI_DQS_DISABLE;

    if (HAL_XSPI_Command(Ctx, &sCommand, HAL_XSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        printf("APS256_GlobalReset: Command failed\r\n");
        return HAL_ERROR;
    }

    /* Wait tRST >= 2us */
    HAL_Delay(2);

    printf("APS256_GlobalReset: Reset complete. Re-run APS256_Configure.\r\n");
    return HAL_OK;
}

/**
  * @brief  Read and verify Vendor ID from MR1[4:0]
  * @note   Reads MR1 register using consistent latency code and verifies
  *         the device is AP Memory (Vendor ID = 0x0D)
  * @param  Ctx  XSPI handle pointer
  * @retval HAL status
  */
HAL_StatusTypeDef APS256_ReadVendorID(XSPI_HandleTypeDef *Ctx)
{
    uint8_t regR_MR1[2] = {0};
    uint8_t vendorID = 0;

    /* Read MR1 register using consistent read latency code */
    if (APS256_ReadReg(Ctx, MR1, regR_MR1, APS256_READ_LATENCY) != HAL_OK)
    {
        printf("APS256_ReadVendorID: Register read failed\r\n");
        return HAL_ERROR;
    }

    /* Vendor ID is in MR1[4:0] */
    vendorID = regR_MR1[0] & 0x1F;

    printf("APS256_ReadVendorID: Raw MR1[0]=0x%02X, VendorID=0x%02X\r\n",
           regR_MR1[0], vendorID);

    /* Verify Vendor ID matches AP Memory */
    if (vendorID != APM_VENDOR_ID)
    {
        printf("APS256_ReadVendorID: MISMATCH - expected 0x%02X, got 0x%02X\r\n",
               APM_VENDOR_ID, vendorID);
        return HAL_ERROR;
    }

    printf("APS256_ReadVendorID: OK - AP Memory confirmed\r\n");
    return HAL_OK;
}
/**
  * @brief  Configure AP Memory PSRAM registers for 200MHz X16 operation
  * @note   Reads initial state, configures MR0, MR4, MR8, and verifies.
  * @param  Ctx  XSPI handle pointer
  * @retval HAL status
  */
HAL_StatusTypeDef APS256_Configure(XSPI_HandleTypeDef *Ctx)
{
    uint8_t regOriginal[2] = {0};

    uint8_t regW_MR0[2] = {MR0_CONFIG, 0x00};
    uint8_t regR_MR0[2] = {0};

    uint8_t regW_MR4[2] = {MR4_CONFIG, 0x00};
    uint8_t regR_MR4[2] = {0};

    uint8_t regW_MR8[2] = {MR8_CONFIG, 0x00};
    uint8_t regR_MR8[2] = {0};

    /* --- MR0: Read latency + drive strength --- */
    // Read original value first
    if (APS256_ReadReg(Ctx, MR0, regOriginal, APS256_READ_LATENCY) != HAL_OK) return HAL_ERROR;

    if (APS256_WriteReg(Ctx, MR0, regW_MR0) != HAL_OK)
    {
        printf("APS256_Configure: MR0 write failed\r\n");
        return HAL_ERROR;
    }

    if (APS256_ReadReg(Ctx, MR0, regR_MR0, APS256_READ_LATENCY) != HAL_OK)
    {
        printf("APS256_Configure: MR0 read failed\r\n");
        return HAL_ERROR;
    }

    printf("APS256_Configure: MR0 -> Original: 0x%02X, Wrote: 0x%02X, Verified: 0x%02X\r\n",
           regOriginal[0], regW_MR0[0], regR_MR0[0]);

    if (regR_MR0[0] != regW_MR0[0]) return HAL_ERROR;


    /* --- MR4: Write latency --- */
    // Read original value first
    if (APS256_ReadReg(Ctx, MR4, regOriginal, APS256_READ_LATENCY) != HAL_OK) return HAL_ERROR;

    if (APS256_WriteReg(Ctx, MR4, regW_MR4) != HAL_OK)
    {
        printf("APS256_Configure: MR4 write failed\r\n");
        return HAL_ERROR;
    }

    if (APS256_ReadReg(Ctx, MR4, regR_MR4, APS256_READ_LATENCY) != HAL_OK)
    {
        printf("APS256_Configure: MR4 read failed\r\n");
        return HAL_ERROR;
    }

    printf("APS256_Configure: MR4 -> Original: 0x%02X, Wrote: 0x%02X, Verified: 0x%02X\r\n",
           regOriginal[0], regW_MR4[0], regR_MR4[0]);

    if (regR_MR4[0] != regW_MR4[0]) return HAL_ERROR;


    /* --- MR8: Burst type/length + X16 mode --- */
    // Read original value first
    if (APS256_ReadReg(Ctx, MR8, regOriginal, APS256_READ_LATENCY) != HAL_OK) return HAL_ERROR;

    if (APS256_WriteReg(Ctx, MR8, regW_MR8) != HAL_OK)
    {
        printf("APS256_Configure: MR8 write failed\r\n");
        return HAL_ERROR;
    }

    if (APS256_ReadReg(Ctx, MR8, regR_MR8, APS256_READ_LATENCY) != HAL_OK)
    {
        printf("APS256_Configure: MR8 read failed\r\n");
        return HAL_ERROR;
    }

    printf("APS256_Configure: MR8 -> Original: 0x%02X, Wrote: 0x%02X, Verified: 0x%02X\r\n",
           regOriginal[0], regW_MR8[0], regR_MR8[0]);

    if (regR_MR8[0] != regW_MR8[0]) return HAL_ERROR;

    printf("APS256_Configure: Configuration complete - 200MHz X16 mode ready\r\n");
    return HAL_OK;
}
