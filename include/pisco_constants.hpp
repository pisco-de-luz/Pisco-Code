#ifndef PISCO_CONSTANTS_HPP
#define PISCO_CONSTANTS_HPP

#include "pisco_types.hpp"
#include <stdint.h>

namespace pisco
{

    enum class LedControlCode : uint8_t
    {
        On       = 0,
        Off      = 1,
        FuncOk   = 100,
        FuncFail = 101 // optional, used in Mock logger
    };

    // LED control codes
    constexpr LedControlCode LED_ON      = LedControlCode::On;
    constexpr LedControlCode LED_OFF     = LedControlCode::Off;
    constexpr LedControlCode LED_FUNC_OK = LedControlCode::FuncOk;

    // Encoding bases
    enum class NumberBase : uint8_t
    {
        BINARY      = 2,
        OCTAL       = 8,
        DECIMAL     = 10,
        HEXADECIMAL = 16
    };

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
