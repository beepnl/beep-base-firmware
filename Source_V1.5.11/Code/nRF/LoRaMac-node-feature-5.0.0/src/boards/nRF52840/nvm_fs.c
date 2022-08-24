#include "nvm_fs.h"

#include <stdint.h>
#include <string.h>
#include "nrf.h"
#include "nordic_common.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "fds.h"
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"


fds_stat_t stat;


/* Array to map FDS return values to strings. */
char const * fds_err_str[] =
{
    "FDS_SUCCESS",
    "FDS_ERR_OPERATION_TIMEOUT",
    "FDS_ERR_NOT_INITIALIZED",
    "FDS_ERR_UNALIGNED_ADDR",
    "FDS_ERR_INVALID_ARG",
    "FDS_ERR_NULL_ARG",
    "FDS_ERR_NO_OPEN_RECORDS",
    "FDS_ERR_NO_SPACE_IN_FLASH",
    "FDS_ERR_NO_SPACE_IN_QUEUES",
    "FDS_ERR_RECORD_TOO_LARGE",
    "FDS_ERR_NOT_FOUND",
    "FDS_ERR_NO_PAGES",
    "FDS_ERR_USER_LIMIT_REACHED",
    "FDS_ERR_CRC_CHECK_FAILED",
    "FDS_ERR_BUSY",
    "FDS_ERR_INTERNAL",
};

/* Array to map FDS events to strings. */
static char const * fds_evt_str[] =
{
    "FDS_EVT_INIT",
    "FDS_EVT_WRITE",
    "FDS_EVT_UPDATE",
    "FDS_EVT_DEL_RECORD",
    "FDS_EVT_DEL_FILE",
    "FDS_EVT_GC",
};



/* Flash configuration data. */
static flash_struct_s flash_cfg =
{
    .Major_version  = 0,
    .Minor_version  = 2,
    .boot_count	    = 0,
	.LoRaWANnvm		= {0},
};

/* A record containing dummy configuration data. */
static fds_record_t const record =
{
    .file_id           = CONFIG_FILE,
    .key               = CONFIG_REC_KEY,
    .data.p_data       = &flash_cfg,
    /* The length of a record is always expressed in 4-byte units (words). */
    .data.length_words = (sizeof(flash_cfg) + 3) / sizeof(uint32_t),
};

typedef struct
{
    volatile bool   initialized;
    volatile bool   dataChanged;
    volatile bool   storeInitiated;
    volatile bool   gc_called;
    uint32_t	    unstoredDataCount;
}NVM_s;    


NVM_s nvm = {
    .initialized		= false,
    .dataChanged		= false,
    .storeInitiated		= false,
    .gc_called			= false,
    .unstoredDataCount	= 0,
};





static void fds_evt_handler(fds_evt_t const * p_evt)
{
    NRF_LOG_INFO("Event: %s received (%s)", fds_evt_str[p_evt->id], fds_err_str[p_evt->result]);

    switch (p_evt->id)
    {
        case FDS_EVT_INIT:
            if (p_evt->result == FDS_SUCCESS)
            {
                nvm.initialized = true;
            }
            break;

        case FDS_EVT_WRITE:
        {
            if (p_evt->result == FDS_SUCCESS)
            {
		NRF_LOG_INFO("FDS_EVT_WRITE");
                NRF_LOG_INFO("Record ID:\t0x%04x",  p_evt->write.record_id);
                NRF_LOG_INFO("File ID:\t0x%04x",    p_evt->write.file_id);
                NRF_LOG_INFO("Record key:\t0x%04x", p_evt->write.record_key);
		nvm.unstoredDataCount	= 0;
                nvm.dataChanged		= false;
		nvm.storeInitiated	= false;

            }
        } break;

        case FDS_EVT_DEL_RECORD:
        {
            if (p_evt->result == FDS_SUCCESS)
            {
		NRF_LOG_INFO("FDS_EVT_DEL_RECORD");
                NRF_LOG_INFO("Record ID:\t0x%04x",  p_evt->del.record_id);
                NRF_LOG_INFO("File ID:\t0x%04x",    p_evt->del.file_id);
                NRF_LOG_INFO("Record key:\t0x%04x", p_evt->del.record_key);
            }
        } break;

	case FDS_EVT_UPDATE:
	    nvm.dataChanged	= false;
	    nvm.storeInitiated	= false;
	    break;

	case FDS_EVT_GC:
	    nvm.dataChanged	= true;
	    nvm.storeInitiated	= false;    
	    nvm.gc_called	= false;
        default:
            break;
    }
}


void nvm_fds_changed(void)
{
    nvm.dataChanged = true;
    nvm.unstoredDataCount++;
}



