#include "CppUTest/TestHarness.h"

#include "helpers/blinker_test_fixture.hpp"
#include "helpers/blinker_test_utils.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace testutils;

TEST_GROUP_BASE(BaseEncodingBlinkerTest, BlinkerTestFixture){};

TEST(BaseEncodingBlinkerTest, CanShowDecimal10)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_10,
        .number_base = NumberBase::DEC,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowZero)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_0,
        .number_base = NumberBase::DEC,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowDecimal12345)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_12345,
        .number_base = NumberBase::DEC,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowBinary5)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_BIN_5,
        .number_base = NumberBase::BIN,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowBinaryNeg7)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_BIN_NEG_7,
        .number_base = NumberBase::BIN,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowHex255)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_HEX_255,
        .number_base = NumberBase::HEX,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowOctal28)
{
    const TestBlinkerCase test_case{
        .blink_code  = CODE_OCT_28,
        .number_base = NumberBase::OCT,
        .trace_check = TraceCheck::ENFORCED,
    };

    checkBlinkerBehavior(blinker, logger, test_case);
}
