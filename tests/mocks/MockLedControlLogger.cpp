#include "MockLedControlLogger.hpp"

MockLedControlLogger::MockLedControlLogger() : currentTime(0) {}

void MockLedControlLogger::setTime(uint8_t t) {
    currentTime = t;
}

void MockLedControlLogger::clear() {
    events.clear();
}

void MockLedControlLogger::log(LedEvent ev) {
    events.push_back({currentTime, ev});
}

bool MockLedControlLogger::handle(uint8_t ctrlLED) {
    LedEvent ev;
    switch (ctrlLED) {
        case PiscoCode::LED_ON:        ev = LED_CALL_ON;       break;
        case PiscoCode::LED_OFF:       ev = LED_CALL_OFF;      break;
        case PiscoCode::LED_FUNC_OK:   ev = LED_CALL_FUNC_OK;  break;
        default:                       ev = LED_CALL_INVALID;  log(ev); return false;
    }

    log(ev);
    return true;
}

const std::vector<LedStateChange>& MockLedControlLogger::getEvents() const {
    return events;
}
