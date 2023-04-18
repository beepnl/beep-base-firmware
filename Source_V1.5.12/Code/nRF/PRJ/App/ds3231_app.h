#ifndef DS3231_APP_H_
#define DS3231_APP_H_
#include <stdint.h>
#include <stdbool.h>
#include "sdk_errors.h"	
#include "nrf_drv_twi.h"

#define DS3231_LOG_ENABLED 1

nrfx_drv_state_t ds3231_state(void);

extern time_t ds3231_timestamp;

struct ds3231_time{
        uint8_t second;
        uint8_t minute;
        uint8_t hour;
        uint8_t day;
        uint8_t date;
        uint8_t month;
        uint8_t year;
        };

void ds3231_init(void);
void ds3231_uninit(void);
void ds3231_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);

void ds3231_getStatus();
void ds3231_setTime(struct tm *ds3231_dateTime);
time_t ds3231_getTime();

ret_code_t ds3231_writeByte(const uint8_t address, const uint8_t reg, uint8_t data, uint8_t length);

uint8_t bcd2dec(uint8_t bcd);
uint8_t dec2bcd(uint8_t dec);

void twi_scanner(void);

#endif // DS3231_APP_H_