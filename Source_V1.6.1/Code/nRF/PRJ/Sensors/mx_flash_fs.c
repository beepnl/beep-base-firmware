#define NRF_LOG_MODULE_NAME MX_FS
#include "mx_flash_fs.h"
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"
#include "nrf.h"
#include "nrf_block_dev.h"
#include "nrf_block_dev_ram.h"
#include "nrf_block_dev_empty.h"
#include "nrf_block_dev_qspi.h"
#include "nrf_block_dev_sdc.h"
#include "nrf_drv_usbd.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "nrf_atomic.h"
#include "nrf_drv_power.h"
#include "ff.h"
#include "diskio_blkdev.h"
#include "app_timer.h"
#include "gpio-board.h"


const char filename[] = {"BEEPlog.txt"};
const char fileNameOffset[] = {"offset.txt"};
static volatile uint32_t logWriteOffset = 0;    // The write offset for the log so it can wrap around when the log is full and old logging data should be overwritten.
static uint32_t writeOffsetStorageCounter = 0; 
volatile bool logOffsetRollOver = false;
volatile uint32_t lastSaveTimeLogOffset = UINT32_MAX;
volatile uint32_t lastLogOffsetTime = 0;
/**
 * @brief  QSPI block device definition
 */
NRF_BLOCK_DEV_QSPI_DEFINE(
    m_block_dev_qspi,
    NRF_BLOCK_DEV_QSPI_CONFIG(
        512,
        NRF_BLOCK_DEV_QSPI_FLAG_CACHE_WRITEBACK,
        NRF_DRV_QSPI_DEFAULT_CONFIG
     ),
     NFR_BLOCK_DEV_INFO_CONFIG("Nordic", "QSPI", "1.00")
);


#define BLOCKDEV_LIST() (                                   \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_ram, block_dev),     \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_empty, block_dev),   \
    NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev)     \
)


static FATFS m_filesystem;



static bool fatfs_mkfs(void)
{
    FRESULT ff_result;

    
    static uint8_t buf[512];
    ff_result = f_mkfs("", FM_FAT, 1024, buf, sizeof(buf));

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Creating filesystem...%s", (ff_result == FR_OK) ? "succeeded" : "failed");
    #endif

    if (ff_result != FR_OK)
    {
        return false;
    }

    ff_result = f_mount(&m_filesystem, "", 1);
        
    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Mounting volume...%s", (ff_result == FR_OK) ? "succeeded" : "failed");
    #endif
    if (ff_result != FR_OK)
    {
        return false;
    }
    return true;
}

void fatfs_uninit(void)
{
    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Un-initializing disk 0 (QSPI)...");
    #endif
    UNUSED_RETURN_VALUE(disk_uninitialize(0));
}


bool fatfs_init(void)
{
    FRESULT ff_result;
    DSTATUS disk_state = STA_NOINIT;

    memset(&m_filesystem, 0, sizeof(FATFS));

    // Initialize FATFS disk I/O interface by providing the block device.
    static diskio_blkdev_t drives[] =
    {
        DISKIO_BLOCKDEV_CONFIG(NRF_BLOCKDEV_BASE_ADDR(m_block_dev_qspi, block_dev), NULL)
    };

    diskio_blockdev_register(drives, ARRAY_SIZE(drives));

    #if MX_FLASH_DEBUG
    // NRF_LOG_INFO("Initializing disk 0 (QSPI)...");
    #endif
    disk_state = disk_initialize(0);
    if (disk_state)
    {
        #if MX_FLASH_DEBUG
            NRF_LOG_ERROR("Disk initialization failed.");
        #endif
        return false;
    }

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Mounting volume...");
    #endif

    ff_result = f_mount(&m_filesystem, "", 1);
    if (ff_result != FR_OK)
    {
        if (ff_result == FR_NO_FILESYSTEM)
        {
            #if MX_FLASH_DEBUG
                NRF_LOG_ERROR("Mount failed. Filesystem not found. Please format device.");
            #endif

            // Create  a new file system.    
            fatfs_mkfs();

            ff_result = f_mount(&m_filesystem, "", 1);
            return (ff_result == FR_OK) ? true : false;
        }
        else
        {
            #if MX_FLASH_DEBUG
                NRF_LOG_ERROR("Mount failed: %u", ff_result);
            #endif
            return false;
        }
    }
    
    return true;
}




