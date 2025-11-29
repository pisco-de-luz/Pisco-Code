/* //--------------------------------------------------------------------------------------------
 * // Linker Script: stm32f410rb.ld
 * // Description:   Minimal memory layout for STM32F410RB (Nucleo-F410RB board)
 * // MCU:           STM32F410RB - ARM Cortex-M4 with FPU
 * // Flash:         128KB
 * // RAM:           32KB
 * // 
 * // Note: Simplified linker script for PiscoCode examples.
 * //       Based on common ARM Cortex-M linker script patterns.
 * //--------------------------------------------------------------------------------------------
 */

MEMORY
{
  FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 128K
  RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 32K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);
ENTRY(Reset_Handler)

SECTIONS
{
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector))
    . = ALIGN(4);
  } > FLASH

  .text :
  {
    *(.text*)
    *(.rodata*)
    KEEP(*(.init))
    KEEP(*(.fini))
  } > FLASH

  . = ALIGN(4);

  /* libgcc unwind tables in FLASH */
  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  . = ALIGN(4);
  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH
  __exidx_end = .;

  /* flash source for .data comes after extab/exidx */
  . = ALIGN(4);
  _sidata = .;

  .data : AT ( _sidata )
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE(end = .);
    PROVIDE(_end = .);
    . = . + 0;
    . = ALIGN(8);
  } > RAM
}
