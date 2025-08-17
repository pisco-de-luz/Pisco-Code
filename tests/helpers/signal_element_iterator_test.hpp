#pragma once
#include "CppUTest/SimpleString.h"

#include "signal_element.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

namespace pisco_code
{
    // Equality for tests
    inline bool operator==(const SignalElement& a,
                           const SignalElement& b) noexcept
    {
        return to_value(a.get_level()) == to_value(b.get_level()) &&
               to_value(a.get_duration()) == to_value(b.get_duration()) &&
               to_value(a.get_times()) == to_value(b.get_times());
    }

    inline bool operator!=(const SignalElement& a,
                           const SignalElement& b) noexcept
    {
        return !(a == b);
    }
} // namespace pisco_code

// Must be in the global namespace so CppUTest finds it
inline SimpleString
StringFrom(const pisco_code::SignalElement& e)
{
    return StringFromFormat("{L=%u D=%u T=%u}",
                            static_cast<unsigned>(to_value(e.get_level())),
                            static_cast<unsigned>(to_value(e.get_duration())),
                            static_cast<unsigned>(to_value(e.get_times())));
}