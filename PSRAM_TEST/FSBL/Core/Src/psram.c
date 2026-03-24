#include "main.h"
#include <stdio.h>
#include <stdint.h>

#ifdef PSRAM_TEST

/* ===== CONFIG ===== */
#define KB                  (1024U)
#define MB                  (1024U * 1024U)
#define PSRAM_SIZE_BYTES    (32U * MB)

#define BASE_ADDR ((volatile uint8_t *)XSPI1_BASE)

/* ===== GLOBAL ===== */
static uint32_t errorCount1 = 0;
static uint32_t errorCount0 = 0;
static uint32_t errorCountP = 0;

/* ===== UTIL ===== */
static void report_error(uint32_t addr, uint8_t expected, uint8_t actual, uint8_t error) {
    printf("ERR @0x%08lX: exp=0x%02X got=0x%02X\r\n", addr, expected, actual);
    switch (error) {
		case 0:
			errorCount0++;

			break;
		case 1:
			errorCount1++;
			break;
		case 2:
			errorCountP++;
			break;
		default:
			Error_Handler();
			break;
	}
//    errorCount++;
}

/* =========================================================
   1. ADDRESS LINE TEST
   ========================================================= */
static void psram_address_test(void) {
    printf("\r\n[Address Test]\r\n\r\n\r\n");

    uint32_t max_offset = PSRAM_SIZE_BYTES - 1;

    BASE_ADDR[0] = 0xAA;

    for (uint32_t offset = 1; offset <= max_offset; offset <<= 1) {
        BASE_ADDR[offset] = 0x55;

        printf("Testing address Line : 0x%08lX\r\n", offset);

        if (BASE_ADDR[0] != 0xAA) {
            printf("Address fault at offset: 0x%08lX\r\n", offset);
//            errorCount++;
        }
    }

    HAL_Delay(1);
}

/* =========================================================
   2. WALKING BIT TEST (DATA LINES)
   ========================================================= */
static void psram_walking_bit_test(void) {
    printf("\r\n[Walking Bit Test]\r\n\r\n\r\n");

    for (uint8_t bit = 0; bit < 8; bit++) {


        uint8_t pattern = (1 << bit);
        printf(" \r\nTesting bit idx: %u", bit);

        printf("\r\n ---- normal pattern ---- \r\n\r\n");

        for (uint32_t i = 0; i < PSRAM_SIZE_BYTES; i++) {
            BASE_ADDR[i] = pattern;
        }

        HAL_Delay(1);

        for (uint32_t i = 0; i < PSRAM_SIZE_BYTES; i++) {
            if (BASE_ADDR[i] != pattern) {
                report_error(i, pattern, BASE_ADDR[i], 1U);
            }
        }

        printf("\r\n ---- inverse pattern ----  \r\n\r\n");

        pattern = ~(1 << bit);

        for (uint32_t i = 0; i < PSRAM_SIZE_BYTES; i++) {
            BASE_ADDR[i] = pattern;
        }

        HAL_Delay(1);

        for (uint32_t i = 0; i < PSRAM_SIZE_BYTES; i++) {
            if (BASE_ADDR[i] != pattern) {
                report_error(i, pattern, BASE_ADDR[i], 0U);
            }
        }
    }
}

/* =========================================================
   3. PATTERN TEST (IMPROVED)
   ========================================================= */
static void psram_pattern_test(void) {
    printf("\r\n[Pattern Test]\r\n\r\n\r\n");

    /* ---- write ---- */
    for (uint32_t i = 0; i < PSRAM_SIZE_BYTES; i++) {
        uint8_t pattern = (uint8_t)((i & 0xFF) ^ (i >> 8));
        BASE_ADDR[i] = pattern;
    }

    HAL_Delay(1);

    /* ---- read ---- */
    for (uint32_t i = 0; i < PSRAM_SIZE_BYTES; i++) {
        uint8_t expected = (uint8_t)((i & 0xFF) ^ (i >> 8));
        uint8_t actual = BASE_ADDR[i];

        if (actual != expected) {
            report_error(i, expected, actual, 2U);
        }
    }
}

/* =========================================================
   MAIN TEST ENTRY
   ========================================================= */
void psram_test(void) {

    printf("\r\n==== PSRAM TEST START ====\r\n");

    errorCount0 = 0;
    errorCount1 = 0;
    errorCountP = 0;



    psram_address_test();
    psram_walking_bit_test();
    psram_pattern_test();

    if (errorCount0 || errorCount1 || errorCountP) {
        printf("\r\n==== FAIL ====\r\n %lu BIT 1 ERRORS \r\n %lu BIT 0 ERRORS \r\n %lu PATTERN ERRORS \r\n", errorCount1, errorCount0, errorCountP);

    } else {
        printf("\r\n==== SUCCESS: NO ERRORS ====\r\n");
    }
}

#endif
