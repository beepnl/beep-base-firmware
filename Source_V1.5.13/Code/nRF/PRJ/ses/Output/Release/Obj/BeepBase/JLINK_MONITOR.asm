	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"JLINK_MONITOR.c"
	.text
	.global	MAIN_MonCnt
	.section	.bss.MAIN_MonCnt,"aw",%nobits
	.align	2
	.type	MAIN_MonCnt, %object
	.size	MAIN_MonCnt, 4
MAIN_MonCnt:
	.space	4
	.section	.text.JLINK_MONITOR_OnExit,"ax",%progbits
	.align	1
	.global	JLINK_MONITOR_OnExit
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	JLINK_MONITOR_OnExit, %function
JLINK_MONITOR_OnExit:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	nop
	bx	lr
	.size	JLINK_MONITOR_OnExit, .-JLINK_MONITOR_OnExit
	.section	.text.JLINK_MONITOR_OnEnter,"ax",%progbits
	.align	1
	.global	JLINK_MONITOR_OnEnter
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	JLINK_MONITOR_OnEnter, %function
JLINK_MONITOR_OnEnter:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	nop
	bx	lr
	.size	JLINK_MONITOR_OnEnter, .-JLINK_MONITOR_OnEnter
	.section	.text.JLINK_MONITOR_OnPoll,"ax",%progbits
	.align	1
	.global	JLINK_MONITOR_OnPoll
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	JLINK_MONITOR_OnPoll, %function
JLINK_MONITOR_OnPoll:
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	ldr	r3, .L4
	ldr	r3, [r3]
	adds	r3, r3, #1
	ldr	r2, .L4
	str	r3, [r2]
	nop
	bx	lr
.L5:
	.align	2
.L4:
	.word	MAIN_MonCnt
	.size	JLINK_MONITOR_OnPoll, .-JLINK_MONITOR_OnPoll
	.ident	"GCC: (based on arm-10.3-2021.07 GCC) 10.3.1 20210621 (release)"
