// signal_emitter.hpp
#ifndef PISCO_CODE_BLINKER_HPP
#define PISCO_CODE_BLINKER_HPP

#include "led_controller.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_element.hpp"
#include "signal_pulse_iterator.hpp"
#include "signal_sequencer.hpp"
#include "signal_types.hpp"

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

        static BlinkMode signalLevelToBlinkMode(SignalLevel level);
        static PhaseDuration
        signalDurationToPhaseDuration(SignalDuration duration);

        void handleIdle();
        void handleBeginDigit();
        void handleDisplayNegativeSign();
        void handlePauseAfterNegative();
        void handleLoadNextDigit();
        void handleEmitBlink();
        void handlePauseBetweenBlinks();
        void handlePauseBetweenDigits();
        void handleDisplayZero();
        void handleEndOfDigitCycle();
        void handlePauseBeforeStart();
        void handlePauseAfterFinish();
        void handleHasMoreSignalCodeToSequence();
        void handleHasMoreSignalElements();
        void handlePopNextCodeToSequence();
        void handlePopNextSignalElement();
        void handleHasMorePulse();
        void handlePopNextPulse();

        enum class PhaseLoop : PhaseType
        {
            STARTING = 0,
            APPLYING_PULSE,
            IDLE,
        };

        enum class Phase : PhaseType
        {
            PAUSE_BEFORE_START = 0,
            BEGIN_DIGIT,
            DISPLAY_NEGATIVE_SIGN,
            PAUSE_AFTER_NEGATIVE,
            LOAD_NEXT_DIGIT,
            EMIT_BLINK,
            PAUSE_BETWEEN_BLINKS,
            PAUSE_BETWEEN_DIGITS,
            DISPLAY_ZERO,
            END_OF_DIGIT_CYCLE,
            PREPARE_REPEAT,
            PAUSE_AFTER_FINISH,
            IDLE,
            HAS_MORE_SIGNAL_CODE_TO_SEQUENCE,
            POP_NEXT_CODE_TO_SEQUENCE,
            HAS_MORE_SIGNAL_ELEMENTS,
            POP_NEXT_SIGNAL_ELEMENT,
            HAS_MORE_PULSE,
            POP_NEXT_PULSE,
            LAST_PHASE, // Used to determine the size of the enum
        };

        using BlinkPhaseHandler = void (SignalEmitter::*)();

        struct PhaseTableEntry
        {
            BlinkPhaseHandler handler    = nullptr;
            Phase             id         = Phase::IDLE;
            BlinkMode         blink_mode = BlinkMode::NONE;
            PhaseDuration     duration   = 0;
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
                 to_phase_duration(INIT_PHASE_MS),
                 },
                {
                 &SignalEmitter::handleBeginDigit,
                 SignalEmitter::Phase::BEGIN_DIGIT,
                 BlinkMode::DIMMED,
                 to_phase_duration(INIT_DIMMED_PHASE_MS),
                 },
                {
                 &SignalEmitter::handleDisplayNegativeSign,
                 SignalEmitter::Phase::DISPLAY_NEGATIVE_SIGN,
                 BlinkMode::PULSE,
                 to_phase_duration(NEGATIVE_BLINK_LONG_MS),
                 },
                {
                 &SignalEmitter::handlePauseAfterNegative,
                 SignalEmitter::Phase::PAUSE_AFTER_NEGATIVE,
                 BlinkMode::DIMMED,
                 to_phase_duration(NEGATIVE_BLINK_LONG_MS),
                 },
                {
                 &SignalEmitter::handleLoadNextDigit,
                 SignalEmitter::Phase::LOAD_NEXT_DIGIT, BlinkMode::DIMMED,
                 to_phase_duration(0),
                 // to_phase_duration(BETWEEN_BLINK_MS),
                },
                {
                 &SignalEmitter::handleEmitBlink,
                 SignalEmitter::Phase::EMIT_BLINK,
                 BlinkMode::PULSE,
                 to_phase_duration(SHORT_BLINK_MS),
                 },
                {
                 &SignalEmitter::handlePauseBetweenBlinks,
                 SignalEmitter::Phase::PAUSE_BETWEEN_BLINKS,
                 BlinkMode::DIMMED,
                 to_phase_duration(BETWEEN_BLINK_MS),
                 },
                {
                 &SignalEmitter::handlePauseBetweenDigits,
                 SignalEmitter::Phase::PAUSE_BETWEEN_DIGITS,
                 BlinkMode::DIMMED,
                 to_phase_duration(BETWEEN_DIGITS_MS),
                 },
                {
                 &SignalEmitter::handleEndOfDigitCycle,
                 SignalEmitter::Phase::END_OF_DIGIT_CYCLE,
                 BlinkMode::DIMMED,
                 to_phase_duration(END_DIMMED_PHASE_MS),
                 },
                {
                 &SignalEmitter::handlePauseBeforeStart,
                 SignalEmitter::Phase::PREPARE_REPEAT,
                 BlinkMode::NONE,
                 to_phase_duration(BETWEEN_CODES_MS),
                 },
                {
                 &SignalEmitter::handlePauseAfterFinish,
                 SignalEmitter::Phase::PAUSE_AFTER_FINISH,
                 BlinkMode::NONE,
                 to_phase_duration(END_PHASE_MS),
                 },
                {
                 &SignalEmitter::handleIdle,
                 SignalEmitter::Phase::IDLE,
                 BlinkMode::NONE,
                 to_phase_duration(0),
                 },
                {
                 &SignalEmitter::handleDisplayZero,
                 SignalEmitter::Phase::DISPLAY_ZERO,
                 BlinkMode::NONE,
                 to_phase_duration(ZERO_DIGIT_BLINK_MS),
                 },
                {
                 &SignalEmitter::handleHasMoreSignalCodeToSequence,
                 SignalEmitter::Phase::HAS_MORE_SIGNAL_CODE_TO_SEQUENCE,
                 BlinkMode::NONE,
                 to_phase_duration(0),
                 },
                {
                 &SignalEmitter::handlePopNextCodeToSequence,
                 SignalEmitter::Phase::POP_NEXT_CODE_TO_SEQUENCE,
                 BlinkMode::NONE,
                 to_phase_duration(0),
                 },
                {
                 &SignalEmitter::handleHasMoreSignalElements,
                 SignalEmitter::Phase::HAS_MORE_SIGNAL_ELEMENTS,
                 BlinkMode::NONE,
                 to_phase_duration(0),
                 },
                {
                 &SignalEmitter::handlePopNextSignalElement,
                 SignalEmitter::Phase::POP_NEXT_SIGNAL_ELEMENT,
                 BlinkMode::NONE,
                 to_phase_duration(0),
                 },
                {
                 &SignalEmitter::handleHasMorePulse,
                 SignalEmitter::Phase::HAS_MORE_PULSE,
                 BlinkMode::NONE,
                 to_phase_duration(0),
                 },
                {
                 &SignalEmitter::handlePopNextPulse,
                 SignalEmitter::Phase::POP_NEXT_PULSE,
                 BlinkMode::NONE,
                 to_phase_duration(0),
                 },
        };

        PhaseTableEntry getPhaseEntry(Phase phase) const;
        void            transitionTo(Phase next);
        void pulseAs(Phase next, SignalLevel level, SignalDuration duration);
        LedController*      controller_ = nullptr;
        SignalSequencer     sequencer_;
        SignalPulseIterator pulse_iterator_;
        SignalElement       element_;
        PhaseTableEntry     last_phase_entry_ = {
            &SignalEmitter::handleIdle,
            SignalEmitter::Phase::IDLE,
            BlinkMode::NONE,
        };

        Index     current_digit_index_               = 0;
        NumDigits least_significant_digit_           = 0;
        Counter   blink_counts_[MAX_DIGITS_ABSOLUTE] = {};
        NumDigits digits_[MAX_DIGITS_ABSOLUTE]       = {};
        NumDigits max_digits_                        = 0;

        TickCounter   start_time_         = 0;
        PhaseDuration phase_duration_     = 0;
        Phase         current_phase_      = Phase::IDLE;
        PhaseLoop     current_phase_loop_ = PhaseLoop::IDLE;
        bool          is_negative_        = false;
        bool          is_running_         = false;
        bool          is_in_gap_level_    = false;
        LedLevel      peak_level_         = DEFAULT_PULSE_LEVEL;
        LedLevel      dimmed_level_       = DEFAULT_DIMMED_LEVEL;
    };

} // namespace pisco_code

#endif // PISCO_CODE_BLINKER_HPP
