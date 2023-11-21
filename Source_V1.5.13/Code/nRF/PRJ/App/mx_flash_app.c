#define NRF_LOG_MODULE_NAME MX_APP
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "mx_flash_app.h"
#include "mx_flash_fs.h"
#include "nrf_log_ctrl.h"
#include "nrf.h"
#include "nvm_fs.h"
#include "BLE_setup.h"
#include "app_timer.h"
#include "app_util.h"
#include "main.h"
#include "gpio-board.h"
#include "nrf_delay.h"
#include "log_time.h"
#include "power_app.h"
#include "ds3231_app.h"


struct LINE_s
{
    bool            claimed;
    uint8_t         index;
    uint8_t         dataBytes;
    uint8_t         buff[BLE_NUS_MAX_DATA_LEN];
}line;

volatile app_flash_evt_type_t  whileCMD       = UINT8_MAX;
volatile CONTROL_SOURCE        whileCMDsource = INTERNAL_SOURCE;
volatile uint32_t   whileAttempt;
volatile uint32_t   newOffset;
FLASH_BUFFs         buff;
volatile bool       readNewBufferData = false;
volatile bool       clientDisconnected = false;
volatile bool       TXnotificationEnabled = false; 
char                fileName[] = "BEEPlog.txt";
uint32_t			timestampStartErase;
FIL                 fp;
READ_DATA_STATEs    readState = READ_IDLE;
uint32_t            bytesTransferred;
uint32_t            startTimestamp;
uint8_t				eraseType;
FSIZE_t             read_offset;
UINT                bytesRead = 0;

volatile bool       writeTimeChangedLog = false;
time_t              oldTimeStamp, newTimeStamp;


ADC_s log_adc = {.battVoltage_mV = 3000, .vccVoltage_mV = 3300, .battPercentage = 100};
HX711_CONV_s log_hx = {.channel=0, .samples=1, .value={0}};
DS18B20_RESULTS_s log_ds = {.devices= 1, .temperatures={0}};
BME280_RESULT_s	  log_bme = {.humidity=500, .temperature=0, .airPressure=1000};
FFT_RESULTS     log_fft = {.bins=10, .start=0, .stop=255, .values={0}};    
extern volatile bool logOffsetRollOver; // declared in mx_flash_fs.c
uint32_t updateFillLogOffset = 0;


void flash_newLineClear(void)
{
    memset(line.buff, 0, BLE_NUS_MAX_DATA_LEN);
    line.index   = 0;
    line.claimed = false;
}


bool flash_newLineStart(BEEP_STATUS status)
{
    if(line.claimed)
    {
        return false;
    }

    flash_newLineClear();
    line.claimed    = true;
    line.buff[0]    = (uint8_t) status;
    line.buff[1]    = 0; // data bytes
    line.index      = 2;
    line.dataBytes  = 0;
    return true;
}


bool flash_newLineAppendBeep(BEEP_protocol_s * prot)
{
    uint32_t ret;
    

    if(!line.claimed && line.index < BLE_NUS_MAX_DATA_LEN)
    {
        return false;
    }

    #if 1 // 1= binair, 0 = ASCII
        ret = beep_protocol_encode(true, prot, &line.buff[line.index], &line.dataBytes, BLE_NUS_MAX_DATA_LEN);
        if(ret != NRF_SUCCESS)
        {
            return false;
        }

    #else 
        // Encode the data according to the protocol
        uint8_t i, index = 0;
        uint8_t tempHex[BLE_NUS_MAX_DATA_LEN] = {0};

        ret = beep_protocol_encode(true, prot, tempHex, &index, BLE_NUS_MAX_DATA_LEN);
        if(ret != NRF_SUCCESS)
        {
            return false;
        }

        // Add the data in ASCII hex to the storage array
        for(i=0; i<index; i++)
        {
            line.index += uint8_ASCIIencode(tempHex[i], &line.buff[line.index], &line.dataBytes);
        }
    #endif

    return true;
}


