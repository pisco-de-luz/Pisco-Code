#include "signal_units.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace pisco_code
{

    [[nodiscard]] bool SignalSequence::add(SignalUnit unit) noexcept
    {
        if (count_ >= MAX_SIGNAL_UNITS)
        {
            return false;
        }
        units_[count_++] = unit;
        return true;
    }

    [[nodiscard]] const SignalUnit& SignalSequence::at(Index index) const noexcept
    {
        // No bounds checking for performance; must be validated externally
        return units_[index];
    }

    [[nodiscard]] Counter SignalSequence::size() const noexcept
    {
        return count_;
    }

    void SignalSequence::clear() noexcept
    {
        count_ = 0;
    }

    void SignalSequence::generateFromCode(BlinkCode code, NumberBase base,
                                          NumDigits min_digits) noexcept
    {
        clear();

        const bool is_negative = (code < 0);
        BlinkCode  abs_code    = is_negative ? -code : code;

        const NumDigits max_digits    = to_digit(MAX_SIGNAL_UNITS); // Simplified placeholder
        Index           first_nonzero = to_index(max_digits - 1);

        DigitValue digits[MAX_SIGNAL_UNITS]{};

        for (Index i = max_digits - 1; i >= 0; --i)
        {
            const DigitValue digit = to_digit(abs_code % to_value(base));
            digits[to_index(i)]    = digit;
            if (digit > 0)
            {
                first_nonzero = i;
            }
            abs_code /= to_value(base);
        }

        const Index start_index = (min_digits > 0 && min_digits <= max_digits)
                                      ? to_index(max_digits - min_digits)
                                      : first_nonzero;

        if (is_negative)
        {
            (void) add(SignalUnit(SignalLevel::PEAK, 1, SignalDuration::LONG));
        }

        for (Index i = start_index; i < max_digits; ++i)
        {
            const DigitValue digit = digits[to_index(i)];
            if (digit == 0)
            {
                (void) add(SignalUnit(SignalLevel::GAP, 1, SignalDuration::SHORT));
            }
            else
            {
                (void) add(SignalUnit(SignalLevel::PEAK, digit, SignalDuration::SHORT));
            }
        }
    }

} // namespace pisco_code
