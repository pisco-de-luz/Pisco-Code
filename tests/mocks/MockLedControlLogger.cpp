#include "MockLedControlLogger.hpp"
#include "pisco_constants.hpp"

#ifdef new
#undef new
#endif

#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <map>
#include <string>
#include <vector>

MockLedControlLogger::MockLedControlLogger() : currentTime_(0)
{
}

void MockLedControlLogger::setTime(Timestamp currTime)
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

void MockLedControlLogger::log(LedEvent ledEvent)
{
    if (ledEvent == lastState_)
    {
        return; // Still in same state, extend current duration
    }

    // If lastState_ was valid, flush it to the log
    if (lastState_ != LED_CALL_INVALID)
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
    if (lastState_ != LED_CALL_INVALID)
    {
        LedStateChange stateChange;
        stateChange.timestamp = lastTime_;
        stateChange.state     = lastState_;
        stateChange.duration  = currentTime_ - lastTime_;
        events_.push_back(stateChange);
    }

    lastTime_  = currentTime_;
    lastState_ = LED_CALL_INVALID;
}

bool MockLedControlLogger::handle(uint8_t ctrlLED)
{
    LedEvent ledEvent{LED_CALL_INVALID};
    switch (ctrlLED)
    {
        case pisco::LED_ON:
            ledEvent = LED_CALL_ON;
            break;
        case pisco::LED_OFF:
            ledEvent = LED_CALL_OFF;
            break;
        case pisco::LED_FUNC_OK:
            ledEvent = LED_CALL_FUNC_OK;
            break;
        default:
            ledEvent = LED_CALL_FUNC_FAIL;
    }

    log(ledEvent);
    return (ledEvent == LED_CALL_ON || ledEvent == LED_CALL_OFF || ledEvent == LED_CALL_FUNC_OK);
}

const std::vector<LedStateChange>& MockLedControlLogger::getEvents() const
{
    return events_;
}

void MockLedControlLogger::setTraceResolution(Timestamp resolutionMs)
{
    traceResolutionMs_ = resolutionMs;
}

std::string MockLedControlLogger::traceLogToString() const
{
    std::string result{""};
    Timestamp   next_duty_cycle_timestamp{0};
    uint8_t     last_pwm_level{0};
    Timestamp   last_timestamp_pwm_level_changed{0};
    Timestamp   duration_without_pwm_changed{0};
    std::string token;

    for (const auto& entry : events_)
    {
        if (entry.state == LED_CALL_ON && entry.timestamp != next_duty_cycle_timestamp)
        {
            break; // Events vector is incorrect, stop processing
        }
        Timestamp next_timestamp_event = entry.timestamp + entry.duration;
        while (next_duty_cycle_timestamp < next_timestamp_event)
        {
            uint8_t pwm_level{0};
            if (entry.state == LED_CALL_ON)
            {
                pwm_level = entry.duration;
            }
            if (pwm_level != last_pwm_level)
            {
                duration_without_pwm_changed =
                    next_duty_cycle_timestamp - last_timestamp_pwm_level_changed;
                result += getNextPulseCodeString(last_pwm_level, duration_without_pwm_changed);
                last_pwm_level                   = pwm_level;
                last_timestamp_pwm_level_changed = next_duty_cycle_timestamp;
            }
            next_duty_cycle_timestamp += pisco::PWM_MAX + 1;
        }
    }
    duration_without_pwm_changed = next_duty_cycle_timestamp - last_timestamp_pwm_level_changed;
    result += getNextPulseCodeString(last_pwm_level, duration_without_pwm_changed);
    return result;
}

std::string MockLedControlLogger::getNextPulseCodeString(uint8_t   pulseCode,
                                                         Timestamp duration) const
{
    std::string token;
    if (pulseCode < 10)
    {
        token += static_cast<char>('0' + pulseCode);
    }
    else
    {
        token += static_cast<char>('a' + pulseCode - 10);
    }
    if (duration < 1100)
    {
        token += "S";
    }
    else if (duration < 2200)
    {
        token += "M";
    }
    else
    {
        token += "L";
    }

    return token;
}
