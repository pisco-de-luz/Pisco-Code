#include "signal_sequencer.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    void SignalSequencer::clear() noexcept
    {
        signal_stack_.clear();
        repeat_count_       = 1;
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

    [[nodiscard]] bool
    SignalSequencer::hasMoreSignalCodeToSequence() const noexcept
    {
        return repeat_index_ < repeat_count_ && signal_stack_.size() > 0;
    }

    [[nodiscard]] bool SignalSequencer::hasMoreSignalElements() const noexcept
    {
        return element_index_ < element_count_;
    }

    [[nodiscard]] bool SignalSequencer::hasMorePulse() const noexcept
    {
        return pulse_repeat_index_ < pulse_repeat_count_;
    }

    [[nodiscard]] SignalPulseIterator
    SignalSequencer::createPulseIterator() const noexcept
    {
        return SignalPulseIterator{signal_stack_};
    }

    void SignalSequencer::loadSignalCode(SignalCode code, NumberBase base,
                                         NumDigits num_digits) noexcept
    {
        signal_stack_.clear();

        const DigitValue base_val = to_value(base);
        if (!base_supported(base) || !isCodeInRange(base, code))
        {
            return;
        }

        const bool is_negative = (code < 0);
        SignalCode abs_code    = is_negative ? -code : code;

        const NumDigits max_digits = max_digits_for_base(base);
        const bool      is_num_digits_valid =
            (num_digits > 0 && num_digits <= max_digits);
        const NumDigits max_digits_to_show =
            is_num_digits_valid ? num_digits : max_digits;

        NumDigits digit_count = 0;
        for (;;)
        {
            const DigitValue digit = to_digit(abs_code % base_val);
            abs_code /= base_val;

            signal_stack_.push(signal_element_from_digit(digit));
            ++digit_count;

            if (abs_code == 0 || digit_count >= max_digits_to_show)
            {
                break;
            }
        }

        // Pad with leading zeros only when a fixed width was requested
        if (is_num_digits_valid)
        {
            while (digit_count < num_digits)
            {
                signal_stack_.push(SIGNAL_ELEMENT_ZERO);
                ++digit_count;
            }
        }

        if (is_negative)
        {
            signal_stack_.push(SIGNAL_ELEMENT_NEGATIVE);
        }
        signal_stack_.rewind();
    }

    SignalElement SignalSequencer::popNextSignalElement() noexcept
    {
        pulse_repeat_index_ = 0;
        if (hasMoreSignalElements())
        {
            const SignalElement element = signal_stack_.pop();
            ++element_index_;
            pulse_repeat_count_ = element.get_times();
            return element;
        }
        pulse_repeat_count_ = 0;
        return SIGNAL_ELEMENT_NOT_DEFINED; // Return a default element if no
                                           // more elements
    }

    void SignalSequencer::popNextPulse() noexcept
    {
        if (hasMorePulse())
        {
            ++pulse_repeat_index_;
        }
    }
    void SignalSequencer::popNextCodeToSequence() noexcept
    {
        if (hasMoreSignalCodeToSequence())
        {
            ++repeat_index_;
            signal_stack_.rewind();
            element_count_      = signal_stack_.size();
            element_index_      = 0;
            pulse_repeat_index_ = 0;
            pulse_repeat_count_ = 0;
        }
    }
} // namespace pisco_code