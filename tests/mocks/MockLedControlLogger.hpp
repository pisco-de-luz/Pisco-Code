#pragma once

#include "../helpers/LedBlinkPattern.hpp"
// #include "../helpers/tests_constants.hpp"
#include "../helpers/tests_types.hpp"
#include "code_blinker.hpp"
#include "pisco_constants.hpp"
#include <array>
#include <cstdint>
#include <string>
#include <vector>

// using pisco::LedLevel;
// using pisco::Timestamp;

struct LedStateChange
{
    pisco::Timestamp    timestamp{0};
    testutils::LedEvent state{testutils::LedEvent::Invalid};
    pisco::DurationMs   duration{0};
    bool                isLedBeingUsedNow{false};
    bool                isRunning{false};
};

class MockLedControlLogger
{
  public:
    MockLedControlLogger();
    void setBlinker(pisco::CodeBlinker* blinker);

    void                               setTime(pisco::Timestamp currTime);
    bool                               handle(uint8_t ctrlLED);
    void                               clear();
    const std::vector<LedStateChange>& getEvents() const;
    void                               flush();
    void            setTraceResolution(pisco::Timestamp resolutionMs); // default: 100
    pisco::LedLevel getDimmedLevel() const { return led_blink_pattern_.getDimmedLevel(); }
    pisco::LedLevel getPulseLevel() const { return led_blink_pattern_.getPulseLevel(); }

    std::string traceLogToString() const;

  private:
    void                        log(testutils::LedEvent ev);
    pisco::Timestamp            currentTime_{0};
    pisco::Timestamp            lastTime_{0};
    pisco::DurationMs           duration_{1};
    mutable LedBlinkPattern     led_blink_pattern_{};
    testutils::LedEvent         lastState_{testutils::LedEvent::Invalid};
    std::vector<LedStateChange> events_{};
    pisco::Timestamp            traceResolutionMs_{100};
    pisco::CodeBlinker*         blinker_{nullptr};
};
