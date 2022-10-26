/* Pisco-LED-Code.ino
 * 
 * This sketch demonstrates how to use the Pisco-LED-Code
 * library to show decimal or hexadecimal values using just
 * a single LED. 
 * 
 * These values can be shown as positive or negative when needed.
 * 
 * The Pisco-LED-Code library is a nom blocking function that 
 * should be called frequently from the loop function. 
 * 
 * This example code is in the Public Domain (or CC0 licensed, at your option.)
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * Andre Viegas
 */


#include <stdio.h>
#include "driver/ledc.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_err.h"
#include <Pisco-LED-Code.h>

#define LED_PISCO_CODE                     GPIO_NUM_25

bool turnLedOnOff(uint8_t ctrlLED);

/**************************************************************************************
 * GLOBAL VARIABLES
 **************************************************************************************/
PiscoCode       ledBuiltin;                                                     // declare an object of class PiscoCode
bool            ledBuiltinOK;                                                   // It is safe to show codes with ledBuiltin?
uint16_t        numberCounterTimes = 0;                                         // Simple counter variable to increment the numbers used on showCode function


void led_pisco_code(void *pvParams) {

    while (1) {
        vTaskDelay(5000/portTICK_PERIOD_MS);
        if ( ledBuiltinOK && ! ledBuiltin.isSequencing() ) {                    // If ledBuiltin was set up and is not sequencing any code
            ledBuiltin.setPWM(10);                                              // Define the new value of default pwm.
            ledBuiltin.showCode(++numberCounterTimes,PiscoCode::DECIMAL);       // display some number on BUILTIN led repeatedly.    
        }
    }    
}

extern "C" {
    void app_main(void);
}

void app_main() {    
    if ( (ledBuiltinOK = ledBuiltin.setup(&turnLedOnOff)) ) {                   // calling the PiscoCode class constructor.     
       xTaskCreate(&led_pisco_code,"LED_PISCO_CODE",55120,NULL,5,NULL);
    } 
    while (ledBuiltinOK) {
        // We must call the LOOP function regularly and pass as a parameter an 8-bit counter that
        // increments every 64 miliseconds or other value defined on the mSec_perLoopCounter constant.
        ledBuiltin.loop( (uint32_t)(esp_timer_get_time()>>16) & 0xff );         
        vTaskDelay(1/portTICK_PERIOD_MS);
    }    
}


// Before using this function to turn the LED on and off, the caller will check if it is a valid
// pointer to a correct function, and it should respond to a LED_FUNC_OK call returning true. 
//
// This function will return true only if one of these three commands are received, LED_ON,
// LED_OFF, and LED_FUNC_OK. All other values will return false. 
bool turnLedOnOff(uint8_t ctrlLED) {
  gpio_pad_select_gpio(LED_PISCO_CODE);
  gpio_set_direction (LED_PISCO_CODE,GPIO_MODE_OUTPUT);
  bool funcOK = true;
  if ( ctrlLED == PiscoCode::LED_ON ) {              gpio_set_level(LED_PISCO_CODE,1);
  } else if ( ctrlLED == PiscoCode::LED_OFF ) {      gpio_set_level(LED_PISCO_CODE,0);  
  } else if ( ctrlLED != PiscoCode::LED_FUNC_OK ) {  funcOK = false; }
  return( funcOK );
}
