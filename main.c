/* wolfCrypt benchmark test application for i.MX RT1060-EVK
 *
 * Copyright (C) 2020 wolfSSL Inc.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */
#include "user_settings.h"
#include "wolfssl/wolfcrypt/settings.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_trng.h"

#include "board.h"
#include <stdio.h>

#include "pin_mux.h"
#include "clock_config.h"
#include "wolfcrypt/benchmark/benchmark.h"

#include <time.h>

volatile uint32_t g_systickCounter;
struct timezone;

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
    (void)tz;
    tv->tv_sec = g_systickCounter / 1000;
    tv->tv_usec = (g_systickCounter % 1000) * 1000;
    return 0;
}

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    g_systickCounter++;
}

int32_t cust_rand_generate_block(uint8_t *rndb, uint32_t sz)
{
    status_t status;
    status = TRNG_GetRandomData(TRNG, rndb,sz);
    if (status != kStatus_Success) {
        PRINTF("ERROR: TRNG STATUS: %d\r\n");
        return -1;
    }
    return 0;
}


void main(void)
{
    char ch;
    trng_config_t trngConfig;
    /* Init board hardware. */
    BOARD_ConfigMPU();
    BOARD_InitPins();
    BOARD_InitBootClocks();
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000U);
    BOARD_InitDebugConsole();
    TRNG_GetDefaultConfig(&trngConfig);
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;
    benchmark_init();
    benchmark_test(NULL);

   while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }

}
