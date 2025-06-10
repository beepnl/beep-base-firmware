#ifndef DS3231_APP_H_
#define DS3231_APP_H_

#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include "sdk_errors.h"	
//#include "nrf_drv_twi.h"

#define     DS3231_ENABLED
#define     DS3231_LOG_ENABLED

// #define RTC_DEBUG

/**\name API success code */
#define DS3231_OK                         INT8_C(0)

/**\name API error codes */
#define DS3231_E_NULL_PTR                 INT8_C(-1)
#define DS3231_E_DEV_NOT_FOUND            INT8_C(-2)
#define DS3231_E_INVALID_LEN              INT8_C(-3)
#define DS3231_E_COMM_FAIL                INT8_C(-4)
#define DS3231_E_SLEEP_MODE_FAIL          INT8_C(-5)
#define DS3231_E_NVM_COPY_FAILED          INT8_C(-6)

typedef enum {
    DS3231_IDLE,
    DS3231_READY,
    DS3231_DISABLED,
} DS3231_STATUS_t;

//char * get_logtime_string(time_t timeValue);
//char * get_logtime_string_tm(struct tm *tmValue);

/** @brief Function for initializing the DS3231 driver.
  * 
  * Initializes the DS3231 RTC driver and populates it's local variables with RTC availability. Use ds3231_status() to check RTC availability.
  */
void                ds3231_init(void);

/**
 * @brief Function for checking the status of the DS3231 driver.
 * @return DS3231_STATUS_t: The status of the DS3231 driver.
 * 
 * Use this function to check the status of the DS3231 driver. The status can be one of the following:
 * - DS3231_IDLE: The driver is uninitialized.
 * - DS3231_READY: The driver is ready to use and the RTC is available.
 * - DS3231_DISABLED: The driver is disabled and/or the RTC is not available.
 */
DS3231_STATUS_t     ds3231_status(void);

/**
 * @brief Function for uninitializing the DS3231 driver.
 * 
 * Uninitializes the DS3231 RTC driver and sets the RTC availability to DS3231_IDLE. Use ds3231_status() to check RTC availability.
 */
void                ds3231_uninit(void);

//nrfx_drv_state_t ds3231_status(void);
//
//void             ds3231_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);
//
//ret_code_t       ds3231_writeByte(const uint8_t address, const uint8_t reg, uint8_t data, uint8_t length);
//void             ds3231_start_clock_osc(void);
void                ds3231_set_long(time_t newtime);
time_t              ds3231_get_long(void);
//
uint8_t             bcd2dec(uint8_t bcd);
uint8_t             dec2bcd(uint8_t dec);

#endif // DS3231_APP_H_