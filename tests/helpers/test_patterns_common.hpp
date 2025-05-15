#include "BlinkerTestUtils.hpp"
#include "MockLedControlLogger.hpp"
#include "MockLedControllerAdapter.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

// - Example DEC: 9, 88, 777, ..., 111111111
// - Example HEX: F, EE, DDD, ..., 111111111
inline void runSameDigitsUpToMaxDigitsForBase(pisco::NumberBase base, pisco::CodeBlinker& blinker,
                                              MockLedControlLogger& logger)
{
    for (pisco::NumDigits num_digits = 1; num_digits <= pisco::MAX_DIGITS; ++num_digits)
    {
        logger.clear();
        const pisco::DigitValue digit = ((to_value(base) - num_digits) % (to_value(base)));

        const pisco::BlinkCode code = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SameDigit, base, num_digits, digit});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

// - Example DEC: 1, 12, 123, 1234, ..., 123456789
// - Example HEX: 1, 12, 123, 1234, ..., 12345678
inline void runSequentialUpDigitsUpToMaxDigitsForBase(pisco::NumberBase     base,
                                                      pisco::CodeBlinker&   blinker,
                                                      MockLedControlLogger& logger)
{
    for (pisco::NumDigits num_digits = 1; num_digits <= pisco::MAX_DIGITS; ++num_digits)
    {
        logger.clear();
        const pisco::DigitValue digit = ((to_value(base) - num_digits) % (to_value(base)));

        const pisco::BlinkCode code = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SequencialUp, base, num_digits});

        // show code in decimal and hexadecimal
        // std::cout << "code: " << std::dec << code << " (decimal), " << std::hex << code
        //           << " (hexadecimal)" << std::endl;
        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}