bool flash_newLineEnd(void)
{
    UINT bytesWritten;
    if(!line.claimed)
    {
        return false;
    }

	if(whileCMD != UINT8_MAX && whileCMD != APP_FLASH_FILL_LOG_B)
	{
		line.claimed = false;
		return false;
	}

    // Add the number of data bytes in the message to the array on index[1]
    line.buff[1] = line.dataBytes;
    line.index  += line.dataBytes;

    // Add the line end character.
    line.buff[line.index++] = '\n';

    // Write the data to the Flash
    bytesWritten = fatfs_write(line.buff, (UINT) line.index);

    // check if all bytes have been written
    if(bytesWritten != line.index)
    {
        NRF_LOG_INFO("The number of bytes written to the flash is NOK: Line lenght:%u, Written: %u", line.index, bytesWritten);
        line.claimed = false;
        return false;
    }
    #if MX_FLASH_DEBUG
        NRF_LOG_INFO("The number of bytes written to the flash is: %u, %u databytes", bytesWritten, line.dataBytes);
        NRF_LOG_HEXDUMP_INFO(line.buff, line.index);
        NRF_LOG_FLUSH();
        nrf_delay_ms(10);
    #endif

    // Release the line buffer. Will be cleared upon claiming.
    line.claimed = false;
    return true;
}


bool flash_WriteStartUpLine(void)
{
    BEEP_protocol_s prot[STARTUP_LINE_ELEMENTS];
    BEEP_CID cmds[STARTUP_LINE_ELEMENTS_MIN] = {READ_FIRMWARE_VERSION, READ_HARDWARE_VERSION, READ_ATECC_READ_ID, READ_BOOT_COUNT, READ_DS18B20_STATE, READ_APPLICATION_CONFIG};
    uint8_t i;

    // Retrieve all command parameters from the EEPROM
    for(i=0; i<STARTUP_LINE_ELEMENTS_MIN; i++)
    {
        memset(&prot[i], 0, sizeof(BEEP_protocol_s));
        prot[i].command = cmds[i];
        nvm_fds_eeprom_get(&prot[i]);
    }

    // Set the log time from DS3231 or from internal RTC
    memset(&prot[STARTUP_LINE_ELEMENTS_MIN], 0, sizeof(BEEP_protocol_s));
    if(ds3231_enabled == 0)
    {
    prot[STARTUP_LINE_ELEMENTS_MIN].command     = READ_TIME;
    }
    if(ds3231_enabled == 1)
    {
    prot[STARTUP_LINE_ELEMENTS_MIN].command     = READ_TIME_DS3231;
    }


    prot[STARTUP_LINE_ELEMENTS_MIN].param.time  = get_logtime_value();


    return flash_Write_BeepProtocol(BEEP_SENSOR_ON, prot, STARTUP_LINE_ELEMENTS);
}

void flash_queWriteTimeChanged(time_t  oldTime, time_t newTime)
{
    oldTimeStamp        = oldTime;
    newTimeStamp        = newTime;
    writeTimeChangedLog = true;
}

bool flash_WriteTimeChange(time_t oldTime, time_t newTime)
{ 
    BEEP_protocol_s Time[TIME_ARRAY_LENGHT];

    memset(&Time, 0, sizeof(BEEP_protocol_s) * TIME_ARRAY_LENGHT);
    if(ds3231_enabled == 1)
    {
    Time[0].command     = READ_TIME_DS3231;
    Time[0].param.time  = oldTime;
    Time[1].command     = READ_TIME_DS3231;
    Time[1].param.time  = newTime;
    }
    if(ds3231_enabled == 0)
    {
    Time[0].command     = READ_TIME;
    Time[0].param.time  = oldTime;
    Time[1].command     = READ_TIME;
    Time[1].param.time  = newTime;
    }
    NRF_LOG_INFO("Flash Time changed! old:%u new:%u", oldTime, newTime);

    return flash_Write_BeepProtocol(BEEP_TIME_CHANGE, Time, TIME_ARRAY_LENGHT);
}



FRESULT flash_info(FILINFO * fno)
{
    FRESULT retVal;

    if(fno == NULL)
    {
        return FR_INVALID_PARAMETER;
    }
    
    retVal = f_stat(fileName, fno);
    return retVal;
}


uint32_t flash_size(void)
{
    FRESULT retVal;
    FILINFO stat;
    
    retVal = f_stat(fileName, &stat);
    return stat.fsize;
}




