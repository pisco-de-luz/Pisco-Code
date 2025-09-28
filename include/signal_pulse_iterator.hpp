#pragma once

#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_element_iterator.hpp"
#include "signal_stack.hpp"

namespace pisco_code
{
    /// Produces a sequence with leading FRAMING, inter-symbol separators,
    /// the symbol elements from SignalElementIterator, and trailing FRAMING.

    class SignalPulseIterator
    {
      public:
        explicit SignalPulseIterator(const SignalStack& signal_stack) noexcept :
            symbols_(signal_stack), symbol_remaining_(symbols_.size())
        {
        }

        [[nodiscard]] SignalElement next() noexcept
        {
            SignalElement element{SIGNAL_ELEMENT_NOT_DEFINED};
            if (!hasNext())
            {
                return SIGNAL_ELEMENT_NOT_DEFINED;
            }
            if (need_inter_symbol_)
            {
                if (symbols_.isEmpty())
                {
                    current_phase_ = Phase::TRAILING_FRAME;
                }
                else
                {
                    current_symbol_ = symbols_.pop();
                    --symbol_remaining_;
                    element_iterator_ = SignalElementIterator(current_symbol_);
                    current_phase_    = Phase::IN_ELEMENTS;
                }

                need_inter_symbol_ = false;
                element            = SIGNAL_ELEMENT_INTER_SYMBOL;
            }
            else
            {
                switch (current_phase_)
                {
                    case Phase::LEADING_FRAME:
                    {
                        need_inter_symbol_ = true;
                        element            = SIGNAL_ELEMENT_FRAMING;
                        break;
                    }
                    case Phase::IN_ELEMENTS:
                    {
                        if (!element_iterator_.hasNext())
                        {
                            break;
                        }
                        element = element_iterator_.next();
                        if (!element_iterator_.hasNext())
                        {
                            need_inter_symbol_ = true;
                            if (allPulsesProcessed())
                            {
                                current_phase_ = Phase::TRAILING_FRAME;
                            }
                        }
                        break;
                    }
                    case Phase::TRAILING_FRAME:
                    {
                        current_phase_     = Phase::DONE;
                        need_inter_symbol_ = false;
                        element            = SIGNAL_ELEMENT_FRAMING;
                        break;
                    }
                    // Should never come here, but added for completeness
                    case Phase::DONE:
                    default:
                    {
                        return SIGNAL_ELEMENT_NOT_DEFINED;
                    }
                }
            }
            return element;
        }

        [[nodiscard]] bool hasNext() const noexcept
        {
            if (current_phase_ == Phase::DONE)
            {
                return false;
            }
            if (current_phase_ == Phase::LEADING_FRAME)
            {
                return (symbol_remaining_ > 0);
            }
            return true;
        }

        void reset() noexcept
        {
            current_phase_    = Phase::LEADING_FRAME;
            symbol_remaining_ = symbols_.size();
            element_iterator_ =
                SignalElementIterator(SIGNAL_ELEMENT_NOT_DEFINED);
            current_symbol_    = SIGNAL_ELEMENT_NOT_DEFINED;
            need_inter_symbol_ = false;
            symbols_.rewind();
        }

      private:
        enum class Phase : PhaseType
        {
            LEADING_FRAME = 0,
            IN_ELEMENTS,
            TRAILING_FRAME,
            DONE,
            LAST_PHASE,
        };

        SignalStack           symbols_;
        SignalElementIterator element_iterator_{SIGNAL_ELEMENT_NOT_DEFINED};
        SignalElement         current_symbol_{SIGNAL_ELEMENT_NOT_DEFINED};
        Index                 symbol_remaining_{0};
        bool                  need_inter_symbol_{false};
        Phase                 current_phase_{Phase::LEADING_FRAME};
        [[nodiscard]] bool    allPulsesProcessed() const noexcept
        {
            return (symbol_remaining_ == 0) && (need_inter_symbol_);
        }
    };
    constexpr auto MAX_STACK_BYTES_PASS_BY_VALUE = 16;

    static_assert(sizeof(SignalStack) <= MAX_STACK_BYTES_PASS_BY_VALUE,
                  "Stack grew, reconsider pass-by-value");

} // namespace pisco_code