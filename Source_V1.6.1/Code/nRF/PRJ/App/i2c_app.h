#ifndef I2C_APPLICATION_LAYER_H
#define	I2C_APPLICATION_LAYER_H

	#include <stdint.h>
	#include <stdbool.h>

	// #define I2C_DEBUG

	/*
	 * Enumeration of the different sensors that making use of the I2C port that need to be enabled.
	 */
	typedef enum
	{
		I2C_BME280	= 0,
        I2C_DS3231  = 1,
        I2C_ALL     = 2,
	} i2c_t;


	bool i2cApp_getEnabled	(i2c_t index);
	void i2cApp_Enable		(bool enable, i2c_t index);


#endif	/* POWER_APPLICATION_LAYER_H */

