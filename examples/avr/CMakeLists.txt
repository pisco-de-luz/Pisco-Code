include(${CMAKE_CURRENT_LIST_DIR}/avr_example_macros.cmake)


add_avr_example(basic_example
  SOURCE basic_example.cpp
  MCU atmega328p
)

# Put the map next to the ELF
target_link_options(basic_example PRIVATE
  -Wl,-Map=$<TARGET_FILE_DIR:basic_example>/basic_example.map
  -Wl,--cref
)

target_sources(basic_example PRIVATE
  avr_exit_stub.c
)

target_link_libraries(basic_example gcc)

# add_avr_example(one_led_clock
#   SOURCE one_led_clock.cpp
#   MCU atmega328p
# )

