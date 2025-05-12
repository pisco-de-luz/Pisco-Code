#ifndef PISCO_CONSTANTS_HPP
#define PISCO_CONSTANTS_HPP

#include <stdint.h>

namespace pisco
{

    // LED control codes
    constexpr uint8_t LED_ON      = 0;
    constexpr uint8_t LED_OFF     = 1;
    constexpr uint8_t LED_FUNC_OK = 100;

    // Encoding bases
    enum class base_t : uint8_t
    {
        BINARY      = 2,
        OCTAL       = 8,
        DECIMAL     = 10,
        HEXADECIMAL = 16
    };

    using Timestamp     = uint32_t;
    using StateDuration = uint16_t;
    using LedLevel      = uint8_t;

    // LED brightness control
    constexpr uint8_t MAX_DIGITS         = 10;
    constexpr uint8_t INITIAL_DIMMED_PWM = 3;
    constexpr uint8_t PWM_MAX            = 15;

    // Loop timing constants
    constexpr uint32_t LOOP_INTERVAL_MS       = 64;
    constexpr uint32_t INIT_PHASE_MS          = 2000;
    constexpr uint32_t INIT_DIMMED_PHASE_MS   = 1300;
    constexpr uint32_t END_DIMMED_PHASE_MS    = 300;
    constexpr uint32_t NEGATIVE_BLINK_LONG_MS = 1800;
    constexpr uint32_t SHORT_BLINK_MS         = 350;
    constexpr uint32_t ZERO_DIGIT_BLINK_MS    = 440;
    constexpr uint32_t BETWEEN_BLINK_MS       = 350;
    constexpr uint32_t BETWEEN_DIGITS_MS      = 1200;
    constexpr uint32_t BETWEEN_CODES_MS       = 1500;
    constexpr uint32_t END_PHASE_MS           = 2000;

    // Conversion from ms to loop ticks
    constexpr uint8_t to_loop_count(uint32_t ms)
    {
        return static_cast<uint8_t>(ms / LOOP_INTERVAL_MS);
    }

} // namespace pisco

#endif // PISCO_CONSTANTS_HPP
