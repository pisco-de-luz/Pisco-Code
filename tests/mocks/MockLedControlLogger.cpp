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
        ++durantion_;
        return;  // Ignore duplicate timestamps
    }
    ++durantion_;
    // Create a new LedStateChange object and add it to the events vector
    LedStateChange ledStateChange;
    ledStateChange.timestamp = currentTime_;
    if ( LED_CALL_INVALID == lastState_ ) {
        ledStateChange.state = ledEvent;
    } else {
        ledStateChange.state = lastState_;
    }
    ledStateChange.duration = durantion_;  // Duration is not used in this mock
    printf("log()\t T=%d  STATE=%d  DURATION=%d\n", ledStateChange.timestamp, ledStateChange.state, ledStateChange.duration);
    events_.push_back(ledStateChange);
    currentTime_ += durantion_;
    lastState_ = ledEvent;
    durantion_ = 0;  // Reset duration for the next event
}

bool MockLedControlLogger::handle(uint8_t ctrlLED) {
    LedEvent ledEvent{LED_CALL_INVALID};
    switch (ctrlLED) {
        case PiscoCode::LED_ON:        ledEvent = LED_CALL_ON;       break;
        case PiscoCode::LED_OFF:       ledEvent = LED_CALL_OFF;      break;
        case PiscoCode::LED_FUNC_OK:   ledEvent = LED_CALL_FUNC_OK;  break;
        default:                       ledEvent = LED_CALL_FUNC_FAIL; 
    }
    
    printf("ctrlLED=%d\t ledEvent=%d\n", ctrlLED, ledEvent);
    log(ledEvent);
    return (ledEvent == LED_CALL_ON || ledEvent == LED_CALL_OFF || ledEvent == LED_CALL_FUNC_OK);
}

const std::vector<LedStateChange>& MockLedControlLogger::getEvents() const {
    return events_;
}
