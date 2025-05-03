#ifndef MOCK_LED_CONTROL_LOGGER_HPP
#define MOCK_LED_CONTROL_LOGGER_HPP

#include <vector>
#include <stdint.h>
#include "Pisco-Code.hpp"

enum LedEvent {
    LED_CALL_ON,
    LED_CALL_OFF,
    LED_CALL_FUNC_OK,
    LED_CALL_INVALID
};

struct LedStateChange {
    uint8_t time;
    LedEvent state;
};

class MockLedControlLogger {
public:
    MockLedControlLogger();

    void setTime(uint8_t t);
    bool handle(uint8_t ctrlLED);
    void clear();
    const std::vector<LedStateChange>& getEvents() const;

private:
    void log(LedEvent ev);

    uint8_t currentTime;
    std::vector<LedStateChange> events;
};

#endif
