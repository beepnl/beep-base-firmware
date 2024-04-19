	.cpu cortex-m4
	.eabi_attribute 27, 1
	.eabi_attribute 28, 1
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 4
	.eabi_attribute 34, 1
	.eabi_attribute 18, 4
	.file	"pb_common.c"
	.text
.Ltext0:
	.section	.text.pb_field_iter_begin,"ax",%progbits
	.align	1
	.global	pb_field_iter_begin
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	pb_field_iter_begin, %function
pb_field_iter_begin:
.LVL0:
.LFB0:
	.file 1 "/home/orlo/beep-base-firmware/Source_V1.5.12/Code/nRF/nRF5_SDK_15.3/external/nano-pb/pb_common.c"
	.loc 1 9 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	.loc 1 10 5 view .LVU1
	.loc 1 12 32 is_stmt 0 view .LVU2
	movs	r3, #0
	.loc 1 13 23 view .LVU3
	strd	r3, r2, [r0, #8]
	.loc 1 14 49 view .LVU4
	ldrh	r3, [r1, #4]
	.loc 1 14 38 view .LVU5
	add	r2, r2, r3
.LVL1:
	.loc 1 15 49 view .LVU6
	ldrsh	r3, [r1, #6]
	.loc 1 14 17 view .LVU7
	str	r2, [r0, #16]
	.loc 1 15 38 view .LVU8
	add	r2, r2, r3
	.loc 1 11 15 view .LVU9
	strd	r1, r1, [r0]
	.loc 1 12 5 is_stmt 1 view .LVU10
	.loc 1 14 5 view .LVU11
	.loc 1 15 5 view .LVU12
	.loc 1 15 17 is_stmt 0 view .LVU13
	str	r2, [r0, #20]
	.loc 1 17 5 is_stmt 1 view .LVU14
	.loc 1 17 28 is_stmt 0 view .LVU15
	ldrh	r0, [r1]
.LVL2:
	.loc 1 18 1 view .LVU16
	subs	r0, r0, #0
	it	ne
	movne	r0, #1
	bx	lr
.LFE0:
	.size	pb_field_iter_begin, .-pb_field_iter_begin
	.section	.text.pb_field_iter_next,"ax",%progbits
	.align	1
	.global	pb_field_iter_next
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	pb_field_iter_next, %function
pb_field_iter_next:
.LVL3:
.LFB1:
	.loc 1 21 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 22 5 view .LVU18
	.loc 1 22 23 is_stmt 0 view .LVU19
	ldr	r2, [r0, #4]
.LVL4:
	.loc 1 24 5 is_stmt 1 view .LVU20
	.loc 1 24 8 is_stmt 0 view .LVU21
	ldrh	r1, [r2]
	.loc 1 21 1 view .LVU22
	push	{r3, r4, r5, lr}
.LCFI0:
	.loc 1 21 1 view .LVU23
	mov	r3, r0
	.loc 1 24 8 view .LVU24
	cmp	r1, #0
	beq	.L9
	.loc 1 31 5 is_stmt 1 view .LVU25
	.loc 1 33 8 is_stmt 0 view .LVU26
	ldrh	r4, [r2, #16]
	.loc 1 31 14 view .LVU27
	add	r1, r2, #16
	str	r1, [r0, #4]
	.loc 1 33 5 is_stmt 1 view .LVU28
	.loc 1 33 8 is_stmt 0 view .LVU29
	cbnz	r4, .L4
	.loc 1 36 9 is_stmt 1 view .LVU30
	.loc 1 36 15 is_stmt 0 view .LVU31
	ldr	r2, [r0, #12]
.LVL5:
	.loc 1 36 15 view .LVU32
	ldr	r1, [r0]
.LVL6:
	.loc 1 36 15 view .LVU33
	bl	pb_field_iter_begin
.LVL7:
	.loc 1 37 9 is_stmt 1 view .LVU34
	.loc 1 37 16 is_stmt 0 view .LVU35
	mov	r0, r4
.L3:
	.loc 1 76 1 view .LVU36
	pop	{r3, r4, r5, pc}
.LVL8:
.L4:
.LBB2:
	.loc 1 42 9 is_stmt 1 view .LVU37
	.loc 1 44 13 is_stmt 0 view .LVU38
	ldrb	r0, [r2, #2]	@ zero_extendqisi2
.LVL9:
	.loc 1 42 16 view .LVU39
	ldrh	r1, [r2, #8]
.LVL10:
	.loc 1 44 9 is_stmt 1 view .LVU40
	and	r5, r0, #48
	.loc 1 44 12 is_stmt 0 view .LVU41
	cmp	r5, #48
	bne	.L5
	.loc 1 44 58 discriminator 1 view .LVU42
	ldrb	r4, [r2, #18]	@ zero_extendqisi2
	and	r4, r4, #48
	cmp	r4, #48
	bne	.L6
	.loc 1 48 13 is_stmt 1 view .LVU43
.LVL11:
	.loc 1 49 13 view .LVU44
	.loc 1 49 58 is_stmt 0 view .LVU45
	ldrh	r0, [r2, #4]
	.loc 1 49 46 view .LVU46
	ldr	r1, [r3, #16]
	subs	r1, r1, r0
	.loc 1 49 25 view .LVU47
	str	r1, [r3, #16]
	.loc 1 65 9 is_stmt 1 view .LVU48
	.loc 1 48 23 is_stmt 0 view .LVU49
	movs	r1, #0
.LVL12:
.L7:
	.loc 1 72 9 is_stmt 1 view .LVU50
	.loc 1 72 65 is_stmt 0 view .LVU51
	ldrh	r0, [r2, #20]
	.loc 1 73 53 view .LVU52
	ldrsh	r2, [r2, #22]
.LVL13:
	.loc 1 72 54 view .LVU53
	add	r0, r0, r1
	ldr	r1, [r3, #16]
	add	r1, r1, r0
	.loc 1 72 21 view .LVU54
	str	r1, [r3, #16]
	.loc 1 73 9 is_stmt 1 view .LVU55
	.loc 1 73 42 is_stmt 0 view .LVU56
	add	r1, r1, r2
	.loc 1 73 21 view .LVU57
	str	r1, [r3, #20]
	.loc 1 74 9 is_stmt 1 view .LVU58
	.loc 1 74 16 is_stmt 0 view .LVU59
	movs	r0, #1
	b	.L3
.LVL14:
.L5:
	.loc 1 51 14 is_stmt 1 view .LVU60
	.loc 1 51 17 is_stmt 0 view .LVU61
	and	r4, r0, #240
	cmp	r4, #32
	bne	.L6
	.loc 1 56 13 is_stmt 1 view .LVU62
	.loc 1 56 36 is_stmt 0 view .LVU63
	ldrh	r0, [r2, #10]
	.loc 1 56 23 view .LVU64
	muls	r1, r0, r1
.LVL15:
.L8:
	.loc 1 65 9 is_stmt 1 view .LVU65
	.loc 1 65 12 is_stmt 0 view .LVU66
	cmp	r5, #0
	bne	.L7
	.loc 1 69 13 is_stmt 1 view .LVU67
	.loc 1 69 39 is_stmt 0 view .LVU68
	ldr	r0, [r3, #8]
	adds	r0, r0, #1
	str	r0, [r3, #8]
	b	.L7
.L6:
	.loc 1 58 14 is_stmt 1 view .LVU69
	.loc 1 58 17 is_stmt 0 view .LVU70
	and	r0, r0, #192
	.loc 1 62 23 view .LVU71
	cmp	r0, #128
	it	eq
	moveq	r1, #4
.LVL16:
	.loc 1 62 23 view .LVU72
	b	.L8
.LVL17:
.L9:
	.loc 1 62 23 view .LVU73
.LBE2:
	.loc 1 28 16 view .LVU74
	mov	r0, r1
.LVL18:
	.loc 1 28 16 view .LVU75
	b	.L3
.LFE1:
	.size	pb_field_iter_next, .-pb_field_iter_next
	.section	.text.pb_field_iter_find,"ax",%progbits
	.align	1
	.global	pb_field_iter_find
	.syntax unified
	.thumb
	.thumb_func
	.fpu fpv4-sp-d16
	.type	pb_field_iter_find, %function
pb_field_iter_find:
.LVL19:
.LFB2:
	.loc 1 79 1 is_stmt 1 view -0
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	.loc 1 80 5 view .LVU77
	.loc 1 79 1 is_stmt 0 view .LVU78
	push	{r4, r5, r6, lr}
.LCFI1:
	.loc 1 80 23 view .LVU79
	ldr	r6, [r0, #4]
.LVL20:
	.loc 1 79 1 view .LVU80
	mov	r4, r0
	mov	r5, r1
.LVL21:
.L13:
	.loc 1 82 5 is_stmt 1 view .LVU81
	.loc 1 83 9 view .LVU82
	.loc 1 83 17 is_stmt 0 view .LVU83
	ldr	r3, [r4, #4]
	.loc 1 83 22 view .LVU84
	ldrh	r2, [r3]
	.loc 1 83 12 view .LVU85
	cmp	r2, r5
	bne	.L11
	.loc 1 83 35 discriminator 1 view .LVU86
	ldrb	r3, [r3, #2]	@ zero_extendqisi2
	and	r3, r3, #15
	cmp	r3, #8
	bne	.L14
.L11:
	.loc 1 90 9 is_stmt 1 view .LVU87
	.loc 1 90 15 is_stmt 0 view .LVU88
	mov	r0, r4
	bl	pb_field_iter_next
.LVL22:
	.loc 1 91 13 is_stmt 1 view .LVU89
	.loc 1 91 5 is_stmt 0 view .LVU90
	ldr	r3, [r4, #4]
	cmp	r3, r6
	bne	.L13
	.loc 1 94 12 view .LVU91
	movs	r0, #0
.L12:
	.loc 1 95 1 view .LVU92
	pop	{r4, r5, r6, pc}
.LVL23:
.L14:
	.loc 1 87 20 view .LVU93
	movs	r0, #1
	b	.L12
.LFE2:
	.size	pb_field_iter_find, .-pb_field_iter_find
	.section	.debug_frame,"",%progbits
.Lframe0:
	.4byte	.LECIE0-.LSCIE0
.LSCIE0:
	.4byte	0xffffffff
	.byte	0x3
	.ascii	"\000"
	.uleb128 0x1
	.sleb128 -4
	.uleb128 0xe
	.byte	0xc
	.uleb128 0xd
	.uleb128 0
	.align	2
.LECIE0:
.LSFDE0:
	.4byte	.LEFDE0-.LASFDE0
.LASFDE0:
	.4byte	.Lframe0
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.align	2
.LEFDE0:
.LSFDE2:
	.4byte	.LEFDE2-.LASFDE2
.LASFDE2:
	.4byte	.Lframe0
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.byte	0x4
	.4byte	.LCFI0-.LFB1
	.byte	0xe
	.uleb128 0x10
	.byte	0x83
	.uleb128 0x4
	.byte	0x84
	.uleb128 0x3
	.byte	0x85
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE2:
.LSFDE4:
	.4byte	.LEFDE4-.LASFDE4
.LASFDE4:
	.4byte	.Lframe0
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.byte	0x4
	.4byte	.LCFI1-.LFB2
	.byte	0xe
	.uleb128 0x10
	.byte	0x84
	.uleb128 0x4
	.byte	0x85
	.uleb128 0x3
	.byte	0x86
	.uleb128 0x2
	.byte	0x8e
	.uleb128 0x1
	.align	2
.LEFDE4:
	.text
.Letext0:
	.file 2 "/usr/share/segger_embedded_studio_for_arm_5.42a/include/stdint.h"
	.file 3 "/usr/share/segger_embedded_studio_for_arm_5.42a/include/stddef.h"
	.file 4 "/home/orlo/beep-base-firmware/Source_V1.5.12/Code/nRF/nRF5_SDK_15.3/external/nano-pb/pb.h"
	.file 5 "/home/orlo/beep-base-firmware/Source_V1.5.12/Code/nRF/nRF5_SDK_15.3/external/nano-pb/pb_common.h"
	.section	.debug_info,"",%progbits
.Ldebug_info0:
	.4byte	0x325
	.2byte	0x4
	.4byte	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.4byte	.LASF687
	.byte	0xc
	.4byte	.LASF688
	.4byte	.LASF689
	.4byte	.Ldebug_ranges0+0
	.4byte	0
	.4byte	.Ldebug_line0
	.4byte	.Ldebug_macro0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.4byte	.LASF646
	.uleb128 0x3
	.4byte	.LASF648
	.byte	0x2
	.byte	0x2a
	.byte	0x1c
	.4byte	0x3c
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF647
	.uleb128 0x3
	.4byte	.LASF649
	.byte	0x2
	.byte	0x2f
	.byte	0x1c
	.4byte	0x4f
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.4byte	.LASF650
	.uleb128 0x3
	.4byte	.LASF651
	.byte	0x2
	.byte	0x30
	.byte	0x1c
	.4byte	0x62
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.4byte	.LASF652
	.uleb128 0x4
	.byte	0x4
	.byte	0x5
	.ascii	"int\000"
	.uleb128 0x3
	.4byte	.LASF653
	.byte	0x2
	.byte	0x37
	.byte	0x1c
	.4byte	0x7c
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.4byte	.LASF654
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.4byte	.LASF655
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.4byte	.LASF656
	.uleb128 0x3
	.4byte	.LASF657
	.byte	0x2
	.byte	0x49
	.byte	0x11
	.4byte	0x43
	.uleb128 0x3
	.4byte	.LASF658
	.byte	0x2
	.byte	0x4d
	.byte	0x11
	.4byte	0x30
	.uleb128 0x3
	.4byte	.LASF659
	.byte	0x2
	.byte	0x4e
	.byte	0x12
	.4byte	0x56
	.uleb128 0x5
	.byte	0x4
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.4byte	.LASF660
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.4byte	.LASF661
	.uleb128 0x3
	.4byte	.LASF662
	.byte	0x3
	.byte	0x31
	.byte	0x16
	.4byte	0x7c
	.uleb128 0x2
	.byte	0x8
	.byte	0x4
	.4byte	.LASF663
	.uleb128 0x3
	.4byte	.LASF664
	.byte	0x4
	.byte	0x8f
	.byte	0x17
	.4byte	0x9d
	.uleb128 0x3
	.4byte	.LASF665
	.byte	0x4
	.byte	0xcb
	.byte	0x1c
	.4byte	0xa9
	.uleb128 0x3
	.4byte	.LASF666
	.byte	0x4
	.byte	0xcc
	.byte	0x1b
	.4byte	0x91
	.uleb128 0x3
	.4byte	.LASF667
	.byte	0x4
	.byte	0xe2
	.byte	0x1b
	.4byte	0x10d
	.uleb128 0x6
	.4byte	0xfc
	.uleb128 0x7
	.4byte	.LASF674
	.byte	0x10
	.byte	0x4
	.byte	0xe3
	.byte	0x8
	.4byte	0x176
	.uleb128 0x8
	.ascii	"tag\000"
	.byte	0x4
	.byte	0xe4
	.byte	0xf
	.4byte	0xe4
	.byte	0
	.uleb128 0x9
	.4byte	.LASF668
	.byte	0x4
	.byte	0xe5
	.byte	0xf
	.4byte	0xd8
	.byte	0x2
	.uleb128 0x9
	.4byte	.LASF669
	.byte	0x4
	.byte	0xe6
	.byte	0xf
	.4byte	0xe4
	.byte	0x4
	.uleb128 0x9
	.4byte	.LASF670
	.byte	0x4
	.byte	0xe7
	.byte	0x10
	.4byte	0xf0
	.byte	0x6
	.uleb128 0x9
	.4byte	.LASF671
	.byte	0x4
	.byte	0xe8
	.byte	0xf
	.4byte	0xe4
	.byte	0x8
	.uleb128 0x9
	.4byte	.LASF672
	.byte	0x4
	.byte	0xe9
	.byte	0xf
	.4byte	0xe4
	.byte	0xa
	.uleb128 0x8
	.ascii	"ptr\000"
	.byte	0x4
	.byte	0xee
	.byte	0x11
	.4byte	0x176
	.byte	0xc
	.byte	0
	.uleb128 0xa
	.byte	0x4
	.4byte	0x17c
	.uleb128 0xb
	.uleb128 0x2
	.byte	0x1
	.byte	0x2
	.4byte	.LASF673
	.uleb128 0xa
	.byte	0x4
	.4byte	0x108
	.uleb128 0x7
	.4byte	.LASF675
	.byte	0x18
	.byte	0x5
	.byte	0xf
	.byte	0x8
	.4byte	0x1e6
	.uleb128 0x9
	.4byte	.LASF676
	.byte	0x5
	.byte	0x10
	.byte	0x17
	.4byte	0x184
	.byte	0
	.uleb128 0x8
	.ascii	"pos\000"
	.byte	0x5
	.byte	0x11
	.byte	0x17
	.4byte	0x184
	.byte	0x4
	.uleb128 0x9
	.4byte	.LASF677
	.byte	0x5
	.byte	0x12
	.byte	0xe
	.4byte	0x7c
	.byte	0x8
	.uleb128 0x9
	.4byte	.LASF678
	.byte	0x5
	.byte	0x13
	.byte	0xb
	.4byte	0xb5
	.byte	0xc
	.uleb128 0x9
	.4byte	.LASF679
	.byte	0x5
	.byte	0x14
	.byte	0xb
	.4byte	0xb5
	.byte	0x10
	.uleb128 0x9
	.4byte	.LASF680
	.byte	0x5
	.byte	0x15
	.byte	0xb
	.4byte	0xb5
	.byte	0x14
	.byte	0
	.uleb128 0x3
	.4byte	.LASF681
	.byte	0x5
	.byte	0x17
	.byte	0x20
	.4byte	0x18a
	.uleb128 0xc
	.4byte	.LASF683
	.byte	0x1
	.byte	0x4e
	.byte	0x6
	.4byte	0x17d
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x259
	.uleb128 0xd
	.4byte	.LASF682
	.byte	0x1
	.byte	0x4e
	.byte	0x2a
	.4byte	0x259
	.4byte	.LLST5
	.4byte	.LVUS5
	.uleb128 0xe
	.ascii	"tag\000"
	.byte	0x1
	.byte	0x4e
	.byte	0x39
	.4byte	0x70
	.4byte	.LLST6
	.4byte	.LVUS6
	.uleb128 0xf
	.4byte	.LASF676
	.byte	0x1
	.byte	0x50
	.byte	0x17
	.4byte	0x184
	.4byte	.LLST7
	.4byte	.LVUS7
	.uleb128 0x10
	.4byte	.LVL22
	.4byte	0x25f
	.uleb128 0x11
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x2
	.byte	0x74
	.sleb128 0
	.byte	0
	.byte	0
	.uleb128 0xa
	.byte	0x4
	.4byte	0x1e6
	.uleb128 0xc
	.4byte	.LASF684
	.byte	0x1
	.byte	0x14
	.byte	0x6
	.4byte	0x17d
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x2db
	.uleb128 0xd
	.4byte	.LASF682
	.byte	0x1
	.byte	0x14
	.byte	0x2a
	.4byte	0x259
	.4byte	.LLST2
	.4byte	.LVUS2
	.uleb128 0xf
	.4byte	.LASF685
	.byte	0x1
	.byte	0x16
	.byte	0x17
	.4byte	0x184
	.4byte	.LLST3
	.4byte	.LVUS3
	.uleb128 0x12
	.4byte	.LBB2
	.4byte	.LBE2-.LBB2
	.4byte	0x2c3
	.uleb128 0xf
	.4byte	.LASF686
	.byte	0x1
	.byte	0x2a
	.byte	0x10
	.4byte	0xc5
	.4byte	.LLST4
	.4byte	.LVUS4
	.byte	0
	.uleb128 0x10
	.4byte	.LVL7
	.4byte	0x2db
	.uleb128 0x11
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x3
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.uleb128 0x11
	.uleb128 0x1
	.byte	0x51
	.uleb128 0x2
	.byte	0x71
	.sleb128 0
	.byte	0
	.byte	0
	.uleb128 0x13
	.4byte	.LASF690
	.byte	0x1
	.byte	0x8
	.byte	0x6
	.4byte	0x17d
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0xd
	.4byte	.LASF682
	.byte	0x1
	.byte	0x8
	.byte	0x2b
	.4byte	0x259
	.4byte	.LLST0
	.4byte	.LVUS0
	.uleb128 0x14
	.4byte	.LASF691
	.byte	0x1
	.byte	0x8
	.byte	0x43
	.4byte	0x184
	.uleb128 0x1
	.byte	0x51
	.uleb128 0xd
	.4byte	.LASF678
	.byte	0x1
	.byte	0x8
	.byte	0x51
	.4byte	0xb5
	.4byte	.LLST1
	.4byte	.LVUS1
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",%progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x2134
	.uleb128 0x19
	.uleb128 0x55
	.uleb128 0x17
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x10
	.uleb128 0x17
	.uleb128 0x2119
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.uleb128 0x2137
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.uleb128 0x2137
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.uleb128 0x2137
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x4109
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x410a
	.byte	0
	.uleb128 0x2
	.uleb128 0x18
	.uleb128 0x2111
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",%progbits
.Ldebug_loc0:
.LVUS5:
	.uleb128 0
	.uleb128 .LVU81
	.uleb128 .LVU81
	.uleb128 0
.LLST5:
	.4byte	.LVL19
	.4byte	.LVL21
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL21
	.4byte	.LFE2
	.2byte	0x1
	.byte	0x54
	.4byte	0
	.4byte	0
.LVUS6:
	.uleb128 0
	.uleb128 .LVU81
	.uleb128 .LVU81
	.uleb128 0
.LLST6:
	.4byte	.LVL19
	.4byte	.LVL21
	.2byte	0x1
	.byte	0x51
	.4byte	.LVL21
	.4byte	.LFE2
	.2byte	0x1
	.byte	0x55
	.4byte	0
	.4byte	0
.LVUS7:
	.uleb128 .LVU80
	.uleb128 0
.LLST7:
	.4byte	.LVL20
	.4byte	.LFE2
	.2byte	0x1
	.byte	0x56
	.4byte	0
	.4byte	0
.LVUS2:
	.uleb128 0
	.uleb128 .LVU34
	.uleb128 .LVU34
	.uleb128 .LVU37
	.uleb128 .LVU37
	.uleb128 .LVU39
	.uleb128 .LVU39
	.uleb128 .LVU73
	.uleb128 .LVU73
	.uleb128 .LVU75
	.uleb128 .LVU75
	.uleb128 0
.LLST2:
	.4byte	.LVL3
	.4byte	.LVL7-1
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL7-1
	.4byte	.LVL8
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.4byte	.LVL8
	.4byte	.LVL9
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL9
	.4byte	.LVL17
	.2byte	0x1
	.byte	0x53
	.4byte	.LVL17
	.4byte	.LVL18
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL18
	.4byte	.LFE1
	.2byte	0x1
	.byte	0x53
	.4byte	0
	.4byte	0
.LVUS3:
	.uleb128 .LVU20
	.uleb128 .LVU32
	.uleb128 .LVU32
	.uleb128 .LVU33
	.uleb128 .LVU33
	.uleb128 .LVU34
	.uleb128 .LVU37
	.uleb128 .LVU53
	.uleb128 .LVU53
	.uleb128 .LVU60
	.uleb128 .LVU60
	.uleb128 0
.LLST3:
	.4byte	.LVL4
	.4byte	.LVL5
	.2byte	0x1
	.byte	0x52
	.4byte	.LVL5
	.4byte	.LVL6
	.2byte	0x3
	.byte	0x71
	.sleb128 -16
	.byte	0x9f
	.4byte	.LVL6
	.4byte	.LVL7-1
	.2byte	0x6
	.byte	0x70
	.sleb128 4
	.byte	0x6
	.byte	0x40
	.byte	0x1c
	.byte	0x9f
	.4byte	.LVL8
	.4byte	.LVL13
	.2byte	0x1
	.byte	0x52
	.4byte	.LVL13
	.4byte	.LVL14
	.2byte	0x6
	.byte	0x73
	.sleb128 4
	.byte	0x6
	.byte	0x40
	.byte	0x1c
	.byte	0x9f
	.4byte	.LVL14
	.4byte	.LFE1
	.2byte	0x1
	.byte	0x52
	.4byte	0
	.4byte	0
.LVUS4:
	.uleb128 .LVU40
	.uleb128 .LVU44
	.uleb128 .LVU44
	.uleb128 .LVU50
	.uleb128 .LVU60
	.uleb128 .LVU72
	.uleb128 .LVU72
	.uleb128 .LVU73
.LLST4:
	.4byte	.LVL10
	.4byte	.LVL11
	.2byte	0x1
	.byte	0x51
	.4byte	.LVL11
	.4byte	.LVL12
	.2byte	0x2
	.byte	0x30
	.byte	0x9f
	.4byte	.LVL14
	.4byte	.LVL16
	.2byte	0x1
	.byte	0x51
	.4byte	.LVL16
	.4byte	.LVL17
	.2byte	0x9
	.byte	0x72
	.sleb128 8
	.byte	0x94
	.byte	0x2
	.byte	0xa
	.2byte	0xffff
	.byte	0x1a
	.byte	0x9f
	.4byte	0
	.4byte	0
.LVUS0:
	.uleb128 0
	.uleb128 .LVU16
	.uleb128 .LVU16
	.uleb128 0
.LLST0:
	.4byte	.LVL0
	.4byte	.LVL2
	.2byte	0x1
	.byte	0x50
	.4byte	.LVL2
	.4byte	.LFE0
	.2byte	0x4
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x9f
	.4byte	0
	.4byte	0
.LVUS1:
	.uleb128 0
	.uleb128 .LVU6
	.uleb128 .LVU6
	.uleb128 .LVU16
	.uleb128 .LVU16
	.uleb128 0
.LLST1:
	.4byte	.LVL0
	.4byte	.LVL1
	.2byte	0x1
	.byte	0x52
	.4byte	.LVL1
	.4byte	.LVL2
	.2byte	0x2
	.byte	0x70
	.sleb128 12
	.4byte	.LVL2
	.4byte	.LFE0
	.2byte	0x5
	.byte	0xf3
	.uleb128 0x1
	.byte	0x50
	.byte	0x23
	.uleb128 0xc
	.4byte	0
	.4byte	0
	.section	.debug_pubnames,"",%progbits
	.4byte	0x54
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x329
	.4byte	0x1f2
	.ascii	"pb_field_iter_find\000"
	.4byte	0x25f
	.ascii	"pb_field_iter_next\000"
	.4byte	0x2db
	.ascii	"pb_field_iter_begin\000"
	.4byte	0
	.section	.debug_pubtypes,"",%progbits
	.4byte	0x1b0
	.2byte	0x2
	.4byte	.Ldebug_info0
	.4byte	0x329
	.4byte	0x29
	.ascii	"signed char\000"
	.4byte	0x3c
	.ascii	"unsigned char\000"
	.4byte	0x30
	.ascii	"uint8_t\000"
	.4byte	0x4f
	.ascii	"short int\000"
	.4byte	0x43
	.ascii	"int16_t\000"
	.4byte	0x62
	.ascii	"short unsigned int\000"
	.4byte	0x56
	.ascii	"uint16_t\000"
	.4byte	0x69
	.ascii	"int\000"
	.4byte	0x7c
	.ascii	"unsigned int\000"
	.4byte	0x70
	.ascii	"uint32_t\000"
	.4byte	0x83
	.ascii	"long long int\000"
	.4byte	0x8a
	.ascii	"long long unsigned int\000"
	.4byte	0x91
	.ascii	"int_least16_t\000"
	.4byte	0x9d
	.ascii	"uint_least8_t\000"
	.4byte	0xa9
	.ascii	"uint_least16_t\000"
	.4byte	0xb7
	.ascii	"long int\000"
	.4byte	0xbe
	.ascii	"char\000"
	.4byte	0xc5
	.ascii	"size_t\000"
	.4byte	0xd1
	.ascii	"long double\000"
	.4byte	0xd8
	.ascii	"pb_type_t\000"
	.4byte	0xe4
	.ascii	"pb_size_t\000"
	.4byte	0xf0
	.ascii	"pb_ssize_t\000"
	.4byte	0xfc
	.ascii	"pb_field_t\000"
	.4byte	0x10d
	.ascii	"pb_field_s\000"
	.4byte	0x17d
	.ascii	"_Bool\000"
	.4byte	0x18a
	.ascii	"pb_field_iter_s\000"
	.4byte	0x1e6
	.ascii	"pb_field_iter_t\000"
	.4byte	0
	.section	.debug_aranges,"",%progbits
	.4byte	0x2c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x4
	.byte	0
	.2byte	0
	.2byte	0
	.4byte	.LFB0
	.4byte	.LFE0-.LFB0
	.4byte	.LFB1
	.4byte	.LFE1-.LFB1
	.4byte	.LFB2
	.4byte	.LFE2-.LFB2
	.4byte	0
	.4byte	0
	.section	.debug_ranges,"",%progbits
.Ldebug_ranges0:
	.4byte	.LFB0
	.4byte	.LFE0
	.4byte	.LFB1
	.4byte	.LFE1
	.4byte	.LFB2
	.4byte	.LFE2
	.4byte	0
	.4byte	0
	.section	.debug_macro,"",%progbits
.Ldebug_macro0:
	.2byte	0x4
	.byte	0x2
	.4byte	.Ldebug_line0
	.byte	0x7
	.4byte	.Ldebug_macro2
	.byte	0x3
	.uleb128 0
	.uleb128 0x1
	.byte	0x3
	.uleb128 0x6
	.uleb128 0x5
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF464
	.byte	0x3
	.uleb128 0x8
	.uleb128 0x4
	.byte	0x7
	.4byte	.Ldebug_macro3
	.byte	0x3
	.uleb128 0x41
	.uleb128 0x2
	.byte	0x7
	.4byte	.Ldebug_macro4
	.byte	0x4
	.byte	0x3
	.uleb128 0x42
	.uleb128 0x3
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF530
	.file 6 "/usr/share/segger_embedded_studio_for_arm_5.42a/include/__crossworks.h"
	.byte	0x3
	.uleb128 0x29
	.uleb128 0x6
	.byte	0x7
	.4byte	.Ldebug_macro5
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro6
	.byte	0x4
	.file 7 "/usr/share/segger_embedded_studio_for_arm_5.42a/include/stdbool.h"
	.byte	0x3
	.uleb128 0x43
	.uleb128 0x7
	.byte	0x7
	.4byte	.Ldebug_macro7
	.byte	0x4
	.file 8 "/usr/share/segger_embedded_studio_for_arm_5.42a/include/string.h"
	.byte	0x3
	.uleb128 0x44
	.uleb128 0x8
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF561
	.byte	0x4
	.byte	0x7
	.4byte	.Ldebug_macro8
	.byte	0x4
	.byte	0x4
	.byte	0x4
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.0.a4926be8670181b19156dc4d40636d25,comdat
.Ldebug_macro2:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF0
	.byte	0x5
	.uleb128 0
	.4byte	.LASF1
	.byte	0x5
	.uleb128 0
	.4byte	.LASF2
	.byte	0x5
	.uleb128 0
	.4byte	.LASF3
	.byte	0x5
	.uleb128 0
	.4byte	.LASF4
	.byte	0x5
	.uleb128 0
	.4byte	.LASF5
	.byte	0x5
	.uleb128 0
	.4byte	.LASF6
	.byte	0x5
	.uleb128 0
	.4byte	.LASF7
	.byte	0x5
	.uleb128 0
	.4byte	.LASF8
	.byte	0x5
	.uleb128 0
	.4byte	.LASF9
	.byte	0x5
	.uleb128 0
	.4byte	.LASF10
	.byte	0x5
	.uleb128 0
	.4byte	.LASF11
	.byte	0x5
	.uleb128 0
	.4byte	.LASF12
	.byte	0x5
	.uleb128 0
	.4byte	.LASF13
	.byte	0x5
	.uleb128 0
	.4byte	.LASF14
	.byte	0x5
	.uleb128 0
	.4byte	.LASF15
	.byte	0x5
	.uleb128 0
	.4byte	.LASF16
	.byte	0x5
	.uleb128 0
	.4byte	.LASF17
	.byte	0x5
	.uleb128 0
	.4byte	.LASF18
	.byte	0x5
	.uleb128 0
	.4byte	.LASF19
	.byte	0x5
	.uleb128 0
	.4byte	.LASF20
	.byte	0x5
	.uleb128 0
	.4byte	.LASF21
	.byte	0x5
	.uleb128 0
	.4byte	.LASF22
	.byte	0x5
	.uleb128 0
	.4byte	.LASF23
	.byte	0x5
	.uleb128 0
	.4byte	.LASF24
	.byte	0x5
	.uleb128 0
	.4byte	.LASF25
	.byte	0x5
	.uleb128 0
	.4byte	.LASF26
	.byte	0x5
	.uleb128 0
	.4byte	.LASF27
	.byte	0x5
	.uleb128 0
	.4byte	.LASF28
	.byte	0x5
	.uleb128 0
	.4byte	.LASF29
	.byte	0x5
	.uleb128 0
	.4byte	.LASF30
	.byte	0x5
	.uleb128 0
	.4byte	.LASF31
	.byte	0x5
	.uleb128 0
	.4byte	.LASF32
	.byte	0x5
	.uleb128 0
	.4byte	.LASF33
	.byte	0x5
	.uleb128 0
	.4byte	.LASF34
	.byte	0x5
	.uleb128 0
	.4byte	.LASF35
	.byte	0x5
	.uleb128 0
	.4byte	.LASF36
	.byte	0x5
	.uleb128 0
	.4byte	.LASF37
	.byte	0x5
	.uleb128 0
	.4byte	.LASF38
	.byte	0x5
	.uleb128 0
	.4byte	.LASF39
	.byte	0x5
	.uleb128 0
	.4byte	.LASF40
	.byte	0x5
	.uleb128 0
	.4byte	.LASF41
	.byte	0x5
	.uleb128 0
	.4byte	.LASF42
	.byte	0x5
	.uleb128 0
	.4byte	.LASF43
	.byte	0x5
	.uleb128 0
	.4byte	.LASF44
	.byte	0x5
	.uleb128 0
	.4byte	.LASF45
	.byte	0x5
	.uleb128 0
	.4byte	.LASF46
	.byte	0x5
	.uleb128 0
	.4byte	.LASF47
	.byte	0x5
	.uleb128 0
	.4byte	.LASF48
	.byte	0x5
	.uleb128 0
	.4byte	.LASF49
	.byte	0x5
	.uleb128 0
	.4byte	.LASF50
	.byte	0x5
	.uleb128 0
	.4byte	.LASF51
	.byte	0x5
	.uleb128 0
	.4byte	.LASF52
	.byte	0x5
	.uleb128 0
	.4byte	.LASF53
	.byte	0x5
	.uleb128 0
	.4byte	.LASF54
	.byte	0x5
	.uleb128 0
	.4byte	.LASF55
	.byte	0x5
	.uleb128 0
	.4byte	.LASF56
	.byte	0x5
	.uleb128 0
	.4byte	.LASF57
	.byte	0x5
	.uleb128 0
	.4byte	.LASF58
	.byte	0x5
	.uleb128 0
	.4byte	.LASF59
	.byte	0x5
	.uleb128 0
	.4byte	.LASF60
	.byte	0x5
	.uleb128 0
	.4byte	.LASF61
	.byte	0x5
	.uleb128 0
	.4byte	.LASF62
	.byte	0x5
	.uleb128 0
	.4byte	.LASF63
	.byte	0x5
	.uleb128 0
	.4byte	.LASF64
	.byte	0x5
	.uleb128 0
	.4byte	.LASF65
	.byte	0x5
	.uleb128 0
	.4byte	.LASF66
	.byte	0x5
	.uleb128 0
	.4byte	.LASF67
	.byte	0x5
	.uleb128 0
	.4byte	.LASF68
	.byte	0x5
	.uleb128 0
	.4byte	.LASF69
	.byte	0x5
	.uleb128 0
	.4byte	.LASF70
	.byte	0x5
	.uleb128 0
	.4byte	.LASF71
	.byte	0x5
	.uleb128 0
	.4byte	.LASF72
	.byte	0x5
	.uleb128 0
	.4byte	.LASF73
	.byte	0x5
	.uleb128 0
	.4byte	.LASF74
	.byte	0x5
	.uleb128 0
	.4byte	.LASF75
	.byte	0x5
	.uleb128 0
	.4byte	.LASF76
	.byte	0x5
	.uleb128 0
	.4byte	.LASF77
	.byte	0x5
	.uleb128 0
	.4byte	.LASF78
	.byte	0x5
	.uleb128 0
	.4byte	.LASF79
	.byte	0x5
	.uleb128 0
	.4byte	.LASF80
	.byte	0x5
	.uleb128 0
	.4byte	.LASF81
	.byte	0x5
	.uleb128 0
	.4byte	.LASF82
	.byte	0x5
	.uleb128 0
	.4byte	.LASF83
	.byte	0x5
	.uleb128 0
	.4byte	.LASF84
	.byte	0x5
	.uleb128 0
	.4byte	.LASF85
	.byte	0x5
	.uleb128 0
	.4byte	.LASF86
	.byte	0x5
	.uleb128 0
	.4byte	.LASF87
	.byte	0x5
	.uleb128 0
	.4byte	.LASF88
	.byte	0x5
	.uleb128 0
	.4byte	.LASF89
	.byte	0x5
	.uleb128 0
	.4byte	.LASF90
	.byte	0x5
	.uleb128 0
	.4byte	.LASF91
	.byte	0x5
	.uleb128 0
	.4byte	.LASF92
	.byte	0x5
	.uleb128 0
	.4byte	.LASF93
	.byte	0x5
	.uleb128 0
	.4byte	.LASF94
	.byte	0x5
	.uleb128 0
	.4byte	.LASF95
	.byte	0x5
	.uleb128 0
	.4byte	.LASF96
	.byte	0x5
	.uleb128 0
	.4byte	.LASF97
	.byte	0x5
	.uleb128 0
	.4byte	.LASF98
	.byte	0x5
	.uleb128 0
	.4byte	.LASF99
	.byte	0x5
	.uleb128 0
	.4byte	.LASF100
	.byte	0x5
	.uleb128 0
	.4byte	.LASF101
	.byte	0x5
	.uleb128 0
	.4byte	.LASF102
	.byte	0x5
	.uleb128 0
	.4byte	.LASF103
	.byte	0x5
	.uleb128 0
	.4byte	.LASF104
	.byte	0x5
	.uleb128 0
	.4byte	.LASF105
	.byte	0x5
	.uleb128 0
	.4byte	.LASF106
	.byte	0x5
	.uleb128 0
	.4byte	.LASF107
	.byte	0x5
	.uleb128 0
	.4byte	.LASF108
	.byte	0x5
	.uleb128 0
	.4byte	.LASF109
	.byte	0x5
	.uleb128 0
	.4byte	.LASF110
	.byte	0x5
	.uleb128 0
	.4byte	.LASF111
	.byte	0x5
	.uleb128 0
	.4byte	.LASF112
	.byte	0x5
	.uleb128 0
	.4byte	.LASF113
	.byte	0x5
	.uleb128 0
	.4byte	.LASF114
	.byte	0x5
	.uleb128 0
	.4byte	.LASF115
	.byte	0x5
	.uleb128 0
	.4byte	.LASF116
	.byte	0x5
	.uleb128 0
	.4byte	.LASF117
	.byte	0x5
	.uleb128 0
	.4byte	.LASF118
	.byte	0x5
	.uleb128 0
	.4byte	.LASF119
	.byte	0x5
	.uleb128 0
	.4byte	.LASF120
	.byte	0x5
	.uleb128 0
	.4byte	.LASF121
	.byte	0x5
	.uleb128 0
	.4byte	.LASF122
	.byte	0x5
	.uleb128 0
	.4byte	.LASF123
	.byte	0x5
	.uleb128 0
	.4byte	.LASF124
	.byte	0x5
	.uleb128 0
	.4byte	.LASF125
	.byte	0x5
	.uleb128 0
	.4byte	.LASF126
	.byte	0x5
	.uleb128 0
	.4byte	.LASF127
	.byte	0x5
	.uleb128 0
	.4byte	.LASF128
	.byte	0x5
	.uleb128 0
	.4byte	.LASF129
	.byte	0x5
	.uleb128 0
	.4byte	.LASF130
	.byte	0x5
	.uleb128 0
	.4byte	.LASF131
	.byte	0x5
	.uleb128 0
	.4byte	.LASF132
	.byte	0x5
	.uleb128 0
	.4byte	.LASF133
	.byte	0x5
	.uleb128 0
	.4byte	.LASF134
	.byte	0x5
	.uleb128 0
	.4byte	.LASF135
	.byte	0x5
	.uleb128 0
	.4byte	.LASF136
	.byte	0x5
	.uleb128 0
	.4byte	.LASF137
	.byte	0x5
	.uleb128 0
	.4byte	.LASF138
	.byte	0x5
	.uleb128 0
	.4byte	.LASF139
	.byte	0x5
	.uleb128 0
	.4byte	.LASF140
	.byte	0x5
	.uleb128 0
	.4byte	.LASF141
	.byte	0x5
	.uleb128 0
	.4byte	.LASF142
	.byte	0x5
	.uleb128 0
	.4byte	.LASF143
	.byte	0x5
	.uleb128 0
	.4byte	.LASF144
	.byte	0x5
	.uleb128 0
	.4byte	.LASF145
	.byte	0x5
	.uleb128 0
	.4byte	.LASF146
	.byte	0x5
	.uleb128 0
	.4byte	.LASF147
	.byte	0x5
	.uleb128 0
	.4byte	.LASF148
	.byte	0x5
	.uleb128 0
	.4byte	.LASF149
	.byte	0x5
	.uleb128 0
	.4byte	.LASF150
	.byte	0x5
	.uleb128 0
	.4byte	.LASF151
	.byte	0x5
	.uleb128 0
	.4byte	.LASF152
	.byte	0x5
	.uleb128 0
	.4byte	.LASF153
	.byte	0x5
	.uleb128 0
	.4byte	.LASF154
	.byte	0x5
	.uleb128 0
	.4byte	.LASF155
	.byte	0x5
	.uleb128 0
	.4byte	.LASF156
	.byte	0x5
	.uleb128 0
	.4byte	.LASF157
	.byte	0x5
	.uleb128 0
	.4byte	.LASF158
	.byte	0x5
	.uleb128 0
	.4byte	.LASF159
	.byte	0x5
	.uleb128 0
	.4byte	.LASF160
	.byte	0x5
	.uleb128 0
	.4byte	.LASF161
	.byte	0x5
	.uleb128 0
	.4byte	.LASF162
	.byte	0x5
	.uleb128 0
	.4byte	.LASF163
	.byte	0x5
	.uleb128 0
	.4byte	.LASF164
	.byte	0x5
	.uleb128 0
	.4byte	.LASF165
	.byte	0x5
	.uleb128 0
	.4byte	.LASF166
	.byte	0x5
	.uleb128 0
	.4byte	.LASF167
	.byte	0x5
	.uleb128 0
	.4byte	.LASF168
	.byte	0x5
	.uleb128 0
	.4byte	.LASF169
	.byte	0x5
	.uleb128 0
	.4byte	.LASF170
	.byte	0x5
	.uleb128 0
	.4byte	.LASF171
	.byte	0x5
	.uleb128 0
	.4byte	.LASF172
	.byte	0x5
	.uleb128 0
	.4byte	.LASF173
	.byte	0x5
	.uleb128 0
	.4byte	.LASF174
	.byte	0x5
	.uleb128 0
	.4byte	.LASF175
	.byte	0x5
	.uleb128 0
	.4byte	.LASF176
	.byte	0x5
	.uleb128 0
	.4byte	.LASF177
	.byte	0x5
	.uleb128 0
	.4byte	.LASF178
	.byte	0x5
	.uleb128 0
	.4byte	.LASF179
	.byte	0x5
	.uleb128 0
	.4byte	.LASF180
	.byte	0x5
	.uleb128 0
	.4byte	.LASF181
	.byte	0x5
	.uleb128 0
	.4byte	.LASF182
	.byte	0x5
	.uleb128 0
	.4byte	.LASF183
	.byte	0x5
	.uleb128 0
	.4byte	.LASF184
	.byte	0x5
	.uleb128 0
	.4byte	.LASF185
	.byte	0x5
	.uleb128 0
	.4byte	.LASF186
	.byte	0x5
	.uleb128 0
	.4byte	.LASF187
	.byte	0x5
	.uleb128 0
	.4byte	.LASF188
	.byte	0x5
	.uleb128 0
	.4byte	.LASF189
	.byte	0x5
	.uleb128 0
	.4byte	.LASF190
	.byte	0x5
	.uleb128 0
	.4byte	.LASF191
	.byte	0x5
	.uleb128 0
	.4byte	.LASF192
	.byte	0x5
	.uleb128 0
	.4byte	.LASF193
	.byte	0x5
	.uleb128 0
	.4byte	.LASF194
	.byte	0x5
	.uleb128 0
	.4byte	.LASF195
	.byte	0x5
	.uleb128 0
	.4byte	.LASF196
	.byte	0x5
	.uleb128 0
	.4byte	.LASF197
	.byte	0x5
	.uleb128 0
	.4byte	.LASF198
	.byte	0x5
	.uleb128 0
	.4byte	.LASF199
	.byte	0x5
	.uleb128 0
	.4byte	.LASF200
	.byte	0x5
	.uleb128 0
	.4byte	.LASF201
	.byte	0x5
	.uleb128 0
	.4byte	.LASF202
	.byte	0x5
	.uleb128 0
	.4byte	.LASF203
	.byte	0x5
	.uleb128 0
	.4byte	.LASF204
	.byte	0x5
	.uleb128 0
	.4byte	.LASF205
	.byte	0x5
	.uleb128 0
	.4byte	.LASF206
	.byte	0x5
	.uleb128 0
	.4byte	.LASF207
	.byte	0x5
	.uleb128 0
	.4byte	.LASF208
	.byte	0x5
	.uleb128 0
	.4byte	.LASF209
	.byte	0x5
	.uleb128 0
	.4byte	.LASF210
	.byte	0x5
	.uleb128 0
	.4byte	.LASF211
	.byte	0x5
	.uleb128 0
	.4byte	.LASF212
	.byte	0x5
	.uleb128 0
	.4byte	.LASF213
	.byte	0x5
	.uleb128 0
	.4byte	.LASF214
	.byte	0x5
	.uleb128 0
	.4byte	.LASF215
	.byte	0x5
	.uleb128 0
	.4byte	.LASF216
	.byte	0x5
	.uleb128 0
	.4byte	.LASF217
	.byte	0x5
	.uleb128 0
	.4byte	.LASF218
	.byte	0x5
	.uleb128 0
	.4byte	.LASF219
	.byte	0x5
	.uleb128 0
	.4byte	.LASF220
	.byte	0x5
	.uleb128 0
	.4byte	.LASF221
	.byte	0x5
	.uleb128 0
	.4byte	.LASF222
	.byte	0x5
	.uleb128 0
	.4byte	.LASF223
	.byte	0x5
	.uleb128 0
	.4byte	.LASF224
	.byte	0x5
	.uleb128 0
	.4byte	.LASF225
	.byte	0x5
	.uleb128 0
	.4byte	.LASF226
	.byte	0x5
	.uleb128 0
	.4byte	.LASF227
	.byte	0x5
	.uleb128 0
	.4byte	.LASF228
	.byte	0x5
	.uleb128 0
	.4byte	.LASF229
	.byte	0x5
	.uleb128 0
	.4byte	.LASF230
	.byte	0x5
	.uleb128 0
	.4byte	.LASF231
	.byte	0x5
	.uleb128 0
	.4byte	.LASF232
	.byte	0x5
	.uleb128 0
	.4byte	.LASF233
	.byte	0x5
	.uleb128 0
	.4byte	.LASF234
	.byte	0x5
	.uleb128 0
	.4byte	.LASF235
	.byte	0x5
	.uleb128 0
	.4byte	.LASF236
	.byte	0x5
	.uleb128 0
	.4byte	.LASF237
	.byte	0x5
	.uleb128 0
	.4byte	.LASF238
	.byte	0x5
	.uleb128 0
	.4byte	.LASF239
	.byte	0x5
	.uleb128 0
	.4byte	.LASF240
	.byte	0x5
	.uleb128 0
	.4byte	.LASF241
	.byte	0x5
	.uleb128 0
	.4byte	.LASF242
	.byte	0x5
	.uleb128 0
	.4byte	.LASF243
	.byte	0x5
	.uleb128 0
	.4byte	.LASF244
	.byte	0x5
	.uleb128 0
	.4byte	.LASF245
	.byte	0x5
	.uleb128 0
	.4byte	.LASF246
	.byte	0x5
	.uleb128 0
	.4byte	.LASF247
	.byte	0x5
	.uleb128 0
	.4byte	.LASF248
	.byte	0x5
	.uleb128 0
	.4byte	.LASF249
	.byte	0x5
	.uleb128 0
	.4byte	.LASF250
	.byte	0x5
	.uleb128 0
	.4byte	.LASF251
	.byte	0x5
	.uleb128 0
	.4byte	.LASF252
	.byte	0x5
	.uleb128 0
	.4byte	.LASF253
	.byte	0x5
	.uleb128 0
	.4byte	.LASF254
	.byte	0x5
	.uleb128 0
	.4byte	.LASF255
	.byte	0x5
	.uleb128 0
	.4byte	.LASF256
	.byte	0x5
	.uleb128 0
	.4byte	.LASF257
	.byte	0x5
	.uleb128 0
	.4byte	.LASF258
	.byte	0x5
	.uleb128 0
	.4byte	.LASF259
	.byte	0x5
	.uleb128 0
	.4byte	.LASF260
	.byte	0x5
	.uleb128 0
	.4byte	.LASF261
	.byte	0x5
	.uleb128 0
	.4byte	.LASF262
	.byte	0x5
	.uleb128 0
	.4byte	.LASF263
	.byte	0x5
	.uleb128 0
	.4byte	.LASF264
	.byte	0x5
	.uleb128 0
	.4byte	.LASF265
	.byte	0x5
	.uleb128 0
	.4byte	.LASF266
	.byte	0x5
	.uleb128 0
	.4byte	.LASF267
	.byte	0x5
	.uleb128 0
	.4byte	.LASF268
	.byte	0x5
	.uleb128 0
	.4byte	.LASF269
	.byte	0x5
	.uleb128 0
	.4byte	.LASF270
	.byte	0x5
	.uleb128 0
	.4byte	.LASF271
	.byte	0x5
	.uleb128 0
	.4byte	.LASF272
	.byte	0x5
	.uleb128 0
	.4byte	.LASF273
	.byte	0x5
	.uleb128 0
	.4byte	.LASF274
	.byte	0x5
	.uleb128 0
	.4byte	.LASF275
	.byte	0x5
	.uleb128 0
	.4byte	.LASF276
	.byte	0x5
	.uleb128 0
	.4byte	.LASF277
	.byte	0x5
	.uleb128 0
	.4byte	.LASF278
	.byte	0x5
	.uleb128 0
	.4byte	.LASF279
	.byte	0x5
	.uleb128 0
	.4byte	.LASF280
	.byte	0x5
	.uleb128 0
	.4byte	.LASF281
	.byte	0x5
	.uleb128 0
	.4byte	.LASF282
	.byte	0x5
	.uleb128 0
	.4byte	.LASF283
	.byte	0x5
	.uleb128 0
	.4byte	.LASF284
	.byte	0x5
	.uleb128 0
	.4byte	.LASF285
	.byte	0x5
	.uleb128 0
	.4byte	.LASF286
	.byte	0x5
	.uleb128 0
	.4byte	.LASF287
	.byte	0x5
	.uleb128 0
	.4byte	.LASF288
	.byte	0x5
	.uleb128 0
	.4byte	.LASF289
	.byte	0x5
	.uleb128 0
	.4byte	.LASF290
	.byte	0x5
	.uleb128 0
	.4byte	.LASF291
	.byte	0x5
	.uleb128 0
	.4byte	.LASF292
	.byte	0x5
	.uleb128 0
	.4byte	.LASF293
	.byte	0x5
	.uleb128 0
	.4byte	.LASF294
	.byte	0x5
	.uleb128 0
	.4byte	.LASF295
	.byte	0x5
	.uleb128 0
	.4byte	.LASF296
	.byte	0x5
	.uleb128 0
	.4byte	.LASF297
	.byte	0x5
	.uleb128 0
	.4byte	.LASF298
	.byte	0x5
	.uleb128 0
	.4byte	.LASF299
	.byte	0x5
	.uleb128 0
	.4byte	.LASF300
	.byte	0x5
	.uleb128 0
	.4byte	.LASF301
	.byte	0x5
	.uleb128 0
	.4byte	.LASF302
	.byte	0x5
	.uleb128 0
	.4byte	.LASF303
	.byte	0x5
	.uleb128 0
	.4byte	.LASF304
	.byte	0x5
	.uleb128 0
	.4byte	.LASF305
	.byte	0x5
	.uleb128 0
	.4byte	.LASF306
	.byte	0x5
	.uleb128 0
	.4byte	.LASF307
	.byte	0x5
	.uleb128 0
	.4byte	.LASF308
	.byte	0x5
	.uleb128 0
	.4byte	.LASF309
	.byte	0x5
	.uleb128 0
	.4byte	.LASF310
	.byte	0x5
	.uleb128 0
	.4byte	.LASF311
	.byte	0x5
	.uleb128 0
	.4byte	.LASF312
	.byte	0x5
	.uleb128 0
	.4byte	.LASF313
	.byte	0x5
	.uleb128 0
	.4byte	.LASF314
	.byte	0x5
	.uleb128 0
	.4byte	.LASF315
	.byte	0x5
	.uleb128 0
	.4byte	.LASF316
	.byte	0x5
	.uleb128 0
	.4byte	.LASF317
	.byte	0x5
	.uleb128 0
	.4byte	.LASF318
	.byte	0x5
	.uleb128 0
	.4byte	.LASF319
	.byte	0x5
	.uleb128 0
	.4byte	.LASF320
	.byte	0x5
	.uleb128 0
	.4byte	.LASF321
	.byte	0x5
	.uleb128 0
	.4byte	.LASF322
	.byte	0x5
	.uleb128 0
	.4byte	.LASF323
	.byte	0x5
	.uleb128 0
	.4byte	.LASF324
	.byte	0x5
	.uleb128 0
	.4byte	.LASF325
	.byte	0x5
	.uleb128 0
	.4byte	.LASF326
	.byte	0x5
	.uleb128 0
	.4byte	.LASF327
	.byte	0x5
	.uleb128 0
	.4byte	.LASF328
	.byte	0x5
	.uleb128 0
	.4byte	.LASF329
	.byte	0x5
	.uleb128 0
	.4byte	.LASF330
	.byte	0x5
	.uleb128 0
	.4byte	.LASF331
	.byte	0x5
	.uleb128 0
	.4byte	.LASF332
	.byte	0x5
	.uleb128 0
	.4byte	.LASF333
	.byte	0x5
	.uleb128 0
	.4byte	.LASF334
	.byte	0x5
	.uleb128 0
	.4byte	.LASF335
	.byte	0x5
	.uleb128 0
	.4byte	.LASF336
	.byte	0x5
	.uleb128 0
	.4byte	.LASF337
	.byte	0x5
	.uleb128 0
	.4byte	.LASF338
	.byte	0x5
	.uleb128 0
	.4byte	.LASF339
	.byte	0x5
	.uleb128 0
	.4byte	.LASF340
	.byte	0x5
	.uleb128 0
	.4byte	.LASF341
	.byte	0x5
	.uleb128 0
	.4byte	.LASF342
	.byte	0x5
	.uleb128 0
	.4byte	.LASF343
	.byte	0x5
	.uleb128 0
	.4byte	.LASF344
	.byte	0x5
	.uleb128 0
	.4byte	.LASF345
	.byte	0x5
	.uleb128 0
	.4byte	.LASF346
	.byte	0x5
	.uleb128 0
	.4byte	.LASF347
	.byte	0x5
	.uleb128 0
	.4byte	.LASF348
	.byte	0x5
	.uleb128 0
	.4byte	.LASF349
	.byte	0x5
	.uleb128 0
	.4byte	.LASF350
	.byte	0x5
	.uleb128 0
	.4byte	.LASF351
	.byte	0x5
	.uleb128 0
	.4byte	.LASF352
	.byte	0x5
	.uleb128 0
	.4byte	.LASF353
	.byte	0x5
	.uleb128 0
	.4byte	.LASF354
	.byte	0x5
	.uleb128 0
	.4byte	.LASF355
	.byte	0x5
	.uleb128 0
	.4byte	.LASF356
	.byte	0x5
	.uleb128 0
	.4byte	.LASF357
	.byte	0x5
	.uleb128 0
	.4byte	.LASF358
	.byte	0x5
	.uleb128 0
	.4byte	.LASF359
	.byte	0x5
	.uleb128 0
	.4byte	.LASF360
	.byte	0x5
	.uleb128 0
	.4byte	.LASF361
	.byte	0x5
	.uleb128 0
	.4byte	.LASF362
	.byte	0x5
	.uleb128 0
	.4byte	.LASF363
	.byte	0x5
	.uleb128 0
	.4byte	.LASF364
	.byte	0x5
	.uleb128 0
	.4byte	.LASF365
	.byte	0x5
	.uleb128 0
	.4byte	.LASF366
	.byte	0x5
	.uleb128 0
	.4byte	.LASF367
	.byte	0x5
	.uleb128 0
	.4byte	.LASF368
	.byte	0x5
	.uleb128 0
	.4byte	.LASF369
	.byte	0x5
	.uleb128 0
	.4byte	.LASF370
	.byte	0x5
	.uleb128 0
	.4byte	.LASF371
	.byte	0x5
	.uleb128 0
	.4byte	.LASF372
	.byte	0x5
	.uleb128 0
	.4byte	.LASF373
	.byte	0x5
	.uleb128 0
	.4byte	.LASF374
	.byte	0x5
	.uleb128 0
	.4byte	.LASF375
	.byte	0x5
	.uleb128 0
	.4byte	.LASF376
	.byte	0x5
	.uleb128 0
	.4byte	.LASF377
	.byte	0x5
	.uleb128 0
	.4byte	.LASF378
	.byte	0x5
	.uleb128 0
	.4byte	.LASF379
	.byte	0x5
	.uleb128 0
	.4byte	.LASF380
	.byte	0x5
	.uleb128 0
	.4byte	.LASF381
	.byte	0x6
	.uleb128 0
	.4byte	.LASF382
	.byte	0x5
	.uleb128 0
	.4byte	.LASF383
	.byte	0x6
	.uleb128 0
	.4byte	.LASF384
	.byte	0x6
	.uleb128 0
	.4byte	.LASF385
	.byte	0x6
	.uleb128 0
	.4byte	.LASF386
	.byte	0x6
	.uleb128 0
	.4byte	.LASF387
	.byte	0x5
	.uleb128 0
	.4byte	.LASF388
	.byte	0x6
	.uleb128 0
	.4byte	.LASF389
	.byte	0x6
	.uleb128 0
	.4byte	.LASF390
	.byte	0x6
	.uleb128 0
	.4byte	.LASF391
	.byte	0x5
	.uleb128 0
	.4byte	.LASF392
	.byte	0x5
	.uleb128 0
	.4byte	.LASF393
	.byte	0x6
	.uleb128 0
	.4byte	.LASF394
	.byte	0x5
	.uleb128 0
	.4byte	.LASF395
	.byte	0x5
	.uleb128 0
	.4byte	.LASF396
	.byte	0x5
	.uleb128 0
	.4byte	.LASF397
	.byte	0x6
	.uleb128 0
	.4byte	.LASF398
	.byte	0x5
	.uleb128 0
	.4byte	.LASF399
	.byte	0x5
	.uleb128 0
	.4byte	.LASF400
	.byte	0x6
	.uleb128 0
	.4byte	.LASF401
	.byte	0x5
	.uleb128 0
	.4byte	.LASF402
	.byte	0x5
	.uleb128 0
	.4byte	.LASF403
	.byte	0x5
	.uleb128 0
	.4byte	.LASF404
	.byte	0x5
	.uleb128 0
	.4byte	.LASF405
	.byte	0x5
	.uleb128 0
	.4byte	.LASF406
	.byte	0x5
	.uleb128 0
	.4byte	.LASF407
	.byte	0x6
	.uleb128 0
	.4byte	.LASF408
	.byte	0x5
	.uleb128 0
	.4byte	.LASF409
	.byte	0x5
	.uleb128 0
	.4byte	.LASF410
	.byte	0x5
	.uleb128 0
	.4byte	.LASF411
	.byte	0x6
	.uleb128 0
	.4byte	.LASF412
	.byte	0x5
	.uleb128 0
	.4byte	.LASF413
	.byte	0x6
	.uleb128 0
	.4byte	.LASF414
	.byte	0x6
	.uleb128 0
	.4byte	.LASF415
	.byte	0x6
	.uleb128 0
	.4byte	.LASF416
	.byte	0x6
	.uleb128 0
	.4byte	.LASF417
	.byte	0x6
	.uleb128 0
	.4byte	.LASF418
	.byte	0x6
	.uleb128 0
	.4byte	.LASF419
	.byte	0x5
	.uleb128 0
	.4byte	.LASF420
	.byte	0x6
	.uleb128 0
	.4byte	.LASF421
	.byte	0x6
	.uleb128 0
	.4byte	.LASF422
	.byte	0x6
	.uleb128 0
	.4byte	.LASF423
	.byte	0x5
	.uleb128 0
	.4byte	.LASF424
	.byte	0x5
	.uleb128 0
	.4byte	.LASF425
	.byte	0x5
	.uleb128 0
	.4byte	.LASF426
	.byte	0x5
	.uleb128 0
	.4byte	.LASF427
	.byte	0x6
	.uleb128 0
	.4byte	.LASF428
	.byte	0x5
	.uleb128 0
	.4byte	.LASF429
	.byte	0x5
	.uleb128 0
	.4byte	.LASF430
	.byte	0x5
	.uleb128 0
	.4byte	.LASF431
	.byte	0x6
	.uleb128 0
	.4byte	.LASF432
	.byte	0x5
	.uleb128 0
	.4byte	.LASF433
	.byte	0x6
	.uleb128 0
	.4byte	.LASF434
	.byte	0x6
	.uleb128 0
	.4byte	.LASF435
	.byte	0x6
	.uleb128 0
	.4byte	.LASF436
	.byte	0x6
	.uleb128 0
	.4byte	.LASF437
	.byte	0x6
	.uleb128 0
	.4byte	.LASF438
	.byte	0x6
	.uleb128 0
	.4byte	.LASF439
	.byte	0x5
	.uleb128 0
	.4byte	.LASF440
	.byte	0x5
	.uleb128 0
	.4byte	.LASF441
	.byte	0x5
	.uleb128 0
	.4byte	.LASF442
	.byte	0x5
	.uleb128 0
	.4byte	.LASF425
	.byte	0x5
	.uleb128 0
	.4byte	.LASF443
	.byte	0x5
	.uleb128 0
	.4byte	.LASF444
	.byte	0x5
	.uleb128 0
	.4byte	.LASF445
	.byte	0x5
	.uleb128 0
	.4byte	.LASF446
	.byte	0x5
	.uleb128 0
	.4byte	.LASF447
	.byte	0x5
	.uleb128 0
	.4byte	.LASF448
	.byte	0x5
	.uleb128 0
	.4byte	.LASF449
	.byte	0x5
	.uleb128 0
	.4byte	.LASF450
	.byte	0x5
	.uleb128 0
	.4byte	.LASF451
	.byte	0x5
	.uleb128 0
	.4byte	.LASF452
	.byte	0x5
	.uleb128 0
	.4byte	.LASF453
	.byte	0x5
	.uleb128 0
	.4byte	.LASF454
	.byte	0x5
	.uleb128 0
	.4byte	.LASF455
	.byte	0x5
	.uleb128 0
	.4byte	.LASF456
	.byte	0x5
	.uleb128 0
	.4byte	.LASF457
	.byte	0x5
	.uleb128 0
	.4byte	.LASF458
	.byte	0x5
	.uleb128 0
	.4byte	.LASF459
	.byte	0x5
	.uleb128 0
	.4byte	.LASF460
	.byte	0x5
	.uleb128 0
	.4byte	.LASF461
	.byte	0x5
	.uleb128 0
	.4byte	.LASF462
	.byte	0x5
	.uleb128 0
	.4byte	.LASF463
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.pb.h.6.613a7b6671c29fec275a11de5b2d5a48,comdat
.Ldebug_macro3:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x6
	.4byte	.LASF465
	.byte	0x5
	.uleb128 0x12
	.4byte	.LASF466
	.byte	0x5
	.uleb128 0x19
	.4byte	.LASF467
	.byte	0x5
	.uleb128 0x1f
	.4byte	.LASF468
	.byte	0x5
	.uleb128 0x31
	.4byte	.LASF469
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdint.h.39.fe42d6eb18d369206696c6985313e641,comdat
.Ldebug_macro4:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF470
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF471
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF472
	.byte	0x5
	.uleb128 0x7c
	.4byte	.LASF473
	.byte	0x5
	.uleb128 0x7e
	.4byte	.LASF474
	.byte	0x5
	.uleb128 0x80
	.4byte	.LASF475
	.byte	0x5
	.uleb128 0x81
	.4byte	.LASF476
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF477
	.byte	0x5
	.uleb128 0x84
	.4byte	.LASF478
	.byte	0x5
	.uleb128 0x85
	.4byte	.LASF479
	.byte	0x5
	.uleb128 0x87
	.4byte	.LASF480
	.byte	0x5
	.uleb128 0x88
	.4byte	.LASF481
	.byte	0x5
	.uleb128 0x89
	.4byte	.LASF482
	.byte	0x5
	.uleb128 0x8b
	.4byte	.LASF483
	.byte	0x5
	.uleb128 0x8c
	.4byte	.LASF484
	.byte	0x5
	.uleb128 0x8d
	.4byte	.LASF485
	.byte	0x5
	.uleb128 0x90
	.4byte	.LASF486
	.byte	0x5
	.uleb128 0x91
	.4byte	.LASF487
	.byte	0x5
	.uleb128 0x92
	.4byte	.LASF488
	.byte	0x5
	.uleb128 0x93
	.4byte	.LASF489
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF490
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF491
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF492
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF493
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF494
	.byte	0x5
	.uleb128 0x99
	.4byte	.LASF495
	.byte	0x5
	.uleb128 0x9a
	.4byte	.LASF496
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF497
	.byte	0x5
	.uleb128 0x9d
	.4byte	.LASF498
	.byte	0x5
	.uleb128 0x9e
	.4byte	.LASF499
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF500
	.byte	0x5
	.uleb128 0xa0
	.4byte	.LASF501
	.byte	0x5
	.uleb128 0xa1
	.4byte	.LASF502
	.byte	0x5
	.uleb128 0xa2
	.4byte	.LASF503
	.byte	0x5
	.uleb128 0xa3
	.4byte	.LASF504
	.byte	0x5
	.uleb128 0xa4
	.4byte	.LASF505
	.byte	0x5
	.uleb128 0xa5
	.4byte	.LASF506
	.byte	0x5
	.uleb128 0xa6
	.4byte	.LASF507
	.byte	0x5
	.uleb128 0xa7
	.4byte	.LASF508
	.byte	0x5
	.uleb128 0xa8
	.4byte	.LASF509
	.byte	0x5
	.uleb128 0xad
	.4byte	.LASF510
	.byte	0x5
	.uleb128 0xae
	.4byte	.LASF511
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF512
	.byte	0x5
	.uleb128 0xb1
	.4byte	.LASF513
	.byte	0x5
	.uleb128 0xb2
	.4byte	.LASF514
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF515
	.byte	0x5
	.uleb128 0xc3
	.4byte	.LASF516
	.byte	0x5
	.uleb128 0xc4
	.4byte	.LASF517
	.byte	0x5
	.uleb128 0xc5
	.4byte	.LASF518
	.byte	0x5
	.uleb128 0xc6
	.4byte	.LASF519
	.byte	0x5
	.uleb128 0xc7
	.4byte	.LASF520
	.byte	0x5
	.uleb128 0xc8
	.4byte	.LASF521
	.byte	0x5
	.uleb128 0xc9
	.4byte	.LASF522
	.byte	0x5
	.uleb128 0xca
	.4byte	.LASF523
	.byte	0x5
	.uleb128 0xcc
	.4byte	.LASF524
	.byte	0x5
	.uleb128 0xcd
	.4byte	.LASF525
	.byte	0x5
	.uleb128 0xd7
	.4byte	.LASF526
	.byte	0x5
	.uleb128 0xd8
	.4byte	.LASF527
	.byte	0x5
	.uleb128 0xe3
	.4byte	.LASF528
	.byte	0x5
	.uleb128 0xe4
	.4byte	.LASF529
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.__crossworks.h.39.ff21eb83ebfc80fb95245a821dd1e413,comdat
.Ldebug_macro5:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF531
	.byte	0x5
	.uleb128 0x3b
	.4byte	.LASF532
	.byte	0x6
	.uleb128 0x3d
	.4byte	.LASF533
	.byte	0x5
	.uleb128 0x3f
	.4byte	.LASF534
	.byte	0x5
	.uleb128 0x43
	.4byte	.LASF535
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF536
	.byte	0x5
	.uleb128 0x56
	.4byte	.LASF537
	.byte	0x5
	.uleb128 0x5d
	.4byte	.LASF532
	.byte	0x5
	.uleb128 0x63
	.4byte	.LASF538
	.byte	0x5
	.uleb128 0x64
	.4byte	.LASF539
	.byte	0x5
	.uleb128 0x65
	.4byte	.LASF540
	.byte	0x5
	.uleb128 0x66
	.4byte	.LASF541
	.byte	0x5
	.uleb128 0x67
	.4byte	.LASF542
	.byte	0x5
	.uleb128 0x68
	.4byte	.LASF543
	.byte	0x5
	.uleb128 0x69
	.4byte	.LASF544
	.byte	0x5
	.uleb128 0x6a
	.4byte	.LASF545
	.byte	0x5
	.uleb128 0x6d
	.4byte	.LASF546
	.byte	0x5
	.uleb128 0x6e
	.4byte	.LASF547
	.byte	0x5
	.uleb128 0x6f
	.4byte	.LASF548
	.byte	0x5
	.uleb128 0x70
	.4byte	.LASF549
	.byte	0x5
	.uleb128 0x73
	.4byte	.LASF550
	.byte	0x5
	.uleb128 0xd8
	.4byte	.LASF551
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stddef.h.44.3483ea4b5d43bc7237f8a88f13989923,comdat
.Ldebug_macro6:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x2c
	.4byte	.LASF552
	.byte	0x5
	.uleb128 0x40
	.4byte	.LASF553
	.byte	0x5
	.uleb128 0x45
	.4byte	.LASF554
	.byte	0x5
	.uleb128 0x4c
	.4byte	.LASF555
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.stdbool.h.39.3758cb47b714dfcbf7837a03b10a6ad6,comdat
.Ldebug_macro7:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x27
	.4byte	.LASF556
	.byte	0x5
	.uleb128 0x2b
	.4byte	.LASF557
	.byte	0x5
	.uleb128 0x2f
	.4byte	.LASF558
	.byte	0x5
	.uleb128 0x30
	.4byte	.LASF559
	.byte	0x5
	.uleb128 0x32
	.4byte	.LASF560
	.byte	0
	.section	.debug_macro,"G",%progbits,wm4.pb.h.80.bea31d1a99bf85e0dd1f928ccaaf7703,comdat
.Ldebug_macro8:
	.2byte	0x4
	.byte	0
	.byte	0x5
	.uleb128 0x50
	.4byte	.LASF562
	.byte	0x5
	.uleb128 0x51
	.4byte	.LASF563
	.byte	0x5
	.uleb128 0x52
	.4byte	.LASF564
	.byte	0x5
	.uleb128 0x6b
	.4byte	.LASF565
	.byte	0x5
	.uleb128 0x79
	.4byte	.LASF566
	.byte	0x5
	.uleb128 0x7a
	.4byte	.LASF567
	.byte	0x5
	.uleb128 0x7b
	.4byte	.LASF568
	.byte	0x5
	.uleb128 0x83
	.4byte	.LASF569
	.byte	0x5
	.uleb128 0x94
	.4byte	.LASF570
	.byte	0x5
	.uleb128 0x95
	.4byte	.LASF571
	.byte	0x5
	.uleb128 0x96
	.4byte	.LASF572
	.byte	0x5
	.uleb128 0x97
	.4byte	.LASF573
	.byte	0x5
	.uleb128 0x98
	.4byte	.LASF574
	.byte	0x5
	.uleb128 0x9b
	.4byte	.LASF575
	.byte	0x5
	.uleb128 0x9f
	.4byte	.LASF576
	.byte	0x5
	.uleb128 0xa3
	.4byte	.LASF577
	.byte	0x5
	.uleb128 0xa7
	.4byte	.LASF578
	.byte	0x5
	.uleb128 0xab
	.4byte	.LASF579
	.byte	0x5
	.uleb128 0xae
	.4byte	.LASF580
	.byte	0x5
	.uleb128 0xaf
	.4byte	.LASF581
	.byte	0x5
	.uleb128 0xb3
	.4byte	.LASF582
	.byte	0x5
	.uleb128 0xb4
	.4byte	.LASF583
	.byte	0x5
	.uleb128 0xb5
	.4byte	.LASF584
	.byte	0x5
	.uleb128 0xb6
	.4byte	.LASF585
	.byte	0x5
	.uleb128 0xb7
	.4byte	.LASF586
	.byte	0x5
	.uleb128 0xbb
	.4byte	.LASF587
	.byte	0x5
	.uleb128 0xbc
	.4byte	.LASF588
	.byte	0x5
	.uleb128 0xbd
	.4byte	.LASF589
	.byte	0x5
	.uleb128 0xbe
	.4byte	.LASF590
	.byte	0x5
	.uleb128 0xc0
	.4byte	.LASF591
	.byte	0x5
	.uleb128 0xc1
	.4byte	.LASF592
	.byte	0x5
	.uleb128 0xc2
	.4byte	.LASF593
	.byte	0x5
	.uleb128 0xd1
	.4byte	.LASF594
	.byte	0x5
	.uleb128 0xff
	.4byte	.LASF595
	.byte	0x5
	.uleb128 0x100
	.4byte	.LASF596
	.byte	0x5
	.uleb128 0x175
	.4byte	.LASF597
	.byte	0x5
	.uleb128 0x179
	.4byte	.LASF598
	.byte	0x5
	.uleb128 0x17b
	.4byte	.LASF599
	.byte	0x5
	.uleb128 0x17d
	.4byte	.LASF600
	.byte	0x5
	.uleb128 0x17f
	.4byte	.LASF601
	.byte	0x5
	.uleb128 0x183
	.4byte	.LASF602
	.byte	0x5
	.uleb128 0x185
	.4byte	.LASF603
	.byte	0x5
	.uleb128 0x187
	.4byte	.LASF604
	.byte	0x5
	.uleb128 0x18f
	.4byte	.LASF605
	.byte	0x5
	.uleb128 0x194
	.4byte	.LASF606
	.byte	0x5
	.uleb128 0x19b
	.4byte	.LASF607
	.byte	0x5
	.uleb128 0x1a3
	.4byte	.LASF608
	.byte	0x5
	.uleb128 0x1a8
	.4byte	.LASF609
	.byte	0x5
	.uleb128 0x1ad
	.4byte	.LASF610
	.byte	0x5
	.uleb128 0x1b3
	.4byte	.LASF611
	.byte	0x5
	.uleb128 0x1b7
	.4byte	.LASF612
	.byte	0x5
	.uleb128 0x1bb
	.4byte	.LASF613
	.byte	0x5
	.uleb128 0x1c0
	.4byte	.LASF614
	.byte	0x5
	.uleb128 0x1c6
	.4byte	.LASF615
	.byte	0x5
	.uleb128 0x1c9
	.4byte	.LASF616
	.byte	0x5
	.uleb128 0x1cd
	.4byte	.LASF617
	.byte	0x5
	.uleb128 0x1ce
	.4byte	.LASF618
	.byte	0x5
	.uleb128 0x1cf
	.4byte	.LASF619
	.byte	0x5
	.uleb128 0x1d0
	.4byte	.LASF620
	.byte	0x5
	.uleb128 0x1d1
	.4byte	.LASF621
	.byte	0x5
	.uleb128 0x1d2
	.4byte	.LASF622
	.byte	0x5
	.uleb128 0x1d3
	.4byte	.LASF623
	.byte	0x5
	.uleb128 0x1d4
	.4byte	.LASF624
	.byte	0x5
	.uleb128 0x1d5
	.4byte	.LASF625
	.byte	0x5
	.uleb128 0x1d6
	.4byte	.LASF626
	.byte	0x5
	.uleb128 0x1d7
	.4byte	.LASF627
	.byte	0x5
	.uleb128 0x1d8
	.4byte	.LASF628
	.byte	0x5
	.uleb128 0x1d9
	.4byte	.LASF629
	.byte	0x5
	.uleb128 0x1da
	.4byte	.LASF630
	.byte	0x5
	.uleb128 0x1db
	.4byte	.LASF631
	.byte	0x5
	.uleb128 0x1dc
	.4byte	.LASF632
	.byte	0x5
	.uleb128 0x1dd
	.4byte	.LASF633
	.byte	0x5
	.uleb128 0x1de
	.4byte	.LASF634
	.byte	0x5
	.uleb128 0x1df
	.4byte	.LASF635
	.byte	0x5
	.uleb128 0x1f0
	.4byte	.LASF636
	.byte	0x5
	.uleb128 0x1f8
	.4byte	.LASF637
	.byte	0x5
	.uleb128 0x1fd
	.4byte	.LASF638
	.byte	0x5
	.uleb128 0x202
	.4byte	.LASF639
	.byte	0x5
	.uleb128 0x207
	.4byte	.LASF640
	.byte	0x5
	.uleb128 0x20c
	.4byte	.LASF641
	.byte	0x5
	.uleb128 0x211
	.4byte	.LASF642
	.byte	0x5
	.uleb128 0x223
	.4byte	.LASF643
	.byte	0x5
	.uleb128 0x224
	.4byte	.LASF644
	.byte	0x5
	.uleb128 0x22a
	.4byte	.LASF645
	.byte	0
	.section	.debug_line,"",%progbits
.Ldebug_line0:
	.section	.debug_str,"MS",%progbits,1
.LASF674:
	.ascii	"pb_field_s\000"
.LASF667:
	.ascii	"pb_field_t\000"
.LASF171:
	.ascii	"__DBL_NORM_MAX__ ((double)1.1)\000"
.LASF95:
	.ascii	"__SIG_ATOMIC_MAX__ 0x7fffffff\000"
.LASF224:
	.ascii	"__FLT64_HAS_QUIET_NAN__ 1\000"
.LASF256:
	.ascii	"__UFRACT_IBIT__ 0\000"
.LASF207:
	.ascii	"__FLT32_HAS_INFINITY__ 1\000"
.LASF483:
	.ascii	"INTMAX_MIN (-9223372036854775807LL-1)\000"
.LASF63:
	.ascii	"__UINT_FAST8_TYPE__ unsigned int\000"
.LASF214:
	.ascii	"__FLT64_MAX_EXP__ 1024\000"
.LASF587:
	.ascii	"PB_ATYPE_STATIC 0x00\000"
.LASF624:
	.ascii	"PB_LTYPE_MAP_FLOAT PB_LTYPE_FIXED32\000"
.LASF406:
	.ascii	"__thumb2__ 1\000"
.LASF229:
	.ascii	"__FLT32X_MAX_EXP__ 1024\000"
.LASF129:
	.ascii	"__INT_FAST16_WIDTH__ 32\000"
.LASF65:
	.ascii	"__UINT_FAST32_TYPE__ unsigned int\000"
.LASF518:
	.ascii	"INT16_C(x) (x)\000"
.LASF454:
	.ascii	"FLOAT_ABI_HARD 1\000"
.LASF309:
	.ascii	"__ULACCUM_EPSILON__ 0x1P-32ULK\000"
.LASF550:
	.ascii	"__RAL_WCHAR_T __WCHAR_TYPE__\000"
.LASF201:
	.ascii	"__FLT32_MAX__ 1.1\000"
.LASF194:
	.ascii	"__FLT32_MANT_DIG__ 24\000"
.LASF661:
	.ascii	"char\000"
.LASF564:
	.ascii	"pb_packed \000"
.LASF119:
	.ascii	"__UINT8_C(c) c\000"
.LASF421:
	.ascii	"__ARM_NEON__\000"
.LASF377:
	.ascii	"__SIZEOF_WINT_T__ 4\000"
.LASF461:
	.ascii	"S140 1\000"
.LASF321:
	.ascii	"__QQ_IBIT__ 0\000"
.LASF337:
	.ascii	"__UDQ_IBIT__ 0\000"
.LASF584:
	.ascii	"PB_HTYPE_REPEATED 0x20\000"
.LASF248:
	.ascii	"__USFRACT_MAX__ 0XFFP-8UHR\000"
.LASF231:
	.ascii	"__FLT32X_DECIMAL_DIG__ 17\000"
.LASF165:
	.ascii	"__DBL_MIN_EXP__ (-1021)\000"
.LASF85:
	.ascii	"__LONG_LONG_WIDTH__ 64\000"
.LASF365:
	.ascii	"__GCC_ATOMIC_CHAR16_T_LOCK_FREE 2\000"
.LASF435:
	.ascii	"__ARM_FEATURE_CDE_COPROC\000"
.LASF397:
	.ascii	"__ARM_SIZEOF_WCHAR_T 4\000"
.LASF414:
	.ascii	"__ARM_FP16_FORMAT_IEEE\000"
.LASF583:
	.ascii	"PB_HTYPE_OPTIONAL 0x10\000"
.LASF387:
	.ascii	"__ARM_FEATURE_COMPLEX\000"
.LASF271:
	.ascii	"__LLFRACT_IBIT__ 0\000"
.LASF567:
	.ascii	"PB_STATIC_ASSERT_MSG(MSG,LINE,COUNTER) PB_STATIC_AS"
	.ascii	"SERT_MSG_(MSG, LINE, COUNTER)\000"
.LASF573:
	.ascii	"PB_LTYPE_FIXED32 0x03\000"
.LASF644:
	.ascii	"PB_GET_ERROR(stream) \"(errmsg disabled)\"\000"
.LASF247:
	.ascii	"__USFRACT_MIN__ 0.0UHR\000"
.LASF560:
	.ascii	"__bool_true_false_are_defined 1\000"
.LASF6:
	.ascii	"__GNUC_MINOR__ 2\000"
.LASF388:
	.ascii	"__ARM_32BIT_STATE 1\000"
.LASF505:
	.ascii	"INT_FAST64_MAX INT64_MAX\000"
.LASF430:
	.ascii	"__ARM_FEATURE_IDIV 1\000"
.LASF431:
	.ascii	"__ARM_ASM_SYNTAX_UNIFIED__ 1\000"
.LASF496:
	.ascii	"UINT_LEAST32_MAX UINT32_MAX\000"
.LASF654:
	.ascii	"unsigned int\000"
.LASF689:
	.ascii	"/home/orlo/beep-base-firmware/Source_V1.5.12/Code/n"
	.ascii	"RF/BTLDR/ses\000"
.LASF382:
	.ascii	"__ARM_FEATURE_CRYPTO\000"
.LASF121:
	.ascii	"__UINT16_C(c) c\000"
.LASF25:
	.ascii	"__SIZEOF_SIZE_T__ 4\000"
.LASF40:
	.ascii	"__CHAR16_TYPE__ short unsigned int\000"
.LASF586:
	.ascii	"PB_HTYPE_MASK 0x30\000"
.LASF24:
	.ascii	"__SIZEOF_LONG_DOUBLE__ 8\000"
.LASF626:
	.ascii	"PB_LTYPE_MAP_INT64 PB_LTYPE_VARINT\000"
.LASF154:
	.ascii	"__FLT_MAX__ 1.1\000"
.LASF126:
	.ascii	"__INT_FAST8_MAX__ 0x7fffffff\000"
.LASF29:
	.ascii	"__ORDER_BIG_ENDIAN__ 4321\000"
.LASF278:
	.ascii	"__ULLFRACT_MAX__ 0XFFFFFFFFFFFFFFFFP-64ULLR\000"
.LASF551:
	.ascii	"__MAX_CATEGORY 5\000"
.LASF379:
	.ascii	"__ARM_FEATURE_DSP 1\000"
.LASF638:
	.ascii	"PB_ONEOF_POINTER(u,tag,st,m,fd,ltype,ptr) {tag, PB_"
	.ascii	"ATYPE_POINTER | PB_HTYPE_ONEOF | ltype, fd, pb_delt"
	.ascii	"a(st, which_ ## u, u.m), pb_membersize(st, u.m[0]),"
	.ascii	" 0, ptr}\000"
.LASF354:
	.ascii	"__UTA_FBIT__ 64\000"
.LASF163:
	.ascii	"__DBL_MANT_DIG__ 53\000"
.LASF58:
	.ascii	"__UINT_LEAST64_TYPE__ long long unsigned int\000"
.LASF235:
	.ascii	"__FLT32X_EPSILON__ 1.1\000"
.LASF72:
	.ascii	"__INT_MAX__ 0x7fffffff\000"
.LASF12:
	.ascii	"__ATOMIC_RELEASE 3\000"
.LASF35:
	.ascii	"__PTRDIFF_TYPE__ int\000"
.LASF359:
	.ascii	"__CHAR_UNSIGNED__ 1\000"
.LASF44:
	.ascii	"__INT16_TYPE__ short int\000"
.LASF585:
	.ascii	"PB_HTYPE_ONEOF 0x30\000"
.LASF384:
	.ascii	"__ARM_FEATURE_QRDMX\000"
.LASF179:
	.ascii	"__LDBL_DIG__ 15\000"
.LASF234:
	.ascii	"__FLT32X_MIN__ 1.1\000"
.LASF494:
	.ascii	"UINT_LEAST8_MAX UINT8_MAX\000"
.LASF311:
	.ascii	"__LLACCUM_IBIT__ 32\000"
.LASF10:
	.ascii	"__ATOMIC_SEQ_CST 5\000"
.LASF456:
	.ascii	"NO_VTOR_CONFIG 1\000"
.LASF21:
	.ascii	"__SIZEOF_SHORT__ 2\000"
.LASF612:
	.ascii	"PB_OPTIONAL_CALLBACK(tag,st,m,fd,ltype,ptr) {tag, P"
	.ascii	"B_ATYPE_CALLBACK | PB_HTYPE_OPTIONAL | ltype, fd, 0"
	.ascii	", pb_membersize(st, m), 0, ptr}\000"
.LASF189:
	.ascii	"__LDBL_EPSILON__ 1.1\000"
.LASF681:
	.ascii	"pb_field_iter_t\000"
.LASF210:
	.ascii	"__FLT64_MANT_DIG__ 53\000"
.LASF473:
	.ascii	"INT8_MIN (-128)\000"
.LASF93:
	.ascii	"__UINTMAX_C(c) c ## ULL\000"
.LASF375:
	.ascii	"__PRAGMA_REDEFINE_EXTNAME 1\000"
.LASF465:
	.ascii	"PB_H_INCLUDED \000"
.LASF31:
	.ascii	"__BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF684:
	.ascii	"pb_field_iter_next\000"
.LASF197:
	.ascii	"__FLT32_MIN_10_EXP__ (-37)\000"
.LASF175:
	.ascii	"__DBL_HAS_DENORM__ 1\000"
.LASF261:
	.ascii	"__LFRACT_IBIT__ 0\000"
.LASF549:
	.ascii	"__CTYPE_PRINT (__CTYPE_BLANK | __CTYPE_PUNCT | __CT"
	.ascii	"YPE_UPPER | __CTYPE_LOWER | __CTYPE_DIGIT)\000"
.LASF499:
	.ascii	"INT_FAST16_MIN INT32_MIN\000"
.LASF402:
	.ascii	"__ARM_ARCH 7\000"
.LASF3:
	.ascii	"__STDC_UTF_32__ 1\000"
.LASF109:
	.ascii	"__INT_LEAST16_MAX__ 0x7fff\000"
.LASF457:
	.ascii	"NRF52840_XXAA 1\000"
.LASF70:
	.ascii	"__SCHAR_MAX__ 0x7f\000"
.LASF316:
	.ascii	"__ULLACCUM_IBIT__ 32\000"
.LASF394:
	.ascii	"__ARM_FEATURE_NUMERIC_MAXMIN\000"
.LASF276:
	.ascii	"__ULLFRACT_IBIT__ 0\000"
.LASF211:
	.ascii	"__FLT64_DIG__ 15\000"
.LASF492:
	.ascii	"INT_LEAST32_MAX INT32_MAX\000"
.LASF9:
	.ascii	"__ATOMIC_RELAXED 0\000"
.LASF554:
	.ascii	"__RAL_WCHAR_T_DEFINED \000"
.LASF419:
	.ascii	"__ARM_FEATURE_FP16_FML\000"
.LASF80:
	.ascii	"__SIZE_MAX__ 0xffffffffU\000"
.LASF566:
	.ascii	"PB_STATIC_ASSERT(COND,MSG) typedef char PB_STATIC_A"
	.ascii	"SSERT_MSG(MSG, __LINE__, __COUNTER__)[(COND)?1:-1];"
	.ascii	"\000"
.LASF302:
	.ascii	"__LACCUM_MIN__ (-0X1P31LK-0X1P31LK)\000"
.LASF318:
	.ascii	"__ULLACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULLK\000"
.LASF307:
	.ascii	"__ULACCUM_MIN__ 0.0ULK\000"
.LASF413:
	.ascii	"__ARM_FP 4\000"
.LASF460:
	.ascii	"NRF_SD_BLE_API_VERSION 6\000"
.LASF160:
	.ascii	"__FLT_HAS_INFINITY__ 1\000"
.LASF609:
	.ascii	"PB_OPTIONAL_POINTER(tag,st,m,fd,ltype,ptr) {tag, PB"
	.ascii	"_ATYPE_POINTER | PB_HTYPE_OPTIONAL | ltype, fd, 0, "
	.ascii	"pb_membersize(st, m[0]), 0, ptr}\000"
.LASF532:
	.ascii	"__THREAD __thread\000"
.LASF410:
	.ascii	"__ARMEL__ 1\000"
.LASF603:
	.ascii	"PB_DATAOFFSET_OTHER(st,m1,m2) (offsetof(st, m1) - o"
	.ascii	"ffsetof(st, m2) - pb_membersize(st, m2))\000"
.LASF353:
	.ascii	"__UDA_IBIT__ 32\000"
.LASF504:
	.ascii	"INT_FAST32_MAX INT32_MAX\000"
.LASF62:
	.ascii	"__INT_FAST64_TYPE__ long long int\000"
.LASF107:
	.ascii	"__INT8_C(c) c\000"
.LASF209:
	.ascii	"__FP_FAST_FMAF32 1\000"
.LASF646:
	.ascii	"signed char\000"
.LASF428:
	.ascii	"__FDPIC__\000"
.LASF236:
	.ascii	"__FLT32X_DENORM_MIN__ 1.1\000"
.LASF111:
	.ascii	"__INT_LEAST16_WIDTH__ 16\000"
.LASF508:
	.ascii	"UINT_FAST32_MAX UINT32_MAX\000"
.LASF317:
	.ascii	"__ULLACCUM_MIN__ 0.0ULLK\000"
.LASF653:
	.ascii	"uint32_t\000"
.LASF386:
	.ascii	"__ARM_FEATURE_DOTPROD\000"
.LASF422:
	.ascii	"__ARM_NEON\000"
.LASF557:
	.ascii	"bool _Bool\000"
.LASF79:
	.ascii	"__PTRDIFF_MAX__ 0x7fffffff\000"
.LASF390:
	.ascii	"__ARM_FEATURE_CMSE\000"
.LASF68:
	.ascii	"__UINTPTR_TYPE__ unsigned int\000"
.LASF157:
	.ascii	"__FLT_EPSILON__ 1.1\000"
.LASF206:
	.ascii	"__FLT32_HAS_DENORM__ 1\000"
.LASF533:
	.ascii	"__RAL_SIZE_T\000"
.LASF360:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_1 1\000"
.LASF520:
	.ascii	"INT32_C(x) (x ##L)\000"
.LASF506:
	.ascii	"UINT_FAST8_MAX UINT8_MAX\000"
.LASF343:
	.ascii	"__SA_IBIT__ 16\000"
.LASF273:
	.ascii	"__LLFRACT_MAX__ 0X7FFFFFFFFFFFFFFFP-63LLR\000"
.LASF132:
	.ascii	"__INT_FAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF162:
	.ascii	"__FP_FAST_FMAF 1\000"
.LASF148:
	.ascii	"__FLT_DIG__ 6\000"
.LASF117:
	.ascii	"__INT_LEAST64_WIDTH__ 64\000"
.LASF659:
	.ascii	"uint_least16_t\000"
.LASF361:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_2 1\000"
.LASF479:
	.ascii	"INT32_MIN (-2147483647L-1)\000"
.LASF344:
	.ascii	"__DA_FBIT__ 31\000"
.LASF396:
	.ascii	"__ARM_SIZEOF_MINIMAL_ENUM 1\000"
.LASF617:
	.ascii	"PB_LTYPE_MAP_BOOL PB_LTYPE_VARINT\000"
.LASF526:
	.ascii	"WCHAR_MIN __WCHAR_MIN__\000"
.LASF141:
	.ascii	"__GCC_IEC_559 0\000"
.LASF656:
	.ascii	"long long unsigned int\000"
.LASF625:
	.ascii	"PB_LTYPE_MAP_INT32 PB_LTYPE_VARINT\000"
.LASF289:
	.ascii	"__USACCUM_EPSILON__ 0x1P-8UHK\000"
.LASF649:
	.ascii	"int16_t\000"
.LASF208:
	.ascii	"__FLT32_HAS_QUIET_NAN__ 1\000"
.LASF308:
	.ascii	"__ULACCUM_MAX__ 0XFFFFFFFFFFFFFFFFP-32ULK\000"
.LASF258:
	.ascii	"__UFRACT_MAX__ 0XFFFFP-16UR\000"
.LASF243:
	.ascii	"__SFRACT_MAX__ 0X7FP-7HR\000"
.LASF568:
	.ascii	"PB_STATIC_ASSERT_MSG_(MSG,LINE,COUNTER) pb_static_a"
	.ascii	"ssertion_ ##MSG ##LINE ##COUNTER\000"
.LASF427:
	.ascii	"__ARM_EABI__ 1\000"
.LASF118:
	.ascii	"__UINT_LEAST8_MAX__ 0xff\000"
.LASF101:
	.ascii	"__INT64_MAX__ 0x7fffffffffffffffLL\000"
.LASF468:
	.ascii	"PB_NO_ERRMSG 1\000"
.LASF98:
	.ascii	"__INT8_MAX__ 0x7f\000"
.LASF269:
	.ascii	"__ULFRACT_EPSILON__ 0x1P-32ULR\000"
.LASF53:
	.ascii	"__INT_LEAST32_TYPE__ long int\000"
.LASF542:
	.ascii	"__CTYPE_PUNCT 0x10\000"
.LASF266:
	.ascii	"__ULFRACT_IBIT__ 0\000"
.LASF310:
	.ascii	"__LLACCUM_FBIT__ 31\000"
.LASF124:
	.ascii	"__UINT_LEAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF146:
	.ascii	"__FLT_RADIX__ 2\000"
.LASF670:
	.ascii	"size_offset\000"
.LASF277:
	.ascii	"__ULLFRACT_MIN__ 0.0ULLR\000"
.LASF177:
	.ascii	"__DBL_HAS_QUIET_NAN__ 1\000"
.LASF297:
	.ascii	"__UACCUM_MIN__ 0.0UK\000"
.LASF453:
	.ascii	"CONFIG_GPIO_AS_PINRESET 1\000"
.LASF264:
	.ascii	"__LFRACT_EPSILON__ 0x1P-31LR\000"
.LASF565:
	.ascii	"PB_UNUSED(x) (void)(x)\000"
.LASF59:
	.ascii	"__INT_FAST8_TYPE__ int\000"
.LASF223:
	.ascii	"__FLT64_HAS_INFINITY__ 1\000"
.LASF444:
	.ascii	"__ARM_ARCH_FPV4_SP_D16__ 1\000"
.LASF205:
	.ascii	"__FLT32_DENORM_MIN__ 1.1\000"
.LASF417:
	.ascii	"__ARM_FEATURE_FP16_SCALAR_ARITHMETIC\000"
.LASF538:
	.ascii	"__CTYPE_UPPER 0x01\000"
.LASF280:
	.ascii	"__SACCUM_FBIT__ 7\000"
.LASF36:
	.ascii	"__WCHAR_TYPE__ unsigned int\000"
.LASF203:
	.ascii	"__FLT32_MIN__ 1.1\000"
.LASF399:
	.ascii	"__ARM_ARCH_PROFILE 77\000"
.LASF442:
	.ascii	"__SIZEOF_WCHAR_T 4\000"
.LASF252:
	.ascii	"__FRACT_MIN__ (-0.5R-0.5R)\000"
.LASF535:
	.ascii	"__RAL_SIZE_MAX 4294967295UL\000"
.LASF369:
	.ascii	"__GCC_ATOMIC_INT_LOCK_FREE 2\000"
.LASF90:
	.ascii	"__INTMAX_MAX__ 0x7fffffffffffffffLL\000"
.LASF527:
	.ascii	"WCHAR_MAX __WCHAR_MAX__\000"
.LASF299:
	.ascii	"__UACCUM_EPSILON__ 0x1P-16UK\000"
.LASF478:
	.ascii	"INT32_MAX 2147483647L\000"
.LASF604:
	.ascii	"PB_DATAOFFSET_CHOOSE(st,m1,m2) (int)(offsetof(st, m"
	.ascii	"1) == offsetof(st, m2) ? PB_DATAOFFSET_FIRST(st, m1"
	.ascii	", m2) : PB_DATAOFFSET_OTHER(st, m1, m2))\000"
.LASF291:
	.ascii	"__ACCUM_IBIT__ 16\000"
.LASF642:
	.ascii	"PB_ANONYMOUS_ONEOF_FIELD(union_name,tag,type,rules,"
	.ascii	"allocation,placement,message,field,prevfield,ptr) P"
	.ascii	"B_ANONYMOUS_ONEOF_ ## allocation(union_name, tag, m"
	.ascii	"essage, field, PB_DATAOFFSET_ ## placement(message,"
	.ascii	" field, prevfield), PB_LTYPE_MAP_ ## type, ptr)\000"
.LASF327:
	.ascii	"__DQ_IBIT__ 0\000"
.LASF594:
	.ascii	"PB_SIZE_MAX ((pb_size_t)-1)\000"
.LASF215:
	.ascii	"__FLT64_MAX_10_EXP__ 308\000"
.LASF281:
	.ascii	"__SACCUM_IBIT__ 8\000"
.LASF378:
	.ascii	"__SIZEOF_PTRDIFF_T__ 4\000"
.LASF105:
	.ascii	"__UINT64_MAX__ 0xffffffffffffffffULL\000"
.LASF475:
	.ascii	"INT16_MIN (-32767-1)\000"
.LASF351:
	.ascii	"__USA_IBIT__ 16\000"
.LASF588:
	.ascii	"PB_ATYPE_POINTER 0x80\000"
.LASF429:
	.ascii	"__ARM_ARCH_EXT_IDIV__ 1\000"
.LASF383:
	.ascii	"__ARM_FEATURE_UNALIGNED 1\000"
.LASF514:
	.ascii	"INTPTR_MAX INT32_MAX\000"
.LASF22:
	.ascii	"__SIZEOF_FLOAT__ 4\000"
.LASF445:
	.ascii	"__HEAP_SIZE__ 0\000"
.LASF288:
	.ascii	"__USACCUM_MAX__ 0XFFFFP-8UHK\000"
.LASF123:
	.ascii	"__UINT32_C(c) c ## UL\000"
.LASF352:
	.ascii	"__UDA_FBIT__ 32\000"
.LASF671:
	.ascii	"data_size\000"
.LASF662:
	.ascii	"size_t\000"
.LASF605:
	.ascii	"PB_REQUIRED_STATIC(tag,st,m,fd,ltype,ptr) {tag, PB_"
	.ascii	"ATYPE_STATIC | PB_HTYPE_REQUIRED | ltype, fd, 0, pb"
	.ascii	"_membersize(st, m), 0, ptr}\000"
.LASF440:
	.ascii	"__GXX_TYPEINFO_EQUALITY_INLINE 0\000"
.LASF582:
	.ascii	"PB_HTYPE_REQUIRED 0x00\000"
.LASF679:
	.ascii	"pData\000"
.LASF407:
	.ascii	"__THUMBEL__ 1\000"
.LASF188:
	.ascii	"__LDBL_MIN__ 1.1\000"
.LASF641:
	.ascii	"PB_ANONYMOUS_ONEOF_POINTER(u,tag,st,m,fd,ltype,ptr)"
	.ascii	" {tag, PB_ATYPE_POINTER | PB_HTYPE_ONEOF | ltype, f"
	.ascii	"d, pb_delta(st, which_ ## u, m), pb_membersize(st, "
	.ascii	"m[0]), 0, ptr}\000"
.LASF345:
	.ascii	"__DA_IBIT__ 32\000"
.LASF27:
	.ascii	"__BIGGEST_ALIGNMENT__ 8\000"
.LASF301:
	.ascii	"__LACCUM_IBIT__ 32\000"
.LASF548:
	.ascii	"__CTYPE_GRAPH (__CTYPE_PUNCT | __CTYPE_UPPER | __CT"
	.ascii	"YPE_LOWER | __CTYPE_DIGIT)\000"
.LASF673:
	.ascii	"_Bool\000"
.LASF134:
	.ascii	"__UINT_FAST8_MAX__ 0xffffffffU\000"
.LASF28:
	.ascii	"__ORDER_LITTLE_ENDIAN__ 1234\000"
.LASF374:
	.ascii	"__HAVE_SPECULATION_SAFE_VALUE 1\000"
.LASF462:
	.ascii	"SOFTDEVICE_PRESENT 1\000"
.LASF312:
	.ascii	"__LLACCUM_MIN__ (-0X1P31LLK-0X1P31LLK)\000"
.LASF503:
	.ascii	"INT_FAST16_MAX INT32_MAX\000"
.LASF686:
	.ascii	"prev_size\000"
.LASF16:
	.ascii	"__OPTIMIZE__ 1\000"
.LASF342:
	.ascii	"__SA_FBIT__ 15\000"
.LASF213:
	.ascii	"__FLT64_MIN_10_EXP__ (-307)\000"
.LASF610:
	.ascii	"PB_REPEATED_POINTER(tag,st,m,fd,ltype,ptr) {tag, PB"
	.ascii	"_ATYPE_POINTER | PB_HTYPE_REPEATED | ltype, fd, pb_"
	.ascii	"delta(st, m ## _count, m), pb_membersize(st, m[0]),"
	.ascii	" 0, ptr}\000"
.LASF484:
	.ascii	"INTMAX_MAX 9223372036854775807LL\000"
.LASF284:
	.ascii	"__SACCUM_EPSILON__ 0x1P-7HK\000"
.LASF60:
	.ascii	"__INT_FAST16_TYPE__ int\000"
.LASF486:
	.ascii	"INT_LEAST8_MIN INT8_MIN\000"
.LASF200:
	.ascii	"__FLT32_DECIMAL_DIG__ 9\000"
.LASF48:
	.ascii	"__UINT16_TYPE__ short unsigned int\000"
.LASF348:
	.ascii	"__UHA_FBIT__ 8\000"
.LASF86:
	.ascii	"__WCHAR_WIDTH__ 32\000"
.LASF358:
	.ascii	"__GNUC_STDC_INLINE__ 1\000"
.LASF333:
	.ascii	"__UHQ_IBIT__ 0\000"
.LASF519:
	.ascii	"UINT16_C(x) (x ##U)\000"
.LASF393:
	.ascii	"__ARM_FEATURE_CLZ 1\000"
.LASF74:
	.ascii	"__LONG_LONG_MAX__ 0x7fffffffffffffffLL\000"
.LASF491:
	.ascii	"INT_LEAST16_MAX INT16_MAX\000"
.LASF46:
	.ascii	"__INT64_TYPE__ long long int\000"
.LASF152:
	.ascii	"__FLT_MAX_10_EXP__ 38\000"
.LASF313:
	.ascii	"__LLACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LLK\000"
.LASF449:
	.ascii	"DEBUG_NRF 1\000"
.LASF529:
	.ascii	"WINT_MAX 2147483647L\000"
.LASF314:
	.ascii	"__LLACCUM_EPSILON__ 0x1P-31LLK\000"
.LASF287:
	.ascii	"__USACCUM_MIN__ 0.0UHK\000"
.LASF225:
	.ascii	"__FLT32X_MANT_DIG__ 53\000"
.LASF432:
	.ascii	"__ARM_FEATURE_COPROC\000"
.LASF563:
	.ascii	"PB_PACKED_STRUCT_END \000"
.LASF267:
	.ascii	"__ULFRACT_MIN__ 0.0ULR\000"
.LASF147:
	.ascii	"__FLT_MANT_DIG__ 24\000"
.LASF530:
	.ascii	"__stddef_H \000"
.LASF521:
	.ascii	"UINT32_C(x) (x ##UL)\000"
.LASF401:
	.ascii	"__ARM_ARCH\000"
.LASF216:
	.ascii	"__FLT64_DECIMAL_DIG__ 17\000"
.LASF658:
	.ascii	"uint_least8_t\000"
.LASF677:
	.ascii	"required_field_index\000"
.LASF196:
	.ascii	"__FLT32_MIN_EXP__ (-125)\000"
.LASF296:
	.ascii	"__UACCUM_IBIT__ 16\000"
.LASF135:
	.ascii	"__UINT_FAST16_MAX__ 0xffffffffU\000"
.LASF64:
	.ascii	"__UINT_FAST16_TYPE__ unsigned int\000"
.LASF616:
	.ascii	"PB_OPTEXT_CALLBACK(tag,st,m,fd,ltype,ptr) PB_OPTION"
	.ascii	"AL_CALLBACK(tag, st, m, fd, ltype, ptr)\000"
.LASF541:
	.ascii	"__CTYPE_SPACE 0x08\000"
.LASF166:
	.ascii	"__DBL_MIN_10_EXP__ (-307)\000"
.LASF286:
	.ascii	"__USACCUM_IBIT__ 8\000"
.LASF69:
	.ascii	"__GXX_ABI_VERSION 1014\000"
.LASF590:
	.ascii	"PB_ATYPE_MASK 0xC0\000"
.LASF195:
	.ascii	"__FLT32_DIG__ 6\000"
.LASF497:
	.ascii	"UINT_LEAST64_MAX UINT64_MAX\000"
.LASF470:
	.ascii	"__stdint_H \000"
.LASF222:
	.ascii	"__FLT64_HAS_DENORM__ 1\000"
.LASF143:
	.ascii	"__FLT_EVAL_METHOD__ 0\000"
.LASF569:
	.ascii	"PB_MAX_REQUIRED_FIELDS 64\000"
.LASF237:
	.ascii	"__FLT32X_HAS_DENORM__ 1\000"
.LASF493:
	.ascii	"INT_LEAST64_MAX INT64_MAX\000"
.LASF115:
	.ascii	"__INT_LEAST64_MAX__ 0x7fffffffffffffffLL\000"
.LASF464:
	.ascii	"PB_COMMON_H_INCLUDED \000"
.LASF691:
	.ascii	"fields\000"
.LASF290:
	.ascii	"__ACCUM_FBIT__ 15\000"
.LASF488:
	.ascii	"INT_LEAST32_MIN INT32_MIN\000"
.LASF367:
	.ascii	"__GCC_ATOMIC_WCHAR_T_LOCK_FREE 2\000"
.LASF306:
	.ascii	"__ULACCUM_IBIT__ 32\000"
.LASF634:
	.ascii	"PB_LTYPE_MAP_UINT64 PB_LTYPE_UVARINT\000"
.LASF270:
	.ascii	"__LLFRACT_FBIT__ 63\000"
.LASF683:
	.ascii	"pb_field_iter_find\000"
.LASF592:
	.ascii	"PB_HTYPE(x) ((x) & PB_HTYPE_MASK)\000"
.LASF675:
	.ascii	"pb_field_iter_s\000"
.LASF108:
	.ascii	"__INT_LEAST8_WIDTH__ 8\000"
.LASF350:
	.ascii	"__USA_FBIT__ 16\000"
.LASF102:
	.ascii	"__UINT8_MAX__ 0xff\000"
.LASF466:
	.ascii	"PB_NO_PACKED_STRUCTS 1\000"
.LASF103:
	.ascii	"__UINT16_MAX__ 0xffff\000"
.LASF448:
	.ascii	"DEBUG 1\000"
.LASF156:
	.ascii	"__FLT_MIN__ 1.1\000"
.LASF547:
	.ascii	"__CTYPE_ALNUM (__CTYPE_UPPER | __CTYPE_LOWER | __CT"
	.ascii	"YPE_DIGIT)\000"
.LASF471:
	.ascii	"UINT8_MAX 255\000"
.LASF8:
	.ascii	"__VERSION__ \"10.2.1 20201103 (release)\"\000"
.LASF648:
	.ascii	"uint8_t\000"
.LASF511:
	.ascii	"PTRDIFF_MAX INT32_MAX\000"
.LASF94:
	.ascii	"__INTMAX_WIDTH__ 64\000"
.LASF434:
	.ascii	"__ARM_FEATURE_CDE\000"
.LASF260:
	.ascii	"__LFRACT_FBIT__ 31\000"
.LASF643:
	.ascii	"PB_SET_ERROR(stream,msg) PB_UNUSED(stream)\000"
.LASF443:
	.ascii	"__SES_ARM 1\000"
.LASF528:
	.ascii	"WINT_MIN (-2147483647L-1)\000"
.LASF52:
	.ascii	"__INT_LEAST16_TYPE__ short int\000"
.LASF198:
	.ascii	"__FLT32_MAX_EXP__ 128\000"
.LASF627:
	.ascii	"PB_LTYPE_MAP_MESSAGE PB_LTYPE_SUBMESSAGE\000"
.LASF599:
	.ascii	"pb_arraysize(st,m) (pb_membersize(st, m) / pb_membe"
	.ascii	"rsize(st, m[0]))\000"
.LASF447:
	.ascii	"__GNU_LINKER 1\000"
.LASF164:
	.ascii	"__DBL_DIG__ 15\000"
.LASF558:
	.ascii	"true 1\000"
.LASF106:
	.ascii	"__INT_LEAST8_MAX__ 0x7f\000"
.LASF408:
	.ascii	"__ARM_ARCH_ISA_THUMB\000"
.LASF300:
	.ascii	"__LACCUM_FBIT__ 31\000"
.LASF339:
	.ascii	"__UTQ_IBIT__ 0\000"
.LASF633:
	.ascii	"PB_LTYPE_MAP_UINT32 PB_LTYPE_UVARINT\000"
.LASF174:
	.ascii	"__DBL_DENORM_MIN__ ((double)1.1)\000"
.LASF370:
	.ascii	"__GCC_ATOMIC_LONG_LOCK_FREE 2\000"
.LASF226:
	.ascii	"__FLT32X_DIG__ 15\000"
.LASF294:
	.ascii	"__ACCUM_EPSILON__ 0x1P-15K\000"
.LASF485:
	.ascii	"UINTMAX_MAX 18446744073709551615ULL\000"
.LASF336:
	.ascii	"__UDQ_FBIT__ 64\000"
.LASF187:
	.ascii	"__LDBL_NORM_MAX__ 1.1\000"
.LASF618:
	.ascii	"PB_LTYPE_MAP_BYTES PB_LTYPE_BYTES\000"
.LASF409:
	.ascii	"__ARM_ARCH_ISA_THUMB 2\000"
.LASF579:
	.ascii	"PB_LTYPE_EXTENSION 0x08\000"
.LASF338:
	.ascii	"__UTQ_FBIT__ 128\000"
.LASF155:
	.ascii	"__FLT_NORM_MAX__ 1.1\000"
.LASF655:
	.ascii	"long long int\000"
.LASF26:
	.ascii	"__CHAR_BIT__ 8\000"
.LASF51:
	.ascii	"__INT_LEAST8_TYPE__ signed char\000"
.LASF139:
	.ascii	"__INTPTR_WIDTH__ 32\000"
.LASF55:
	.ascii	"__UINT_LEAST8_TYPE__ unsigned char\000"
.LASF577:
	.ascii	"PB_LTYPE_STRING 0x06\000"
.LASF17:
	.ascii	"__FINITE_MATH_ONLY__ 0\000"
.LASF283:
	.ascii	"__SACCUM_MAX__ 0X7FFFP-7HK\000"
.LASF400:
	.ascii	"__arm__ 1\000"
.LASF611:
	.ascii	"PB_REQUIRED_CALLBACK(tag,st,m,fd,ltype,ptr) {tag, P"
	.ascii	"B_ATYPE_CALLBACK | PB_HTYPE_REQUIRED | ltype, fd, 0"
	.ascii	", pb_membersize(st, m), 0, ptr}\000"
.LASF458:
	.ascii	"NRF_DFU_SETTINGS_VERSION 2\000"
.LASF178:
	.ascii	"__LDBL_MANT_DIG__ 53\000"
.LASF329:
	.ascii	"__TQ_IBIT__ 0\000"
.LASF657:
	.ascii	"int_least16_t\000"
.LASF404:
	.ascii	"__GCC_ASM_FLAG_OUTPUTS__ 1\000"
.LASF372:
	.ascii	"__GCC_ATOMIC_TEST_AND_SET_TRUEVAL 1\000"
.LASF602:
	.ascii	"PB_DATAOFFSET_FIRST(st,m1,m2) (offsetof(st, m1))\000"
.LASF600:
	.ascii	"pb_delta(st,m1,m2) ((int)offsetof(st, m1) - (int)of"
	.ascii	"fsetof(st, m2))\000"
.LASF441:
	.ascii	"__ELF__ 1\000"
.LASF522:
	.ascii	"INT64_C(x) (x ##LL)\000"
.LASF110:
	.ascii	"__INT16_C(c) c\000"
.LASF380:
	.ascii	"__ARM_FEATURE_QBIT 1\000"
.LASF635:
	.ascii	"PB_LTYPE_MAP_EXTENSION PB_LTYPE_EXTENSION\000"
.LASF255:
	.ascii	"__UFRACT_FBIT__ 16\000"
.LASF303:
	.ascii	"__LACCUM_MAX__ 0X7FFFFFFFFFFFFFFFP-31LK\000"
.LASF130:
	.ascii	"__INT_FAST32_MAX__ 0x7fffffff\000"
.LASF173:
	.ascii	"__DBL_EPSILON__ ((double)1.1)\000"
.LASF11:
	.ascii	"__ATOMIC_ACQUIRE 2\000"
.LASF636:
	.ascii	"PB_FIELD(tag,type,rules,allocation,placement,messag"
	.ascii	"e,field,prevfield,ptr) PB_ ## rules ## _ ## allocat"
	.ascii	"ion(tag, message, field, PB_DATAOFFSET_ ## placemen"
	.ascii	"t(message, field, prevfield), PB_LTYPE_MAP_ ## type"
	.ascii	", ptr)\000"
.LASF589:
	.ascii	"PB_ATYPE_CALLBACK 0x40\000"
.LASF543:
	.ascii	"__CTYPE_CNTRL 0x20\000"
.LASF531:
	.ascii	"__crossworks_H \000"
.LASF631:
	.ascii	"PB_LTYPE_MAP_SINT64 PB_LTYPE_SVARINT\000"
.LASF433:
	.ascii	"__ARM_FEATURE_COPROC 15\000"
.LASF0:
	.ascii	"__STDC__ 1\000"
.LASF546:
	.ascii	"__CTYPE_ALPHA (__CTYPE_UPPER | __CTYPE_LOWER)\000"
.LASF295:
	.ascii	"__UACCUM_FBIT__ 16\000"
.LASF167:
	.ascii	"__DBL_MAX_EXP__ 1024\000"
.LASF14:
	.ascii	"__ATOMIC_CONSUME 1\000"
.LASF131:
	.ascii	"__INT_FAST32_WIDTH__ 32\000"
.LASF439:
	.ascii	"__ARM_BF16_FORMAT_ALTERNATIVE\000"
.LASF678:
	.ascii	"dest_struct\000"
.LASF37:
	.ascii	"__WINT_TYPE__ unsigned int\000"
.LASF423:
	.ascii	"__ARM_NEON_FP\000"
.LASF73:
	.ascii	"__LONG_MAX__ 0x7fffffffL\000"
.LASF99:
	.ascii	"__INT16_MAX__ 0x7fff\000"
.LASF265:
	.ascii	"__ULFRACT_FBIT__ 32\000"
.LASF81:
	.ascii	"__SCHAR_WIDTH__ 8\000"
.LASF575:
	.ascii	"PB_LTYPE_LAST_PACKABLE 0x04\000"
.LASF292:
	.ascii	"__ACCUM_MIN__ (-0X1P15K-0X1P15K)\000"
.LASF509:
	.ascii	"UINT_FAST64_MAX UINT64_MAX\000"
.LASF23:
	.ascii	"__SIZEOF_DOUBLE__ 8\000"
.LASF7:
	.ascii	"__GNUC_PATCHLEVEL__ 1\000"
.LASF665:
	.ascii	"pb_size_t\000"
.LASF608:
	.ascii	"PB_REQUIRED_POINTER(tag,st,m,fd,ltype,ptr) {tag, PB"
	.ascii	"_ATYPE_POINTER | PB_HTYPE_REQUIRED | ltype, fd, 0, "
	.ascii	"pb_membersize(st, m[0]), 0, ptr}\000"
.LASF682:
	.ascii	"iter\000"
.LASF78:
	.ascii	"__WINT_MIN__ 0U\000"
.LASF305:
	.ascii	"__ULACCUM_FBIT__ 32\000"
.LASF257:
	.ascii	"__UFRACT_MIN__ 0.0UR\000"
.LASF676:
	.ascii	"start\000"
.LASF2:
	.ascii	"__STDC_UTF_16__ 1\000"
.LASF251:
	.ascii	"__FRACT_IBIT__ 0\000"
.LASF346:
	.ascii	"__TA_FBIT__ 63\000"
.LASF192:
	.ascii	"__LDBL_HAS_INFINITY__ 1\000"
.LASF142:
	.ascii	"__GCC_IEC_559_COMPLEX 0\000"
.LASF104:
	.ascii	"__UINT32_MAX__ 0xffffffffUL\000"
.LASF467:
	.ascii	"PB_FIELD_16BIT 1\000"
.LASF202:
	.ascii	"__FLT32_NORM_MAX__ 1.1\000"
.LASF180:
	.ascii	"__LDBL_MIN_EXP__ (-1021)\000"
.LASF392:
	.ascii	"__ARM_FEATURE_LDREX 7\000"
.LASF275:
	.ascii	"__ULLFRACT_FBIT__ 64\000"
.LASF376:
	.ascii	"__SIZEOF_WCHAR_T__ 4\000"
.LASF562:
	.ascii	"PB_PACKED_STRUCT_START \000"
.LASF32:
	.ascii	"__FLOAT_WORD_ORDER__ __ORDER_LITTLE_ENDIAN__\000"
.LASF293:
	.ascii	"__ACCUM_MAX__ 0X7FFFFFFFP-15K\000"
.LASF89:
	.ascii	"__SIZE_WIDTH__ 32\000"
.LASF389:
	.ascii	"__ARM_FEATURE_MVE\000"
.LASF555:
	.ascii	"offsetof(s,m) __builtin_offsetof(s, m)\000"
.LASF96:
	.ascii	"__SIG_ATOMIC_MIN__ (-__SIG_ATOMIC_MAX__ - 1)\000"
.LASF274:
	.ascii	"__LLFRACT_EPSILON__ 0x1P-63LLR\000"
.LASF523:
	.ascii	"UINT64_C(x) (x ##ULL)\000"
.LASF84:
	.ascii	"__LONG_WIDTH__ 32\000"
.LASF240:
	.ascii	"__SFRACT_FBIT__ 7\000"
.LASF41:
	.ascii	"__CHAR32_TYPE__ long unsigned int\000"
.LASF241:
	.ascii	"__SFRACT_IBIT__ 0\000"
.LASF666:
	.ascii	"pb_ssize_t\000"
.LASF451:
	.ascii	"BLE_STACK_SUPPORT_REQD 1\000"
.LASF482:
	.ascii	"UINT64_MAX 18446744073709551615ULL\000"
.LASF663:
	.ascii	"long double\000"
.LASF323:
	.ascii	"__HQ_IBIT__ 0\000"
.LASF227:
	.ascii	"__FLT32X_MIN_EXP__ (-1021)\000"
.LASF651:
	.ascii	"uint16_t\000"
.LASF357:
	.ascii	"__USER_LABEL_PREFIX__ \000"
.LASF536:
	.ascii	"__RAL_PTRDIFF_T int\000"
.LASF57:
	.ascii	"__UINT_LEAST32_TYPE__ long unsigned int\000"
.LASF614:
	.ascii	"PB_OPTEXT_STATIC(tag,st,m,fd,ltype,ptr) {tag, PB_AT"
	.ascii	"YPE_STATIC | PB_HTYPE_OPTIONAL | ltype, 0, 0, pb_me"
	.ascii	"mbersize(st, m), 0, ptr}\000"
.LASF326:
	.ascii	"__DQ_FBIT__ 63\000"
.LASF242:
	.ascii	"__SFRACT_MIN__ (-0.5HR-0.5HR)\000"
.LASF221:
	.ascii	"__FLT64_DENORM_MIN__ 1.1\000"
.LASF97:
	.ascii	"__SIG_ATOMIC_WIDTH__ 32\000"
.LASF477:
	.ascii	"UINT32_MAX 4294967295UL\000"
.LASF438:
	.ascii	"__ARM_FEATURE_BF16_VECTOR_ARITHMETIC\000"
.LASF500:
	.ascii	"INT_FAST32_MIN INT32_MIN\000"
.LASF71:
	.ascii	"__SHRT_MAX__ 0x7fff\000"
.LASF416:
	.ascii	"__ARM_FP16_ARGS\000"
.LASF619:
	.ascii	"PB_LTYPE_MAP_DOUBLE PB_LTYPE_FIXED64\000"
.LASF30:
	.ascii	"__ORDER_PDP_ENDIAN__ 3412\000"
.LASF325:
	.ascii	"__SQ_IBIT__ 0\000"
.LASF5:
	.ascii	"__GNUC__ 10\000"
.LASF450:
	.ascii	"NRF_DFU_DEBUG_VERSION 1\000"
.LASF489:
	.ascii	"INT_LEAST64_MIN INT64_MIN\000"
.LASF153:
	.ascii	"__FLT_DECIMAL_DIG__ 9\000"
.LASF100:
	.ascii	"__INT32_MAX__ 0x7fffffffL\000"
.LASF87:
	.ascii	"__WINT_WIDTH__ 32\000"
.LASF507:
	.ascii	"UINT_FAST16_MAX UINT32_MAX\000"
.LASF516:
	.ascii	"INT8_C(x) (x)\000"
.LASF632:
	.ascii	"PB_LTYPE_MAP_STRING PB_LTYPE_STRING\000"
.LASF446:
	.ascii	"__SES_VERSION 54201\000"
.LASF330:
	.ascii	"__UQQ_FBIT__ 8\000"
.LASF335:
	.ascii	"__USQ_IBIT__ 0\000"
.LASF356:
	.ascii	"__REGISTER_PREFIX__ \000"
.LASF501:
	.ascii	"INT_FAST64_MIN INT64_MIN\000"
.LASF650:
	.ascii	"short int\000"
.LASF259:
	.ascii	"__UFRACT_EPSILON__ 0x1P-16UR\000"
.LASF639:
	.ascii	"PB_ONEOF_FIELD(union_name,tag,type,rules,allocation"
	.ascii	",placement,message,field,prevfield,ptr) PB_ONEOF_ #"
	.ascii	"# allocation(union_name, tag, message, field, PB_DA"
	.ascii	"TAOFFSET_ ## placement(message, union_name.field, p"
	.ascii	"revfield), PB_LTYPE_MAP_ ## type, ptr)\000"
.LASF332:
	.ascii	"__UHQ_FBIT__ 16\000"
.LASF45:
	.ascii	"__INT32_TYPE__ long int\000"
.LASF228:
	.ascii	"__FLT32X_MIN_10_EXP__ (-307)\000"
.LASF437:
	.ascii	"__ARM_FEATURE_BF16_SCALAR_ARITHMETIC\000"
.LASF660:
	.ascii	"long int\000"
.LASF19:
	.ascii	"__SIZEOF_LONG__ 4\000"
.LASF182:
	.ascii	"__LDBL_MAX_EXP__ 1024\000"
.LASF324:
	.ascii	"__SQ_FBIT__ 31\000"
.LASF581:
	.ascii	"PB_LTYPE_MASK 0x0F\000"
.LASF463:
	.ascii	"SVC_INTERFACE_CALL_AS_NORMAL_FUNCTION 1\000"
.LASF262:
	.ascii	"__LFRACT_MIN__ (-0.5LR-0.5LR)\000"
.LASF513:
	.ascii	"INTPTR_MIN INT32_MIN\000"
.LASF544:
	.ascii	"__CTYPE_BLANK 0x40\000"
.LASF76:
	.ascii	"__WCHAR_MIN__ 0U\000"
.LASF472:
	.ascii	"INT8_MAX 127\000"
.LASF537:
	.ascii	"__CODE \000"
.LASF122:
	.ascii	"__UINT_LEAST32_MAX__ 0xffffffffUL\000"
.LASF498:
	.ascii	"INT_FAST8_MIN INT8_MIN\000"
.LASF405:
	.ascii	"__thumb__ 1\000"
.LASF193:
	.ascii	"__LDBL_HAS_QUIET_NAN__ 1\000"
.LASF43:
	.ascii	"__INT8_TYPE__ signed char\000"
.LASF77:
	.ascii	"__WINT_MAX__ 0xffffffffU\000"
.LASF561:
	.ascii	"__string_H \000"
.LASF185:
	.ascii	"__LDBL_DECIMAL_DIG__ 17\000"
.LASF328:
	.ascii	"__TQ_FBIT__ 127\000"
.LASF249:
	.ascii	"__USFRACT_EPSILON__ 0x1P-8UHR\000"
.LASF621:
	.ascii	"PB_LTYPE_MAP_UENUM PB_LTYPE_UVARINT\000"
.LASF481:
	.ascii	"INT64_MAX 9223372036854775807LL\000"
.LASF137:
	.ascii	"__UINT_FAST64_MAX__ 0xffffffffffffffffULL\000"
.LASF136:
	.ascii	"__UINT_FAST32_MAX__ 0xffffffffU\000"
.LASF510:
	.ascii	"PTRDIFF_MIN INT32_MIN\000"
.LASF245:
	.ascii	"__USFRACT_FBIT__ 8\000"
.LASF116:
	.ascii	"__INT64_C(c) c ## LL\000"
.LASF322:
	.ascii	"__HQ_FBIT__ 15\000"
.LASF672:
	.ascii	"array_size\000"
.LASF349:
	.ascii	"__UHA_IBIT__ 8\000"
.LASF395:
	.ascii	"__ARM_FEATURE_SIMD32 1\000"
.LASF75:
	.ascii	"__WCHAR_MAX__ 0xffffffffU\000"
.LASF20:
	.ascii	"__SIZEOF_LONG_LONG__ 8\000"
.LASF474:
	.ascii	"UINT16_MAX 65535\000"
.LASF47:
	.ascii	"__UINT8_TYPE__ unsigned char\000"
.LASF82:
	.ascii	"__SHRT_WIDTH__ 16\000"
.LASF412:
	.ascii	"__ARM_FP\000"
.LASF685:
	.ascii	"prev_field\000"
.LASF629:
	.ascii	"PB_LTYPE_MAP_SFIXED64 PB_LTYPE_FIXED64\000"
.LASF480:
	.ascii	"INT64_MIN (-9223372036854775807LL-1)\000"
.LASF54:
	.ascii	"__INT_LEAST64_TYPE__ long long int\000"
.LASF552:
	.ascii	"__RAL_SIZE_T_DEFINED \000"
.LASF49:
	.ascii	"__UINT32_TYPE__ long unsigned int\000"
.LASF181:
	.ascii	"__LDBL_MIN_10_EXP__ (-307)\000"
.LASF664:
	.ascii	"pb_type_t\000"
.LASF232:
	.ascii	"__FLT32X_MAX__ 1.1\000"
.LASF33:
	.ascii	"__SIZEOF_POINTER__ 4\000"
.LASF298:
	.ascii	"__UACCUM_MAX__ 0XFFFFFFFFP-16UK\000"
.LASF411:
	.ascii	"__VFP_FP__ 1\000"
.LASF591:
	.ascii	"PB_ATYPE(x) ((x) & PB_ATYPE_MASK)\000"
.LASF268:
	.ascii	"__ULFRACT_MAX__ 0XFFFFFFFFP-32ULR\000"
.LASF690:
	.ascii	"pb_field_iter_begin\000"
.LASF340:
	.ascii	"__HA_FBIT__ 7\000"
.LASF88:
	.ascii	"__PTRDIFF_WIDTH__ 32\000"
.LASF539:
	.ascii	"__CTYPE_LOWER 0x02\000"
.LASF282:
	.ascii	"__SACCUM_MIN__ (-0X1P7HK-0X1P7HK)\000"
.LASF83:
	.ascii	"__INT_WIDTH__ 32\000"
.LASF574:
	.ascii	"PB_LTYPE_FIXED64 0x04\000"
.LASF373:
	.ascii	"__GCC_ATOMIC_POINTER_LOCK_FREE 2\000"
.LASF125:
	.ascii	"__UINT64_C(c) c ## ULL\000"
.LASF304:
	.ascii	"__LACCUM_EPSILON__ 0x1P-31LK\000"
.LASF515:
	.ascii	"UINTPTR_MAX UINT32_MAX\000"
.LASF244:
	.ascii	"__SFRACT_EPSILON__ 0x1P-7HR\000"
.LASF331:
	.ascii	"__UQQ_IBIT__ 0\000"
.LASF334:
	.ascii	"__USQ_FBIT__ 32\000"
.LASF341:
	.ascii	"__HA_IBIT__ 8\000"
.LASF620:
	.ascii	"PB_LTYPE_MAP_ENUM PB_LTYPE_VARINT\000"
.LASF366:
	.ascii	"__GCC_ATOMIC_CHAR32_T_LOCK_FREE 2\000"
.LASF1:
	.ascii	"__STDC_VERSION__ 199901L\000"
.LASF571:
	.ascii	"PB_LTYPE_UVARINT 0x01\000"
.LASF645:
	.ascii	"PB_RETURN_ERROR(stream,msg) return PB_SET_ERROR(str"
	.ascii	"eam, msg), false\000"
.LASF158:
	.ascii	"__FLT_DENORM_MIN__ 1.1\000"
.LASF254:
	.ascii	"__FRACT_EPSILON__ 0x1P-15R\000"
.LASF4:
	.ascii	"__STDC_HOSTED__ 1\000"
.LASF138:
	.ascii	"__INTPTR_MAX__ 0x7fffffff\000"
.LASF172:
	.ascii	"__DBL_MIN__ ((double)1.1)\000"
.LASF615:
	.ascii	"PB_OPTEXT_POINTER(tag,st,m,fd,ltype,ptr) PB_OPTIONA"
	.ascii	"L_POINTER(tag, st, m, fd, ltype, ptr)\000"
.LASF233:
	.ascii	"__FLT32X_NORM_MAX__ 1.1\000"
.LASF455:
	.ascii	"INITIALIZE_USER_SECTIONS 1\000"
.LASF476:
	.ascii	"INT16_MAX 32767\000"
.LASF495:
	.ascii	"UINT_LEAST16_MAX UINT16_MAX\000"
.LASF593:
	.ascii	"PB_LTYPE(x) ((x) & PB_LTYPE_MASK)\000"
.LASF371:
	.ascii	"__GCC_ATOMIC_LLONG_LOCK_FREE 1\000"
.LASF362:
	.ascii	"__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4 1\000"
.LASF151:
	.ascii	"__FLT_MAX_EXP__ 128\000"
.LASF668:
	.ascii	"type\000"
.LASF13:
	.ascii	"__ATOMIC_ACQ_REL 4\000"
.LASF66:
	.ascii	"__UINT_FAST64_TYPE__ long long unsigned int\000"
.LASF647:
	.ascii	"unsigned char\000"
.LASF39:
	.ascii	"__UINTMAX_TYPE__ long long unsigned int\000"
.LASF534:
	.ascii	"__RAL_SIZE_T unsigned\000"
.LASF545:
	.ascii	"__CTYPE_XDIGIT 0x80\000"
.LASF596:
	.ascii	"PB_BYTES_ARRAY_T_ALLOCSIZE(n) ((size_t)n + offsetof"
	.ascii	"(pb_bytes_array_t, bytes))\000"
.LASF67:
	.ascii	"__INTPTR_TYPE__ int\000"
.LASF426:
	.ascii	"__ARM_PCS_VFP 1\000"
.LASF145:
	.ascii	"__DEC_EVAL_METHOD__ 2\000"
.LASF204:
	.ascii	"__FLT32_EPSILON__ 1.1\000"
.LASF246:
	.ascii	"__USFRACT_IBIT__ 0\000"
.LASF114:
	.ascii	"__INT_LEAST32_WIDTH__ 32\000"
.LASF630:
	.ascii	"PB_LTYPE_MAP_SINT32 PB_LTYPE_SVARINT\000"
.LASF688:
	.ascii	"/home/orlo/beep-base-firmware/Source_V1.5.12/Code/n"
	.ascii	"RF/nRF5_SDK_15.3/external/nano-pb/pb_common.c\000"
.LASF217:
	.ascii	"__FLT64_MAX__ 1.1\000"
.LASF601:
	.ascii	"PB_LAST_FIELD {0,(pb_type_t) 0,0,0,0,0,0}\000"
.LASF687:
	.ascii	"GNU C99 10.2.1 20201103 (release) -fmessage-length="
	.ascii	"0 -mcpu=cortex-m4 -mlittle-endian -mfloat-abi=hard "
	.ascii	"-mfpu=fpv4-sp-d16 -mthumb -mtp=soft -munaligned-acc"
	.ascii	"ess -std=gnu99 -g3 -gpubnames -Os -fomit-frame-poin"
	.ascii	"ter -fno-dwarf2-cfi-asm -fno-builtin -ffunction-sec"
	.ascii	"tions -fdata-sections -fshort-enums -fno-common\000"
.LASF190:
	.ascii	"__LDBL_DENORM_MIN__ 1.1\000"
.LASF398:
	.ascii	"__ARM_ARCH_PROFILE\000"
.LASF637:
	.ascii	"PB_ONEOF_STATIC(u,tag,st,m,fd,ltype,ptr) {tag, PB_A"
	.ascii	"TYPE_STATIC | PB_HTYPE_ONEOF | ltype, fd, pb_delta("
	.ascii	"st, which_ ## u, u.m), pb_membersize(st, u.m), 0, p"
	.ascii	"tr}\000"
.LASF553:
	.ascii	"NULL 0\000"
.LASF34:
	.ascii	"__SIZE_TYPE__ unsigned int\000"
.LASF669:
	.ascii	"data_offset\000"
.LASF469:
	.ascii	"NANOPB_VERSION nanopb-0.3.6-dev\000"
.LASF315:
	.ascii	"__ULLACCUM_FBIT__ 32\000"
.LASF517:
	.ascii	"UINT8_C(x) (x ##U)\000"
.LASF525:
	.ascii	"UINTMAX_C(x) (x ##ULL)\000"
.LASF598:
	.ascii	"pb_membersize(st,m) (sizeof ((st*)0)->m)\000"
.LASF623:
	.ascii	"PB_LTYPE_MAP_FIXED64 PB_LTYPE_FIXED64\000"
.LASF606:
	.ascii	"PB_OPTIONAL_STATIC(tag,st,m,fd,ltype,ptr) {tag, PB_"
	.ascii	"ATYPE_STATIC | PB_HTYPE_OPTIONAL | ltype, fd, pb_de"
	.ascii	"lta(st, has_ ## m, m), pb_membersize(st, m), 0, ptr"
	.ascii	"}\000"
.LASF250:
	.ascii	"__FRACT_FBIT__ 15\000"
.LASF368:
	.ascii	"__GCC_ATOMIC_SHORT_LOCK_FREE 2\000"
.LASF15:
	.ascii	"__OPTIMIZE_SIZE__ 1\000"
.LASF355:
	.ascii	"__UTA_IBIT__ 64\000"
.LASF524:
	.ascii	"INTMAX_C(x) (x ##LL)\000"
.LASF150:
	.ascii	"__FLT_MIN_10_EXP__ (-37)\000"
.LASF212:
	.ascii	"__FLT64_MIN_EXP__ (-1021)\000"
.LASF144:
	.ascii	"__FLT_EVAL_METHOD_TS_18661_3__ 0\000"
.LASF176:
	.ascii	"__DBL_HAS_INFINITY__ 1\000"
.LASF170:
	.ascii	"__DBL_MAX__ ((double)1.1)\000"
.LASF61:
	.ascii	"__INT_FAST32_TYPE__ int\000"
.LASF161:
	.ascii	"__FLT_HAS_QUIET_NAN__ 1\000"
.LASF18:
	.ascii	"__SIZEOF_INT__ 4\000"
.LASF38:
	.ascii	"__INTMAX_TYPE__ long long int\000"
.LASF595:
	.ascii	"PB_BYTES_ARRAY_T(n) struct { pb_size_t size; pb_byt"
	.ascii	"e_t bytes[n]; }\000"
.LASF556:
	.ascii	"__stdbool_h \000"
.LASF91:
	.ascii	"__INTMAX_C(c) c ## LL\000"
.LASF628:
	.ascii	"PB_LTYPE_MAP_SFIXED32 PB_LTYPE_FIXED32\000"
.LASF253:
	.ascii	"__FRACT_MAX__ 0X7FFFP-15R\000"
.LASF418:
	.ascii	"__ARM_FEATURE_FP16_VECTOR_ARITHMETIC\000"
.LASF403:
	.ascii	"__APCS_32__ 1\000"
.LASF220:
	.ascii	"__FLT64_EPSILON__ 1.1\000"
.LASF502:
	.ascii	"INT_FAST8_MAX INT8_MAX\000"
.LASF50:
	.ascii	"__UINT64_TYPE__ long long unsigned int\000"
.LASF239:
	.ascii	"__FLT32X_HAS_QUIET_NAN__ 1\000"
.LASF168:
	.ascii	"__DBL_MAX_10_EXP__ 308\000"
.LASF218:
	.ascii	"__FLT64_NORM_MAX__ 1.1\000"
.LASF680:
	.ascii	"pSize\000"
.LASF652:
	.ascii	"short unsigned int\000"
.LASF364:
	.ascii	"__GCC_ATOMIC_CHAR_LOCK_FREE 2\000"
.LASF347:
	.ascii	"__TA_IBIT__ 64\000"
.LASF320:
	.ascii	"__QQ_FBIT__ 7\000"
.LASF238:
	.ascii	"__FLT32X_HAS_INFINITY__ 1\000"
.LASF490:
	.ascii	"INT_LEAST8_MAX INT8_MAX\000"
.LASF570:
	.ascii	"PB_LTYPE_VARINT 0x00\000"
.LASF56:
	.ascii	"__UINT_LEAST16_TYPE__ short unsigned int\000"
.LASF127:
	.ascii	"__INT_FAST8_WIDTH__ 32\000"
.LASF597:
	.ascii	"PB_PROTO_HEADER_VERSION 30\000"
.LASF149:
	.ascii	"__FLT_MIN_EXP__ (-125)\000"
.LASF487:
	.ascii	"INT_LEAST16_MIN INT16_MIN\000"
.LASF436:
	.ascii	"__ARM_FEATURE_MATMUL_INT8\000"
.LASF576:
	.ascii	"PB_LTYPE_BYTES 0x05\000"
.LASF363:
	.ascii	"__GCC_ATOMIC_BOOL_LOCK_FREE 2\000"
.LASF184:
	.ascii	"__DECIMAL_DIG__ 17\000"
.LASF424:
	.ascii	"__THUMB_INTERWORK__ 1\000"
.LASF113:
	.ascii	"__INT32_C(c) c ## L\000"
.LASF578:
	.ascii	"PB_LTYPE_SUBMESSAGE 0x07\000"
.LASF415:
	.ascii	"__ARM_FP16_FORMAT_ALTERNATIVE\000"
.LASF640:
	.ascii	"PB_ANONYMOUS_ONEOF_STATIC(u,tag,st,m,fd,ltype,ptr) "
	.ascii	"{tag, PB_ATYPE_STATIC | PB_HTYPE_ONEOF | ltype, fd,"
	.ascii	" pb_delta(st, which_ ## u, m), pb_membersize(st, m)"
	.ascii	", 0, ptr}\000"
.LASF183:
	.ascii	"__LDBL_MAX_10_EXP__ 308\000"
.LASF230:
	.ascii	"__FLT32X_MAX_10_EXP__ 308\000"
.LASF140:
	.ascii	"__UINTPTR_MAX__ 0xffffffffU\000"
.LASF128:
	.ascii	"__INT_FAST16_MAX__ 0x7fffffff\000"
.LASF385:
	.ascii	"__ARM_FEATURE_CRC32\000"
.LASF199:
	.ascii	"__FLT32_MAX_10_EXP__ 38\000"
.LASF622:
	.ascii	"PB_LTYPE_MAP_FIXED32 PB_LTYPE_FIXED32\000"
.LASF381:
	.ascii	"__ARM_FEATURE_SAT 1\000"
.LASF263:
	.ascii	"__LFRACT_MAX__ 0X7FFFFFFFP-31LR\000"
.LASF580:
	.ascii	"PB_LTYPES_COUNT 9\000"
.LASF459:
	.ascii	"NRF_DFU_SVCI_ENABLED 1\000"
.LASF425:
	.ascii	"__ARM_ARCH_7EM__ 1\000"
.LASF452:
	.ascii	"CUSTOM_BOARD_INC beepbaseboard\000"
.LASF559:
	.ascii	"false 0\000"
.LASF613:
	.ascii	"PB_REPEATED_CALLBACK(tag,st,m,fd,ltype,ptr) {tag, P"
	.ascii	"B_ATYPE_CALLBACK | PB_HTYPE_REPEATED | ltype, fd, 0"
	.ascii	", pb_membersize(st, m), 0, ptr}\000"
.LASF607:
	.ascii	"PB_REPEATED_STATIC(tag,st,m,fd,ltype,ptr) {tag, PB_"
	.ascii	"ATYPE_STATIC | PB_HTYPE_REPEATED | ltype, fd, pb_de"
	.ascii	"lta(st, m ## _count, m), pb_membersize(st, m[0]), p"
	.ascii	"b_arraysize(st, m), ptr}\000"
.LASF112:
	.ascii	"__INT_LEAST32_MAX__ 0x7fffffffL\000"
.LASF186:
	.ascii	"__LDBL_MAX__ 1.1\000"
.LASF572:
	.ascii	"PB_LTYPE_SVARINT 0x02\000"
.LASF540:
	.ascii	"__CTYPE_DIGIT 0x04\000"
.LASF92:
	.ascii	"__UINTMAX_MAX__ 0xffffffffffffffffULL\000"
.LASF42:
	.ascii	"__SIG_ATOMIC_TYPE__ int\000"
.LASF512:
	.ascii	"SIZE_MAX INT32_MAX\000"
.LASF272:
	.ascii	"__LLFRACT_MIN__ (-0.5LLR-0.5LLR)\000"
.LASF219:
	.ascii	"__FLT64_MIN__ 1.1\000"
.LASF420:
	.ascii	"__ARM_FEATURE_FMA 1\000"
.LASF279:
	.ascii	"__ULLFRACT_EPSILON__ 0x1P-64ULLR\000"
.LASF191:
	.ascii	"__LDBL_HAS_DENORM__ 1\000"
.LASF120:
	.ascii	"__UINT_LEAST16_MAX__ 0xffff\000"
.LASF391:
	.ascii	"__ARM_FEATURE_LDREX\000"
.LASF319:
	.ascii	"__ULLACCUM_EPSILON__ 0x1P-32ULLK\000"
.LASF285:
	.ascii	"__USACCUM_FBIT__ 8\000"
.LASF159:
	.ascii	"__FLT_HAS_DENORM__ 1\000"
.LASF133:
	.ascii	"__INT_FAST64_WIDTH__ 64\000"
.LASF169:
	.ascii	"__DBL_DECIMAL_DIG__ 17\000"
	.ident	"GCC: (GNU) 10.2.1 20201103 (release)"
