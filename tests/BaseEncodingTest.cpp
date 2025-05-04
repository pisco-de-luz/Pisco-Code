#include "CppUTest/TestHarness.h"
#include "Pisco-Code.hpp"
#include "helpers/TestLoopRunner.hpp"
#include "mocks/MockLedControlLogger.hpp"

#include <cstdint>
#include <memory>
#include <string>

namespace
{
    std::unique_ptr<MockLedControlLogger> logger;

    bool ledMockHAL(uint8_t ctrlLED)
    {
        return (logger != nullptr) ? logger->handle(ctrlLED) : false;
    }
} // namespace

TEST_GROUP(BaseEncodingTest)
{
    PiscoCode code;

    void setup()
    {
        logger = std::make_unique<MockLedControlLogger>();
        code.setup(&ledMockHAL);
    }

    void teardown()
    {
        logger.reset();
    }
};

TEST(BaseEncodingTest, ShouldShowDecimal_12345)
{
    code.showCode(12345, PiscoCode::DECIMAL);
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1MgS1MgS1SgS1MgS1SgS1SgS1MgS1SgS1SgS1SgS1MgS1SgS1SgS1SgS1SgS1L0M", trace.c_str());
}

TEST(BaseEncodingTest, ShouldShowBinary_5)
{
    code.showCode(5, PiscoCode::BINARY); // binary: 101
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1MgS1M0S1MgS1L0M", trace.c_str());
}

TEST(BaseEncodingTest, ShouldShowHex_255)
{
    code.showCode(255, PiscoCode::HEXADECIMAL); // hex: FF
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1MgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1MgS1SgS1SgS1SgS1SgS1"
                 "SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1SgS1L0M",
                 trace.c_str());
}

TEST(BaseEncodingTest, ShouldShowOctal_28)
{
    code.showCode(28, PiscoCode::OCTAL); // octal: 34
    testutils::runSequencer(code, logger.get());

    const std::string trace = logger->traceLogToString();
    STRCMP_EQUAL("1MgS1SgS1SgS1MgS1SgS1SgS1SgS1L0M", trace.c_str());
}
