#pragma once

#include "../helpers/LedBlinkPattern.hpp"
// #include "../helpers/tests_constants.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "../helpers/tests_types.hpp"
#include "code_blinker.hpp"
#include "pisco_constants.hpp"

using namespace pisco_code;

struct LedStateChange
{
    Timestamp           timestamp{0};
    testutils::LedEvent state{testutils::LedEvent::Invalid};
    DurationMs          duration{0};
    bool                isLedBeingUsedNow{false};
    bool                isRunning{false};
};

class MockLedControlLogger
{
  public:
    MockLedControlLogger();
    void setBlinker(CodeBlinker* blinker);

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
    void                        log(testutils::LedEvent ev);
    Timestamp                   currentTime_{0};
    Timestamp                   lastTime_{0};
    DurationMs                  duration_{1};
    mutable LedBlinkPattern     led_blink_pattern_{};
    testutils::LedEvent         lastState_{testutils::LedEvent::Invalid};
    std::vector<LedStateChange> events_{};
    Timestamp                   traceResolutionMs_{100};
    CodeBlinker*                blinker_{nullptr};
};