bool flash_Write_BeepProtocol(BEEP_STATUS status, BEEP_protocol_s * param, uint8_t lenght)
{
    bool retBool;
    uint8_t i;

    if(beep_flash_busy()){
        return false;
    }

    // Initialize the filesystem and the QSPI interface. Disable them on error or on completion.
    fatfs_init();

    if(!flash_newLineStart(status))
    {
        MXflash_FSdeinit();
        return false;
    }

    // Retrieve all command parameters from the EEPROM and add them to the newLine 
    for(i=0; i<lenght; i++)
    {
        retBool = flash_newLineAppendBeep(&param[i]);
    }

    flash_newLineEnd();

    MXflash_FSdeinit();
    return true;
}


uint32_t flashApp_cmd_handler(app_flash_evt_type_t type, BEEP_protocol_s * prot)
{
    uint32_t err;

	if(whileCMD != UINT8_MAX && type != APP_FLASH_STOP_FILL)
	{
		return NRF_ERROR_BUSY;
	}

    switch(type)
    {
        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_READ:
        {
            TXnotificationEnabled = ble_beep_notificationEnabled();

            if(TXnotificationEnabled)
            {
                #if MX_FLASH_APP_LOG_ENABLED
                    NRF_LOG_INFO("READ_START Handler");  
                #endif
                ble_beep_clearPackerCounter();
                whileCMD        = APP_FLASH_READ;
                whileCMDsource  = prot->source;
                read_offset     = prot->param.size;
            }
            else
            {
                #if MX_FLASH_APP_LOG_ENABLED
                    NRF_LOG_INFO("Beep TX notification not enabled.");  
                #endif
                return NRF_ERROR_INVALID_STATE;
            }
            break;
        }

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_ERASE:
            #if MX_FLASH_APP_LOG_ENABLED
                NRF_LOG_INFO("ERASE_FS");
            #endif
            whileCMDsource  = prot->source;
            whileCMD        = (prot->param.status.statusflag != 0) ? APP_FLASH_MX_ERASE_A : APP_FLASH_FATFS_ERASE;
            whileAttempt    = 0;
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_SIZE_FLASH:
            whileCMDsource  = prot->source;
            whileCMD        = APP_FLASH_SIZE_FLASH;
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_TX_RDY_FLASH:
            #if MX_FLASH_APP_LOG_ENABLED
                NRF_LOG_INFO("APP_TX_RDY_FLASH, bytes read: %u, transferred: %u", bytesRead, bytesTransferred);
            #endif
            readNewBufferData = true;
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_COMM_CONNECTED:
            #if MX_FLASH_APP_LOG_ENABLED
                NRF_LOG_INFO("APP_FLASH_COMM_CONNECTED");
            #endif
            clientDisconnected = false;
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_COMM_DISCONNECTED:
            #if MX_FLASH_APP_LOG_ENABLED
                NRF_LOG_INFO("APP_FLASH_COMM_DISCONNECTED");
            #endif
            clientDisconnected = true;
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_COMM_STARTED:
            // Notification from the BEEP service that a connection have been established and notifications have been enabled.
            TXnotificationEnabled = true;
            #if MX_FLASH_APP_LOG_ENABLED
                NRF_LOG_INFO("APP_FLASH_COMM_STARTED");
            #endif
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_COMM_STOPPED:
            // Notification from the BEEP service that the connection is disabled and notifications have been disabled.
            TXnotificationEnabled = false;
            #if MX_FLASH_APP_LOG_ENABLED
                NRF_LOG_INFO("APP_FLASH_COMM_STOPPED");
            #endif
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_READ_OFFSET:
            whileCMDsource  = prot->source;
            whileCMD        = type;
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_FILL_LOG_A:
            whileCMDsource  = prot->source;
            whileCMD        = type;
            logOffsetRollOver = false;  // Clear the boolean. Will be set when the logOffset reaches it's limit.
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_STOP_FILL:
            if(whileCMD != APP_FLASH_FILL_LOG_A && whileCMD != APP_FLASH_FILL_LOG_B)
            {
                return NRF_ERROR_INVALID_STATE;
            }
            else
            {
                NRF_LOG_INFO("Stopping Log Fill action");
                logOffsetRollOver = true;
            }
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        case APP_FLASH_WRITE_OFFSET:
            whileCMDsource  = prot->source;
            whileCMD        = type;
            newOffset       = prot->param.size;
            break;

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
        default:
            return NRF_ERROR_INVALID_ADDR;
            break;
    }
    return NRF_SUCCESS;
}


bool beep_flash_busy(void)
{
    return (readState == READ_ACCESS_FILE) || (whileCMD != 0xFF) ? true : false;
}



