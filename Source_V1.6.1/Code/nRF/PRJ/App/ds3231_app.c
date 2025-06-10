#define NRF_LOG_MODULE_NAME DS3231_APP
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"

#include "ds3231_app.h"
#include "power_app.h"
#include "i2c_app.h"
#include "log_time.h"
#include "time.h"
#include "I2C2.h"

#define     DS3231_ADDR 0x68U
#define     DS3231_REG_TIME 0x00U
#define     DS3231_REG_CONTROL 0x0EU

DS3231_STATUS_t state = DS3231_IDLE;

static const char * ds3231_reg_names[] = {
    "Seconds",
    "Minutes",
    "Hours",
    "Day",
    "Date",
    "Month",
    "Year",
    "Alarm 1 Seconds",
    "Alarm 1 Minutes",
    "Alarm 1 Hours",
    "Alarm 1 Day",
    "Alarm 2 Minutes",
    "Alarm 2 Hours",
    "Alarm 2 Day",
    "Control",
    "Status",
    "Aging Offset",
    "MSB Temp",
    "LSB Temp"
};

/**
 * DS3231 PRIVATE FUNCTIONS
 */

/**
 * @brief Function to read from DS3231 registers
 * @param reg_addr: Register address to start reading from
 * @param data: Pointer to the data buffer to store the read data
 * @param len: Length of the data to read
 * 
 * @return int8_t: DS3231_OK if successful, error code otherwise
 */
static int8_t ds3231_read(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	uint32_t ret = 0;

    // Enable I2C bus Mutex
    powerApp_Enable(true, PWR_DS3231);
    i2cApp_Enable(true, I2C_DS3231);

    // Read data from the register
    ret = I2C2_readArray(DS3231_ADDR, reg_addr, data, len);

    // Disable I2C bus Mutex
    i2cApp_Enable(false, I2C_DS3231);
    powerApp_Enable(false, PWR_DS3231);

    if(ret != NRF_SUCCESS){
        return DS3231_E_COMM_FAIL;
    }

    return DS3231_OK;
}

/**
 * @brief Function to write to DS3231 registers
 * @param reg_addr: Register address to start writing to
 * @param data: Pointer to the data buffer to write
 * @param len: Length of the data to write
 * 
 * @return int8_t: DS3231_OK if successful, error code otherwise
 */
static int8_t ds3231_write(uint8_t reg_addr, uint8_t *data, uint16_t len)
{
	uint32_t ret = 0;

    // Enable I2C bus Mutex
    powerApp_Enable(true, PWR_DS3231);
    i2cApp_Enable(true, I2C_DS3231);

	// Write data to the register
    ret = I2C2_writeArray(DS3231_ADDR, reg_addr, data, len);

    // Disable I2C bus Mutex
    i2cApp_Enable(false, I2C_DS3231);
    powerApp_Enable(false, PWR_DS3231);

    if(ret != NRF_SUCCESS)
	{
        return DS3231_E_COMM_FAIL;
    }
    return DS3231_OK;
}

/**
 * @Brief Function for checking the presence of the DS3231 RTC.
 * @return bool: True if the RTC is detected, false otherwise.
 */
static bool ds3231_detected()
{
    ret_code_t err_code;
    uint8_t rx_buf[1];
    bool ds3231_found;

    // Detect presence of RTC 
    // NRF_LOG_INFO("Scanning for DS3231 Presence");
    err_code = ds3231_read(0, rx_buf, sizeof(rx_buf));

    // Branch on return code
    if(err_code == NRF_SUCCESS) {
        // NRF_LOG_INFO("DS3231 Detected");
        ds3231_found = true;
    }
    else
    {
        // NRF_LOG_INFO("DS3231 Not detected");
        ds3231_found = false;
    }

    // Flush logging and return
    NRF_LOG_FLUSH();
    return ds3231_found;
}

/**
 * @brief Function to convert decimal to BCD
 * @param dec: Decimal value to convert
 * @return uint8_t: BCD value
 */
