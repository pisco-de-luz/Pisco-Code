#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_types.hpp"

using namespace pisco_code;

namespace pisco_code
{
    class SignalStack
    {
      public:
        [[nodiscard]] constexpr SignalStack() noexcept = default;

        void                        pushNewSignalUnit(SignalElement unit) noexcept;
        [[nodiscard]] bool          hasNextSignalUnit() const noexcept;
        [[nodiscard]] SignalElement popNextSignalUnit() noexcept;
        [[nodiscard]] Counter       size() const noexcept;
        void                        clear() noexcept;
        void                        rewind() noexcept;

        // Placeholder for logic to populate based on number
        void generateFromCode(BlinkCode code, NumberBase base, NumDigits min_digits = 0) noexcept;

      private:
        SignalElement signal_elements_[MAX_SIGNAL_ELEMENTS]{};
        Counter       count_{0};
        Index         read_index_{0};
    };
} // namespace pisco_code