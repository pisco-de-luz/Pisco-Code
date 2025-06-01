#include "signal_sequencer.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"

namespace pisco_code
{
    void SignalSequencer::clear() noexcept
    {
        signal_stack_.clear();
        repeat_times_ = 0;
        signal_size_  = 0;
        signal_index_ = 0;
        signal_times_ = 0;
    }

    void SignalSequencer::loadFromCode(BlinkCode code, NumberBase base,
                                       NumDigits min_digits) noexcept
    {
        signal_stack_.clear();

        const bool is_negative = (code < 0);
        BlinkCode  abs_code    = is_negative ? -code : code;

        const DigitValue base_val    = to_value(base);
        NumDigits        digit_count = 0;

        do
        {
            const DigitValue digit = to_digit(abs_code % base_val);
            abs_code /= base_val;

            signal_stack_.push(digit == 0 ? SIGNAL_ELEMENT_ZERO_GAP
                                          : signal_element_digit_peak(digit));
            ++digit_count;
        } while (abs_code > 0);

        while (digit_count < min_digits)
        {
            signal_stack_.push(SIGNAL_ELEMENT_ZERO_GAP);
            ++digit_count;
        }

        if (is_negative)
        {
            signal_stack_.push(SIGNAL_ELEMENT_NEGATIVE_PEAK);
        }
    }
} // namespace pisco_code