FRESULT fatfs_open(FIL * fp, char * fname, FSIZE_t offset)
{
    DIR dir;
    FRESULT ff_result;
    FILINFO stat;
    
    UINT    bytesRead = 0;

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Listing directory: /");
    #endif
    ff_result = f_opendir(&dir, "/");
    if (ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            NRF_LOG_ERROR("Directory listing failed: %u", ff_result);
        #endif
        return ff_result;
    }

    // Open the file
    ff_result = f_open(fp, fname, FA_READ);
    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("Opening the file didn't succeed: %u", ff_result);
        #endif
        return ff_result;
    }  
         
    ff_result = f_stat(fname, &stat);
    if(ff_result != FR_OK)
    {
        return ff_result;
    }

    // Check if the offsetis larger or equal to the file size.
    if(offset >= stat.fsize)
    {
        #if 1 // MX_FLASH_DEBUG
            // NRF_LOG_INFO("Invalid offset: %u", ff_result);
        #endif

        #if 1
            offset = 0; 
        #else   
            return FR_INVALID_PARAMETER;
        #endif
    }
    
    ff_result = f_lseek(fp, offset);
    return ff_result;
}



FRESULT fatfs_close(FIL * fp)
{
    FRESULT ff_result;
    
    ff_result = f_close(fp);	/* close the file */
    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("closing the file didn't succeed: %u", ff_result);
        #endif
    }
    return ff_result;
}


FRESULT fatfs_readBuffer(FIL * fp, char * readBuff, UINT size, UINT * bytesRead)
{
    FRESULT ff_result;
 
    //  Read data from the file
    ff_result = f_read(fp, (void*) readBuff, size, bytesRead);
    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("Opening the file didn't succeed: %u", ff_result);
        #endif
    }
    else
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("Bytes Read: %u", *bytesRead);
            // NRF_LOG_HEXDUMP_INFO(readBuff, *bytesRead);
            NRF_LOG_FLUSH();
        #endif
    }
    return ff_result;
}


void fatfs_read(void)
{
    DIR dir;
    FRESULT ff_result;
    FIL fp;
    UINT    bytesRead = 0;

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Listing directory: /");
    #endif
    ff_result = f_opendir(&dir, "/");
    if (ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            NRF_LOG_ERROR("Directory listing failed: %u", ff_result);
        #endif
        return;
    }

    // Open the file
    ff_result = f_open(&fp, filename, FA_READ);
    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("Opening the file didn't succeed: %u", ff_result);
        #endif
        return;
    }


    char readBuff[100] = {0};
    // FRESULT f_read (FIL* fp, void* buff, UINT btr, UINT* br);			/* Read data from the file */
    do
    {
        ff_result = f_read(&fp, (void*) readBuff, sizeof(readBuff), &bytesRead);
        if(ff_result != FR_OK)
        {
            #if MX_FLASH_DEBUG
                // NRF_LOG_INFO("Opening the file didn't succeed: %u", ff_result);
            #endif
            return;
        }
        else
        {
            #if MX_FLASH_DEBUG
                // NRF_LOG_INFO("Bytes Read: %u", bytesRead);
                // NRF_LOG_HEXDUMP_INFO(readBuff, bytesRead);
                NRF_LOG_FLUSH();
            #endif
        }
    // Continue to read the file contents until all data has been read
    }while(bytesRead != 0 && (100 == bytesRead));


    ff_result = f_close(&fp);	/* close the file */
    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("closing the file didn't succeed: %u", ff_result);
        #endif
        return;
    }
}



