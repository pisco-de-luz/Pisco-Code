#ifndef PISCO_CONSTANTS_HPP
#define PISCO_CONSTANTS_HPP

#include <stdint.h>

#include "pisco_types.hpp"

namespace pisco_code
{

    enum class LedControlCode : LedCodeType
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
    enum class NumberBase : BaseType
    {
        BINARY      = 2,
        OCTAL       = 8,
        DECIMAL     = 10,
        HEXADECIMAL = 16
    };

    constexpr DigitValue to_value(NumberBase base)
    {
        return static_cast<DigitValue>(base);
    }

    // Lookup table indexed by base value (2 to 16), all other entries are zero
    constexpr NumDigits MAX_DIGITS_LUT[to_value(NumberBase::HEXADECIMAL) + 1] = {
        0,  // 0
        0,  // 1
        24, // 2 = BINARY
        0,  0, 0, 0, 0,
        9, // 8 = OCTAL
        0, // 9
        9, // 10 = DECIMAL
        0,  0, 0, 0, 0,
        7 // 16 = HEXADECIMAL
    };

    constexpr NumDigits maxDigitsForBase(NumberBase base)
    {
        return MAX_DIGITS_LUT[to_value(base)];
    }

    // Compile-time checks to catch omissions
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::BINARY)] > 0, "Missing BINARY max digits");
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::OCTAL)] > 0, "Missing OCTAL max digits");
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::DECIMAL)] > 0, "Missing DECIMAL max digits");
    static_assert(MAX_DIGITS_LUT[to_value(NumberBase::HEXADECIMAL)] > 0,
                  "Missing HEXADECIMAL max digits");

    // Maximum number of digits limited to 9 for 32-bit integer representation
    constexpr uint8_t MAX_DIGITS_ABSOLUTE = 31; // for int32_t in binary base

    constexpr uint8_t  DEFAULT_DIMMED_LEVEL = 3;
    constexpr uint8_t  PWM_MAX              = 15;
    constexpr LedLevel MIN_PULSE_DIMMED_GAP = 2;
    constexpr uint8_t  DEFAULT_PULSE_LEVEL  = PWM_MAX;

    // Loop timing constants
    constexpr uint32_t LOOP_INTERVAL_MS       = 64;
    constexpr uint32_t INIT_PHASE_MS          = 2000;
    constexpr uint32_t INIT_DIMMED_PHASE_MS   = 1300;
    constexpr uint32_t END_DIMMED_PHASE_MS    = 300;
    constexpr uint32_t NEGATIVE_BLINK_LONG_MS = 1100;
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

} // namespace pisco_code

#endif // PISCO_CONSTANTS_HPP
