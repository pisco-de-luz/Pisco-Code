#include "Pisco-Code.hpp"
#include <limits.h>
#include <stdint.h>

PiscoCode::PiscoCode(void)
{
    currentDigit          = 0;
    leastSignificantDigit = 0;
    currentPhase          = PAUSED;
    isNegative            = false;
    minNumDigits          = 0;

    // Set the initial PWM sequence to the maximum value
    pwmSequence    = pwmMax;
    _pwmSequence   = pwmSequence;
    pwmCounter     = 0;
    sequenceTimes  = 0;
    _sequenceTimes = sequenceTimes;

    // Set the initial dimmed PWM to the predefined initial value
    dimmedPWM  = initialDimmedPWM;
    _dimmedPWM = dimmedPWM;

    // Initialize the start time of the last phase to zero
    startTimeLastPhase   = 0;
    currentPhaseDuration = 0;

    // Initialize the LED function pointer to null
    LedOnOff = nullptr;
}

bool PiscoCode::setup(bool (*LedOnOffFunc)(uint8_t ctrlLED))
{

    bool setupOK = false;
    currentPhase = PAUSED;
    pwmCounter   = 0;

    // Set the LED activation function
    LedOnOff = LedOnOffFunc;

    // Check if the external LED activation function is working correctly
    if (_isExternalLedFuncOk())
    {
        setupOK = true;
        (void) _switchLED(TURN_LED_OFF);
    }

    return setupOK;
}

// Function to check if there is a current sequence running.
bool PiscoCode::isSequencing(void)
{
    return (currentPhase != PAUSED);
}

void PiscoCode::setPWM(uint8_t pwm)
{
    if (pwm > pwmMax)
    {
        pwm = pwmMax;
    }
    pwmSequence = pwm;
}

void PiscoCode::setDimPWM(uint8_t dimPWM)
{
    if (dimPWM > pwmMax)
    {
        dimPWM = pwmMax;
    }
    dimmedPWM = dimPWM;
}

void PiscoCode::setRepeat(uint8_t times)
{
    sequenceTimes = times;
}

// Define the minimum number of digits to show.
void PiscoCode::setMinDigits(uint8_t minDigits)
{
    if (minDigits > MAX_DIGITS)
    {
        minDigits = MAX_DIGITS;
    }
    minNumDigits = minDigits;
}

// This method encapsulates the hardware-dependent LED function.
bool PiscoCode::_switchLED(bool turnItON)
{
    // Initialize the function status to false.
    // The function will return true only if the LedOnOff external function returns true.
    bool statusFuncOK = false;

    // Check if the hardware-dependent LED function is set correctly.
    // If the function pointer is not null, it means it's set correctly.
    if (LedOnOff != nullptr)
    {
        // If the turnItON parameter is true, we want to turn the LED on.
        if (turnItON)
        {
            // Call the external function to turn the LED on.
            // The return value of the external function is stored in statusFuncOK.
            statusFuncOK = LedOnOff(LED_ON);
        }
        else
        {
            // Call the external function to turn the LED off.
            // The return value of the external function is stored in statusFuncOK.
            statusFuncOK = LedOnOff(LED_OFF);
        }
    }
    else
    {
        // If the function pointer is null, it means the external function is not set correctly.
        // In this case, set the function status to false.
        statusFuncOK = false;
    }

    // Return the function status.
    // This will be true if the external function was called and returned true, and false otherwise.
    return (statusFuncOK);
}

// This method checks if the external function for controlling the LED is working correctly.
bool PiscoCode::_isExternalLedFuncOk(void)
{
    constexpr uint16_t UPPER_LIMIT_TO_CHECK = UINT8_MAX + 1;
    bool               statusFuncOK         = true;
    if (LedOnOff != nullptr)
    {
        for (uint16_t ctrlLED = 0; ctrlLED < UPPER_LIMIT_TO_CHECK; ctrlLED++)
        {
            const bool ledFuncReturnTrue = LedOnOff(ctrlLED);
            if (ledFuncReturnTrue && ctrlLED != LED_ON && ctrlLED != LED_OFF &&
                ctrlLED != LED_FUNC_OK)
            {
                statusFuncOK = false;
            }
        }
    }
    else
    {
        statusFuncOK = false;
    }
    return (statusFuncOK);
}