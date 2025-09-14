# //--------------------------------------------------------------------------------------------
# // File:        CMakeLists.txt
# // Description: CMake file for examples/avr/ directory
# // Author:      Andre Viegas
# // Date:        2025.04
# // Version:     1.1
# //--------------------------------------------------------------------------------------------

add_library(avr_shared OBJECT
    avr_systick.cpp
)

target_include_directories(avr_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(avr_shared PUBLIC PiscoCodeCore)

function(add_avr_example NAME BOARD_DIR HAL_CPP)
  add_executable(${NAME}
      basic_example.cpp
      $<TARGET_OBJECTS:avr_shared>
      boards/${BOARD_DIR}/board_config.hpp
      boards/${BOARD_DIR}/${HAL_CPP}
  )
  target_include_directories(${NAME} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
      ${CMAKE_CURRENT_SOURCE_DIR}/boards/${BOARD_DIR}
      ${CMAKE_SOURCE_DIR}/include
  )

  target_link_libraries(${NAME} PRIVATE PiscoCodeCore)

  target_link_options(${NAME} PRIVATE
    -Wl,-Map=$<TARGET_FILE_DIR:${NAME}>/${NAME}.map
    -Wl,--gc-sections
    -Wl,--cref
  )

  target_link_options(${NAME} PRIVATE -Wl,--start-group)
  target_link_libraries(${NAME} PRIVATE c gcc m)
  target_link_options(${NAME} PRIVATE -Wl,--end-group)
endfunction()


add_avr_example(avr_arduino_nano  arduino-nano  hal_led_arduino_nano.cpp)
# add_avr_example(avr_arduino_mega  arduino-mega  hal_led_arduino_mega.cpp)
# add_avr_example(avr_arduino_uno   arduino-uno-r3 hal_led_arduino_uno.cpp)


