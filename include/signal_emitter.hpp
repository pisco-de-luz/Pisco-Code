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
        void loop(TickCounter tick_counter);
        bool isRunning() const;
        bool isLedBeingUsedNow() const;
        void setPeakLevel(LedLevel level);
        void setDimmedLevel(LedLevel level);

      private:
        bool phaseElapsed(TickCounter tick_counter) const;

        bool hasMoreBlinks() const;
        bool hasMoreDigits() const;
        bool shouldRepeat() const;

        void handleIdle();
        void handleBeginDigit();
        void handleDisplayNegativeSign();
        void handlePauseAfterNegative();
        void handleLoadNextDigit();
        void handleEmitBlink();
        void handlePauseBetweenBlinks();
        void handleDisplayZero();
        void handleEndOfDigitCycle();
        void handlePauseBeforeStart();
        void handlePauseAfterFinish();
        void handleHasMoreSignalCodeToSequence();
        void handleHasMoreSignalElements();
        void handlePopNextCodeToSequence();
        void handlePopNextSignalElement();

        enum class Phase : PhaseType
        {
            PAUSE_BEFORE_START = 0,
            BEGIN_DIGIT,
            DISPLAY_NEGATIVE_SIGN,
            PAUSE_AFTER_NEGATIVE,
            LOAD_NEXT_DIGIT,
            EMIT_BLINK,
            PAUSE_BETWEEN_BLINKS,
            DISPLAY_ZERO,
            END_OF_DIGIT_CYCLE,
            PREPARE_REPEAT,
            PAUSE_AFTER_FINISH,
            IDLE,
            HAS_MORE_SIGNAL_CODE_TO_SEQUENCE,
            POP_NEXT_CODE_TO_SEQUENCE,
            HAS_MORE_SIGNAL_ELEMENTS,
            POP_NEXT_SIGNAL_ELEMENT,
            LAST_PHASE, // Used to determine the size of the enum
        };

        using BlinkPhaseHandler = void (SignalEmitter::*)();

        struct PhaseTableEntry
        {
            BlinkPhaseHandler handler;
            Phase             id;
            BlinkMode         blink_mode;
        };

        // Declare the static constexpr member array
        // This declares that an array named PHASE_TABLE exists and belongs to
        // SignalEmitter, but its actual definition (the contents) will be in
        // the .cpp file.
        static constexpr PhaseTableEntry
            PHASE_TABLE[static_cast<PhaseType>(Phase::LAST_PHASE)] = {
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
                {
                 &SignalEmitter::handlePopNextCodeToSequence,
                 SignalEmitter::Phase::POP_NEXT_CODE_TO_SEQUENCE,
                 BlinkMode::NONE,
                 },
                {
                 &SignalEmitter::handleHasMoreSignalElements,
                 SignalEmitter::Phase::HAS_MORE_SIGNAL_ELEMENTS,
                 BlinkMode::NONE,
                 },
                {
                 &SignalEmitter::handlePopNextSignalElement,
                 SignalEmitter::Phase::POP_NEXT_SIGNAL_ELEMENT,
                 BlinkMode::NONE,
                 },
        };

        PhaseTableEntry getPhaseEntry(Phase phase) const;
        void            transitionTo(Phase next, PhaseDuration duration);
        LedController*  controller_ = nullptr;
        SignalSequencer sequencer_;
        SignalElement   element_;
        PhaseTableEntry last_phase_entry_ = {
            &SignalEmitter::handleIdle,
            SignalEmitter::Phase::IDLE,
            BlinkMode::NONE,
        };

        Index     current_digit_index_               = 0;
        NumDigits least_significant_digit_           = 0;
        Counter   blink_counts_[MAX_DIGITS_ABSOLUTE] = {};
        NumDigits digits_[MAX_DIGITS_ABSOLUTE]       = {};
        NumDigits max_digits_                        = 0;

        RepeatTimes         repeats_total_     = 0;
        mutable RepeatTimes repeats_remaining_ = 0;

        TickCounter   start_time_     = 0;
        PhaseDuration phase_duration_ = 0;
        Phase         current_phase_  = Phase::IDLE;

        bool            is_negative_       = false;
        LedLevel        peak_level_        = DEFAULT_PULSE_LEVEL;
        PwmTickPosition pwm_tick_position_ = 0;
        LedLevel        dimmed_level_      = DEFAULT_DIMMED_LEVEL;
    };

} // namespace pisco_code

#endif // PISCO_CODE_BLINKER_HPP
