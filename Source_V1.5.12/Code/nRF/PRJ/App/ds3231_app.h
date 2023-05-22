#ifndef DS3231_APP_H_
#define DS3231_APP_H_
#include <stdint.h>
#include <stdbool.h>
#include "sdk_errors.h"	
#include "nrf_drv_twi.h"

#define          DS3231_LOG_ENABLED 1

#define          DS3231_ADDR 0x68U

extern time_t    ds3231_timestamp;

extern uint8_t ds3231_enabled;

bool             ds3231_detected(void);
void             ds3231_init(void);
void             ds3231_uninit(void);

nrfx_drv_state_t ds3231_status(void);

void             ds3231_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);

ret_code_t       ds3231_writeByte(const uint8_t address, const uint8_t reg, uint8_t data, uint8_t length);
void             ds3231_start_clock_osc(void);
void             ds3231_setTime(struct tm ds3231_dateTime_s);
time_t           ds3231_getTime(void);

uint8_t          bcd2dec(uint8_t bcd);
uint8_t          dec2bcd(uint8_t dec);

#endif // DS3231_APP_H_