UINT fatfs_write(char * dataToWrite, UINT lenghtToWrite)
{
    DIR dir;
    FRESULT ff_result = FR_OK;
    FIL fp;
    FILINFO f_statics;
    UINT    bytesWritten = 0;

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Listing directory: /");
    #endif

    ff_result = f_opendir(&dir, "/");

    if (ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            NRF_LOG_ERROR("Directory listing failed: %u", ff_result);
        #endif
        return 0;
    }

    // Open te file, but do not set the write pointer to the end of the file.
    ff_result = f_open(&fp, filename, FA_WRITE | FA_READ);	/* Open or create a file */

    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("Opening the file didn't succeed: %u", ff_result);
        #endif
        return 0;
    }

    // Fall through if the previous operation was unsuccesfull, so the file is closed
    if(ff_result == FR_OK)
    {
        // If the beeplog file excist, retrieve the file statics
        ff_result = f_stat(filename, &f_statics);
    }

    // Fall through if the previous operation was unsuccesfull, so the file is closed
    if(ff_result == FR_OK)
    {
        /* When the log is not kept open, check the size of the file and the logOffset value.
         * When a valid writeOffset pointer is given, set the write location to the given value if it doesn't go beyond the file size.
        */
        if(logWriteOffset > f_statics.fsize)
        {
            // NRF_LOG_INFO("Log write Offset is invalid=%u > fileSize=%u", logWriteOffset, f_statics.fsize);

            // Prevent using a logWriteOffset larger than the file currently is, so f_lseek doesn't expand the file.
            logWriteOffset = f_statics.fsize;
        }

        FSIZE_t offset = logWriteOffset;
        f_lseek(&fp, offset);
    }

    // Fall through if the previous operation was unsuccesfull
    if(ff_result == FR_OK)
    {
        ff_result = f_write(&fp, (const void*) dataToWrite, lenghtToWrite, &bytesWritten);
        if(ff_result != FR_OK)
        {
            #if MX_FLASH_DEBUG
                // NRF_LOG_INFO("Writing the file didn't succeed: %u", ff_result);
            #endif
        }
        else
        {
            // If writing is succesfull, return the current write pointer value.
            logWriteOffset = (uint32_t) f_tell(&fp);
            writeOffsetStorageCounter++;
             #if MX_FLASH_DEBUG
                // NRF_LOG_INFO("Bytes Written: %u, new Offset=%u, storageCount=%u", bytesWritten, logWriteOffset, writeOffsetStorageCounter);
            #endif
        }
    }


    ff_result = f_close(&fp);	/* close the file */
    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("closing the file didn't succeed: %u", ff_result);
        #endif
        bytesWritten = 0;
    }

    // If the bytes could not be written
    if(lenghtToWrite != bytesWritten)
    {
        FRESULT res;
        FATFS *fs;
        DWORD fre_clust, fre_sect, tot_sect;


        /* Get volume information and free clusters of drive 1 */
        res = f_getfree("", &fre_clust, &fs);

        /* Get total sectors and free sectors */
        tot_sect = ((fs->n_fatent - 2) * fs->csize) / 2;
        fre_sect = (fre_clust * fs->csize) / 2;

        // When less then 10 KiB is available, restart the flash log. 
        if(fre_sect < 10)
        {
            // NRF_LOG_INFO("!!WriteOffset roll-over flash full: %u!!", logWriteOffset);
            NRF_LOG_FLUSH();
            logWriteOffset = 0;
            logOffsetRollOver = true;
            writeLogOffsetToFile(logWriteOffset);
        }
    }

    // Check if the new writeOffset is above the maximum size and reset it to zero.
    if(logWriteOffset > LOG_SIZE_MAX)
    {
        // NRF_LOG_INFO("!!WriteOffset roll-over: %u!!", logWriteOffset);
        NRF_LOG_FLUSH();
        logWriteOffset = 0;
        logOffsetRollOver = true;
        writeLogOffsetToFile(logWriteOffset);
    }

    /*
     * Store the writeOffset to the flash every 96 stores, when logging ever 15 minutes this is exactly a day. This
     * should reduce flash wear from saving the writeOffset after each succesfull flash storage.
     * If the log file size isn't at the maximum value, the init function will detect the discrepenacy and set the 
     * writeOffset to the filesize.
     * When the log file is at it's maximum size, a power reset can thus cause a data loss of up to 95 measurements,
     * since the init checks can't check if there's new data after the given writeOffset value.
     */
    if(writeOffsetStorageCounter > 96)
    {
        writeOffsetStorageCounter = 0; // Will also be cleared with each writeLogOffsetToFile() call

        #if 1
            const uint32_t currentTime = app_timer_cnt_get();
            if(lastLogOffsetTime != UINT32_MAX)
            {
                const uint64_t diffBytes = app_timer_cnt_diff_compute(logWriteOffset, lastSaveTimeLogOffset);
                const uint64_t diffTicks = app_timer_cnt_diff_compute(currentTime, lastLogOffsetTime);
                const uint64_t f = 1000UL;
                const uint64_t diffTime_ms = ((diffTicks * f) / APP_TIMER_CLOCK_FREQ);
                const uint64_t bytesToWrite = LOG_SIZE_MAX - logWriteOffset;

                const uint64_t timeRemaining_ms = (bytesToWrite * diffTime_ms) / diffBytes;
                const uint32_t timeRemining_min = timeRemaining_ms / 60000UL;

                // NRF_LOG_INFO("dBytes=%u, dT=%u/%u ms, toWrite=%u, TimeRemain=%u Min", diffBytes, diffTicks, diffTime_ms, bytesToWrite, timeRemining_min);
            }

            lastLogOffsetTime = currentTime;
            lastSaveTimeLogOffset = logWriteOffset;
        #endif 

        writeLogOffsetToFile(logWriteOffset);
        // NRF_LOG_INFO("WriteOffset auto stored: %u", logWriteOffset);
    }

    return bytesWritten;
}



