#include "MockLedControlLogger.hpp"
#include "Pisco-Code.hpp"

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
    if (events_.empty())
        return "";

    const Timestamp endTime   = events_.back().timestamp + events_.back().duration;
    const size_t    slotCount = endTime / traceResolutionMs_ + 1;

    std::string result(slotCount, '0');

    constexpr size_t  MAX_DURATION_TRACK    = 64;
    constexpr uint8_t MIN_EVENTS_FIRST_SLOT = 6;
    using Histogram                         = uint32_t[MAX_DURATION_TRACK];
    uint8_t totalNumEventsFirstSlot{0};

    for (size_t slotIndex = 0; slotIndex < slotCount; ++slotIndex)
    {
        const Timestamp slotStart = static_cast<Timestamp>(slotIndex * traceResolutionMs_);
        const Timestamp slotEnd   = slotStart + traceResolutionMs_;

        Histogram histogram   = {};
        Timestamp totalOnTime = 0;
        uint8_t   ledCallOnEventsCounted{0};

        for (const auto& entry : events_)
        {
            if (entry.timestamp == 0 || (entry.state != LED_CALL_ON && entry.state != LED_CALL_OFF))
            {
                continue;
            }

            const Timestamp start = entry.timestamp;
            const Timestamp end   = entry.timestamp + entry.duration;

            if (end <= slotStart || start >= slotEnd)
            {
                continue;
            }

            if (slotIndex == 0 && (entry.state == LED_CALL_ON || entry.state == LED_CALL_OFF))
            {
                ++totalNumEventsFirstSlot;
            }

            const Timestamp overlapStart = std::max(start, slotStart);
            const Timestamp overlapEnd   = std::min(end, slotEnd);
            const Timestamp overlapDur   = overlapEnd - overlapStart;

            if (entry.state == LED_CALL_ON && entry.duration < MAX_DURATION_TRACK)
            {
                ++histogram[entry.duration];
                totalOnTime += overlapDur;
                ++ledCallOnEventsCounted;
            }
        }

        if (totalNumEventsFirstSlot > MIN_EVENTS_FIRST_SLOT)
        {
            if (ledCallOnEventsCounted < (totalNumEventsFirstSlot / 4))
            {
                result[slotIndex] = '0';
            }
            else
            {
                size_t maxIndex = 0;
                for (size_t i = 0; i < MAX_DURATION_TRACK; ++i)
                {
                    if (histogram[i] > histogram[maxIndex])
                    {
                        maxIndex = i;
                    }
                }
                ++maxIndex;
                result[slotIndex] = (maxIndex < 10) ? ('0' + maxIndex) : ('a' + maxIndex - 10);
            }
        }
    }
    std::string compressed;
    if (!result.empty())
    {
        char     current = result[0];
        uint16_t count   = 1;

        for (size_t i = 1; i < result.size(); ++i)
        {
            if (result[i] == current)
            {
                ++count;
            }
            else
            {
                // Emit previous group
                compressed += current;
                if (count < 11)
                {
                    compressed += 'S';
                }
                else if (count < 22)
                {
                    compressed += 'M';
                }
                else
                {
                    compressed += 'L';
                }
                current = result[i];
                count   = 1;
            }
        }

        // Emit last group
        compressed += current;
        if (count < 10)
        {
            compressed += 'S';
        }
        else if (count < 30)
        {
            compressed += 'M';
        }
        else
        {
            compressed += 'L';
        }
    }

    return compressed;
}
