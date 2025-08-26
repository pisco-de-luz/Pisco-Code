#include "mock_led_control_logger.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_types.hpp"

using namespace pisco_code;

MockLedControlLogger::MockLedControlLogger() : currentTime_(0)
{
}

void
MockLedControlLogger::setTime(Timestamp currTime)
{
    currentTime_ = currTime;
}

void
MockLedControlLogger::setBlinker(SignalEmitter* blinker)
{
    blinker_ = blinker;
}

void
MockLedControlLogger::clear()
{
    events_.clear();
}

void
MockLedControlLogger::log(testutils::LedEvent ledEvent)
{
    if (ledEvent == lastState_)
    {
        return; // Still in same state, extend current duration
    }

    // If lastState_ was valid, flush it to the log
    if (lastState_ != testutils::LedEvent::INVALID)
    {
        LedStateChange state_change;
        state_change.timestamp = lastTime_;
        state_change.state     = lastState_;
        state_change.duration  = currentTime_ - lastTime_;

        // Capture context if blinker is set
        if (blinker_ != nullptr)
        {
            state_change.isLedBeingUsedNow = blinker_->isLedBeingUsedNow();
            state_change.isRunning         = blinker_->isRunning();
        }
        events_.push_back(state_change);
    }

    // Start counting new state
    lastState_ = ledEvent;
    lastTime_  = currentTime_;
}

void
MockLedControlLogger::flush()
{
    if (lastState_ != testutils::LedEvent::INVALID)
    {
        LedStateChange state_change;
        state_change.timestamp = lastTime_;
        state_change.state     = lastState_;
        state_change.duration  = currentTime_ - lastTime_;
        events_.push_back(state_change);
    }

    lastTime_  = currentTime_;
    lastState_ = testutils::LedEvent::INVALID;
}

bool
MockLedControlLogger::handle(LedControlCode ctrl_code)
{
    testutils::LedEvent led_event{testutils::LedEvent::INVALID};
    switch (ctrl_code)
    {
        case LedControlCode::ON:
            led_event = testutils::LedEvent::ON;
            break;
        case LedControlCode::OFF:
            led_event = testutils::LedEvent::OFF;
            break;
        case LedControlCode::FUNC_OK:
            led_event = testutils::LedEvent::FUNC_OK;
            break;
        default:
            led_event = testutils::LedEvent::FUNC_FAIL;
    }

    log(led_event);
    return (led_event == testutils::LedEvent::ON ||
            led_event == testutils::LedEvent::OFF ||
            led_event == testutils::LedEvent::FUNC_OK);
}

const std::vector<LedStateChange>&
MockLedControlLogger::getEvents() const
{
    return events_;
}

void
MockLedControlLogger::setTraceResolution(Timestamp resolutionMs)
{
    traceResolutionMs_ = resolutionMs;
}

testutils::TraceCode
MockLedControlLogger::traceLogToString() const
{
    testutils::TraceCode result;
    Timestamp            next_duty_cycle_timestamp{0};
    LedLevel             last_pwm_level{0};
    Timestamp            last_timestamp_pwm_level_changed{0};
    DurationMs           duration_without_pwm_changed{0};

    led_blink_pattern_.reset();
    for (const auto& event : events_)
    {
        if (event.state == testutils::LedEvent::ON &&
            event.timestamp != next_duty_cycle_timestamp)
        {
            break; // Events vector is incorrect, stop processing
        }
        const Timestamp next_timestamp_event = event.timestamp + event.duration;
        while (next_duty_cycle_timestamp < next_timestamp_event)
        {
            LedLevel pwm_level{0};
            if (event.state == testutils::LedEvent::ON)
            {
                pwm_level = event.duration;
            }
            if (pwm_level != last_pwm_level)
            {
                duration_without_pwm_changed = next_duty_cycle_timestamp -
                                               last_timestamp_pwm_level_changed;

                led_blink_pattern_.append(last_pwm_level,
                                          duration_without_pwm_changed);

                last_pwm_level                   = pwm_level;
                last_timestamp_pwm_level_changed = next_duty_cycle_timestamp;
            }
            next_duty_cycle_timestamp += PWM_MAX + 1;
        }
    }
    duration_without_pwm_changed =
        next_duty_cycle_timestamp - last_timestamp_pwm_level_changed;

    led_blink_pattern_.append(last_pwm_level, duration_without_pwm_changed);

    result = led_blink_pattern_.tracePatternToString();
    return result;
}