uint8_t dec2bcd(uint8_t dec)
{
    return ((dec / 10) << 4) | (dec % 10);
}

/**
 * @brief Function to convert BCD to decimal
 * @param bcd: BCD value to convert
 * @return uint8_t: Decimal value
 */
uint8_t bcd2dec(uint8_t bcd)
{
    return (bcd & 0x0F) + ((bcd >> 4) * 10);
}

/**
 * DS3231 PUBLIC FUNCTIONS
 */

DS3231_STATUS_t ds3231_status(void)
{
    return state;
}

void ds3231_enable_osc(void)
{
    // Local data structures
    ret_code_t err;
    uint8_t control_bit;

    // Set bits 2, 6, unset others
    control_bit = 0b01000100;
    err = ds3231_write(DS3231_REG_CONTROL, &control_bit, 1);

    return;
}

void ds3231_set_long(time_t newtime)
{
    // Debug logging
    // NRF_LOG_INFO("RTC set time: %s, %u/0x%04X", get_logtime_string(newtime), newtime, newtime)

    // Local data structures
    ret_code_t err;
    struct tm *newtime_tm;
    uint8_t tx_buf[7];
    
    // Populate data structures
    newtime_tm = gmtime(&newtime);
    tx_buf[0] = dec2bcd(newtime_tm->tm_sec);
    tx_buf[1] = dec2bcd(newtime_tm->tm_min);
    tx_buf[2] = dec2bcd(newtime_tm->tm_hour) & 0x3F; // always enable 24-hour mode (bit 3)
    tx_buf[3] = dec2bcd(newtime_tm->tm_wday + 1);
    tx_buf[4] = dec2bcd(newtime_tm->tm_mday + 1);
    tx_buf[5] = dec2bcd(newtime_tm->tm_mon + 1) | 0x80; // set century bit
    tx_buf[6] = dec2bcd(newtime_tm->tm_year - 100);

    // Enable oscillator
    ds3231_enable_osc();

    // Send new time
    err = ds3231_write(DS3231_REG_TIME, tx_buf, sizeof(tx_buf));

    // Flush logging and return
    NRF_LOG_FLUSH();
    return;
}

time_t ds3231_get_long(void)
{
    // Local data structures
    ret_code_t err;
    struct tm newtime_tm;
    uint8_t rx_buf[7];

    // Receive time
    err = ds3231_read(DS3231_REG_TIME, rx_buf, sizeof(rx_buf));

    // Time structure conversion
    newtime_tm.tm_sec = bcd2dec(rx_buf[0] & 0x7F);
    newtime_tm.tm_min = bcd2dec(rx_buf[1] & 0x7F);
    newtime_tm.tm_hour = bcd2dec(rx_buf[2] & 0x3F);
    newtime_tm.tm_wday = bcd2dec(rx_buf[3] & 0x07) - 1;
    newtime_tm.tm_mday = bcd2dec(rx_buf[4] & 0x3F) - 1;
    newtime_tm.tm_mon = bcd2dec(rx_buf[5] & 0x1F) - 1;
    newtime_tm.tm_year = bcd2dec(rx_buf[6]) + 100;
    newtime_tm.tm_isdst = 0;

    // Convert to time_t
    time_t newtime = mktime(&newtime_tm);

    // Debug logging
    #ifdef RTC_DEBUG
        // NRF_LOG_INFO("getter_func: %s, %u/0x%04X", get_logtime_string(newtime), newtime, newtime)
    #endif // RTC_DEBUG

    return newtime;
}

void ds3231_init(void)
{
    #ifndef DS3231_ENABLED
        state = DS3231_DISABLED;
        // NRF_LOG_INFO("DS3132 disabled by compiler");
        return;
    #else
        if (ds3231_detected()) 
        {
            
            state = DS3231_READY;
        }
        else
        {
            state = DS3231_DISABLED;
        }
    #endif
}