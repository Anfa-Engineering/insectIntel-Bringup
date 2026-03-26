#include "main.h"
#include <stdio.h>
#include <stdint.h>

#ifdef PSRAM_TEST

/* ===== CONFIG ===== */
#define KB                  (1024U)
#define MB                  (1024U * 1024U)
#define PSRAM_SIZE_BYTES    (32U * MB)
#define PSRAM_SIZE_WORDS    (PSRAM_SIZE_BYTES / 2U)      /* 16M words */
#define PAGE_SIZE_WORDS     (1024U)                       /* 1K words per page in X16 mode */
#define PAGE_SIZE_BYTES     (PAGE_SIZE_WORDS * 2U)        /* 2KB per page */

/* BASE_ADDR is now 16-bit pointer */
#define BASE_ADDR ((volatile uint16_t *)XSPI1_BASE)

/* ===== GLOBAL ===== */
static uint32_t errorCount = 0;

/* ===== UTIL ===== */
static void report_error(uint32_t word_idx, uint16_t expected, uint16_t actual) {
//    printf("ERR @word[0x%08lX] byte_addr=0x%08lX: exp=0x%04X got=0x%04X\r\n",
//           word_idx, word_idx * 2U, expected, actual);
    errorCount++;
}

/* =========================================================
   1. ADDRESS LINE TEST
   ========================================================= */
static void psram_address_test(void) {
    printf("\r\n[Address Test]\r\n\r\n");

    /* max offset in words */
    uint32_t max_offset = PSRAM_SIZE_WORDS - 1U;

    BASE_ADDR[0] = 0xAAAA;

    for (uint32_t offset = 1U; offset <= max_offset; offset <<= 1U) {
        BASE_ADDR[offset] = 0x5555;

        printf("Testing address line: 0x%08lX (byte addr: 0x%08lX)\r\n",
               offset, offset * 2U);

        if (BASE_ADDR[0] != 0xAAAA) {
            printf("Address fault at word offset: 0x%08lX\r\n", offset);
            errorCount++;
        }
    }

    HAL_Delay(1);
}

/* =========================================================
   2. WALKING BIT TEST (DATA LINES)
   ========================================================= */
static void psram_walking_bit_test(void) {
    printf("\r\n[Walking Bit Test]\r\n\r\n");

    /* 16 bits now so walk all 16 bits */
    for (uint8_t bit = 0U; bit < 16U; bit++) {

        uint16_t pattern = (uint16_t)(1U << bit);
        printf("\r\nTesting bit idx: %u", bit);

        printf("\r\n ---- normal pattern ----\r\n\r\n");

        for (uint32_t i = 0U; i < PSRAM_SIZE_WORDS; i++) {
            BASE_ADDR[i] = pattern;
        }

        HAL_Delay(1);

        for (uint32_t i = 0U; i < PSRAM_SIZE_WORDS; i++) {
            uint16_t actual = BASE_ADDR[i];
            if (actual != pattern) {
                report_error(i, pattern, actual);
            }
        }

        printf("\r\n ---- inverse pattern ----\r\n\r\n");

        uint16_t inv_pattern = (uint16_t)(~(1U << bit));

        for (uint32_t i = 0U; i < PSRAM_SIZE_WORDS; i++) {
            BASE_ADDR[i] = inv_pattern;
        }

        HAL_Delay(1);

        for (uint32_t i = 0U; i < PSRAM_SIZE_WORDS; i++) {
            uint16_t actual = BASE_ADDR[i];
            if (actual != inv_pattern) {
                report_error(i, inv_pattern, actual);
            }
        }
    }
}

/* =========================================================
   3. PATTERN TEST
   ========================================================= */
static void psram_pattern_test(void) {
    printf("\r\n[Pattern Test]\r\n\r\n");

    /* ---- write ---- */
    for (uint32_t i = 0U; i < PSRAM_SIZE_WORDS; i++) {
        /* pattern uses both bytes of the 16-bit word */
        uint16_t pattern = (uint16_t)(
            ((i & 0xFFU) ^ ((i >> 8U) & 0xFFU)) |          /* low byte */
            ((((i + 1U) & 0xFFU) ^ ((i >> 7U) & 0xFFU)) << 8U)  /* high byte */
        );
        BASE_ADDR[i] = pattern;
    }

    HAL_Delay(1);

    /* ---- read ---- */
    for (uint32_t i = 0U; i < PSRAM_SIZE_WORDS; i++) {
        uint16_t expected = (uint16_t)(
            ((i & 0xFFU) ^ ((i >> 8U) & 0xFFU)) |
            ((((i + 1U) & 0xFFU) ^ ((i >> 7U) & 0xFFU)) << 8U)
        );
        uint16_t actual = BASE_ADDR[i];

        if (actual != expected) {
            report_error(i, expected, actual);
        }
    }
}

/* =========================================================
   MAIN TEST ENTRY
   ========================================================= */
void psram_test(void) {

    printf("\r\n==== PSRAM TEST START ====\r\n");
    printf("Mode: X16 | Size: %lu MB | Pages: %lu x %u bytes\r\n",
           PSRAM_SIZE_BYTES / MB,
           PSRAM_SIZE_BYTES / PAGE_SIZE_BYTES,
           PAGE_SIZE_BYTES);

    errorCount = 0;

//    psram_address_test();
//    psram_walking_bit_test();
    psram_pattern_test();

    if (errorCount == 0) {
        printf("\r\n==== SUCCESS: NO ERRORS ====\r\n");
    } else {
        printf("\r\n==== FAIL: %lu ERRORS ====\r\n", errorCount);
    }
}

#endif
