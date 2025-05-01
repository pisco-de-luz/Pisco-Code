set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

# Specify the AVR-GCC compiler
set(CMAKE_C_COMPILER avr-gcc)
set(CMAKE_CXX_COMPILER avr-g++)

# Add AVR-specific flags
set(CMAKE_C_FLAGS "-mmcu=atmega328p -Wall -Wextra")
set(CMAKE_CXX_FLAGS "-mmcu=atmega328p -Wall -Wextra")

# Specify the linker flags
set(CMAKE_EXE_LINKER_FLAGS "-mmcu=atmega328p")

# Ensure the output is an ELF file
set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)


# Prevent CMake from searching for libraries and includes in the host system
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)