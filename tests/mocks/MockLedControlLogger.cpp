#include "MockLedControlLogger.hpp"
#include "Pisco-Code.hpp"
#include <cstdint>
#include <cstdio>
#include <vector>

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
