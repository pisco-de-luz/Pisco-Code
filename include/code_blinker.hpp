// code_blinker.hpp
#ifndef PISCO_CODE_BLINKER_HPP
#define PISCO_CODE_BLINKER_HPP

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include <stdint.h>

namespace pisco
{

    class CodeBlinker
    {
      public:
        explicit CodeBlinker(LedController* controller);

        bool showCode(int32_t code, base_t base, uint8_t min_digits, uint8_t repeats);
        void loop(uint8_t loop_counter);
        bool isRunning() const;
        bool isLedBeingUsedNow() const;
        void setPeakLevel(uint8_t level);
        void setDimmedLevel(uint8_t level);

      private:
        enum class Phase : uint8_t
        {
            Paused = 0,
            Start,
            NegativeOn,
            NegativeOff,
            ReadNextDigit,
            PulseOn,
            PulseOff,
            End,
            Repeat,
            FinalPause,
            ZeroDigit,
        };

        void transitionTo(Phase next, uint8_t duration, uint8_t loop_counter);
        bool phaseElapsed(uint8_t loop_counter) const;

        bool hasMoreBlinks() const;
        bool hasMoreDigits() const;
        bool shouldRepeat() const;

        void handlePaused(uint8_t loop_counter);
        void handleStart(uint8_t loop_counter);
        void handleNegativeOn(uint8_t loop_counter);
        void handleNegativeOff(uint8_t loop_counter);
        void handleReadNextDigit(uint8_t loop_counter);
        void handlePulseOn(uint8_t loop_counter);
        void handlePulseOff(uint8_t loop_counter);
        void handleZeroDigit(uint8_t loop_counter);
        void handleEnd(uint8_t loop_counter);
        void handleRepeat(uint8_t loop_counter);
        void handleFinalPause(uint8_t loop_counter);

        LedController* controller_ = nullptr;

        uint8_t current_digit_            = 0;
        uint8_t least_significant_digit_  = 0;
        int8_t  blink_counts_[MAX_DIGITS] = {};
        uint8_t digits_[MAX_DIGITS]       = {};

        uint8_t         repeats_total_     = 0;
        mutable uint8_t repeats_remaining_ = 0;

        uint8_t start_time_     = 0;
        uint8_t phase_duration_ = 0;
        Phase   current_phase_  = Phase::Paused;

        bool    is_negative_  = false;
        uint8_t peak_level_   = PWM_MAX;
        uint8_t pwm_counter_  = 0;
        uint8_t dimmed_level_ = INITIAL_DIMMED_PWM;
    };

} // namespace pisco

#endif // PISCO_CODE_BLINKER_HPP
