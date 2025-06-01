#include "blinker_test_utils.hpp"
#include "mock_led_control_logger.hpp"
#include "mock_led_controller_adapter.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
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

        const SignalCode code = testutils::generatePatternOfDigits(
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

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const SignalCode code = testutils::generatePatternOfDigits(
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
// - Example OCT: 1, 21, 321, 4321, ..., 765432107
// - Example BIN: 1, 10, 101, 1010, ..., 1010101010
inline void runSequentialDownDigitsUpToMaxDigitsForBase(NumberBase base, CodeBlinker& blinker,
                                                        MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const SignalCode code = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SequencialDown, base, num_digits});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

// - Example DEC: 000000001, 000000012, 000000123, ..., 123456789
// - Example HEX: 0000001, 0000012, 0000123, ..., 1234567
// - Example OCT: 000000001, 000000012, 000000123, ..., 123456701
// - Example BIN: 0{23}1, 0{22}10, 0{21}101, ..., 1010{6}
inline void runSequentialDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase            base,
                                                                     CodeBlinker&          blinker,
                                                                     MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const SignalCode code = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SequencialUp, base, num_digits});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

// - Example DEC: 000000009, 000000088, 000000777, ..., 111111111
// - Example HEX: 0000007, 0000066, 0000555, ..., 1111111
// - Example OCT: 000000002, 000000011, 000000777, ..., 111111111
// - Example BIN: 0{23}1, 0{22}11, 0{21}111, ..., 1{24}
inline void runSameDigitsUpToMaxDigitsPaddedToMaxDigitsForBase(NumberBase            base,
                                                               CodeBlinker&          blinker,
                                                               MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const DigitValue digit_to_show = ((max_digits - num_digits) % (to_value(base) - 1)) + 1;
        const SignalCode code          = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SameDigit, base, num_digits, digit_to_show});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

// - Example DEC: 0001, 0012, 0123, 1234, 2345, ..., 6789
// - Example HEX: 001, 012, 123, 234, ..., 567
// - Example OCT: 0001, 0012, 0123, 1234, 2345,..., 6701
// - Example BIN: 0{11}1, 0{10}10, 0{9}101, ..., 1010{3}
inline void
runSequentialDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(NumberBase base, CodeBlinker& blinker,
                                                             MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const SignalCode code = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SequencialUp, base, num_digits});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits / 2,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

// - Example DEC: 0009, 0088, 0777, 6666, 5555, 4444, ..., 1111
// - Example HEX: 007, 066, 555, 444, 333, 222, 111
// - Example OCT: 0002, 0011, 0777, 6666, 5555, ..., 1111
// - Example BIN: 0{11}1, 0{10}11, 0{9}111, ..., 1{12}
inline void runSameDigitsUpToMaxDigitsPaddedToHalfMaxDigitsForBase(NumberBase            base,
                                                                   CodeBlinker&          blinker,
                                                                   MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const DigitValue digit_to_show = ((max_digits - num_digits) % (to_value(base) - 1)) + 1;
        const SignalCode code          = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SameDigit, base, num_digits, digit_to_show});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
            .numDigits   = max_digits / 2,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}

// - Example DEC: 9, 99, 999, ..., 999999999
// - Example HEX: F, FF, FFF, ..., FFFFFFFF
// - Example OCT: 7, 77, 777, ..., 777777777
// - Example BIN: 1, 11, 111, ..., 1{24}
inline void runSameMaxBaseDigitUpToMaxDigitsNineTimesForBase(NumberBase base, CodeBlinker& blinker,
                                                             MockLedControlLogger& logger)
{
    const NumDigits max_digits = max_digits_for_base(base);
    const auto      base_value = to_value(base);

    for (NumDigits num_digits = 1; num_digits <= max_digits; ++num_digits)
    {
        logger.clear();

        const DigitValue digit_to_show = base_value - 1;
        const SignalCode code          = testutils::generatePatternOfDigits(
            {testutils::PatternOption::SameDigit, base, num_digits, digit_to_show});

        const testutils::TestBlinkerCase test_case{
            .blink_code  = code,
            .number_base = base,
            .trace_check = testutils::TraceCheck::Enforced,
            .repeats     = 9,
        };

        checkBlinkerBehavior(blinker, logger, test_case);
    }
}