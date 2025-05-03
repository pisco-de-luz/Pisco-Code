#include "MockLedControlLogger.hpp"
#include "Pisco-Code.hpp"
#include <cstdint>
#include <vector>

MockLedControlLogger::MockLedControlLogger() : currentTime(0) {}

void MockLedControlLogger::setTime(uint8_t currTime) {
    currentTime = currTime;
}

void MockLedControlLogger::clear() {
    events.clear();
}

void MockLedControlLogger::log(LedEvent ledEvent) {
    events.push_back({currentTime, ledEvent});
}

bool MockLedControlLogger::handle(uint8_t ctrlLED) {
    LedEvent ledEvent{LED_CALL_INVALID};
    switch (ctrlLED) {
        case PiscoCode::LED_ON:        ledEvent = LED_CALL_ON;       break;
        case PiscoCode::LED_OFF:       ledEvent = LED_CALL_OFF;      break;
        case PiscoCode::LED_FUNC_OK:   ledEvent = LED_CALL_FUNC_OK;  break;
        default:                       ledEvent = LED_CALL_INVALID;  log(ledEvent); return false;
    }

    log(ledEvent);
    return true;
}

const std::vector<LedStateChange>& MockLedControlLogger::getEvents() const {
    return events;
}
