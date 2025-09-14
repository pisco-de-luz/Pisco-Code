#include "avr_systick.hpp"

#include <avr/interrupt.h>
#include <avr/io.h>

namespace avr_systick
{
    volatile uint32_t g_ms_ticks = 0;

    void init_1ms() noexcept
    {
        cli();
        // 1 kHz with prescaler 64
        const uint32_t ocr = (static_cast<uint32_t>(F_CPU) / 64u / 1000u) - 1u;

        TCCR1A = 0u;
        TCCR1B = 0u;

        // CTC mode on OCR1A
        TCCR1B |= (1u << WGM12);

        OCR1A = static_cast<uint16_t>(ocr);
        TCNT1 = 0u;

        // Enable compare A interrupt
        TIMSK1 |= (1u << OCIE1A);

        // Start timer, prescaler 64
        TCCR1B |= (1u << CS11) | (1u << CS10);

        sei();
    }

    // Busy wait using the millisecond tick
    void delay_ms(uint32_t ms) noexcept
    {
        const uint32_t start = g_ms_ticks;
        while ((g_ms_ticks - start) < ms)
        {
        }
    }

} // namespace avr_systick

ISR(TIMER1_COMPA_vect)
{
    ++avr_systick::g_ms_ticks;
}
