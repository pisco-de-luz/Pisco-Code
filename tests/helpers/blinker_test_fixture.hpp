#pragma once

#include "CppUTest/TestHarness.h"

#include "mock_led_control_logger.hpp"
#include "mock_led_controller_adapter.hpp"
#include "signal_emitter.hpp"

using namespace pisco_code;

// Shared fixture for all TEST_GROUPs that drive a SignalEmitter through a
// MockLedControllerAdapter.  Use via TEST_GROUP_BASE(GroupName, BlinkerTestFixture).
struct BlinkerTestFixture : public Utest
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    SignalEmitter            blinker{controller};
};