static void nvm_fds_store(void)
{
//    NRF_LOG_INFO(">Store Flash data");
    
    fds_record_desc_t desc  = {0};
    fds_find_token_t  token = {0};

    ret_code_t rc = fds_record_find(CONFIG_FILE, CONFIG_REC_KEY, &desc, &token);

    if (rc == FDS_SUCCESS)
    {
        fds_flash_record_t config = {0};

        rc = fds_record_update(&desc, &record);
        if(rc == FDS_ERR_NO_SPACE_IN_FLASH) 
        {
            NRF_LOG_INFO("FDS_ERR_NO_SPACE_IN_FLASH, initiating garbage collect");
            if(!nvm.gc_called)
            {
               nvm.gc_called = true;
               fds_gc();
            }
        } 
        else if(rc == FDS_ERR_NO_SPACE_IN_QUEUES)
        {
            NRF_LOG_ERROR("FDS_ERR_NO_SPACE_IN_QUEUES after fds_record_update() call");
        }
        else if(rc != FDS_SUCCESS)
        {
			NRF_LOG_INFO("Update return value: %04X", rc);
            APP_ERROR_CHECK(rc);
        }
    }
    else
    {
        /* struct not found; write a new one. */
        NRF_LOG_INFO("Writing inital data to flash...");

        rc = fds_record_write(&desc, &record);
        APP_ERROR_CHECK(rc);
    }
}


void nvm_fds_check_pending(void)
{
    if(nvm.dataChanged && !nvm.storeInitiated)
    {
		nvm.storeInitiated = true;
        nvm_fds_store();
    }
//    else if(nvm.unstoredDataCount > 10)
//    {
//	nvm.storeInitiated = false;
//    }
}

 

void nvm_fds_init(void)
{
    uint32_t retVal;
    NRF_LOG_INFO("FDS init started.")

    /* Register first to receive an event when initialization is complete. */
    fds_register(fds_evt_handler);

    NRF_LOG_INFO("Initializing fds...");

    retVal = fds_init();

    while (!nvm.initialized)
    {
        #ifdef SOFTDEVICE_PRESENT
	    (void) sd_app_evt_wait();
	#else
	    __WFE();
	#endif
    }

    // Clear the stat structure
    memset(&stat, 0, sizeof(fds_stat_t));

    retVal = fds_stat(&stat);
    APP_ERROR_CHECK(retVal);

    NRF_LOG_INFO("Found %d valid records and %d dirty records (ready to be garbage collected).", stat.valid_records, stat.dirty_records);

    fds_record_desc_t desc = {0};
    fds_find_token_t  tok  = {0};

    // Find the record in the flash.
    retVal = fds_record_find(CONFIG_FILE, CONFIG_REC_KEY, &desc, &tok);

    if (retVal == FDS_SUCCESS)
    {
        /* A config file is in flash. Let's update it. */
        fds_flash_record_t config = {0};

        /* Open the record and read its contents. */
        retVal = fds_record_open(&desc, &config);
        APP_ERROR_CHECK(retVal); 

		// Copy the Flash data contents in the ram to the NVM_s structure.
		memcpy(&flash_cfg, config.p_data, sizeof(flash_struct_s));

        /* Close the record when done reading. */
        retVal = fds_record_close(&desc);
        APP_ERROR_CHECK(retVal);

        /* Write the updated record to flash. */
        retVal = fds_record_update(&desc, &record);

	if(retVal != FDS_SUCCESS)
	{
	    NRF_LOG_INFO("fds_record_update error: %u/0x%04X = %s", retVal, retVal, fds_err_str[retVal]);

            if(retVal == FDS_ERR_NO_SPACE_IN_FLASH)
	    {
			// Perform a garbage collect to remove all dirty records, then retry updating the record.
			fds_gc();
			retVal = fds_record_update(&desc, &record);
	    }
	    APP_ERROR_CHECK(retVal);
	}

        NRF_LOG_INFO("Config file found revision: %u.%u, updating boot count to %d.", flash_cfg.Major_version, flash_cfg.Minor_version, flash_cfg.boot_count);

        /* Update boot count. */
        flash_cfg.boot_count++;
    }
    else
    {
        /* System config not found; write a new one. */
        NRF_LOG_INFO("Config file not found, writing new file...");

        retVal = fds_record_write(&desc, &record);
        APP_ERROR_CHECK(retVal);
    }
}


void nvm_fds_eeprom_wrapper_read( uint16_t addr, uint8_t *buffer, uint16_t size )
{
	uint16_t i;

	if((addr + size) >= LORAWAN_NVM_SIZE){
		return;
	}

	for( i = 0 ; i < size ; i++)
	{
		buffer[i] = flash_cfg.LoRaWANnvm[addr + i];
	}
}


void nvm_fds_eeprom_wrapper_write( uint16_t addr, uint8_t *buffer, uint16_t size )
{
	uint16_t i;

	if((addr + size) >= LORAWAN_NVM_SIZE){
		return;
	}

	for( i = 0 ; i < size ; i++)
	{
		flash_cfg.LoRaWANnvm[addr + i] = buffer[i];
	}
}