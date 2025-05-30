#include "signal_units.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace pisco_code
{
    // [[nodiscard]] constexpr SignalStack::SignalStack() noexcept
    // {
    //     clear();
    // }

    [[nodiscard]] Counter SignalStack::size() const noexcept
    {
        return count_;
    }

    void SignalStack::clear() noexcept
    {
        for (Index i = 0; i < MAX_SIGNAL_UNITS; ++i)
        {
            signal_units_[i] = SIGNAL_UNIT_NOT_DEFINED;
        }
        count_      = 0;
        read_index_ = 0;
    }

    void SignalStack::pushNewSignalUnit(SignalUnit unit) noexcept
    {
        if (count_ < MAX_SIGNAL_UNITS)
        {
            signal_units_[MAX_SIGNAL_UNITS - 1 - count_] = unit;
            ++count_;
        }
    }

    bool SignalStack::hasNextSignalUnit() const noexcept
    {
        return read_index_ < count_;
    }

    SignalUnit SignalStack::popNextSignalUnit() noexcept
    {
        if (read_index_ < count_)
        {
            return signal_units_[MAX_SIGNAL_UNITS - count_ + read_index_++];
        }
        return SIGNAL_UNIT_NOT_DEFINED;
    }

    void SignalStack::generateFromCode(BlinkCode code, NumberBase base,
                                       NumDigits min_digits) noexcept
    {
        clear();

        const bool is_negative = (code < 0);
        BlinkCode  abs_code    = is_negative ? -code : code;

        const DigitValue base_val    = to_value(base);
        NumDigits        digit_count = 0;

        do
        {
            const DigitValue digit = to_digit(abs_code % base_val);
            abs_code /= base_val;

            pushNewSignalUnit(digit == 0 ? SIGNAL_UNIT_ZERO_GAP : signal_unit_digit_peak(digit));
            ++digit_count;
        } while (abs_code > 0);

        while (digit_count < min_digits)
        {
            pushNewSignalUnit(SIGNAL_UNIT_ZERO_GAP);
            ++digit_count;
        }

        if (is_negative)
        {
            pushNewSignalUnit(SIGNAL_UNIT_NEGATIVE_PEAK);
        }
    }
} // namespace pisco_code
