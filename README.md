# Pisco LED Code

The easiest way to show decimal or hexadecimal values using just a single LED. These values can be shown as positive or negative when needed.

# Why

How many times in the course of developing some microcontroller project, do we miss a simple mechanism to display variable values? Of course, in some projects, we already have a serial output, debug tools or even a display that we can use in the early stages of the project! However, this is not always what we have.

At other times, we miss a simple and intuitive mechanism, where people who will perform field maintenance on the hardware we are developing, can read values ​​from internal registers, to facilitate diagnosis.

# The Simple and Current way to Implement this

Currently, in the vast majority of cases, we develop some blinking patterns on the status LEDs to indicate certain conditions in the software have been reached.
To indicate numerical values between 1 and 20, for example, we can blink a status LED x times, indicating the desired value.

Other approaches I found for larger numbers were creating sequences of blinks.

Status LED showing the code 312 using a sequences of blinks.
![code312.png](https://github.com/andreviegas/Pisco-LED-Code/blob/6c91251ed2d3033bc74d414339b4a64d40685235/graphics/code312.png)

This works well for pre-defined codes that do not have a zero digit. It would be tricky to show the code 302, for example, and expect the observer to notice a larger gap between the digit 3 and 2, thus concluding that it would be 302

The basic idea of this project is to create a mechanism to display digits, using a sequence of blinks like above, but with the possibility of also displaying the zero digit.

# Egg of Columbus

As can be seen in the graphic below, the working principle of Pisco Code is practically the same. The only difference is that before starting the blinking sequence, the LED is dimmed and it is turned off only after the code has all been "displayed".

![pisco-code-121.png](https://github.com/andreviegas/Pisco-LED-Code/blob/2d7ab53852a28b9fdf24be6b03f43a6450f9fef0/graphics/pisco-code-121.png)

This simple change makes the code easier to read, as it allows the observer to know when the sequence actually starts. When the LED is off and remains dimmed for a few seconds and starts blinking, the observer knows that a new sequence has just started.

# The Origin of Zero

In addition to the benefit mentioned above, this solution also created the possibility of representing the zero digit, in any position of the code to be displayed.
In the graphic below, code 120 is shown. As you can see, once the sequence of codes is started with the LED partially on, the blinks with maximum brightness show numbers between 1-9 while a single blink, turning off the LED, signals the digit zero.

![pisco-code-120.png](https://github.com/andreviegas/Pisco-LED-Code/blob/9d276453b1f99df96e158106dcbb28b7e36d6daf/graphics/pisco-code-120.png)

And the Pisco Code 002 below

![pisco-code-002.png](https://github.com/andreviegas/Pisco-LED-Code/blob/9d276453b1f99df96e158106dcbb28b7e36d6daf/graphics/pisco-code-002.png)


