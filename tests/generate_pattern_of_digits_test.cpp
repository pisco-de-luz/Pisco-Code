#include "CppUTest/TestHarness.h"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include "blinker_test_utils.hpp"
#include "pisco_constants.hpp"
#include "pisco_types.hpp"
#include "signal_emitter.hpp"
#include "tests_constants.hpp"
#include "tests_types.hpp"

using namespace pisco_code;
using namespace testutils;

namespace
{

    TraceCode toBaseString(SignalCode code, NumberBase base,
                           NumDigits num_digits)
    {
        TraceCode        result("");
        const DigitValue base_value = to_value(base);
        for (uint8_t i = 0; i < num_digits; ++i)
        {
            const testutils::Index digit_index = to_index(code % base_value);
            result += "0123456789abcdef"[digit_index];
            code /= base_value;
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

    void checkPattern(NumberBase base, PatternOption pattern,
                      NumDigits num_digits, const char* expected)
    {
        GeneratePatternParams params;
        params.number_base = base;
        params.pattern     = pattern;
        params.num_digits  = num_digits;
        params.digit       = 1U;

        const SignalCode result = generatePatternOfDigits(params);
        const TraceCode  actual = toBaseString(result, base, num_digits);

        STRCMP_EQUAL_TEXT(expected, actual.c_str(),
                          ("Base=" + std::to_string(to_value(base)) +
                           ", Digits=" + std::to_string(num_digits) +
                           ", Result=" + std::to_string(result))
                              .c_str());
    }

} // namespace

// ---------- BINARY ----------
TEST_GROUP(GeneratePattern_BIN){};

TEST(GeneratePattern_BIN, SequencialUp_1)
{
    checkPattern(NumberBase::BIN, PatternOption::SEQUENCIAL_UP, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_BIN, SequencialUp_12)
{
    checkPattern(NumberBase::BIN, PatternOption::SEQUENCIAL_UP, NumDigits{12},
                 "101010101010");
}
TEST(GeneratePattern_BIN, SequencialUp_24)
{
    checkPattern(NumberBase::BIN, PatternOption::SEQUENCIAL_UP, NumDigits{24},
                 "101010101010101010101010");
}

TEST(GeneratePattern_BIN, SequencialDown_1)
{
    checkPattern(NumberBase::BIN, PatternOption::SEQUENCIAL_DOWN, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_BIN, SequencialDown_12)
{
    checkPattern(NumberBase::BIN, PatternOption::SEQUENCIAL_DOWN, NumDigits{12},
                 "101010101010");
}
TEST(GeneratePattern_BIN, SequencialDown_24)
{
    checkPattern(NumberBase::BIN, PatternOption::SEQUENCIAL_DOWN, NumDigits{24},
                 "101010101010101010101010");
}

TEST(GeneratePattern_BIN, DescendingFromMax_1)
{
    checkPattern(NumberBase::BIN, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{1}, "1");
}
TEST(GeneratePattern_BIN, DescendingFromMax_12)
{
    checkPattern(NumberBase::BIN, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{12}, "101010101010");
}
TEST(GeneratePattern_BIN, DescendingFromMax_24)
{
    checkPattern(NumberBase::BIN, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{24}, "101010101010101010101010");
}

TEST(GeneratePattern_BIN, SameDigit_1)
{
    checkPattern(NumberBase::BIN, PatternOption::SAME_DIGIT, NumDigits{1}, "1");
}
TEST(GeneratePattern_BIN, SameDigit_12)
{
    checkPattern(NumberBase::BIN, PatternOption::SAME_DIGIT, NumDigits{12},
                 "111111111111");
}
TEST(GeneratePattern_BIN, SameDigit_24)
{
    checkPattern(NumberBase::BIN, PatternOption::SAME_DIGIT, NumDigits{24},
                 "111111111111111111111111");
}

// ---------- OCTAL ----------
TEST_GROUP(GeneratePattern_OCT){};

TEST(GeneratePattern_OCT, SequencialUp_1)
{
    checkPattern(NumberBase::OCT, PatternOption::SEQUENCIAL_UP, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_OCT, SequencialUp_4)
{
    checkPattern(NumberBase::OCT, PatternOption::SEQUENCIAL_UP, NumDigits{4},
                 "1234");
}
TEST(GeneratePattern_OCT, SequencialUp_9)
{
    checkPattern(NumberBase::OCT, PatternOption::SEQUENCIAL_UP, NumDigits{9},
                 "123456701");
}

TEST(GeneratePattern_OCT, SequencialDown_1)
{
    checkPattern(NumberBase::OCT, PatternOption::SEQUENCIAL_DOWN, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_OCT, SequencialDown_4)
{
    checkPattern(NumberBase::OCT, PatternOption::SEQUENCIAL_DOWN, NumDigits{4},
                 "4321");
}
TEST(GeneratePattern_OCT, SequencialDown_9)
{
    checkPattern(NumberBase::OCT, PatternOption::SEQUENCIAL_DOWN, NumDigits{9},
                 "765432107");
}

TEST(GeneratePattern_OCT, DescendingFromMax_1)
{
    checkPattern(NumberBase::OCT, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{1}, "7");
}
TEST(GeneratePattern_OCT, DescendingFromMax_4)
{
    checkPattern(NumberBase::OCT, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{4}, "7654");
}
TEST(GeneratePattern_OCT, DescendingFromMax_9)
{
    checkPattern(NumberBase::OCT, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{9}, "765432107");
}

TEST(GeneratePattern_OCT, SameDigit_1)
{
    checkPattern(NumberBase::OCT, PatternOption::SAME_DIGIT, NumDigits{1}, "1");
}
TEST(GeneratePattern_OCT, SameDigit_4)
{
    checkPattern(NumberBase::OCT, PatternOption::SAME_DIGIT, NumDigits{4},
                 "1111");
}
TEST(GeneratePattern_OCT, SameDigit_9)
{
    checkPattern(NumberBase::OCT, PatternOption::SAME_DIGIT, NumDigits{9},
                 "111111111");
}

// ---------- DECIMAL ----------
TEST_GROUP(GeneratePattern_DEC){};

TEST(GeneratePattern_DEC, SequencialUp_1)
{
    checkPattern(NumberBase::DEC, PatternOption::SEQUENCIAL_UP, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_DEC, SequencialUp_4)
{
    checkPattern(NumberBase::DEC, PatternOption::SEQUENCIAL_UP, NumDigits{4},
                 "1234");
}
TEST(GeneratePattern_DEC, SequencialUp_9)
{
    checkPattern(NumberBase::DEC, PatternOption::SEQUENCIAL_UP, NumDigits{9},
                 "123456789");
}

TEST(GeneratePattern_DEC, SequencialDown_1)
{
    checkPattern(NumberBase::DEC, PatternOption::SEQUENCIAL_DOWN, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_DEC, SequencialDown_4)
{
    checkPattern(NumberBase::DEC, PatternOption::SEQUENCIAL_DOWN, NumDigits{4},
                 "4321");
}
TEST(GeneratePattern_DEC, SequencialDown_9)
{
    checkPattern(NumberBase::DEC, PatternOption::SEQUENCIAL_DOWN, NumDigits{9},
                 "987654321");
}

TEST(GeneratePattern_DEC, DescendingFromMax_1)
{
    checkPattern(NumberBase::DEC, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{1}, "9");
}
TEST(GeneratePattern_DEC, DescendingFromMax_4)
{
    checkPattern(NumberBase::DEC, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{4}, "9876");
}
TEST(GeneratePattern_DEC, DescendingFromMax_9)
{
    checkPattern(NumberBase::DEC, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{9}, "987654321");
}

TEST(GeneratePattern_DEC, SameDigit_1)
{
    checkPattern(NumberBase::DEC, PatternOption::SAME_DIGIT, NumDigits{1}, "1");
}
TEST(GeneratePattern_DEC, SameDigit_4)
{
    checkPattern(NumberBase::DEC, PatternOption::SAME_DIGIT, NumDigits{4},
                 "1111");
}
TEST(GeneratePattern_DEC, SameDigit_9)
{
    checkPattern(NumberBase::DEC, PatternOption::SAME_DIGIT, NumDigits{9},
                 "111111111");
}

// ---------- HEXADECIMAL ----------
TEST_GROUP(GeneratePattern_HEX){};

TEST(GeneratePattern_HEX, SequencialUp_1)
{
    checkPattern(NumberBase::HEX, PatternOption::SEQUENCIAL_UP, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_HEX, SequencialUp_3)
{
    checkPattern(NumberBase::HEX, PatternOption::SEQUENCIAL_UP, NumDigits{3},
                 "123");
}
TEST(GeneratePattern_HEX, SequencialUp_7)
{
    checkPattern(NumberBase::HEX, PatternOption::SEQUENCIAL_UP, NumDigits{7},
                 "1234567");
}

TEST(GeneratePattern_HEX, SequencialDown_1)
{
    checkPattern(NumberBase::HEX, PatternOption::SEQUENCIAL_DOWN, NumDigits{1},
                 "1");
}
TEST(GeneratePattern_HEX, SequencialDown_3)
{
    checkPattern(NumberBase::HEX, PatternOption::SEQUENCIAL_DOWN, NumDigits{3},
                 "321");
}
TEST(GeneratePattern_HEX, SequencialDown_7)
{
    checkPattern(NumberBase::HEX, PatternOption::SEQUENCIAL_DOWN, NumDigits{7},
                 "7654321");
}

TEST(GeneratePattern_HEX, DescendingFromMax_1)
{
    checkPattern(NumberBase::HEX, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{1}, "f");
}
TEST(GeneratePattern_HEX, DescendingFromMax_3)
{
    checkPattern(NumberBase::HEX, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{3}, "fed");
}
TEST(GeneratePattern_HEX, DescendingFromMax_7)
{
    checkPattern(NumberBase::HEX, PatternOption::DESCENDING_FROM_MAX,
                 NumDigits{7}, "fedcba9");
}

TEST(GeneratePattern_HEX, SameDigit_1)
{
    checkPattern(NumberBase::HEX, PatternOption::SAME_DIGIT, NumDigits{1}, "1");
}
TEST(GeneratePattern_HEX, SameDigit_3)
{
    checkPattern(NumberBase::HEX, PatternOption::SAME_DIGIT, NumDigits{3},
                 "111");
}
TEST(GeneratePattern_HEX, SameDigit_7)
{
    checkPattern(NumberBase::HEX, PatternOption::SAME_DIGIT, NumDigits{7},
                 "1111111");
}