static void fatfs_ls(void)
{
    DIR dir;
    FRESULT ff_result;
    FILINFO fno;
    uint16_t dirCount = 0;

    #if MX_FLASH_LS_DEBUG
        // NRF_LOG_INFO("Listing directory: /");
    #endif
    ff_result = f_opendir(&dir, "/");
    if (ff_result != FR_OK)
    {
        #if MX_FLASH_LS_DEBUG
            NRF_LOG_ERROR("Directory listing failed: %u", ff_result);
        #endif
        return;
    }

    uint32_t entries_count = 0;
    do
    {
        ff_result = f_readdir(&dir, &fno);
        if (ff_result != FR_OK)
        {
            #if MX_FLASH_LS_DEBUG
                NRF_LOG_ERROR("Directory read failed: %u", ff_result);
            #endif
            return;
        }

        if (fno.fname[0])
        {
            #if MX_FLASH_LS_DEBUG
                if (fno.fattrib & AM_DIR)
                {
                    // NRF_LOG_INFO("%u   <DIR>   %s", dirCount++, (uint32_t)fno.fname);
                }
                else
                {
                    // NRF_LOG_INFO("%s, %9lu bytes", (uint32_t)fno.fname, fno.fsize);
                }
            #endif
        }

        ++entries_count;
        NRF_LOG_FLUSH();
    } while (fno.fname[0]);

    #if MX_FLASH_LS_DEBUG
        // NRF_LOG_INFO("Entries count: %u", entries_count);
        NRF_LOG_FLUSH();
    #endif
}

FRESULT delete_node (
    TCHAR* path,    /* Path name buffer with the sub-directory to delete */
    UINT sz_buff,   /* Size of path name buffer (items) */
    FILINFO* fno    /* Name read buffer */
)
{
    UINT i, j;
    FRESULT fr;
    DIR dir;


    fr = f_opendir(&dir, path); /* Open the directory */
    if (fr != FR_OK) return fr;

    for (i = 0; path[i]; i++) ; /* Get current path length */
    path[i++] = _T('/');

    for (;;) {
        fr = f_readdir(&dir, fno);  /* Get a directory item */
        if (fr != FR_OK || !fno->fname[0]) break;   /* End of directory? */
        j = 0;
        do {    /* Make a path name */
            if (i + j >= sz_buff) { /* Buffer over flow? */
                fr = 100; break;    /* Fails with 100 when buffer overflow */
            }
            path[i + j] = fno->fname[j];
        } while (fno->fname[j++]);
        if (fno->fattrib & AM_DIR) {    /* Item is a directory */
            fr = delete_node(path, sz_buff, fno);
        } else {                        /* Item is a file */
            fr = f_unlink(path);
        }
        if (fr != FR_OK) break;
    }

    path[--i] = 0;  /* Restore the path name */
    f_closedir(&dir);

    if (fr == FR_OK) fr = f_unlink(path);  /* Delete the empty directory */
    return fr;
}


