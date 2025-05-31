#include "CppUTest/TestHarness.h"

#include "signal_element.hpp" // or the correct header where SignalElement is defined

using namespace pisco_code;

// TEST_GROUP(SignalElementTests){};

// TEST(SignalElementTests, DefaultConstructor_InitializesToZero)
// {
//     const SignalElement element;
//     CHECK_EQUAL(SignalLevel::NOT_DEFINED, element.get_level());
//     CHECK_EQUAL(SignalDuration::SHORT, element.get_duration()); // default raw = 0
//     CHECK_EQUAL(0, element.get_times());
// }

// TEST(SignalElementTests, ParameterizedConstructor_StoresGivenValues)
// {
//     const SignalElement element(SignalLevel::PEAK, 7, SignalDuration::MEDIUM);
//     CHECK_EQUAL(SignalLevel::PEAK, element.get_level());
//     CHECK_EQUAL(SignalDuration::MEDIUM, element.get_duration());
//     CHECK_EQUAL(7, element.get_times());
// }

// TEST(SignalElementTests, SIGNAL_ELEMENT_NEGATIVE_PEAK_HasExpectedValues)
// {
//     CHECK_EQUAL(SignalLevel::PEAK, SIGNAL_ELEMENT_NEGATIVE_PEAK.get_level());
//     CHECK_EQUAL(SignalDuration::LONG, SIGNAL_ELEMENT_NEGATIVE_PEAK.get_duration());
//     CHECK_EQUAL(1, SIGNAL_ELEMENT_NEGATIVE_PEAK.get_times());
// }

// TEST(SignalElementTests, SIGNAL_ELEMENT_ZERO_GAP_HasExpectedValues)
// {
//     CHECK_EQUAL(SignalLevel::GAP, SIGNAL_ELEMENT_ZERO_GAP.get_level());
//     CHECK_EQUAL(SignalDuration::SHORT, SIGNAL_ELEMENT_ZERO_GAP.get_duration());
//     CHECK_EQUAL(1, SIGNAL_ELEMENT_ZERO_GAP.get_times());
// }

// TEST(SignalElementTests, SIGNAL_ELEMENT_NOT_DEFINED_HasExpectedValues)
// {
//     CHECK_EQUAL(SignalLevel::NOT_DEFINED, SIGNAL_ELEMENT_NOT_DEFINED.get_level());
//     CHECK_EQUAL(SignalDuration::SHORT, SIGNAL_ELEMENT_NOT_DEFINED.get_duration());
//     CHECK_EQUAL(0, SIGNAL_ELEMENT_NOT_DEFINED.get_times());
// }

// TEST(SignalElementTests, signal_element_digit_peak_GeneratesCorrectElement)
// {
//     for (DigitValue val = 1; val <= 9; ++val)
//     {
//         auto element = signal_element_digit_peak(val);
//         CHECK_EQUAL(SignalLevel::PEAK, element.get_level());
//         CHECK_EQUAL(SignalDuration::SHORT, element.get_duration());
//         CHECK_EQUAL(val, element.get_times());
//     }
// }
