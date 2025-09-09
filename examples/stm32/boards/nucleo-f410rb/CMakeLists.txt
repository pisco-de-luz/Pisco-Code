# examples/stm32/boards/nucleo-f410rb/CMakeLists.txt

# Build the STM32 F410RB basic example

enable_language(ASM)

set(BOARD_DIR   ${CMAKE_CURRENT_LIST_DIR})
set(VENDOR_DIR  ${BOARD_DIR}/vendor)

# Use your central linker script. Adjust if you keep a board-local copy.
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/cmake/linker/stm32f410rb.ld)

add_executable(stm32_f410rb_basic
  ${BOARD_DIR}/basic_example.cpp
  ${VENDOR_DIR}/startup_stm32f410rx.s
  ${VENDOR_DIR}/system_stm32f4xx.c
)

# Optional hardfault handler if you add one beside the example later
if(EXISTS "${BOARD_DIR}/hardfault.c")
  target_sources(stm32_f410rb_basic PRIVATE ${BOARD_DIR}/hardfault.c)
endif()

if(EXISTS "${BOARD_DIR}/libc_init_stub.c")
  target_sources(stm32_f410rb_basic PRIVATE ${BOARD_DIR}/libc_init_stub.c)
endif()


# Device macros. Keep both so headers and startup are satisfied.
target_compile_definitions(stm32_f410rb_basic PRIVATE
  STM32F410Rx
  STM32F410xx
)

# Include board headers and mark vendor headers as SYSTEM to keep their warnings out
target_include_directories(stm32_f410rb_basic PRIVATE ${BOARD_DIR})
target_include_directories(stm32_f410rb_basic SYSTEM PRIVATE ${VENDOR_DIR})

set(STM32_CPU_FLAGS  -mcpu=cortex-m4 -mthumb)
set(STM32_FPU_FLAGS  -mfpu=fpv4-sp-d16 -mfloat-abi=softfp)

# Feed those flags to the bus so the library is compiled for Cortex-M4 Thumb too
target_compile_options(pisco_mcu_flags INTERFACE ${STM32_CPU_FLAGS} ${STM32_FPU_FLAGS})
target_link_options(pisco_mcu_flags    INTERFACE ${STM32_CPU_FLAGS} ${STM32_FPU_FLAGS})

# Helpful for some GCC setups that preprocess .s
target_compile_options(stm32_f410rb_basic PRIVATE
  ${STM32_CPU_FLAGS} ${STM32_FPU_FLAGS}
  $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>
)

# Link against the bare-metal variant of your library
target_link_options(stm32_f410rb_basic PRIVATE -Wl,--start-group)
target_link_libraries(stm32_f410rb_basic PRIVATE gcc pisco_code::core_bare)
target_link_options(stm32_f410rb_basic PRIVATE -Wl,--end-group)

# Linker script and map file
target_link_options(stm32_f410rb_basic PRIVATE
   ${STM32_CPU_FLAGS} ${STM32_FPU_FLAGS}
  -T${LINKER_SCRIPT}
  -nostdlib
  -Wl,--gc-sections
  -Wl,-Map=$<TARGET_FILE_DIR:stm32_f410rb_basic>/stm32_f410rb_basic.map
)

# Size, HEX, BIN
add_custom_command(TARGET stm32_f410rb_basic POST_BUILD
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:stm32_f410rb_basic>
  COMMAND ${CMAKE_OBJCOPY} -O ihex   $<TARGET_FILE:stm32_f410rb_basic> stm32_f410rb_basic.hex
  COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:stm32_f410rb_basic> stm32_f410rb_basic.bin
  COMMENT "Size and artifacts"
)

# --- Upload target for Upload.sh ---
# Uses OpenOCD + ST-LINK. Change OPENOCD_INTERFACE or OPENOCD_TARGET if needed.
set(OPENOCD_INTERFACE "interface/stlink.cfg" CACHE STRING "OpenOCD interface cfg")
set(OPENOCD_TARGET    "target/stm32f4x.cfg"  CACHE STRING "OpenOCD target cfg")

add_custom_target(stm32_f410rb_basic_upload
  COMMAND ${CMAKE_COMMAND} -E echo "Programming with OpenOCD..."
  COMMAND openocd -f ${OPENOCD_INTERFACE} -f ${OPENOCD_TARGET}
          -c "program $<TARGET_FILE:stm32_f410rb_basic> verify reset exit"
  DEPENDS stm32_f410rb_basic
  USES_TERMINAL
  COMMENT "Uploading stm32_f410rb_basic via OpenOCD"
)