void fatfs_clearAll(void)
{
    DIR dir;
    FRESULT ff_result;
    FILINFO fno;
    uint16_t dirCount = 0;

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Listing directory: /");
    #endif
    ff_result = f_opendir(&dir, "/");
    if (ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            NRF_LOG_ERROR("Directory listing failed: %u", ff_result);
        #endif
        return;
    }

    uint32_t entries_count = 0;
    do
    {
        ff_result = f_readdir(&dir, &fno);
        if (ff_result != FR_OK)
        {
            #if MX_FLASH_DEBUG
                NRF_LOG_ERROR("Directory read failed: %u", ff_result);
            #endif
            return;
        }

        if (fno.fname[0])
        {
            #if MX_FLASH_DEBUG
                if (fno.fattrib & AM_DIR)
                {
                    ff_result = f_unlink(fno.fname);
                    // NRF_LOG_INFO("%u   <DIR>   %s, unlink result: %u", dirCount++, (uint32_t)fno.fname, ff_result);
                }
                else
                {
                    ff_result = f_unlink(fno.fname);
                    // NRF_LOG_INFO("%s, %9lu bytes, unlink result: %u", (uint32_t)fno.fname, fno.fsize, ff_result);
                }
            #endif
        }

        ++entries_count;
        NRF_LOG_FLUSH();
    } while (fno.fname[0]);

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Entries count: %u", entries_count);
        NRF_LOG_FLUSH();
    #endif
}



FRESULT fatfs_clear(void)
{
    DIR dir;
    FRESULT ff_result;
    FILINFO fno;

    #if MX_FLASH_DEBUG
        // NRF_LOG_INFO("Listing directory: /");
    #endif

    fatfs_init();

    ff_result = f_opendir(&dir, "/");
    if (ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            NRF_LOG_ERROR("Directory listing failed: %u", ff_result);
        #endif
        MXflash_FSdeinit();
        return ff_result;
    }

    uint32_t entries_count = 0;
    do
    {
        ff_result = f_readdir(&dir, &fno);
        if (ff_result != FR_OK)
        {
            #if MX_FLASH_DEBUG
                NRF_LOG_ERROR("Directory read failed: %u", ff_result);
            #endif
            MXflash_FSdeinit();
            return ff_result;
        }

        if (fno.fname[0] != NULL)
        {
            #if MX_FLASH_DEBUG
            if (fno.fattrib & AM_DIR)
            {
                // NRF_LOG_INFO("   <DIR>   %s",(uint32_t)fno.fname);
            }
            else
            {
                // NRF_LOG_INFO("%9lu  %s", fno.fsize, (uint32_t)fno.fname);
            }
            #endif

            ff_result = f_unlink(fno.fname);
            #if MX_FLASH_DEBUG
                NRF_LOG_ERROR("Unlinking file %s status: %u, %s", fno.fname, ff_result, (ff_result == FR_OK) ? "succeeded" : "failed");
            #endif
        }


        ++entries_count;
        NRF_LOG_FLUSH();
    } while (fno.fname[0]);


    ff_result = f_closedir(&dir);

    #if MX_FLASH_DEBUG
        NRF_LOG_ERROR("close dir status: %u, %s", ff_result, (ff_result == FR_OK) ? "succeeded" : "failed");
    #endif

    #if 0// MX_FLASH_DEBUG
        ff_result = f_unlink("/");
        NRF_LOG_ERROR("Unlinking directory status: %u, %s", ff_result, (ff_result == FR_OK) ? "succeeded" : "failed");
        // NRF_LOG_INFO("Entries count: %u", entries_count);
    #endif

    MXflash_FSdeinit();
    return ff_result;
}



