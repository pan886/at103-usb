/**
 * @file main.c
 * @brief flash erase, program and read test case.
 * @author Dong Qin (dong.qin@timesintelli.com)
 * @version 1.0
 * @date 2022-05-17
 * @copyright Copyright (c) 2022 Timesintelli, Inc
 *
 * Test function    : flash erase, program and read test.
 * Board Version    ：AiP32rv001_EVB_V01
 * Hardware setup   : Without.
 */

#include "at103.h"
#include "unity.h"

static void test_func(void);

void unity_test(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_func);
    UNITY_END();
}

void main(void)
{
    pll_init();
    sys_io_init();
    uart_init(UART_BOOT_PORT, UART_PARITY_NONE, UART_STOPBITS_1, UART_DATABITS_8, UART_BOOT_BD);
    debug("FLASH test start!\r\n");
    unity_test();
    while (1) {
        /* test end */
    }
}

static void test_func(void)
{
    FLASH_Unlock();

    FLASH_UpdateLatency();

    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    TEST_ASSERT_MESSAGE(FLASH_GetPrefetchBufferStatus() == SET, "Failed to enable flash prefetch buffer.");

    FLASH_EraseAllPages();
    TEST_ASSERT_MESSAGE(FLASH_GetStatus() == FLASH_COMPLETE, "Failed to erase all flash pages.");

    for (uint32_t i = 0; i < 128 * 257; i++) {
        FLASH_ProgramDoubleWord(FLASH_BASE + i * 8, (uint64_t)i);
        TEST_ASSERT_MESSAGE(FLASH_GetStatus() == FLASH_COMPLETE, "Failed to program all flash pages.");
    }

    for (uint32_t i = 0; i < 128 * 257; i++) {
        TEST_ASSERT_MESSAGE(*(uint64_t *)(FLASH_BASE + i * 8) == i, "Failed to read all flash pages.");
    }

    FLASH_ErasePage(FLASH_BASE);
    TEST_ASSERT_MESSAGE(FLASH_GetStatus() == FLASH_COMPLETE, "Failed to erase flash page0.");

    for (uint32_t i = 0; i < 256; i++) {
        TEST_ASSERT_MESSAGE(*(uint32_t *)(FLASH_BASE + i * 4) == (uint32_t)0xFFFFFFFF, "Failed to read flash page0.");
    }

    TEST_ASSERT_MESSAGE(FLASH_EnableWriteProtection(FLASH_WRProt_Pages32to63) == FLASH_COMPLETE, "Failed to read flash page0.");
    FLASH_ErasePage(FLASH_BASE);
    /* Wait for last operation to be completed */
    if (FLASH_WaitForLastOperation(0xFFFF) == FLASH_COMPLETE) {
        *(__IO uint64_t *)(FLASH_BASE + 1) = (uint64_t)0x02;
        /* Wait for last operation to be completed */
        FLASH_WaitForLastOperation(FLASH_COMPLETE);
    }
    TEST_ASSERT_MESSAGE(FLASH_GetFlagStatus(FLASH_FLAG_UNAL_ERR) == SET, "Failed to trigger flash flag.");
    FLASH_ClearFlag(FLASH_FLAG_UNAL_ERR);
    TEST_ASSERT_MESSAGE(FLASH_GetFlagStatus(FLASH_FLAG_UNAL_ERR) == RESET, "Failed to clear flash flag.");

    FLASH_Lock();

    debug("\r\nFLASH test completed.\r\n");
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    debug("Wrong parameters value: file %s on line %d\r\n", file, line);
    TEST_ASSERT_MESSAGE(0, "FLASH test FAIL.");
}
#endif
