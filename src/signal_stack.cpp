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

} // namespace pisco_code