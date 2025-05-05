include(${CMAKE_CURRENT_LIST_DIR}/avr_example_macros.cmake)


add_avr_example(basic_example
  SOURCE basic_example.cpp
  MCU atmega328p
)

# add_avr_example(one_led_clock
#   SOURCE one_led_clock.cpp
#   MCU atmega328p
# )

