#include "signal_sequencer.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_pulse_iterator.hpp"

namespace pisco_code
{
    void SignalSequencer::clear() noexcept
    {
        signal_stack_.clear();
        repeat_index_       = 0;
        element_count_      = 0;
        element_index_      = 0;
        pulse_repeat_count_ = 0;
        pulse_repeat_index_ = 0;
    }

    void SignalSequencer::setRepeatTimes(RepeatTimes repeat_times) noexcept
    {
        repeat_count_ = repeat_times;
    }

    [[nodiscard]] RepeatTimes SignalSequencer::getRepeatTimes() const noexcept
    {
        return repeat_count_;
    }

    [[nodiscard]] SignalPulseIterator
    SignalSequencer::createPulseIterator() const noexcept
    {
        return SignalPulseIterator{signal_stack_};
    }

    [[nodiscard]] bool SignalSequencer::shouldRepeat() const noexcept
    {
        return repeat_index_ < repeat_count_ - 1;
    }

    void SignalSequencer::incrementRepeatIndex() noexcept
    {
        if (shouldRepeat())
        {
            ++repeat_index_;
        }
    }
    [[nodiscard]] const SignalStack& SignalSequencer::stack() const noexcept
    {
        return signal_stack_;
    }

    void SignalSequencer::loadSignalCode(SignalCode code, NumberBase base,
                                         NumDigits num_digits) noexcept
    {
        if (!isValidCodeForBase(base, code))
        {
            return;
        }

        signal_stack_.clear();

        const NumDigits max_digits = max_digits_for_base(base);
        const bool      is_num_digits_valid =
            (num_digits > 0 && num_digits <= max_digits);
        const NumDigits max_digits_to_show =
            is_num_digits_valid ? num_digits : max_digits;

        const DigitValue base_val    = to_value(base);
        UnsignedCode     remaining   = to_unsigned(code);
        NumDigits        digit_count = 0;
        do
        {
            signal_stack_.push(
                signal_element_from_digit(to_digit(remaining % base_val)));
            remaining /= base_val;
            ++digit_count;
        } while (remaining > 0 && digit_count < max_digits_to_show);

        // Pad with leading zeros only when a fixed width was requested
        if (is_num_digits_valid)
        {
            while (digit_count < num_digits)
            {
                signal_stack_.push(SIGNAL_ELEMENT_ZERO);
                ++digit_count;
            }
        }

        const bool is_negative = (code < 0);
        if (is_negative)
        {
            signal_stack_.push(SIGNAL_ELEMENT_NEGATIVE);
        }
        signal_stack_.rewind();
    }
} // namespace pisco_code