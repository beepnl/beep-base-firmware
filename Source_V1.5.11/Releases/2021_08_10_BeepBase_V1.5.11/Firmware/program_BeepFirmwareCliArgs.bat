@ECHO OFF
SET hw_major=1
SET hw_minor=0
SET hw_ID=190222
SET /A hw_reg_val=%hw_major%*65536 + %hw_minor%

ECHO Start programming HW %hw_major%.%hw_minor%; reg:%hw_reg_val%
start /B /wait nrfjprog --eraseall
start /B /wait nrfjprog --memwr 0x10001080 --val %hw_reg_val%
start /B /wait nrfjprog --memwr 0x10001084 --val %hw_ID%
start /B /wait nrfjprog --memwr 0x10001088 --val %1%
start /B /wait nrfjprog --memwr 0x1000108C --val %2%
start /B /wait nrfjprog --memwr 0x10001090 --val %3%
start /B /wait nrfjprog --program Beepbase.hex
start /B /wait nrfjprog --reset
ECHO Programming Done

GOTO End
:End
pause