static void beep_fileReadTransferStatemachine(void)
{
    FRESULT retfs;
    uint32_t retNRF;

    switch(readState)
    {
        //########################################################################################################################################################################################
        case READ_OPEN_FILE:
        {
            bytesTransferred    = 0;
            startTimestamp      = app_timer_cnt_get();

            // Check if the client has been disconnected
            if(clientDisconnected || !TXnotificationEnabled)
            {
                #if MX_FLASH_APP_LOG_ENABLED    
                    NRF_LOG_INFO("Error: client disconnected: %u, TX notifications: %u, offset: %u", (uint32_t)clientDisconnected, (uint32_t)TXnotificationEnabled);
                    NRF_LOG_FLUSH();
                #endif
                clientDisconnected = false;
                readState = READ_CLOSE_FILE;
                return;
            }
            ble_beep_clearPackerCounter();

            // Initialize the filesystem and the QSPI interface. Disable them on error or on completion.
            fatfs_init();
            retfs = fatfs_open(&fp, fileName, read_offset);
            if(retfs != FR_OK)
            {
                #if MX_FLASH_APP_LOG_ENABLED    
                    NRF_LOG_INFO("Opening Log failed: 0x%04X/%u, offset: %u", retfs, retfs, read_offset);
                    NRF_LOG_FLUSH();
                #endif
                
                readState = READ_CLOSE_FILE;
                sendResponse(whileCMDsource, READ_MX_FLASH, NRF_ERROR_INVALID_LENGTH);
                break;
            }

            readNewBufferData   = true;
            readState           = READ_ACCESS_FILE;
            
            #if MX_FLASH_APP_LOG_ENABLED    
                NRF_LOG_INFO("new state: READ_ACCESS_FILE: 0x%04X/%u, offset: %u", retfs, retfs, (uint32_t)read_offset);
                NRF_LOG_FLUSH();
            #endif
            break;
        }

        //########################################################################################################################################################################################
        case READ_ACCESS_FILE:
        {
            UINT size = MX_FLASH_LINE_LENGHT_MAX;

            // Check if the client has been disconnected
            if(clientDisconnected || !TXnotificationEnabled)
            {
                #if MX_FLASH_APP_LOG_ENABLED    
                    NRF_LOG_INFO("new state: READ_ACCESS_FILE: 0x%04X/%u, offset: %u", retfs, retfs, (uint32_t)read_offset);
                    NRF_LOG_FLUSH();
                #endif
                clientDisconnected = false;
                readState = READ_CLOSE_FILE;
                return;
            }

            // If no buffer needs to be read, don't go further.
            if(!readNewBufferData)
            {
                return;
            }

            retfs = fatfs_readBuffer(&fp, buff.data, size, &bytesRead);
            buff.lenght = bytesRead;
            bytesTransferred += bytesRead;

            if(retfs != FR_OK)
            {
                #if MX_FLASH_APP_LOG_ENABLED
                    NRF_LOG_INFO("readBuffer error: 0x%04X/%u, bufsize: %u, read: %u", retfs, retfs, size, bytesRead);
                #endif
                readNewBufferData = false;
                readState = READ_CLOSE_FILE;
            }
            else
            {
                // Transmit the newly read data.
                readNewBufferData = false;

                // When no more data is available close the file
                if(bytesRead == 0)
                {
                    sendResponse(whileCMDsource, READ_MX_FLASH, NRF_EOF);
                    readState = READ_CLOSE_FILE;
                    #if MX_FLASH_APP_LOG_ENABLED
                        NRF_LOG_INFO("EOF: No Data READ_ACCESS_FILE triggered closed");
                    #endif
                }
                else
                {
                    readState = READ_TRANSMIT_FILE;
                }
            }
            break;
        }
        
        //########################################################################################################################################################################################
        case READ_TRANSMIT_FILE:
        {
            // Check if TX notifications have been enabled, otherwise stop the statemachine
            if(!TXnotificationEnabled)
            {
                readState = READ_CLOSE_FILE;
                sendResponse(whileCMDsource, READ_MX_FLASH, NRF_ERROR_INVALID_FLAGS);
                #if MX_FLASH_APP_LOG_ENABLED
                    NRF_LOG_INFO("Tx notifications READ_TRANSMIT_FILE triggered closed: %u", TXnotificationEnabled);
                #endif
            }

            retNRF = ble_TXdata_send(buff.data, buff.lenght);
            #if MX_FLASH_APP_LOG_ENABLED
                const uint16_t bufferCounter = ble_beep_getPacketCounter();
                NRF_LOG_INFO("ble_bus_data_send[%u] TX err: 0x%04X/%u", bufferCounter, retNRF, retNRF);
            #endif

            if(retNRF == NRF_SUCCESS || retNRF == NRF_ERROR_RESOURCES)
            {
                readState = READ_ACCESS_FILE;
            }
            else if(retNRF == NRF_ERROR_BUSY)
            {
                // Wait for the BLE stack to finish transmission or to return a different error on the next transmission attempt.
                return;
            }
            else
            {
                // Shutdown the read operation
                sendResponse(whileCMDsource, READ_MX_FLASH, NRF_SUCCESS);// NRF_EOF
                readState = READ_CLOSE_FILE;
                #if MX_FLASH_APP_LOG_ENABLED
                    const uint16_t bufferCounter = ble_beep_getPacketCounter();
                    NRF_LOG_INFO("bufferCounter=%u, ble_TXdata_send close file: nRF err: 0x%04X/%u", bufferCounter, retNRF, retNRF);
                #endif
            }
            break;
        }

        //########################################################################################################################################################################################
        case READ_CLOSE_FILE:
        {
            uint32_t time_ms = app_timer_time_since_start_ms(startTimestamp);
            
            retfs = fatfs_close(&fp);
            readState = READ_IDLE;

            #if MX_FLASH_APP_LOG_ENABLED    
                float bytesPerSecond = (time_ms == 0 || bytesTransferred == 0) ? 0.0 : (((float)bytesTransferred * 1000.0) / (float)time_ms);
                NRF_LOG_INFO("Bytes transferred: %u, Time: %u ms, average: "NRF_LOG_FLOAT_MARKER " bytes per second", bytesTransferred, time_ms, NRF_LOG_FLOAT(bytesPerSecond));
                NRF_LOG_INFO("closed file: 0x%04X/%u, new state: READ_IDLE", retfs, retfs);
                NRF_LOG_FLUSH();
            #endif

            MXflash_FSdeinit();
            break;
        }

        //########################################################################################################################################################################################
        default:
        case READ_IDLE:
            break;
    }
}


