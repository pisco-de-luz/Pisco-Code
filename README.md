# Pisco LED Code

The easiest way to show decimal or hexadecimal values using just a single LED. These values can be shown as positive or negative when needed.

# Why

How many times in the course of developing some microcontroller project, do we miss a simple mechanism to display variable values? Of course, in some projects, we already have a serial output, debug tools or even a display that we can use in the early stages of the project! However, this is not always what we have.

At other times, we miss a simple and intuitive mechanism, where people who will perform field maintenance on the hardware we are developing, can read values ​​from internal registers, to facilitate diagnosis.

# The Simple and Current way to Implement this

Currently, in the vast majority of cases, we develop some blinking patterns on the status LEDs to indicate certain conditions in the software have been reached.
To indicate numerical values between 1 and 20, for example, we can blink a status LED x times, indicating the desired value.
Other approaches I found for larger numbers were creating sequences of blinks.


# Egg of Columbus
