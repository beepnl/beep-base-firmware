#ifndef MX_FLASH_FS_H
#define	MX_FLASH_FS_H

	#include <stdint.h>
	#include <stdbool.h>
    #include "ff.h"
    #include "mx_flash_fs.h"
    #include "integer.h"

    #define MX_FLASH_DEBUG  0

    /*
        8128 KiB total storage size according to MXflash_getFree()
        8128 * 1024 = 8323072bytes

        Maximum flash log size for example 8300000 = 8.3M/512 = 16210.9375 blocks, roudning down is 16210
        16210 * 512 = 8299520 bytes

    */
    #define LOG_SIZE_MAX        (8299520UL) 
    #define MX_FLASH_LS_DEBUG   1

    bool    fatfs_init          (void);
    void    fatfs_uninit        (void);
    void    MXflash_FSinit      (void);
    void    MXflash_FSdeinit    (void);
    UINT    fatfs_write         (char * dataToWrite, UINT lenghtToWrite);
    void    fatfs_read          (void);
    FRESULT fatfs_clear         (void);
    void    fatfs_clearAll      (void);
    FRESULT writeLogOffsetToFile(const uint32_t writeOffset);
    FRESULT storeLogOffset(void);
    FRESULT readLogOffsetFromFile(volatile uint32_t * offset_p);
    volatile uint32_t getCurrentWriteOffset(void);

    FRESULT fatfs_open          (FIL * fp, char * fname, FSIZE_t offset);
    FRESULT fatfs_close         (FIL * fp);
    FRESULT fatfs_readBuffer    (FIL * fp, char * readBuff, UINT size, UINT * bytesRead);

#endif	/* MX_FLASH_FS_H */

