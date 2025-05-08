#ifndef TEST_LOOP_RUNNER_HPP
#define TEST_LOOP_RUNNER_HPP

#include "../mocks/MockLedControlLogger.hpp"
#include "code_blinker.hpp"
#include "pisco_constants.hpp"
#include <cstdint>

namespace testutils
{

    // Drives the loop for a maximum simulated time, default 64 seconds.
    inline void runSequencer(pisco::CodeBlinker* code, MockLedControlLogger* logger)
    {
        constexpr uint8_t   LOOP_CALLS_PER_COUNTER = 64;
        constexpr Timestamp MAX_SEQUENCING_LOOP =
            64000UL; // Limit of 64 seconds to prevent infinite loop
        Timestamp loopCounter = 0;
        uint8_t   fakeMillis  = 0;

        while (code->isRunning() && loopCounter < MAX_SEQUENCING_LOOP)
        {
            for (uint8_t i = 0; i < LOOP_CALLS_PER_COUNTER; ++i)
            {
                logger->setTime(loopCounter++);
                code->loop(fakeMillis);
            }

            ++fakeMillis;
        }

        logger->flush();
    }

} // namespace testutils

#endif // TEST_LOOP_RUNNER_HPP