static void fatfs_file_create(void)
{
    FRESULT ff_result;
    FIL file;
    

    // NRF_LOG_INFO("Creating file: %s ...", (uint32_t)filename);
    NRF_LOG_FLUSH();

    ff_result = f_open(&file, filename, FA_CREATE_NEW | FA_WRITE);
    if(ff_result == FR_OK)
    {
        // Clear the write Offset to zero when a new log file is created.
        logWriteOffset = 0;
        writeLogOffsetToFile(logWriteOffset);
    }
    else if(ff_result == FR_EXIST)
    {
        NRF_LOG_ERROR("File with name: %s already exists: %u", filename, ff_result);
        NRF_LOG_FLUSH();
        return;
    }
    else if (ff_result != FR_OK)
    {
        NRF_LOG_ERROR("Unable to open or create file: %u", ff_result);
        NRF_LOG_FLUSH();
    }

    ff_result = f_close(&file);
    if (ff_result != FR_OK)
    {
        NRF_LOG_ERROR("Unable to close file: %u", ff_result);
        NRF_LOG_FLUSH();
    }
}





FRESULT writeLogOffsetToFile(const uint32_t writeOffset)
{
    FRESULT ff_result;
    FIL file;
    UINT bytesWritten = 0;
    
    // Attempt to open the file and create it if it doesn' excist
    ff_result = f_open(&file, fileNameOffset, FA_OPEN_ALWAYS | FA_WRITE);
    if (ff_result != FR_OK)
    {
        NRF_LOG_ERROR("Unable to open or create file: %u", ff_result);
        NRF_LOG_FLUSH();
        return ff_result;
    }

    //  Read the offset from the file
    uint8_t buffer[10] = {0};
    uint32_big_encode(writeOffset, buffer);
    ff_result = f_write(&file, (const void*) buffer, sizeof(writeOffset), &bytesWritten);
    if(ff_result != FR_OK)
    {
        #if MX_FLASH_DEBUG
            // NRF_LOG_INFO("Writing the WOfile didn't succeed: %u", ff_result);
            NRF_LOG_FLUSH();
        #endif
        f_close(&file);
        return ff_result;
    }

    ff_result = f_close(&file);
    if (ff_result != FR_OK)
    {
        NRF_LOG_ERROR("Unable to close WOfile: %u", ff_result);
        NRF_LOG_FLUSH();
    }

    logWriteOffset = writeOffset;
    writeOffsetStorageCounter = 0;
    // NRF_LOG_INFO("Write Offset saved=%u", writeOffset);
    return ff_result;
}


/*@Brief: Wrapper function to store the current logOffsetValue
 */
FRESULT storeLogOffset(void)
{
    return writeLogOffsetToFile(logWriteOffset);
}


FRESULT readLogOffsetFromFile(volatile uint32_t * offset_p)
{
    FRESULT ff_result;
    FIL file;
    uint32_t offset = 0;
    UINT bytesRead = 0;

    // Check if the given pointer is invalid
    if(offset_p == NULL)
    {
        return FR_INVALID_PARAMETER;
    }
    
    // Attempt to open the file and create it if it doesn' excist
    ff_result = f_open(&file, fileNameOffset, FA_READ);
    if(ff_result == FR_EXIST)
    {
        NRF_LOG_ERROR("File %s exists", fileNameOffset);
    }
    else if (ff_result != FR_OK)
    {
        NRF_LOG_ERROR("Unable to open or create file: %u", ff_result);
        NRF_LOG_FLUSH();
        return ff_result;
    }

    //  Read the offset from the file
    uint8_t buffer[10] = {0};
    UINT btr = sizeof(offset);
    ff_result = f_read(&file, (void*) &buffer, btr, &bytesRead);
    if(ff_result != FR_OK)
    {
        // Read operation did not succeed. Still need to close the file.
        f_close(&file);
        return ff_result;
    }
    else
    {
        // Read operation succeeded, retrieve the value.
        offset = uint32_big_decode(buffer);
        (*offset_p) = offset;
        // NRF_LOG_INFO("LogOffset read: %u/0x%08X", offset, offset);
    }

    ff_result = f_close(&file);
    if (ff_result != FR_OK)
    {
        NRF_LOG_ERROR("Unable to close file: %u", ff_result);
        NRF_LOG_FLUSH();
    }
    return ff_result;
}


volatile uint32_t getCurrentWriteOffset(void)
{
    return logWriteOffset;
}


