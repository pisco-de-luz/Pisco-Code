#include <stdint.h>

#include "board_config.hpp"

void
HardFault_Handler(void)
{
    // Use HAL function - clean and reliable
    NVIC_SystemReset();

    // Fallback (should never reach here)
    while (1)
    {
    }
}
