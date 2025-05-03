message(STATUS "Target processor ################-> ${CMAKE_SYSTEM_PROCESSOR}")

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "avr")
  add_subdirectory(avr)
endif()

# if(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
#   add_subdirectory(native)
# endif()

# Future support:
# if(CMAKE_SYSTEM_PROCESSOR STREQUAL "esp32")
#   add_subdirectory(esp32)
# endif()
