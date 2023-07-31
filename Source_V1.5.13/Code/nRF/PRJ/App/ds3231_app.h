#include <stdint.h>
#include <stdbool.h>
#include "beep_types.h"
#include "beep_protocol.h"
#include "sdk_errors.h"	
#include "nrf_drv_twi.h"

nrfx_drv_state_t ds3231_state(void);

void ds3231_init(void);
void ds3231_uninit(void);
void ds3231_handler(nrf_drv_twi_evt_t const * p_event, void * p_context);
void ds3231_start(void);
void ds3231_writeByte(const uint8_t address, const uint8_t reg, uint8_t data);
void ds3231_readTime(void);

void twi_scanner(void);
