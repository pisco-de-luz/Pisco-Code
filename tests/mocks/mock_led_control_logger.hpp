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
    testutils::LedEvent state{testutils::LedEvent::INVALID};
    DurationMs          duration{0};
    bool                isRunning{false};
};

class MockLedControlLogger
{
  public:
    MockLedControlLogger();
    void setBlinker(SignalEmitter* blinker);

    void setTime(Timestamp currTime);
    bool handle(LedControlCode ctrl_code);
    void clear();
    [[nodiscard]] const std::vector<LedStateChange>& getEvents() const;
    void                                             flush();
    void                         setTraceResolution(Timestamp resolutionMs);
    [[nodiscard]] IntensityLevel getBaseLevel() const
    {
        return led_blink_pattern_.getBaseLevel();
    }
    [[nodiscard]] IntensityLevel getPulseLevel() const
    {
        return led_blink_pattern_.getPulseLevel();
    }

    [[nodiscard]] testutils::TraceCode traceLogToString() const;

  private:
    void                               log(testutils::LedEvent ledEvent);
    Timestamp                          currentTime_{0};
    Timestamp                          lastTime_{0};
    DurationMs                         duration_{1};
    mutable testutils::LedBlinkPattern led_blink_pattern_{};
    testutils::LedEvent                lastState_{testutils::LedEvent::INVALID};
    std::vector<LedStateChange>        events_{};
    Timestamp                          traceResolutionMs_{100};
    SignalEmitter*                     blinker_{nullptr};
};
