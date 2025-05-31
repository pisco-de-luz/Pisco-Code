#include "signal_stack.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    [[nodiscard]] Counter SignalStack::size() const noexcept
    {
        return count_;
    }

    void SignalStack::rewind() noexcept
    {
        read_index_ = 0;
    }

    void SignalStack::clear() noexcept
    {
        for (Index i = 0; i < MAX_SIGNAL_ELEMENTS; ++i)
        {
            signal_elements_[i] = SIGNAL_ELEMENT_NOT_DEFINED;
        }
        count_      = 0;
        read_index_ = 0;
    }

    void SignalStack::push(SignalElement unit) noexcept
    {
        if (!isFull())
        {
            signal_elements_[MAX_SIGNAL_ELEMENTS - 1 - count_] = unit;
            ++count_;
        }
    }

    bool SignalStack::isEmpty() const noexcept
    {
        return read_index_ == count_;
    }

    bool SignalStack::isFull() const noexcept
    {
        return count_ == MAX_SIGNAL_ELEMENTS;
    }

    SignalElement SignalStack::pop() noexcept
    {
        if (!isEmpty())
        {
            return signal_elements_[MAX_SIGNAL_ELEMENTS - count_ + read_index_++];
        }
        return SIGNAL_ELEMENT_NOT_DEFINED;
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

            push(digit == 0 ? SIGNAL_ELEMENT_ZERO_GAP : signal_element_digit_peak(digit));
            ++digit_count;
        } while (abs_code > 0);

        while (digit_count < min_digits)
        {
            push(SIGNAL_ELEMENT_ZERO_GAP);
            ++digit_count;
        }

        if (is_negative)
        {
            push(SIGNAL_ELEMENT_NEGATIVE_PEAK);
        }
    }
} // namespace pisco_code