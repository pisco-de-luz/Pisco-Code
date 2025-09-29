#pragma once
#include "CppUTest/SimpleString.h"

#include "signal_element.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"
#include "tests_constants.hpp"

namespace pisco_code
{
    // Equality for tests
    inline bool operator==(const SignalElement& element_a,
                           const SignalElement& element_b) noexcept
    {
        return to_value(element_a.get_mode()) ==
                   to_value(element_b.get_mode()) &&
               to_value(element_a.get_duration()) ==
                   to_value(element_b.get_duration()) &&
               to_value(element_a.get_times()) ==
                   to_value(element_b.get_times());
    }

    inline bool operator!=(const SignalElement& element_a,
                           const SignalElement& element_b) noexcept
    {
        return !(element_a == element_b);
    }
} // namespace pisco_code

// Must be in the global namespace so CppUTest finds it
inline SimpleString
StringFrom(const pisco_code::SignalElement& element)
{
    return StringFromFormat(
        "{L=%u D=%u T=%u}", static_cast<unsigned>(to_value(element.get_mode())),
        static_cast<unsigned>(to_value(element.get_duration())),
        static_cast<unsigned>(to_value(element.get_times())));
}