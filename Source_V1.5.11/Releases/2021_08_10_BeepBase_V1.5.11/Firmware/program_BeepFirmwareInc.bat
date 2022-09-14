@ECHO OFF
SET hw_major=1
SET hw_minor=0
SET hw_type=190222
SET /A hw_reg_val=%hw_major%*65536 + %hw_minor%
SET /A hwidARead=<hwid.txt
ECHO read ID %hwidARead%
SET /P hwidA=hwidARead+1
ECHO %hwidA%>hwid.txt


ECHO Start programming HW %hw_major%.%hw_minor%; reg:%hw_reg_val%, Type:%hw_type%, Num:%hwId%, ID: 0x0142454550%hwidA%
start /B /wait nrfjprog --eraseall
start /B /wait nrfjprog --memwr 0x10001080 --val %hw_reg_val%
start /B /wait nrfjprog --memwr 0x10001084 --val %hw_type%
start /B /wait nrfjprog --memwr 0x10001088 --val %hwidA%
start /B /wait nrfjprog --memwr 0x1000108C --val 0x42454550
start /B /wait nrfjprog --memwr 0x10001090 --val 0x01000000
start /B /wait nrfjprog --program Beepbase.hex
start /B /wait nrfjprog --reset
ECHO Programming Done


GOTO End
:End
pause
