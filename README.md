# Pisco Code

Pisco Code is a lightweight mechanism for representing decimal or hexadecimal values using a single status LED or an actuator such as a vibration motor. The method supports both positive and negative values and is designed for use in embedded systems with minimal hardware resources.

# Why

In many embedded projects, especially during early prototyping or field maintenance, there is no convenient way to observe internal variables. Serial interfaces, debugging tools, or displays are not always available.

Pisco Code provides a simple and intuitive method for conveying values through blink patterns or vibrations, allowing engineers and technicians to interpret internal states or diagnostic codes without additional hardware.

# Conventional Approach

A common practice is to use simple blink patterns on a status LED to indicate conditions or values. For example, blinking the LED _n_ times to represent the value _n_.

For larger values, some implementations concatenate digit sequences. For example, the code `312` may be shown as three blinks, a pause, one blink, a pause, and two blinks.

Status LED showing the code `312` using a sequence of blinks.
![code312.png](https://github.com/pisco-de-luz/Pisco-Code/blob/6c91251ed2d3033bc74d414339b4a64d40685235/graphics/code312.png)

This method is workable but limited: it does not naturally support zero digits and may be ambiguous when interpreting sequences such as `302`.

# Proposed Solution

Pisco Code extends the conventional approach by introducing a framing signal. Before transmitting a numeric sequence, the LED is held at a low brightness (or partial duty cycle) to indicate the start of a new sequence. Once the sequence completes, the LED is turned off.

![Video-Code-121-132x132.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/d0bb059eef8c726219ba4b1a40f07471f648127f/graphics/Video-Code-121-132x132.gif) 
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/2d7ab53852a28b9fdf24be6b03f43a6450f9fef0/graphics/pisco-code-121.png" height="132">

This framing mechanism improves readability, ensuring that observers can reliably distinguish between digit groups, including zeros, and recognize the start of a new value.

# Representation of Zero

The introduction of a framing signal enables explicit representation of the digit zero, which is often ambiguous in conventional blink-based systems.

For example, in the sequence below representing the value `120`, digits `1` and `2` are shown with one and two bright blinks, separated by pauses. The digit 0 is represented not by a blink, but by an intentional gap in the sequence. This makes it possible to represent zero clearly in any position within the code.

![Pisco-Code-120-144x144.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/ba3e80ebc5cc06cf77d8f8e30f36b71e3b5d880f/graphics/Pisco-Code-120-144x144.gif)
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/9d276453b1f99df96e158106dcbb28b7e36d6daf/graphics/pisco-code-120.png" height="144">

In some cases, it is also necessary to define a minimum number of digits to convey information accurately. For example, when displaying a voltage measurement between 0–5 V with two decimal places (e.g., 0.02 V), at least three digits must always be shown. With this convention, the observer can infer the decimal point location.

![Pisco-Code-002-144x144.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/b1c607510b4095d7174fb170666a0196a63d295a/graphics/Pisco-Code-002-144x144.gif)
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/9d276453b1f99df96e158106dcbb28b7e36d6daf/graphics/pisco-code-002.png" height="144">

# Binary and Hexadecimal Support

Pisco Code can be extended to represent binary (0–1) or hexadecimal (0–15) values.

* Binary values are represented as sequences containing only two possible digits, which may result in longer sequences for larger numbers.

* Hexadecimal values allow compact representation but may require multiple blinks for a single digit, making interpretation slower.

For decimal use cases, Pisco Code has been successfully applied in the [Pisco de Luz](https://www.piscodeluz.org/?lang=en) project since 2020. The system is used in the field to read operational data such as lighting usage hours, battery or solar voltage, and temperature, without requiring additional display hardware.

# Representation of Negative Values

Some applications require distinguishing between positive and negative values. To address this, Pisco Code introduces a **negative sign indicator**: a long blink at the beginning of the sequence.

After this initial blink, the digit-by-digit representation proceeds as usual. For example, the sequence shown below represents the value `-12`, with the initial long blink indicating the negative sign.  

![Pisco-Code-Negative12-144x144.gif](https://github.com/pisco-de-luz/Pisco-Code/blob/347a2f6999becbef7c22bfab5b0d4cb3d843b71c/graphics/Pisco-Code-Negative12-144x144.gif)
<img src="https://github.com/pisco-de-luz/Pisco-Code/blob/d46fea2847a2d3f49e9fccbcebee1c75f28df785/graphics/pisco-code-12-negative.png" height="144">

# Usage

```C++
#include "Pisco-Code.h"

PiscoCode      ledBuiltin;                                        // declare an object of class PiscoCode
bool           ledBuiltinOK;                                      // It is safe to show codes with ledBuiltin?

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);                                   // initialize digital pin LED_BUILTIN as an output.                  
  if ( ledBuiltinOK = ledBuiltin.setup(&turnLedOnOff) ) {         // calling the PiscoCode class constructor.
     ledBuiltin.showCode(1024, static_cast<uint8_t>(pisco::NumberBase::DEC));               // display the 1024 number on BUILTIN led.  
  }
}

void loop() {
   if ( ledBuiltinOK && ! ledBuiltin.isSequencing() ) {           // If ledBuiltin was set up and is not sequencing any code
      ledBuiltin.showCode(millis()/1000, PiscoCode::HEX); // display some number on BUILTIN led repeatedly.    
   }
   ledBuiltin.loop(millis());                                     // We should call the LOOP function regularly.

   // run other non-blocking function here
}

// Before using this function to turn the LED on and off, the setup method will check if it is a valid
// pointer to a correct function, and it should respond to a LED_FUNC_OK call returning true. 
//
// This function will return true only if one of these three commands are received, LED_ON,
// LED_OFF, and LED_FUNC_OK. All other values will return false. 
bool turnLedOnOff(uint8_t ctrlLED) {
  bool funcOK = true;
  if ( ctrlLED == LED_ON ) {              digitalWrite(LED_BUILTIN, HIGH);
  } else if ( ctrlLED == LED_OFF ) {      digitalWrite(LED_BUILTIN, LOW);  
  } else if ( ctrlLED != LED_FUNC_OK ) {  funcOK = false; }
  return( funcOK );
}
```
# Using on Arduino IDE
To install, copy the Pisco-Code folder into your arduino sketchbook-libraries folder. More detailed instruction are [here](http://arduino.cc/en/Guide/Libraries).

# Basic Configuration
You first need to include the header file and create one or more PiscoCode objects to use this library.

## Initial Class setup

```C++
#include "Pisco-Code.h"

PiscoCode      ledOne;         // declare an object of class PiscoCode
PiscoCode      ledTwo;         // declare an object of class PiscoCode
```

Before we call the setup method, we have to create a local function that will be passed as a pointer to work as a layer abstraction to the hardware that we 
will use to turn the LED on and off. 

## Functions of Hardware Abstraction Examples

This function should return TRUE only if one of these three commands are received, pisco::LED_ON, pisco::LED_OFF, and pisco::LED_FUNC_OK. All other values should return FALSE. 

When the caller passes pisco::LED_ON, the function should turn the LED on and return TRUE if everything works well. Likewise, when the caller passes pisco::LED_OFF, it should turn the LED off and return TRUE.


```C++
bool turnLed1_OnOff(uint8_t ctrlLED) {
  bool funcOK = true;
  if ( ctrlLED == pisco::LED_ON ) {              digitalWrite(LED1_PORT, HIGH);
  } else if ( ctrlLED == pisco::LED_OFF ) {      digitalWrite(LED1_PORT, LOW);  
  } else if ( ctrlLED != pisco::LED_FUNC_OK ) {  funcOK = false; }
  return( funcOK );
}

bool turnLed2_OnOff(uint8_t ctrlLED) {
  bool funcOK = true;
  if ( ctrlLED == pisco::LED_ON ) {              digitalWrite(LED2_PORT, HIGH);
  } else if ( ctrlLED == pisco::LED_OFF ) {      digitalWrite(LED2_PORT, LOW);  
  } else if ( ctrlLED != pisco::LED_FUNC_OK ) {  funcOK = false; }
  return( funcOK );
}
```

## Calling the Object setup() function.

The setup() function has two main task. The first one is to register the pointer to the hardware abstraction function into PiscoCode::LedOnOff private variable. The second task is to certify that the pointer is valid and it is pointing to a function that seems to be working as it should. 

It receives a pointer to this abstraction function and returns TRUE if everything works ok. Otherwise, it returns FALSE, indicating that the setup process of this object failed. 

```C++
// Global variables
bool           ledOneOK;       // It is safe to show codes with ledOne?
bool           ledTwoOK;       // It is safe to show codes with ledTwo?

void setup() {
  pinMode(LED1_PORT, OUTPUT);                                 // initialize digital pin LED1_PORT as an output.                  
  pinMode(LED2_PORT, OUTPUT);                                 // initialize digital pin LED2_PORT as an output.    
  ledOneOK = ledOne.setup(&turnLed1_OnOff);                   // calling the PiscoCode setup function.
  ledTwoOK = ledTwo.setup(&turnLed2_OnOff);                   // calling the PiscoCode setup function.
}
```

## Showing PiscoCodes, calling showCode() Function
The showCode() function needs two arguments, the "code-to-show" and the "base" to use. It will return three possible status codes.


### Arguments

| Argument               | Description                                            |
| :---:                  | :---                                                   |
| code-to-show (int32_t) | Integer number between -9999999999 and 9999999999 to show in the LED that was set up during this object initialization.  |
| base (uint8_t)         | This function will show the code passed in the first argument using one of the following base systems. PiscoCode::BIN, PiscoCode::OCT, static_cast<uint8_t>(pisco::NumberBase::DEC) or PiscoCode::HEX  |

### Status Code Returned

| Status Code            | Description                                            |
| :---:                  | :---                                                   |
| OK                     | This function accepted the code-to-show and base passed as arguments and will start showing the code shortly. |
| SEQUENCE_RUNNING       | This function is still working in the previous code and can not accept any new tasks. |
| FAILED                 | Something went wrong, and this function failed to accept this code. |

### Simple calling Examples

```C++
// If we could start showing Pisco Code 1024 on ledOne
if ( ledOne.showCode(1024, static_cast<uint8_t>(pisco::NumberBase::DEC)) == static_cast<uint8_t>(pisco::status_t::OK) ) {
   // Do something
} 
// Trying to start showing Pisco Code 13 in binary on ledTwo 
uint8_t statusLedTwo = ledTwo.showCode(13, PiscoCode::BIN);
// If we could not
if ( statusLedTwo != static_cast<uint8_t>(pisco::status_t::OK) ) {
   // Do something
}
```

### Custom Settings
Before calling the showCode() function, it is possible to change some characteristics to fit your needs better. 

The LED could have sixteen bright levels using a PWM-controlled software mechanism. From zero (less bright) to 15 (maximum bright). We need to set two brightness levels to Pisco Code works, one for the dimmed phase and another for the blink phase. 

When creating the Pisco Code object, these values are zero and fifteen, respectively. 

We can use these two functions below to change it.

```C++
ledOne.setDimPwm(3);    // Define the new value of default dimmed pwm. 
ledOne.setPwm(10);      // Define the new value of default pwm.
```

We can also change the others two options, the minimum number of digits to show and how many times the Pisco Code should repeat. 

In the example below, we will set the Pisco Code to have at least four digits and repeat it twice. 

```C++
// Define how many times the code should repeat. As we set it to one, indicate that we want it to show twice.
ledOne.setRepeat(1);       

// Define the minimum number of digits to show. If the code is 12, the system will show 0012.
ledOne.setMinDigits(4);    
```

## More Public Functions

### isSequencing()

As some Pisco Code are big or need to be repeated many times, it could take a while to finish the whole process. 
So, we create a function to check if the current process is still running or has already been finished. 

```C++
if ( ! ledOne.isSequencing() ) {   // If ledOne is not sequencing any more
                                   // We can call ledOne.showCode() again to show new values
}
```

The Pisco Code is a nom blocking library that works using three main functions. 

The first one is setup() that we have to call as soon as we want to start using our object.
After that, the second most crucial function is loop(). We should call it repeatedly at least 1000 times per second.  
The last function we need to call is showCode(), the function that starts the whole sequencing of blinks to show the desired code. 

As we have already shown how to use the setup() and showCode() functions, now we will start describing the mechanisms behind the loop() process. 

### loop()

The loop() function is the most complex algorithm in the whole class. It manages all the timing and dimerization of the Pisco Code one LED blink system. Once the others functions finish passing the information necessary to show the Pisco Code, like brightness, code, base system, times to repeat, etc., it starts the whole sequencing process. 

As it is a nom blocking function, just a handful of line codes are executed and returned briefly. Each time it is called, based on the amount of time passed since the last call, the algorithm determines if it is necessary to turn the LED on, off, or do nothing. 

To assess whether it has to turn the LED on or off, this function works in phases where each phase has its own time to live mechanism.  

The diagram below shows how these phases and timing systems work. 

![How-the-blink-system-programming-logic-works.png](https://github.com/pisco-de-luz/Pisco-Code/blob/891cdfb109bb742c214caaf7e05f41714ec40f46/graphics/How-the-blink-system-programming-logic-works.png)

