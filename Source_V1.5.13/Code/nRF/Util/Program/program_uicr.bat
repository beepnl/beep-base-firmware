@ECHO OFF
SET hw_major=1
SET hw_minor=0
SET hw_ID=190222
SET /A hw_reg_val=%hw_major%*65536 + %hw_minor%
SET jlink_id=682613435

ECHO Start programming HW %hw_major%.%hw_minor%; reg:%hw_reg_val%
start /B /wait nrfjprog --snr %jlink_id% --eraseall
start /B /wait nrfjprog --snr %jlink_id% --memwr 0x10001080 --val %hw_reg_val%
start /B /wait nrfjprog --snr %jlink_id% --memwr 0x10001084 --val %hw_ID%
start /B /wait nrfjprog --snr %jlink_id% --memwr 0x10001088 --val 0x11223344
start /B /wait nrfjprog --snr %jlink_id% --memwr 0x1000108C --val 0x55667788
start /B /wait nrfjprog --snr %jlink_id% --memwr 0x10001090 --val 0x99AABBCC
start /B /wait nrfjprog --snr %jlink_id% --reset
ECHO Programming Done

GOTO End
:End
pause
