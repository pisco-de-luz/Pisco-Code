# //--------------------------------------------------------------------------------------------
# // Board-specific configuration for Arduino Nano (ATmega328P)
# //--------------------------------------------------------------------------------------------

# File names
set(BOARD_HAL_LED_FILE "hal_led_arduino_nano.cpp")

# Upload configuration (avrdude)
# Default to USBasp programmer (no port/baud needed)
# For Arduino bootloader, use: -DBOARD_UPLOAD_PROGRAMMER=arduino -DBOARD_UPLOAD_PORT=/dev/ttyUSB0 -DBOARD_UPLOAD_BAUD=57600
set(BOARD_UPLOAD_PROGRAMMER "usbasp" CACHE STRING "Programmer for avrdude")
