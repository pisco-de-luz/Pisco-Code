#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "led_blink_pattern.hpp"
#include "pisco_constants.hpp"
#include "signal_emitter.hpp"
#include "tests_types.hpp"

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
    void setBlinker(SignalEmitter* blinker);

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
    SignalEmitter*              blinker_{nullptr};
};
