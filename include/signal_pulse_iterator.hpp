#pragma once

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_element_iterator.hpp"
#include "signal_stack.hpp"
#include "signal_types.hpp"

namespace pisco_code
{
    class SignalPulseIterator
    {
      public:
        explicit SignalPulseIterator(SignalStack signal_stack) noexcept :
            symbols_(signal_stack), symbol_remaining_(signal_stack.size())
        {
        }

        [[nodiscard]] SignalElement next() noexcept
        {
            if (!hasNext())
            {
                return SIGNAL_ELEMENT_NOT_DEFINED;
            }
            if (need_inter_symbol_)
            {
                if (need_new_symbol_)
                {
                    if (symbols_.isEmpty())
                    {
                        current_phase_ = Phase::TRAILING_FRAME;
                    }
                    else
                    {
                        current_symbol_ = symbols_.pop();
                        --symbol_remaining_;
                        element_iterator_ =
                            SignalElementIterator(current_symbol_);
                        need_new_symbol_ = false;
                        current_phase_   = Phase::IN_ELEMENTS;
                    }
                }
                need_inter_symbol_ = false;
                return SIGNAL_ELEMENT_INTER_SYMBOL;
            }
            switch (current_phase_)
            {
                case Phase::LEADING_FRAME:
                {
                    need_inter_symbol_ = true;
                    return SIGNAL_ELEMENT_FRAMING;
                }
                case Phase::IN_ELEMENTS:
                {
                    SignalElement element{SIGNAL_ELEMENT_INTER_SYMBOL};
                    if (element_iterator_.hasNext())
                    {
                        need_new_symbol_ = false;
                        element          = element_iterator_.next();
                    }
                    else
                    {
                        need_new_symbol_ = true;
                    }

                    if (allPulsesProcessed())
                    {
                        need_new_symbol_ = false;
                        current_phase_   = Phase::TRAILING_FRAME;
                    }
                    return element;
                }
                case Phase::TRAILING_FRAME:
                {
                    current_phase_     = Phase::DONE;
                    need_inter_symbol_ = false;
                    return SIGNAL_ELEMENT_FRAMING;
                }
                // Should never come here, but added for completeness
                case Phase::DONE:
                default:
                {
                    return SIGNAL_ELEMENT_NOT_DEFINED;
                }
            }
        }

        [[nodiscard]] bool hasNext() const noexcept
        {
            return (current_phase_ != Phase::DONE || need_inter_symbol_);
        }

        void reset() noexcept
        {
            current_phase_     = Phase::LEADING_FRAME;
            symbol_remaining_  = symbols_.size();
            need_inter_symbol_ = false;
            need_new_symbol_   = true;
            symbols_.rewind();
        }

      private:
        enum class Phase : PhaseType
        {
            LEADING_FRAME = 0,
            IN_ELEMENTS,
            TRAILING_FRAME,
            DONE,
            LAST_PHASE, // Used to determine the size of the enum
        };

        SignalStack           symbols_;
        SignalElementIterator element_iterator_{SIGNAL_ELEMENT_NOT_DEFINED};
        SignalElement         current_symbol_{SIGNAL_ELEMENT_NOT_DEFINED};
        Counter               symbol_remaining_{0};
        bool                  need_inter_symbol_{false};
        bool                  need_new_symbol_{true};
        Phase                 current_phase_{Phase::LEADING_FRAME};
        [[nodiscard]] bool    allPulsesProcessed() const noexcept
        {
            return (symbol_remaining_ == 0) && (need_new_symbol_);
        }
    };
    constexpr auto MAX_BYTES_PASS_BY_VALUE = 16;

    static_assert(sizeof(SignalStack) <= MAX_BYTES_PASS_BY_VALUE,
                  "Stack grew, reconsider pass-by-value");

} // namespace pisco_code