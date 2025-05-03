#include "MockLedControl.hpp"
#include "Pisco-Code.hpp"  // for LED_ON, LED_OFF, LED_FUNC_OK

std::vector<LedEvent> ledEventLog;

void clearLedEventLog() {
    ledEventLog.clear();
}

bool mockLedControl(uint8_t ctrlLED) {
    switch (ctrlLED) {
        case PiscoCode::LED_ON:
            ledEventLog.push_back(LED_CALL_ON);
            return true;
        case PiscoCode::LED_OFF:
            ledEventLog.push_back(LED_CALL_OFF);
            return true;
        case PiscoCode::LED_FUNC_OK:
            ledEventLog.push_back(LED_CALL_FUNC_OK);
            return true;
        default:
            ledEventLog.push_back(LED_CALL_INVALID);
            return false;
    }
}
