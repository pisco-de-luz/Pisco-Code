message(STATUS "Target processor -> ${CMAKE_SYSTEM_PROCESSOR}")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "avr")
  add_subdirectory(avr)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm|ARM")
  add_subdirectory(stm32/boards/nucleo-f410rb)
endif()