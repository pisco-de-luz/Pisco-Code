#include "BlinkerTestUtils.hpp"
#include "MockLedControlLogger.hpp"
#include "MockLedControllerAdapter.hpp"

#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

inline void runSameDigitsUpToMaxDigitsForBase(pisco::NumberBase base, pisco::CodeBlinker& blinker,
                                              MockLedControlLogger& logger)
{
    for (pisco::NumDigits num_digits = 1; num_digits <= pisco::MAX_DIGITS; ++num_digits)
    {
        logger.clear();
        const pisco::DigitValue digit =
            ((pisco::MAX_DIGITS - num_digits) % (to_value(base) - 1)) + 1;

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