#include <stdint.h>

#define PERIPH_BASE 0x40000000u
#define AHB1PERIPH_BASE (PERIPH_BASE + 0x00020000u)
#define RCC_BASE (AHB1PERIPH_BASE + 0x3800u)
#define RCC_AHB1ENR (*(volatile uint32_t*) (RCC_BASE + 0x30u))

#define GPIOA_BASE (AHB1PERIPH_BASE + 0x0000u)
#define GPIOA_MODER (*(volatile uint32_t*) (GPIOA_BASE + 0x00u))
#define GPIOA_ODR (*(volatile uint32_t*) (GPIOA_BASE + 0x14u))

#define LD2_PIN 5u

void
HardFault_Handler(void)
{
    // make sure PA5 is an output
    RCC_AHB1ENR |= (1u << 0);
    GPIOA_MODER =
        (GPIOA_MODER & ~(3u << (LD2_PIN * 2u))) | (1u << (LD2_PIN * 2u));

    // fast blink forever to signal a fault
    for (;;)
    {
        GPIOA_ODR ^= (1u << LD2_PIN);
        for (volatile int i = 0; i < 60000; ++i)
        {
        }
    }
}
