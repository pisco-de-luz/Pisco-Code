#ifndef MOCK_LED_CONTROL_LOGGER_HPP
#define MOCK_LED_CONTROL_LOGGER_HPP

// #include "Pisco-Code.hpp"
#include "LedBlinkPattern.hpp"
#include "code_blinker.hpp"
#include "pisco_constants.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

// enum LedEvent : uint8_t
// {
//     LED_CALL_INVALID = 0,
//     LED_CALL_ON,
//     LED_CALL_OFF,
//     LED_CALL_FUNC_OK,
//     LED_CALL_FUNC_FAIL
// };

enum class LedEvent : uint8_t
{
    Invalid,
    On,
    Off,
    FuncOk,
    FuncFail
};

using pisco::DurationMs;
using pisco::LedLevel;
using pisco::Timestamp;
struct LedStateChange
{
    Timestamp  timestamp{0};
    LedEvent   state{LedEvent::Invalid};
    DurationMs duration{0};
    bool       isLedBeingUsedNow{false};
    bool       isRunning{false};
};

class MockLedControlLogger
{
  public:
    MockLedControlLogger();
    void setBlinker(pisco::CodeBlinker* blinker);

    void                               setTime(Timestamp currTime);
    bool                               handle(uint8_t ctrlLED);
    void                               clear();
    const std::vector<LedStateChange>& getEvents() const;
    void                               flush();
    void                               setTraceResolution(Timestamp resolutionMs); // default: 100
    LedLevel getDimmedLevel() const { return led_blink_pattern_.getDimmedLevel(); }
    LedLevel getPulseLevel() const { return led_blink_pattern_.getPulseLevel(); }

    std::string traceLogToString() const;

  private:
    void                        log(LedEvent ev);
    Timestamp                   currentTime_{0};
    Timestamp                   lastTime_{0};
    DurationMs                  duration_{1};
    mutable LedBlinkPattern     led_blink_pattern_{};
    LedEvent                    lastState_{LedEvent::Invalid};
    std::vector<LedStateChange> events_{};
    Timestamp                   traceResolutionMs_{100};
    pisco::CodeBlinker*         blinker_{nullptr};
};

#endif
