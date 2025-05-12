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

        bool showCode(int32_t code, NumberBase base, uint8_t num_digits, uint8_t repeats);
        void loop(uint8_t loop_counter);
        bool isRunning() const;
        bool isLedBeingUsedNow() const;
        void setPeakLevel(uint8_t level);
        void setDimmedLevel(uint8_t level);

      private:
        enum class Phase : uint8_t
        {
            // LED is off (initial state)
            PauseBeforeStart = 0,

            // LED is dimmed (start of digit display)
            BeginDigit,

            // LED is ON (blink for negative sign)
            DisplayNegativeSign,

            // LED is dimmed (cooldown after minus blink)
            PauseAfterNegative,

            // LED is dimmed (preparing next digit)
            LoadNextDigit,

            // LED is ON (main blink pulse)
            EmitBlink,

            // LED is dimmed (between blinks of the same digit)
            PauseBetweenBlinks,

            // LED is dimmed (zero represented as a dim pause)
            DisplayZero,

            // LED is dimmed (pause before repeat or stop)
            EndOfDigitCycle,

            // LED is off (pause before next repetition)
            PrepareRepeat,

            // LED is off (pause after final repetition)
            PauseAfterFinish,

            // LED is off (system paused or stopped)
            Idle
        };

        void transitionTo(Phase next, uint8_t duration, uint8_t loop_counter);
        bool phaseElapsed(uint8_t loop_counter) const;

        bool hasMoreBlinks() const;
        bool hasMoreDigits() const;
        bool shouldRepeat() const;

        void handleIdle(uint8_t loop_counter);
        void handleBeginDigit(uint8_t loop_counter);
        void handleDisplayNegativeSign(uint8_t loop_counter);
        void handlePauseAfterNegative(uint8_t loop_counter);
        void handleLoadNextDigit(uint8_t loop_counter);
        void handleEmitBlink(uint8_t loop_counter);
        void handlePauseBetweenBlinks(uint8_t loop_counter);
        void handleDisplayZero(uint8_t loop_counter);
        void handleEndOfDigitCycle(uint8_t loop_counter);
        void handlePauseBeforeStart(uint8_t loop_counter);
        void handlePauseAfterFinish(uint8_t loop_counter);

        LedController* controller_ = nullptr;

        NumDigits current_digit_            = 0;
        NumDigits least_significant_digit_  = 0;
        int8_t    blink_counts_[MAX_DIGITS] = {};
        uint8_t   digits_[MAX_DIGITS]       = {};

        uint8_t         repeats_total_     = 0;
        mutable uint8_t repeats_remaining_ = 0;

        uint8_t start_time_     = 0;
        uint8_t phase_duration_ = 0;
        Phase   current_phase_  = Phase::Idle;

        bool    is_negative_  = false;
        uint8_t peak_level_   = PWM_MAX;
        uint8_t pwm_counter_  = 0;
        uint8_t dimmed_level_ = INITIAL_DIMMED_PWM;
    };

} // namespace pisco

#endif // PISCO_CODE_BLINKER_HPP
