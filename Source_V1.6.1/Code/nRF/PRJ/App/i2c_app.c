#define NRF_LOG_MODULE_NAME I2C_APP
#include "i2c_app.h"
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"
#include "nrf_delay.h"
#include "I2C2.h"
//#include "app_timer.h"
//#include "gpio-board.h"

const char * deviceStrings[] =
{
    "BME280",
    "DS3231",
    "ALL devices",
};

/*
 * Lock variables to create a mutex around the i2c peripheral while the sensor statemachines handle
 */
static volatile uint8_t peripheralLock = 0;


bool i2cApp_getEnabled(i2c_t index)
{
	if(index > I2C_ALL)
	{
		return false;
	}
	else if(index == I2C_ALL)
	{
		return ((peripheralLock & UINT8_MAX) ? true : false);
	}
	return (peripheralLock & (1<<index) ? true : false);
}



void i2cApp_Enable(bool enable, i2c_t index)
{
    const volatile uint8_t peripheralLock_prev = peripheralLock;

	if(index > I2C_ALL)
	{
		return;
	}
	else if(index == I2C_ALL)
	{
		// Clear or enable all locks on the i2c peripheral
		peripheralLock = (enable) ? UINT8_MAX : 0;
	}
	else
	{
		// Clear or enable for a specific sensor
        if(enable)
        {
            peripheralLock	|= (1<<index);
        }
        else
        {
            peripheralLock	&= ~(1<<index);
        }

	}

	// Check if the peripheral enabled status needs to be changed
	if(peripheralLock == 0 && peripheralLock_prev != 0)
	{
		#ifdef I2C_DEBUG
			// Debug Logging
			// NRF_LOG_INFO("I2C peripheral disabled by %s", deviceStrings[index]); 
			NRF_LOG_FLUSH();
		#endif // I2C_DEBUG

		// Disable the I2C peripheral
        I2C2_uninit();
	}
	else if(peripheralLock != 0 && peripheralLock_prev == 0)
	{
		#ifdef I2C_DEBUG
			// Debug Logging
			// NRF_LOG_INFO("I2C peripheral enabled by %s", deviceStrings[index]);
			NRF_LOG_FLUSH();
		#endif // I2C_DEBUG

		// Enable the I2C peripheral
        I2C2_init();
	} else {

		#ifdef I2C_DEBUG
			// Debug Logging
			// NRF_LOG_INFO("I2C peripheral sign %s by %s -- no change", (enable) ? "On" : "Off", deviceStrings[index]);
			NRF_LOG_FLUSH();
		#endif // I2C_DEBUG
    }
}



