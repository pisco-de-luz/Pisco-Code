// signal_emitter.hpp
#ifndef PISCO_CODE_BLINKER_HPP
#define PISCO_CODE_BLINKER_HPP

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_sequencer.hpp"

namespace pisco_code
{

    class SignalEmitter
    {
      public:
        explicit SignalEmitter(LedController* controller);

        bool showCode(SignalCode code, NumberBase base, NumDigits num_digits,
                      RepeatTimes repeats);
        void loop(LoopCounter loop_counter);
        bool isRunning() const;
        bool isLedBeingUsedNow() const;
        void setPeakLevel(LedLevel level);
        void setDimmedLevel(LedLevel level);

      private:
        bool phaseElapsed(LoopCounter loop_counter) const;

        bool hasMoreBlinks() const;
        bool hasMoreDigits() const;
        bool shouldRepeat() const;

        void handleIdle(LoopCounter loop_counter);
        void handleBeginDigit(LoopCounter loop_counter);
        void handleDisplayNegativeSign(LoopCounter loop_counter);
        void handlePauseAfterNegative(LoopCounter loop_counter);
        void handleLoadNextDigit(LoopCounter loop_counter);
        void handleEmitBlink(LoopCounter loop_counter);
        void handlePauseBetweenBlinks(LoopCounter loop_counter);
        void handleDisplayZero(LoopCounter loop_counter);
        void handleEndOfDigitCycle(LoopCounter loop_counter);
        void handlePauseBeforeStart(LoopCounter loop_counter);
        void handlePauseAfterFinish(LoopCounter loop_counter);
        void handleHasMoreSignalCodeToSequence(LoopCounter loop_counter);

        enum class Phase : PhaseType
        {
            // LED is off (initial state)
            PAUSE_BEFORE_START = 0,

            // LED is dimmed (start of digit display)
            BEGIN_DIGIT,

            // LED is ON (blink for negative sign)
            DISPLAY_NEGATIVE_SIGN,

            // LED is dimmed (cooldown after minus blink)
            PAUSE_AFTER_NEGATIVE,

            // LED is dimmed (preparing next digit)
            LOAD_NEXT_DIGIT,

            // LED is ON (main blink pulse)
            EMIT_BLINK,

            // LED is dimmed (between blinks of the same digit)
            PAUSE_BETWEEN_BLINKS,

            // LED is dimmed (zero represented as a dim pause)
            DISPLAY_ZERO,

            // LED is dimmed (pause before repeat or stop)
            END_OF_DIGIT_CYCLE,

            // LED is off (pause before next repetition)
            PREPARE_REPEAT,

            // LED is off (pause after final repetition)
            PAUSE_AFTER_FINISH,

            // LED is off (system paused or stopped)
            IDLE,

            HAS_MORE_SIGNAL_CODE_TO_SEQUENCE,

            LAST_PHASE, // Used to determine the size of the enum
        };

        using BlinkPhaseHandler = void (SignalEmitter::*)(UInt8);

        struct PhaseTableEntry
        {
            BlinkPhaseHandler handler;
            Phase             id;
            BlinkMode         blink_mode;
        };

        // Declare the static constexpr member array
        // This declares that an array named phase_table exists and belongs to
        // SignalEmitter, but its actual definition (the contents) will be in
        // the .cpp file.
        static constexpr Counter NUM_PHASES = 13; // or whatever the number is
        static constexpr PhaseTableEntry
            phase_table[static_cast<PhaseType>(Phase::LAST_PHASE)] = {
                {
                 &SignalEmitter::handlePauseBeforeStart,
                 SignalEmitter::Phase::PAUSE_BEFORE_START,
                 BlinkMode::NONE,
                 },
                {
                 &SignalEmitter::handleBeginDigit,
                 SignalEmitter::Phase::BEGIN_DIGIT,
                 BlinkMode::DIMMED,
                 },
                {
                 &SignalEmitter::handleDisplayNegativeSign,
                 SignalEmitter::Phase::DISPLAY_NEGATIVE_SIGN,
                 BlinkMode::PULSE,
                 },
                {
                 &SignalEmitter::handlePauseAfterNegative,
                 SignalEmitter::Phase::PAUSE_AFTER_NEGATIVE,
                 BlinkMode::DIMMED,
                 },
                {
                 &SignalEmitter::handleLoadNextDigit,
                 SignalEmitter::Phase::LOAD_NEXT_DIGIT,
                 BlinkMode::DIMMED,
                 },
                {
                 &SignalEmitter::handleEmitBlink,
                 SignalEmitter::Phase::EMIT_BLINK,
                 BlinkMode::PULSE,
                 },
                {
                 &SignalEmitter::handlePauseBetweenBlinks,
                 SignalEmitter::Phase::PAUSE_BETWEEN_BLINKS,
                 BlinkMode::DIMMED,
                 },
                {
                 &SignalEmitter::handleEndOfDigitCycle,
                 SignalEmitter::Phase::END_OF_DIGIT_CYCLE,
                 BlinkMode::DIMMED,
                 },
                {
                 &SignalEmitter::handlePauseBeforeStart,
                 SignalEmitter::Phase::PREPARE_REPEAT,
                 BlinkMode::NONE,
                 },
                {
                 &SignalEmitter::handlePauseAfterFinish,
                 SignalEmitter::Phase::PAUSE_AFTER_FINISH,
                 BlinkMode::NONE,
                 },
                {
                 &SignalEmitter::handleIdle,
                 SignalEmitter::Phase::IDLE,
                 BlinkMode::NONE,
                 },
                {
                 &SignalEmitter::handleDisplayZero,
                 SignalEmitter::Phase::DISPLAY_ZERO,
                 BlinkMode::NONE,
                 },
                {
                 &SignalEmitter::handleHasMoreSignalCodeToSequence,
                 SignalEmitter::Phase::HAS_MORE_SIGNAL_CODE_TO_SEQUENCE,
                 BlinkMode::NONE,
                 },
        };

        void            transitionTo(Phase next, PhaseDuration duration,
                                     LoopCounter loop_counter);
        LedController*  controller_ = nullptr;
        SignalSequencer sequencer_;

        Index     current_digit_index_               = 0;
        NumDigits least_significant_digit_           = 0;
        Counter   blink_counts_[MAX_DIGITS_ABSOLUTE] = {};
        NumDigits digits_[MAX_DIGITS_ABSOLUTE]       = {};
        NumDigits max_digits_                        = 0;

        RepeatTimes         repeats_total_     = 0;
        mutable RepeatTimes repeats_remaining_ = 0;

        LoopCounter   start_time_     = 0;
        PhaseDuration phase_duration_ = 0;
        Phase         current_phase_  = Phase::IDLE;

        bool        is_negative_  = false;
        LedLevel    peak_level_   = DEFAULT_PULSE_LEVEL;
        LoopCounter loop_counter_ = 0;
        LedLevel    dimmed_level_ = DEFAULT_DIMMED_LEVEL;
    };

} // namespace pisco_code

#endif // PISCO_CODE_BLINKER_HPP