/*
 * Statemachine handler for the filetransfer service.
 */
#define MEASURE_WRITE_TIME 0
void beep_fileTransfer_while(void)
{
    uint32_t ret;

    // When a valid command is set from the BLE RX handler, excute it in the while loop
    if(whileCMD != UINT8_MAX)
    {
        switch(whileCMD)
        {
            case APP_FLASH_READ:
            {
                uint8_t i;

                #if MX_FLASH_APP_LOG_ENABLED   
                    NRF_LOG_INFO("READ_START while, size: %u, offset: %u", flash_size(), (uint32_t)read_offset);
                #endif
                
                // Check the origin of the read command, LoRaWAN is not supported.
                if(whileCMDsource == BLE_SOURCE)
                {
                    if(readState == READ_IDLE)
                    {
                        readState = READ_OPEN_FILE;
                    }
                }
                else
                {
                    // When send from any other source then BLE, reply invalid parameter.
                    sendResponse(whileCMDsource, READ_MX_FLASH, NRF_ERROR_INVALID_PARAM);
                }
                whileCMD = UINT8_MAX;
                break;
            }

            case APP_FLASH_FATFS_ERASE:
            {
                #if MX_FLASH_APP_LOG_ENABLED
                    NRF_LOG_INFO("ERASE_FS Clear FATfs");
                #endif

                FRESULT err_fs;
                err_fs = fatfs_clear();
                
                // Re-initialize the flash and set the writeOffset to 0.
                MXflash_FSinit();
                writeLogOffsetToFile(0);

                // Return a status code when erasing is completed.
                sendResponse(whileCMDsource, ERASE_MX_FLASH, err_fs);

                whileCMD = UINT8_MAX;

                const bool retBool = flash_WriteStartUpLine();
                NRF_LOG_INFO("ERASE_FS Write start-up line result: %u, %u", (uint8_t) retBool);
                break;
            }

            case APP_FLASH_MX_ERASE_A:
            {
                // Handle the MX flash erase of the fatfs erase here.
                #if MX_FLASH_APP_LOG_ENABLED
                    NRF_LOG_INFO("ERASE_FS_A WEN MX %u", whileAttempt);
                #endif
                powerApp_Enable(true, PWR_MX);

                fatfs_uninit();

                flash_ReleasePowerDown();        // Wake-up the MX chip
                flash_sendInstructionCode(0x06); // Write Enable command

                // Read the MX flash status register and check if the WIP bit is set.
                uint16_t status = flash_ReadStatusRegister();
                if(status & RDSR_WEL_BIT)
                {
                    timestampStartErase = app_timer_cnt_get();
                    NRF_LOG_INFO("Flash erase MX started, WEL bit set. Status=0x%04X", status);
                    whileCMD = APP_FLASH_MX_ERASE_B;
                    whileAttempt = 0;
                }
                else
                {
                    whileAttempt++;
                    if(whileAttempt >= 10)
                    {
                        NRF_LOG_INFO("Flash WEL failed. Status=0x%02X", status);
                        whileAttempt = 0;
                        whileCMD = UINT8_MAX;
                        sendResponse(whileCMDsource, ERASE_MX_FLASH, NRF_ERROR_TIMEOUT);
                        powerApp_Enable(false, PWR_MX);
                    }
                }
                break;
            }

            case APP_FLASH_MX_ERASE_B:
            {
				// Handle the MX flash erase of the fatfs erase here.
                #if MX_FLASH_APP_LOG_ENABLED
                    NRF_LOG_INFO("ERASE_FS_B Clear MX %u", whileAttempt);
                #endif

                flash_sendInstructionCode(0xC7); // Erase

                // Read the MX flash status register and check if the WIP bit is set.
                uint16_t status = flash_ReadStatusRegister();
                if(status & RDSR_WIP_BIT)
                {
                    timestampStartErase = app_timer_cnt_get();
                    NRF_LOG_INFO("Flash erase MX started, WIP bit set. Status=0x%04X", status);
                    whileCMD = APP_FLASH_MX_ERASE_C;
                    whileAttempt = 0;
                }
                else
                {
                    whileAttempt++;
                    if(whileAttempt >= 10)
                    {
                        NRF_LOG_INFO("Flash erase MX failed. Status=0x%02X", status);
                        whileAttempt = 0;
                        whileCMD = UINT8_MAX;
                        sendResponse(whileCMDsource, ERASE_MX_FLASH, NRF_ERROR_TIMEOUT);
                        powerApp_Enable(false, PWR_MX);
                    }
                }
                break;
            }

            case APP_FLASH_MX_ERASE_C:
            {
                uint16_t status = flash_ReadStatusRegister();
                const uint32_t timeSinceStart = app_timer_time_since_start_ms(timestampStartErase);

                // Wait until 250 seconds have passed or the Write In Progress Bit is cleared
                if((status & RDSR_WIP_BIT) && timeSinceStart < (250 * 1000))
                {
                    break;
                }

                NRF_LOG_INFO("Flash Status=0x%04X", status);

                if(timeSinceStart < (250 * 1000))
                {
                    NRF_LOG_INFO("Flash erase MX completed");
                }
                else
                {
                    NRF_LOG_INFO("Flash erase MX timed-out");
                }

                whileCMD = UINT8_MAX;

                //Init the MX25R6435F Flash IC
                MXflash_FSinit();

                flash_WriteStartUpLine();

                // Return a status code when erasing is completed.
                sendResponse(whileCMDsource, ERASE_MX_FLASH, (timeSinceStart < (250 * 1000)) ? NRF_SUCCESS : NRF_ERROR_TIMEOUT);

                powerApp_Enable(false, PWR_MX);
                break;
            }

            case APP_FLASH_SIZE_FLASH:
            {
                BEEP_protocol_s prot;
                memset(&prot, 0, sizeof(BEEP_protocol_s));
                prot.command    = SIZE_MX_FLASH;
                prot.param.size = flash_size();
                NRF_LOG_INFO("Beep log size=%u/0x%08X", prot.param.size, prot.param.size);
                sendProtocolField(&prot, whileCMDsource);

                // Clear the excuted command
                whileCMD = UINT8_MAX;
                break;
            }

            case APP_FLASH_WRITE_OFFSET:
            {
                FRESULT ret = 0;
                FILINFO f_stats;
                
                // Get the file stats of the beeplog file.
                ret = flash_info(&f_stats);

                if(ret == FR_OK)
                {
                    // Check if the new offset is larger than the file, which must never 
                    if(newOffset > f_stats.fsize)
                    {
                        ret = FR_INVALID_PARAMETER;
                    }
                    else
                    {
                        ret = writeLogOffsetToFile(newOffset);
                    }    
                }

                NRF_LOG_INFO("Write offset=%u, ret=%u", newOffset, ret);

                // Send a message indicating the return value.
                sendResponse(whileCMDsource, WRITE_LOG_OFFSET, ret);

                // Clear the excuted command
                whileCMD = UINT8_MAX;
                break;
            }
              
            case APP_FLASH_READ_OFFSET:
            {
                BEEP_protocol_s prot;
                memset(&prot, 0, sizeof(BEEP_protocol_s));
                prot.command    = READ_LOG_OFFSET;
                prot.param.size = getCurrentWriteOffset();
                sendProtocolField(&prot, whileCMDsource);

                NRF_LOG_INFO("Read wr_offset=%u/0x%04X, ret=%u", prot.param.size, prot.param.size, ret);

                // Clear the excuted command
                whileCMD = UINT8_MAX;
                break;
            }

            case APP_FLASH_FILL_LOG_A:
            {
                // Initialize the filesystem and the QSPI interface. Disable them on error or on completion.
                powerApp_Enable(true, PWR_MX);
                fatfs_init();

                log_adc.battPercentage = 100;
                log_adc.battVoltage_mV = 3000;
                log_adc.vccVoltage_mV = 3300;
                log_hx.channel = 0;
                log_hx.samples = 1;
                log_hx.value[0] = 0;
                log_ds.devices = 1;
                log_ds.temperatures[0] = 0;
                log_fft.start = 0;
                log_fft.stop = 255; // 255*2= bin 510
                log_fft.bins = 10;
                log_bme.airPressure = 1000;
                log_bme.temperature = 0;
                log_bme.humidity = 500;
                updateFillLogOffset = 0;

                int i;
                for(i=0; i<FFT_MAX_BINS; i++)
                {
                    log_fft.values[i] = 0;
                }

                whileCMD = APP_FLASH_FILL_LOG_B;
                break;
            }

            case APP_FLASH_FILL_LOG_B:
            {
                BEEP_protocol_s prot[6];
                uint8_t j;

                for(j=0; j<32; j++)
                {
                    memset(prot, 0, sizeof(prot));

                    #if MEASURE_WRITE_TIME
                        uint32_t start = app_timer_cnt_get();
                    #endif

                    // Increment the battery percentage between 0 - 100
                    log_adc.battPercentage++;
                    if(log_adc.battPercentage >= 100)
                    {
                        log_adc.battPercentage = 0;
                    }

                    // Increment the battery voltage 1mV per log. Range 3600mV - 2000mV
                    log_adc.battVoltage_mV++;
                    if(log_adc.battVoltage_mV >= 3600)
                    {
                        log_adc.battVoltage_mV = 2000;
                    }
                    log_adc.vccVoltage_mV = log_adc.battVoltage_mV;

                    // Copy the values to protocol structure.
                    prot[0].command = READ_nRF_ADC_CONVERSION;
                    memcpy(&prot[0].param.meas_result.result.saadc, &log_adc, sizeof(ADC_s));

                    // Add HX711 value
                    log_hx.value[0]++;
                    if(log_hx.value[0] > UINT16_MAX)
                    {
                        log_hx.value[0] = 0;
                    }
                    HX711_CONV_s log_hx = {.channel=0, .samples=1, .value={0}};
                    prot[1].command = READ_HX711_CONVERSION;
                    prot[1].param.meas_result.type = HX711;
                    memcpy(&prot[1].param.meas_result.result.hx711, &log_hx, sizeof(HX711_CONV_s));


                    // Increment the DS18B20 temperature -20.0 to +85.0 
                    log_ds.temperatures[0]++;
                    if(log_ds.temperatures[0] >= 850)
                    {
                        log_ds.temperatures[0] = -200;
                    }

                    prot[2].command = READ_DS18B20_CONVERSION;
                    prot[2].param.meas_result.type = DS18B20;
                    memcpy(&prot[2].param.meas_result.result.ds18B20, &log_ds, sizeof(DS18B20_RESULTS_s));

                    // Increment TLV bin values.
                    log_fft.start = 0;
                    log_fft.stop = 255; // 255*2= bin 510
                    log_fft.bins = 10;
                    uint16_t i = 0;
                    for(i=0; i<log_fft.bins; i++)
                    {
                        log_fft.values[i] += i+1;
                    }

                    // Add TLV result
                    prot[3].command = READ_AUDIO_ADC_CONVERSION;
                    prot[3].param.meas_result.type = AUDIO_ADC;
                    memcpy(&prot[3].param.meas_result.result.fft, &log_fft, sizeof(FFT_RESULTS));

                    // Simulate the BME values incrementing
                    log_bme.airPressure += 10;
                    if(log_bme.airPressure >= 10000)
                    {
                        log_bme.airPressure = 1000;
                    }

                    log_bme.temperature++;
                    if(log_bme.temperature >= 850)
                    {
                        log_bme.temperature = -200;
                    }

                    log_bme.humidity++;
                    if(log_bme.humidity >= 1000)
                    {
                        log_bme.humidity = 0;
                    }

                    // Add the temperature, Relative Humidity and Barometric pressure to the log
                    prot[4].command = BME280_CONVERSION_READ;
                    prot[4].param.meas_result.type = BME280;
                    memcpy(&prot[4].param.meas_result.result.bme280, &log_bme, sizeof(BME280_RESULT_s));

                    // Add the log time from DS3231 RTC or from internal clock
                    memset(&prot[5], 0, sizeof(BEEP_protocol_s));
                   
                    if(ds3231_enabled == 1)
                    {
                    prot[5].command = READ_TIME_DS3231;
                    prot[5].param.time = get_logtime_value(); 
                    }
                    if(ds3231_enabled == 0)
                    {
                    prot[5].command = READ_TIME; // 0x25
                    prot[5].param.time = get_logtime_value(); 
                    }

                    if(!flash_newLineStart(BEEP_LOG_FILL))
                    {
                        // Re-initialized the fatfs on error.
                        MXflash_FSdeinit();
                        fatfs_init();
                        continue;
                    }

                    // Retrieve all command parameters from the EEPROM and add them to the newLine 
                    uint16_t k;
                    for(k=0; k<6; k++)
                    {
                        flash_newLineAppendBeep(&prot[k]);
                    }

                    bool retEnd = flash_newLineEnd();

                    #if MEASURE_WRITE_TIME
                        uint32_t stop = app_timer_cnt_get();
                        const uint32_t diff = stop - start;
                        const uint32_t time_ms = (diff * 1000) / APP_TIMER_CLOCK_FREQ;
                        NRF_LOG_INFO("Ticks=%u, Time=%u", diff, time_ms);
                    #endif

                    // Send the current log offset periodically, so the user knows the progress while this action is active. 
                    updateFillLogOffset++;
                    if(updateFillLogOffset >= 10000 || logOffsetRollOver || !retEnd)
                    {
                        updateFillLogOffset = 0;
                    
                        BEEP_protocol_s prot;
                        memset(&prot, 0, sizeof(BEEP_protocol_s));
                        prot.command    = READ_LOG_OFFSET;
                        prot.param.size = getCurrentWriteOffset();
                        sendProtocolField(&prot, whileCMDsource);
                    }

                    // Stop writing data when the logOffset value rolls-over or writing the data failed.
                    if(logOffsetRollOver || !retEnd)
                    {
                        // Store the current logOffset 
                        storeLogOffset();

                        MXflash_FSdeinit();

                        // Send a message indicating that the flash log has been filled.
                        sendResponse(whileCMDsource, WRITE_LOG_OFFSET, NRF_EOF);

                        // Clear the current command
                        whileCMD = UINT8_MAX;

                        NRF_LOG_INFO("Log filling stopped");

                        powerApp_Enable(false, PWR_MX);
                        return;
                    } 
                }       
                break;
            }

        //--------------------------------------------------------------------------------------------------------------------------------------------------------------
            default:
                whileCMD = UINT8_MAX;
                break;
        }
    }
    // Write a time changed log message when a new time is set, so the log time doesn't have large gaps.
    else if(writeTimeChangedLog)
    {
        // If writing the log file doesn't succeed because the log is busy, the boolean isn't cleared.
        writeTimeChangedLog = !flash_WriteTimeChange(oldTimeStamp, newTimeStamp);
    }

    // Check the Read Transfer state.    
    beep_fileReadTransferStatemachine();
}


