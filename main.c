#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_trng.h"

#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "wolfcrypt/benchmark/benchmark.h"
            
        
int32_t cust_rand_generate_block(uint8_t *rndb, uint32_t sz)
{
    status_t status;
    status = TRNG_GetRandomData(TRNG, &rndb,sz);
    if (status != kStatus_Success) {
        while(1)
            ;;
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
    BOARD_InitDebugConsole();
    TRNG_GetDefaultConfig(&trngConfig);
    trngConfig.sampleMode = kTRNG_SampleModeVonNeumann;

    PRINTF("wolfCrypt Benchmark!\r\n");
    benchmark_init();
    benchmark_test(NULL);

   while (1)
    {
        ch = GETCHAR();
        PUTCHAR(ch);
    }


}