void static writeOffsetInit()
{
    FRESULT ret = 0;
    FILINFO f_statics;
    FRESULT ret_statics = 0;
    FRESULT ret_getFree = 0;
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;
    bool store = false;
    bool noSpaceAvailable = false;

    // If the beeplog file excist, retrieve the file statics
    ret_statics = f_stat(filename, &f_statics);

    /* Get volume information and free clusters of drive 1 */
    ret_getFree = f_getfree("", &fre_clust, &fs);

    if(ret_getFree == FR_OK)
    {
        /* Get total sectors and free sectors */
        tot_sect = ((fs->n_fatent - 2) * fs->csize) / 2;
        fre_sect = (fre_clust * fs->csize) / 2;

        // Check if less than 10KiB space is available.
        noSpaceAvailable = (fre_sect < 10) ? true : false;
    }

   
    // Attempt to read the low write offset. If the file doesn't excist or the value is incomprehensible, overwrite the file.
    ret = readLogOffsetFromFile(&logWriteOffset);
    if(ret == FR_OK && ret_statics == FR_OK)
    {
        // Check if the logWriteOffset is invalid when the value is larger than the file size.
        if(f_statics.fsize < logWriteOffset)
        {
            // Check if the maximum log size is reached or no space is available.
            if(f_statics.fsize >= LOG_SIZE_MAX || noSpaceAvailable)
            {
                // Start at zero when the logWriteOffset is larger than the threshold
                logWriteOffset = 0;
            }
            else
            {
                // Use the maximum size of the file when it's still below the threshold
                logWriteOffset = f_statics.fsize;
            }
            store = true;
        }
        // Check if the writeOffset is smaller then the log file size and the logfile is smaller than the maximum log size.
        else if(f_statics.fsize > logWriteOffset && f_statics.fsize < LOG_SIZE_MAX)
        {
            logWriteOffset = f_statics.fsize;
            store = true;
        }

        // Check if the maximum log size is reached.
        if(logWriteOffset >= LOG_SIZE_MAX)
        {
            // Start at zero when the logWriteOffset is invalid
            logWriteOffset = 0;
            store = true;
        }
    }
    else
    {
        // Set the writeLogOffset to the beeplog size when the beep log excists and otherwise to zero.
        if(ret_statics == FR_OK)
        {
            logWriteOffset = f_statics.fsize;

            // Check if the maximum log size is reached.
            if(logWriteOffset >= LOG_SIZE_MAX)
            {
                // Start at zero when the logWriteOffset is invalid
                logWriteOffset = 0;
            }
        }
        else
        {
            // When the log file doesn't excist, set the logWriteOffset to 0.
            logWriteOffset = 0;
        }
        store = true;
    }

    if(store)
    {
        writeLogOffsetToFile(logWriteOffset);
    }
}


static void MXflash_getFree(void)
{
    FRESULT res;
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;

    /* Get volume information and free clusters of drive 1 */
    res = f_getfree("", &fre_clust, &fs);

    /* Get total sectors and free sectors */
    tot_sect = ((fs->n_fatent - 2) * fs->csize) / 2;
    fre_sect = (fre_clust * fs->csize) / 2;

    /* Print the free space (assuming 512 bytes/sector) */
    // NRF_LOG_INFO("%lu KiB total drive space. %lu KiB available.", tot_sect, fre_sect);
    NRF_LOG_FLUSH();
    NRF_LOG_FLUSH();
}



void MXflash_FSinit(void)
{
    if(fatfs_init())
    {
        // List the files in the filesystem
        fatfs_file_create();
        fatfs_ls();

        MXflash_getFree();

        writeOffsetInit();
        #if 0
            fatfs_read();
        #endif
        MXflash_FSdeinit();
    }
}


void MXflash_FSdeinit(void)
{
    fatfs_uninit();
    /* Fix for the MX waking-up on unint of the QSPI interface, despite setting the CSN pin as an output and high before calling the uninit. See:
     * https://devzone.nordicsemi.com/f/nordic-q-a/42609/disabling-qspi-draws-much-current-sdk15
     * https://devzone.nordicsemi.com/f/nordic-q-a/33563/qspi-drawing-more-power-when-disabled
     */
    flash_deepPowerDown();
}


