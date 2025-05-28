#include "blinker_test_utils.hpp"
#include "mock_led_control_logger.hpp"
#include "mock_led_controller_adapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;

// - Example DEC: 9, 88, 777, ..., 22222222
// - Example HEX: F, EE, DDD, ..., 88888888
// - Example OCT: 7,66, 555,4444,33333,...,77777777  (roll over)
// - Example BIN: 1,0,111,0,11111..., 0
inline void runSameDigitsUpToMaxDigitsForBase(NumberBase base, CodeBlinker& blinker,
                                              MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);
    const auto      base_value = to_value(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();
        const DigitValue digit = (base_value - (num_digits % base_value)) % base_value;

        const BlinkCode code = testutils::generatePatternOfDigits(
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
// - Example OCT: 1, 12, 123, 1234, ..., 12345670
// - Example BIN: 1, 10, 101, 1010, ..., 1010101010
inline void runSequentialUpDigitsUpToMaxDigitsForBase(NumberBase base, CodeBlinker& blinker,
                                                      MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);
    const auto      base_value = to_value(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const BlinkCode code = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SequencialUp, base, num_digits});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

// - Example DEC: 1, 21, 321, 4321, ..., 987654321
// - Example HEX: 1, 21, 321, 4321, ..., 7654321
// - Example OCT: 1, 21, 321, 4321, ..., 76543210
// - Example BIN: 1, 10, 101, 1010, ..., 1010101010
inline void runSequentialDownDigitsUpToMaxDigitsForBase(NumberBase base, CodeBlinker& blinker,
                                                        MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);
    const auto      base_value = to_value(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const BlinkCode code = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SequencialDown, base, num_digits});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        // std::cout << "code: " << std::dec << code << "(dec), " << std::hex << code << "(hex), "
        //           << std::oct << code << "(oct)" << std::endl;
        checkBlinkerBehavior(blinker, logger, test_case);
    }
}