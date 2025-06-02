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
        constexpr SignalStack() noexcept = default;

        void                        push(SignalElement unit) noexcept;
        [[nodiscard]] bool          isEmpty() const noexcept;
        [[nodiscard]] bool          isFull() const noexcept;
        [[nodiscard]] SignalElement pop() noexcept;
        [[nodiscard]] Counter       size() const noexcept;
        void                        clear() noexcept;
        void                        rewind() noexcept;

      private:
        SignalElement signal_elements_[MAX_SIGNAL_ELEMENTS]{};
        Counter       count_{0};
        Index         read_index_{0};
    };
} // namespace pisco_code