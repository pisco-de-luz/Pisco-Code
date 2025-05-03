#ifndef MOCK_LED_CONTROL_HPP
#define MOCK_LED_CONTROL_HPP

#include <vector>
#include <stdint.h>

enum LedEvent {
    LED_CALL_ON,
    LED_CALL_OFF,
    LED_CALL_FUNC_OK,
    LED_CALL_INVALID
};

extern std::vector<LedEvent> ledEventLog;

void clearLedEventLog();
bool mockLedControl(uint8_t ctrlLED);

#endif
