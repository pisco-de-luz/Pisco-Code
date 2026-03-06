// Targeted tests for deprecated API wrappers.
// These verify that the old setHighLevel()/setLowLevel() names still
// delegate correctly to setPeakLevel()/setBaseLevel(), and that the
// deprecated NumberBase alias still works as a synonym for Radix.
// Remove this file when the deprecated wrappers are deleted in v2.0.0.

#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_fixture.hpp"
#include "helpers/tests_constants.hpp"
#include "pisco_constants.hpp"
#include "signal_emitter.hpp"

using namespace pisco_code;
using namespace testutils;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

TEST_GROUP_BASE(DeprecatedApiCompatibilityTest, BlinkerTestFixture){};

TEST(DeprecatedApiCompatibilityTest, setHighLevelDelegatesToSetPeakLevel)
{
    controller.setHighLevel(MID_PEAK_LEVEL);
    LONGS_EQUAL(MID_PEAK_LEVEL, controller.getPeakLevel());
}

TEST(DeprecatedApiCompatibilityTest, setLowLevelDelegatesToSetBaseLevel)
{
    controller.setLowLevel(MID_BASE_LEVEL);
    LONGS_EQUAL(MID_BASE_LEVEL, controller.getBaseLevel());
}

TEST(DeprecatedApiCompatibilityTest,
     setHighLevelClampsIdenticallyToSetPeakLevel)
{
    controller.setHighLevel(TOO_LOW_PEAK_LEVEL);
    const auto from_deprecated = controller.getPeakLevel();

    controller.setPeakLevel(TOO_LOW_PEAK_LEVEL);
    const auto from_new = controller.getPeakLevel();

    LONGS_EQUAL(from_new, from_deprecated);
}

TEST(DeprecatedApiCompatibilityTest, setLowLevelClampsIdenticallyToSetBaseLevel)
{
    controller.setLowLevel(TOO_HIGH_BASE_LEVEL);
    const auto from_deprecated = controller.getBaseLevel();

    controller.setBaseLevel(TOO_HIGH_BASE_LEVEL);
    const auto from_new = controller.getBaseLevel();

    LONGS_EQUAL(from_new, from_deprecated);
}

// --- NumberBase (deprecated alias for Radix) ---------------------------------

TEST(DeprecatedApiCompatibilityTest, NumberBaseAliasResolvesToRadix)
{
    const NumberBase old_alias = NumberBase::DEC;
    const Radix      new_type  = Radix::DEC;
    CHECK_TRUE(old_alias == new_type);
}

TEST(DeprecatedApiCompatibilityTest, showCodeAcceptsNumberBaseAlias)
{
    const bool result =
        blinker.showCode(SignalCode{42}, NumberBase::DEC, NumDigits{0});
    CHECK_TRUE(result);
    CHECK_TRUE(blinker.isRunning());
}

#pragma GCC diagnostic pop
