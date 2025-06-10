/* Linker script for Nordic Semiconductor nRF52840 with S140 SoftDevice
 * 
 * Based on memory layout from SEGGER project:
 * - FLASH_START = 0x26000 (after S140 SoftDevice)
 * - FLASH_SIZE = 0xCA000 (824KB for application)
 * - RAM_START = 0x20002BB8 (after S140 RAM usage)
 * - RAM_SIZE = 0x3D448 (245KB for application)
 */

SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
  /* Flash memory map:
   * 0x00000 - 0x25FFF: S140 SoftDevice (152KB)
   * 0x26000 - 0xEFFFF: Application (824KB)
   * 0xF0000 - 0xFDFFF: Bootloader (56KB)
   * 0xFE000 - 0xFFFFF: MBR Parameter Storage (8KB)
   */
  FLASH (rx) : ORIGIN = 0x26000, LENGTH = 0xCA000
  
  /* RAM memory map:
   * 0x20000000 - 0x20002BB7: S140 SoftDevice RAM (11KB)
   * 0x20002BB8 - 0x2003FFFF: Application RAM (245KB)
   */
  RAM (rwx) : ORIGIN = 0x20002BB8, LENGTH = 0x3D448
  
  /* UICR bootloader start address */
  UICR_BOOTLOADER (r) : ORIGIN = 0x10001FF8, LENGTH = 0x4
}

/* Stack and heap sizes */
_stack_size = 0x2000;  /* 8KB stack */
_heap_size = 0x2000;   /* 8KB heap */

