#ifndef MOCK_LED_CONTROL_LOGGER_HPP
#define MOCK_LED_CONTROL_LOGGER_HPP

#include "Pisco-Code.hpp"
#include <cstdint>
#include <string>
#include <vector>

enum LedEvent : uint8_t
{
    LED_CALL_INVALID = 0,
    LED_CALL_ON,
    LED_CALL_OFF,
    LED_CALL_FUNC_OK,
    LED_CALL_FUNC_FAIL
};

using Timestamp     = uint32_t;
using StateDuration = uint16_t;
struct LedStateChange
{
    Timestamp     timestamp{0};
    LedEvent      state{LED_CALL_INVALID};
    StateDuration duration{0};
};

class MockLedControlLogger
{
  public:
    MockLedControlLogger();

    void                               setTime(Timestamp currTime);
    bool                               handle(uint8_t ctrlLED);
    void                               clear();
    const std::vector<LedStateChange>& getEvents() const;
    void                               flush();
    void                               setTraceResolution(Timestamp resolutionMs); // default: 100
    std::string                        traceLogToString() const;

  private:
    void                        log(LedEvent ev);
    Timestamp                   currentTime_{0};
    Timestamp                   lastTime_{0};
    StateDuration               duration_{1};
    LedEvent                    lastState_{LED_CALL_INVALID};
    std::vector<LedStateChange> events_{};
    Timestamp                   traceResolutionMs_{100};
};

#endif
