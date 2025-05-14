#include "CppUTest/TestHarness.h"

#include "code_blinker.hpp"
#include "helpers/BlinkerTestUtils.hpp"
#include "mocks/MockLedControlLogger.hpp"
#include "mocks/MockLedControllerAdapter.hpp"
#include "pisco_constants.hpp"

using namespace pisco;
using testutils::runSequencer;

TEST_GROUP(BaseEncodingBlinkerTest)
{
    MockLedControlLogger     logger;
    MockLedControllerAdapter controller{&logger};
    CodeBlinker              blinker{&controller};
};

TEST(BaseEncodingBlinkerTest, CanShowDecimal10)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_10,
        .number_base = NumberBase::DECIMAL,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowZero)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_0,
        .number_base = NumberBase::DECIMAL,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowDecimal12345)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_12345,
        .number_base = NumberBase::DECIMAL,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowBinary5)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_BIN_5,
        .number_base = NumberBase::BINARY,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowBinaryNeg7)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_BIN_NEG_7,
        .number_base = NumberBase::BINARY,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowHex255)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_HEX_255,
        .number_base = NumberBase::HEXADECIMAL,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}

TEST(BaseEncodingBlinkerTest, CanShowOctal28)
{
    const testutils::TestBlinkerCase test_case{
        .blink_code  = testutils::CODE_OCT_28,
        .number_base = NumberBase::OCTAL,
        .trace_check = testutils::TraceCheck::Enforced,
    };

    testutils::checkBlinkerBehavior(blinker, logger, test_case);
}
