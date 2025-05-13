#include "MockLedControlLogger.hpp"

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_types.hpp"

MockLedControlLogger::MockLedControlLogger() : currentTime_(0)
{
}

void MockLedControlLogger::setTime(pisco::Timestamp currTime)
{
    currentTime_ = currTime;
}

void MockLedControlLogger::setBlinker(pisco::CodeBlinker* blinker)
{
    blinker_ = blinker;
}

void MockLedControlLogger::clear()
{
    events_.clear();
}

void MockLedControlLogger::log(testutils::LedEvent ledEvent)
{
    if (ledEvent == lastState_)
    {
        return; // Still in same state, extend current duration
    }

    // If lastState_ was valid, flush it to the log
    if (lastState_ != testutils::LedEvent::Invalid)
    {
        LedStateChange stateChange;
        stateChange.timestamp = lastTime_;
        stateChange.state     = lastState_;
        stateChange.duration  = currentTime_ - lastTime_;

        // Capture context if blinker is set
        if (blinker_ != nullptr)
        {
            stateChange.isLedBeingUsedNow = blinker_->isLedBeingUsedNow();
            stateChange.isRunning         = blinker_->isRunning();
        }
        events_.push_back(stateChange);
    }

    // Start counting new state
    lastState_ = ledEvent;
    lastTime_  = currentTime_;
}

void MockLedControlLogger::flush()
{
    if (lastState_ != testutils::LedEvent::Invalid)
    {
        LedStateChange stateChange;
        stateChange.timestamp = lastTime_;
        stateChange.state     = lastState_;
        stateChange.duration  = currentTime_ - lastTime_;
        events_.push_back(stateChange);
    }

    lastTime_  = currentTime_;
    lastState_ = testutils::LedEvent::Invalid;
}

bool MockLedControlLogger::handle(uint8_t ctrlLED)
{
    testutils::LedEvent ledEvent{testutils::LedEvent::Invalid};
    switch (ctrlLED)
    {
        case static_cast<uint8_t>(pisco::LedControlCode::On):
            ledEvent = testutils::LedEvent::On;
            break;
        case static_cast<uint8_t>(pisco::LedControlCode::Off):
            ledEvent = testutils::LedEvent::Off;
            break;
        case static_cast<uint8_t>(pisco::LedControlCode::FuncOk):
            ledEvent = testutils::LedEvent::FuncOk;
            break;
        default:
            ledEvent = testutils::LedEvent::FuncFail;
    }

    log(ledEvent);
    return (ledEvent == testutils::LedEvent::On || ledEvent == testutils::LedEvent::Off ||
            ledEvent == testutils::LedEvent::FuncOk);
}

const std::vector<LedStateChange>& MockLedControlLogger::getEvents() const
{
    return events_;
}

void MockLedControlLogger::setTraceResolution(pisco::Timestamp resolutionMs)
{
    traceResolutionMs_ = resolutionMs;
}

std::string MockLedControlLogger::traceLogToString() const
{
    std::string       result{""};
    pisco::Timestamp  next_duty_cycle_timestamp{0};
    uint8_t           last_pwm_level{0};
    pisco::Timestamp  last_timestamp_pwm_level_changed{0};
    pisco::DurationMs duration_without_pwm_changed{0};
    std::string       token;
    led_blink_pattern_.reset();

    for (const auto& event : events_)
    {
        if (event.state == testutils::LedEvent::On && event.timestamp != next_duty_cycle_timestamp)
        {
            break; // Events vector is incorrect, stop processing
        }
        const pisco::Timestamp next_timestamp_event = event.timestamp + event.duration;
        while (next_duty_cycle_timestamp < next_timestamp_event)
        {
            uint8_t pwm_level{0};
            if (event.state == testutils::LedEvent::On)
            {
                pwm_level = event.duration;
            }
            if (pwm_level != last_pwm_level)
            {
                duration_without_pwm_changed =
                    next_duty_cycle_timestamp - last_timestamp_pwm_level_changed;

                led_blink_pattern_.append(last_pwm_level, duration_without_pwm_changed);

                last_pwm_level                   = pwm_level;
                last_timestamp_pwm_level_changed = next_duty_cycle_timestamp;
            }
            next_duty_cycle_timestamp += pisco::PWM_MAX + 1;
        }
    }
    duration_without_pwm_changed = next_duty_cycle_timestamp - last_timestamp_pwm_level_changed;

    led_blink_pattern_.append(last_pwm_level, duration_without_pwm_changed);

    result = led_blink_pattern_.tracePatternToString();
    return result;
}
