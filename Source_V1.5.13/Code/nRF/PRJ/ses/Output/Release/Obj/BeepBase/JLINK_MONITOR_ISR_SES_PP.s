# 1 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
# 19 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        .name JLINK_MONITOR_ISR
        .syntax unified

        .extern JLINK_MONITOR_OnEnter
        .extern JLINK_MONITOR_OnExit
        .extern JLINK_MONITOR_OnPoll

        .global DebugMon_Handler
# 108 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        .section .text, "ax"




        .byte 0x4A
        .byte 0x4C
        .byte 0x49
        .byte 0x4E
        .byte 0x4B
        .byte 0x4D
        .byte 0x4F
        .byte 0x4E
        .byte 0x48
        .byte 0x41
        .byte 0x4E
        .byte 0x44
        .byte 0x4C
        .byte 0x45
        .byte 0x52
        .byte 0x00
# 141 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        .thumb_func

DebugMon_Handler:
# 158 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        PUSH {LR}
        BL JLINK_MONITOR_OnEnter
        POP {LR}
        LDR.N R3,_AddrDCRDR
        B.N _IndicateMonReady
_WaitProbeReadIndicateMonRdy:
        LDR R0,[R3, #+0x04]
        LSLS R0,R0,#+12
        BMI.N _WaitProbeReadIndicateMonRdy
_IndicateMonReady:
        LDR R0,[R3, #+0x04]
        ORR R0,R0,#0x80000
        STR R0,[R3, #+0x04]
# 181 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
_WaitForJLinkCmd:
        PUSH {LR}
        BL JLINK_MONITOR_OnPoll
        POP {LR}
        LDR.N R3,_AddrDCRDR
        LDR R0,[R3, #+0x04]
        LSRS R0,R0,#+20
        BCS _WaitForJLinkCmd
# 203 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        LDR R0,[R3, #+0x00]
        LSRS R1,R0,#+8
        LSLS R0,R0,#+24
        LSRS R0,R0,#+24



        CMP R0,#+0
        BEQ.N _HandleGetMonVersion
        CMP R0,#+2
        BEQ.N _HandleReadReg
        BCC.N _HandleRestartCPU
        CMP R0,#+3
        BEQ.N _HandleWriteReg_Veneer
        B.N _IndicateMonReady





_HandleRestartCPU:
        LDR R0,[R3, #+0x04]
        BIC R0,R0,#0x80000
        STR R0,[R3, #+0x04]
        PUSH {LR}
        BL JLINK_MONITOR_OnExit
        POP {PC}



        .section .text, "ax", %progbits
        .align 2
_AddrDCRDR:
        .long 0xE000EDF8
_AddrCPACR:
        .long 0xE000ED88

        .section .text, "ax"
        .thumb_func

;




_HandleGetMonVersion:






        MOVS R0,#+100
        STR R0,[R3, #+0x00]
        LDR R0,[R3, #+0x04]
        ORR R0,R0,#0x80000
        STR R0,[R3, #+0x04]
        B _WaitProbeReadIndicateMonRdy






_HandleWriteReg_Veneer:
        B.N _HandleWriteReg
_HandleReadReg:
# 309 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        LSRS R2,LR,#+3
        ITE CS
        MRSCS R2,PSP
        MRSCC R2,MSP
        CMP R1,#+4
        BCS _HandleReadRegR4
        LDR R0,[R2, R1, LSL #+2]
        B.N _HandleReadRegDone
_HandleReadRegR4:
        CMP R1,#+5
        BCS _HandleReadRegR5
        MOV R0,R4
        B.N _HandleReadRegDone
_HandleReadRegR5:
        CMP R1,#+6
        BCS _HandleReadRegR6
        MOV R0,R5
        B.N _HandleReadRegDone
_HandleReadRegR6:
        CMP R1,#+7
        BCS _HandleReadRegR7
        MOV R0,R6
        B.N _HandleReadRegDone
_HandleReadRegR7:
        CMP R1,#+8
        BCS _HandleReadRegR8
        MOV R0,R7
        B.N _HandleReadRegDone
_HandleReadRegR8:
        CMP R1,#+9
        BCS _HandleReadRegR9
        MOV R0,R8
        B.N _HandleReadRegDone
_HandleReadRegR9:
        CMP R1,#+10
        BCS _HandleReadRegR10
        MOV R0,R9
        B.N _HandleReadRegDone
_HandleReadRegR10:
        CMP R1,#+11
        BCS _HandleReadRegR11
        MOV R0,R10
        B.N _HandleReadRegDone
_HandleReadRegR11:
        CMP R1,#+12
        BCS _HandleReadRegR12
        MOV R0,R11
        B.N _HandleReadRegDone
_HandleReadRegR12:
        CMP R1,#+14
        BCS _HandleReadRegR14
        LDR R0,[R2, #+0x10]
        B.N _HandleReadRegDone
_HandleReadRegR14:
        CMP R1,#+15
        BCS _HandleReadRegR15
        LDR R0,[R2, #+0x14]
        B.N _HandleReadRegDone
_HandleReadRegR15:
        CMP R1,#+16
        BCS _HandleReadRegXPSR
        LDR R0,[R2, #+0x18]
        B.N _HandleReadRegDone
_HandleReadRegXPSR:
        CMP R1,#+17
        BCS _HandleReadRegMSP
        LDR R0,[R2, #+0x1C]
        B.N _HandleReadRegDone
_HandleReadRegMSP:
# 402 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        CMP R1,#+18
        BCS _HandleReadRegPSP
        MRS R0,MSP
        LSRS R1,LR,#+3
        BCS _HandleReadRegDone_Veneer
_HandleSPCorrection:
        LSRS R1,LR,#+5
        ITE CS
        ADDCS R0,R0,#+32
        ADDCC R0,R0,#+72
        LDR R1,[R2, #+0x1C]
        LSRS R1,R1,#+5
        IT CS
        ADDCS R0,R0,#+4
        B _HandleReadRegDone
_HandleReadRegPSP:
        CMP R1,#+19
        BCS _HandleReadRegCFBP
        MRS R0,PSP
        LSRS R1,LR,#+3
        BCC _HandleReadRegDone_Veneer
        B _HandleSPCorrection
_HandleReadRegCFBP:





        CMP R1,#+20
        BCS _HandleReadRegFPU
        MOVS R0,#+0
        MRS R2,PRIMASK
        ORRS R0,R2
        MRS R2,BASEPRI
        LSLS R2,R2,#+8
        ORRS R0,R2
        MRS R2,FAULTMASK
        LSLS R2,R2,#+16
        ORRS R0,R2
        MRS R2,CONTROL
        LSRS R1,LR,#3
        IT CS
        ORRCS R2,R2,#+2
        LSRS R1,LR,#+5
        ITE CS
        BICCS R2,R2,#+0x04
        ORRCC R2,R2,#+0x04
        LSLS R2,R2,#+24
        ORRS R0,R2
        B.N _HandleReadRegDone
_HandleReadRegFPU:
# 573 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        B _HandleReadRegUnknown
_HandleReadRegDone_Veneer:
        B _HandleReadRegDone

_HandleReadRegUnknown:
        MOVS R0,#+0
        B.N _HandleReadRegDone
_HandleReadRegDone:



        STR R0,[R3, #+0x00]
        LDR R0,[R3, #+0x04]
        ORR R0,R0,#0x80000
        STR R0,[R3, #+0x04]
        B _WaitProbeReadIndicateMonRdy



_HandleWriteReg:
# 634 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        LDR R0,[R3, #+0x04]
        ORR R0,R0,#0x80000
        STR R0,[R3, #+0x04]
_HandleWRegWaitUntilDataRecv:
        LDR R0,[R3, #+0x04]
        LSLS R0,R0,#+12
        BMI.N _HandleWRegWaitUntilDataRecv
        LDR R0,[R3, #+0x00]



        LSRS R2,LR,#+3
        ITE CS
        MRSCS R2,PSP
        MRSCC R2,MSP
        CMP R1,#+4
        BCS _HandleWriteRegR4
        STR R0,[R2, R1, LSL #+2]
        B.N _HandleWriteRegDone
_HandleWriteRegR4:
        CMP R1,#+5
        BCS _HandleWriteRegR5
        MOV R4,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR5:
        CMP R1,#+6
        BCS _HandleWriteRegR6
        MOV R5,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR6:
        CMP R1,#+7
        BCS _HandleWriteRegR7
        MOV R6,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR7:
        CMP R1,#+8
        BCS _HandleWriteRegR8
        MOV R7,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR8:
        CMP R1,#+9
        BCS _HandleWriteRegR9
        MOV R8,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR9:
        CMP R1,#+10
        BCS _HandleWriteRegR10
        MOV R9,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR10:
        CMP R1,#+11
        BCS _HandleWriteRegR11
        MOV R10,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR11:
        CMP R1,#+12
        BCS _HandleWriteRegR12
        MOV R11,R0
        B.N _HandleWriteRegDone
_HandleWriteRegR12:
        CMP R1,#+14
        BCS _HandleWriteRegR14
        STR R0,[R2, #+0x10]
        B.N _HandleWriteRegDone
_HandleWriteRegR14:
        CMP R1,#+15
        BCS _HandleWriteRegR15
        STR R0,[R2, #+0x14]
        B.N _HandleWriteRegDone
_HandleWriteRegR15:
        CMP R1,#+16
        BCS _HandleWriteRegXPSR
        STR R0,[R2, #+0x18]
        B.N _HandleWriteRegDone
_HandleWriteRegXPSR:
        CMP R1,#+17
        BCS _HandleWriteRegMSP
        STR R0,[R2, #+0x1C]
        B.N _HandleWriteRegDone
_HandleWriteRegMSP:



        CMP R1,#+18
        BCS _HandleWriteRegPSP
        B.N _HandleWriteRegDone
_HandleWriteRegPSP:
        CMP R1,#+19
        BCS _HandleWriteRegCFBP
        B.N _HandleWriteRegDone
_HandleWriteRegCFBP:





        CMP R1,#+20
        BCS _HandleWriteRegFPU
        LSLS R1,R0,#+24
        LSRS R1,R1,#+24
        MSR PRIMASK,R1
        LSLS R1,R0,#+16
        LSRS R1,R1,#+24
        MSR BASEPRI,R1
        LSLS R1,R0,#+8
        LSRS R1,R1,#+24
        MSR FAULTMASK,R1
        LSRS R1,R0,#+24
        LSRS R0,R1,#2
        ITE CS
        ORRCS LR,LR,#+4
        BICCC LR,LR,#+4
        BIC R1,R1,#+2
        LSRS R0,R1,#+3
        ITE CS
        BICCS LR,LR,#+0x10
        ORRCC LR,LR,#+0x10
        MRS R0,CONTROL
        LSRS R0,R0,#+3
        ITE CS
        ORRCS R1,R1,#+0x04
        BICCC R1,R1,#+0x04
        MSR CONTROL,R1
        ISB
        B.N _HandleWriteRegDone
_HandleWriteRegFPU:
# 881 "/home/orlo/Source_V1.5.13/Code/nRF/PRJ/ses/JLINK_MONITOR_ISR_SES.s"
        B _HandleWriteRegUnknown

_HandleWriteRegUnknown:
        B.N _HandleWriteRegDone
_HandleWriteRegDone:
        B _IndicateMonReady
        .end
