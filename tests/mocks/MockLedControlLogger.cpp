#include "MockLedControlLogger.hpp"
#include "Pisco-Code.hpp"

#ifdef new
#undef new
#endif

#include <cstdint>
#include <cstdio>
#include <vector>
#include <map>
#include <algorithm>
#include <string>
#include <iomanip>




MockLedControlLogger::MockLedControlLogger() : currentTime_(0) {}

void MockLedControlLogger::setTime(Timestamp currTime) {
    currentTime_ = currTime;
}

void MockLedControlLogger::clear() {
    events_.clear();
}

void MockLedControlLogger::log(LedEvent ledEvent) {
    if (ledEvent == lastState_) {
        return;  // Still in same state, extend current duration
    }

    // If lastState_ was valid, flush it to the log
    if (lastState_ != LED_CALL_INVALID) {
        LedStateChange stateChange;
        stateChange.timestamp = lastTime_;
        stateChange.state = lastState_;
        stateChange.duration = currentTime_ - lastTime_;
        events_.push_back(stateChange);
        //printf("log()\t T=%d  STATE=%d  DURATION=%d\n", stateChange.timestamp, stateChange.state, stateChange.duration);
    }

    // Start counting new state
    lastState_ = ledEvent;
    lastTime_ = currentTime_;
}

void MockLedControlLogger::flush() {
    if (lastState_ != LED_CALL_INVALID) {
        LedStateChange stateChange;
        stateChange.timestamp = lastTime_;
        stateChange.state = lastState_;
        stateChange.duration = currentTime_ - lastTime_;
        events_.push_back(stateChange);
        //printf("flush()\t T=%d  STATE=%d  DURATION=%d\n", stateChange.timestamp, stateChange.state, stateChange.duration);
    }

    lastTime_ = currentTime_;
    lastState_ = LED_CALL_INVALID;
}

bool MockLedControlLogger::handle(uint8_t ctrlLED) {
    LedEvent ledEvent{LED_CALL_INVALID};
    switch (ctrlLED) {
        case PiscoCode::LED_ON:        ledEvent = LED_CALL_ON;       break;
        case PiscoCode::LED_OFF:       ledEvent = LED_CALL_OFF;      break;
        case PiscoCode::LED_FUNC_OK:   ledEvent = LED_CALL_FUNC_OK;  break;
        default:                       ledEvent = LED_CALL_FUNC_FAIL; 
    }
    
    //printf("ctrlLED=%d\t ledEvent=%d\n", ctrlLED, ledEvent);
    log(ledEvent);
    return (ledEvent == LED_CALL_ON || ledEvent == LED_CALL_OFF || ledEvent == LED_CALL_FUNC_OK);
}

const std::vector<LedStateChange>& MockLedControlLogger::getEvents() const {
    return events_;
}

void MockLedControlLogger::setTraceResolution(Timestamp resolutionMs) {
    traceResolutionMs_ = resolutionMs;
}

std::string MockLedControlLogger::traceLogToString() const {
    if (events_.empty()) return "";

    const Timestamp endTime = events_.back().timestamp + events_.back().duration;
    const size_t slotCount = endTime / traceResolutionMs_ + 1;

    std::string result(slotCount, '0');

    constexpr size_t MAX_DURATION_TRACK = 64;
    using Histogram = uint32_t[MAX_DURATION_TRACK];

    for (size_t slotIndex = 0; slotIndex < slotCount; ++slotIndex) {
        const Timestamp slotStart = static_cast<Timestamp>(slotIndex * traceResolutionMs_);
        const Timestamp slotEnd   = slotStart + traceResolutionMs_;

        Histogram histogram = {};
        Timestamp totalOnTime = 0;

        for (const auto& entry : events_) {
            if (entry.state != LED_CALL_ON && entry.state != LED_CALL_OFF) continue;

            const Timestamp start = entry.timestamp;
            const Timestamp end   = entry.timestamp + entry.duration;

            if (end <= slotStart || start >= slotEnd) continue;

            const Timestamp overlapStart = std::max(start, slotStart);
            const Timestamp overlapEnd   = std::min(end, slotEnd);
            const Timestamp overlapDur   = overlapEnd - overlapStart;

            if (entry.state == LED_CALL_ON && entry.duration < MAX_DURATION_TRACK) {
                ++histogram[entry.duration];
                totalOnTime += overlapDur;
            }
        }

        const Timestamp totalOffTime = traceResolutionMs_ - totalOnTime;
        if (totalOffTime > traceResolutionMs_ / 2) {
            result[slotIndex] = '0';
        } else {
            // Find most frequent duration
            size_t maxIndex = 0;
            for (size_t i = 1; i < MAX_DURATION_TRACK; ++i) {
                if (histogram[i] > histogram[maxIndex]) {
                    maxIndex = i;
                }
            }

            result[slotIndex] = (maxIndex < 10) ? ('0' + maxIndex) : ('A' + maxIndex - 10);
        }
    }

    return result;
}


// std::string MockLedControlLogger::traceLogToString() const {
//     if (events_.empty()) return "";

//     const Timestamp endTime = events_.back().timestamp + events_.back().duration;
//     const size_t slotCount = endTime / traceResolutionMs_ + 1;

//     std::string result(slotCount, '0');

//     for (size_t slotIndex = 0; slotIndex < slotCount; ++slotIndex) {
//         const Timestamp slotStart = static_cast<uint32_t>(slotIndex * traceResolutionMs_);
//         const Timestamp slotEnd   = slotStart + traceResolutionMs_;

//         std::map<StateDuration, Timestamp> onDurations;  // key: duration, value: frequency
//         Timestamp totalOnTime = 0;

//         for (const auto& entry : events_) {
//             if (entry.state != LED_CALL_ON && entry.state != LED_CALL_OFF) continue;

//             const Timestamp start = entry.timestamp;
//             const Timestamp end   = entry.timestamp + entry.duration;

//             if (end <= slotStart || start >= slotEnd) continue;

//             const Timestamp overlapStart = std::max(start, slotStart);
//             const Timestamp overlapEnd   = std::min(end, slotEnd);
//             const Timestamp overlapDur   = overlapEnd - overlapStart;

//             if (entry.state == LED_CALL_ON) {
//                 ++onDurations[entry.duration];
//                 totalOnTime += overlapDur;
//             }
//         }

//         if (onDurations.empty() || totalOnTime < traceResolutionMs_ / 4) {
//             result[slotIndex] = '0';
//         } else {
//             auto mostFrequent = std::max_element(
//                 onDurations.begin(),
//                 onDurations.end(),
//                 [](const auto& a, const auto& b) { return a.second < b.second; });

//             const uint8_t pwmVal = mostFrequent->first;
//             result[slotIndex] = (pwmVal < 10) ? ('0' + pwmVal) : ('A' + pwmVal - 10);
//         }
//     }

//     return result;
// }