/* Section definitions */
SECTIONS
{
  /* Place the interrupt vectors first */
  .vectors :
  {
    KEEP(*(.isr_vector))
    KEEP(*(.vectors))
  } > FLASH

  /* Application code */
  .text :
  {
    *(.text*)
    *(.rodata*)
    
    /* Nordic SDK observer sections */
    . = ALIGN(4);
    PROVIDE(__start_sdh_soc_observers = .);
    KEEP(*(SORT(.sdh_soc_observers*)))
    PROVIDE(__stop_sdh_soc_observers = .);
    
    . = ALIGN(4);
    PROVIDE(__start_sdh_ble_observers = .);
    KEEP(*(SORT(.sdh_ble_observers*)))
    PROVIDE(__stop_sdh_ble_observers = .);
    
    . = ALIGN(4);
    PROVIDE(__start_sdh_req_observers = .);
    KEEP(*(SORT(.sdh_req_observers*)))
    PROVIDE(__stop_sdh_req_observers = .);
    
    . = ALIGN(4);
    PROVIDE(__start_sdh_state_observers = .);
    KEEP(*(SORT(.sdh_state_observers*)))
    PROVIDE(__stop_sdh_state_observers = .);
    
    . = ALIGN(4);
    PROVIDE(__start_sdh_stack_observers = .);
    KEEP(*(SORT(.sdh_stack_observers*)))
    PROVIDE(__stop_sdh_stack_observers = .);
    
    /* Power management data */
    . = ALIGN(4);
    PROVIDE(__start_pwr_mgmt_data = .);
    KEEP(*(SORT(.pwr_mgmt_data*)))
    PROVIDE(__stop_pwr_mgmt_data = .);
    
    /* Queue section */
    . = ALIGN(4);
    PROVIDE(__start_nrf_queue = .);
    KEEP(*(.nrf_queue*))
    PROVIDE(__stop_nrf_queue = .);
    
    /* Balloc section */
    . = ALIGN(4);
    PROVIDE(__start_nrf_balloc = .);
    KEEP(*(.nrf_balloc*))
    PROVIDE(__stop_nrf_balloc = .);
    
    /* CLI commands */
    . = ALIGN(4);
    PROVIDE(__start_cli_command = .);
    KEEP(*(.cli_command*))
    PROVIDE(__stop_cli_command = .);
    
    /* Crypto data */
    . = ALIGN(4);
    PROVIDE(__start_crypto_data = .);
    KEEP(*(SORT(.crypto_data*)))
    PROVIDE(__stop_crypto_data = .);
    
    /* Log sections */
    . = ALIGN(4);
    PROVIDE(__start_log_const_data = .);
    KEEP(*(SORT(.log_const_data*)))
    PROVIDE(__stop_log_const_data = .);
    
    . = ALIGN(4);
    PROVIDE(__start_log_backends = .);
    KEEP(*(SORT(.log_backends*)))
    PROVIDE(__stop_log_backends = .);
    
    /* nRF sections */
    . = ALIGN(4);
    PROVIDE(__start_nrf_sections = .);
    PROVIDE(__stop_nrf_sections = .);
    
    /* C++ exception handling */
    . = ALIGN(4);
    KEEP(*(.eh_frame*))
  } > FLASH

  /* ARM exception handling */
  .ARM.extab :
  {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > FLASH

  .ARM.exidx :
  {
    PROVIDE(__exidx_start = .);
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    PROVIDE(__exidx_end = .);
  } > FLASH

  /* Static constructors and destructors */
  .preinit_array :
  {
    PROVIDE(__preinit_array_start = .);
    KEEP(*(.preinit_array))
    PROVIDE(__preinit_array_end = .);
  } > FLASH

  .init_array :
  {
    PROVIDE(__init_array_start = .);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    PROVIDE(__init_array_end = .);
  } > FLASH

  .fini_array :
  {
    PROVIDE(__fini_array_start = .);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    PROVIDE(__fini_array_end = .);
  } > FLASH

  /* Used to calculate ROM usage */
  _etext = .;

  /* Data section (initialized data) */
  .data : AT (_etext)
  {
    _sdata = .;
    PROVIDE(__data_start__ = .);
    
    *(.data*)
    
    /* CLI sorted commands in RAM */
    . = ALIGN(4);
    PROVIDE(__start_cli_sorted_cmd_ptrs = .);
    KEEP(*(.cli_sorted_cmd_ptrs*))
    PROVIDE(__stop_cli_sorted_cmd_ptrs = .);
    
    /* File system data in RAM */
    . = ALIGN(4);
    PROVIDE(__start_fs_data = .);
    KEEP(*(.fs_data*))
    PROVIDE(__stop_fs_data = .);
    
    /* Log dynamic data in RAM */
    . = ALIGN(4);
    PROVIDE(__start_log_dynamic_data = .);
    KEEP(*(SORT(.log_dynamic_data*)))
    PROVIDE(__stop_log_dynamic_data = .);
    
    /* Log filter data in RAM */
    . = ALIGN(4);
    PROVIDE(__start_log_filter_data = .);
    KEEP(*(SORT(.log_filter_data*)))
    PROVIDE(__stop_log_filter_data = .);
    
    . = ALIGN(4);
    _edata = .;
    PROVIDE(__data_end__ = .);
  } > RAM

  /* BSS section (uninitialized data) */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    PROVIDE(__bss_start__ = .);
    
    *(.bss*)
    *(COMMON)
    
    . = ALIGN(4);
    _ebss = .;
    PROVIDE(__bss_end__ = .);
  } > RAM

  /* Heap section */
  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE(__heap_start__ = .);
    PROVIDE(__HeapBase = .);
    
    . = . + _heap_size;
    
    PROVIDE(__heap_end__ = .);
    PROVIDE(__HeapLimit = .);
  } > RAM

  /* Stack section */
  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    PROVIDE(__stack_start__ = .);
    PROVIDE(__StackLimit = .);
    
    . = . + _stack_size;
    
    PROVIDE(__stack_end__ = .);
    PROVIDE(__StackTop = .);
  } > RAM

  /* Check for stack/heap collision */
  ASSERT(__HeapLimit <= __StackLimit, "Heap and stack collision detected!")

  /* UICR bootloader start address */
  .uicr_bootloader_start_address :
  {
    PROVIDE(__start_uicr_bootloader_start_address = .);
    KEEP(*(.uicr_bootloader_start_address))
    PROVIDE(__stop_uicr_bootloader_start_address = .);
  } > UICR_BOOTLOADER

  /* Calculate RAM usage */
  PROVIDE(__ram_used__ = __bss_end__ - ORIGIN(RAM));
  PROVIDE(__ram_free__ = LENGTH(RAM) - __ram_used__ - _stack_size - _heap_size);
}

/* Entry point */
ENTRY(Reset_Handler)