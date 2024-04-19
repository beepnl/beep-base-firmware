# 1 "/home/orlo/beep-base-firmware/Source_V1.5.12/Code/nRF/nRF5_SDK_15.3/modules/nrfx/mdk/ses_startup_nrf_common.s"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/orlo/beep-base-firmware/Source_V1.5.12/Code/nRF/nRF5_SDK_15.3/modules/nrfx/mdk/ses_startup_nrf_common.s"
# 82 "/home/orlo/beep-base-firmware/Source_V1.5.12/Code/nRF/nRF5_SDK_15.3/modules/nrfx/mdk/ses_startup_nrf_common.s"
  .syntax unified

  .global Reset_Handler

  .global InitializeUserMemorySections

  .extern _vectors
  .extern nRFInitialize

  .section .init, "ax"
  .thumb_func

  .equ VTOR_REG, 0xE000ED08
  .equ FPU_CPACR_REG, 0xE000ED88





Reset_Handler:


  ldr r0, =nRFInitialize
  blx r0



  ldr r0, =__RAM_segment_end__
  ldr r1, =0x7
  bics r0, r1
  mov sp, r0




  ldr r0, =SystemInit
  blx r0
# 155 "/home/orlo/beep-base-firmware/Source_V1.5.12/Code/nRF/nRF5_SDK_15.3/modules/nrfx/mdk/ses_startup_nrf_common.s"
  ldr r0, =FPU_CPACR_REG
  ldr r1, [r0]
  orr r1, r1, #(0xF << 20)
  str r1, [r0]
  dsb
  isb



  b _start


  .thumb_func
InitializeUserMemorySections:
  ldr r0, =__start_nrf_sections
  ldr r1, =__start_nrf_sections_run
  ldr r2, =__end_nrf_sections_run
  cmp r0, r1
  beq 2f
  subs r2, r2, r1
  beq 2f
1:
  ldrb r3, [r0]
  adds r0, r0, #1
  strb r3, [r1]
  adds r1, r1, #1
  subs r2, r2, #1
  bne 1b
2:
  bx lr
