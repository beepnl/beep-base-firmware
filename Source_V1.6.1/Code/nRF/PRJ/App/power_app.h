#ifndef POWER_APPLICATION_LAYER_H
#define	POWER_APPLICATION_LAYER_H

	#include <stdint.h>
	#include <stdbool.h>

	/*
	 * Enumeration of the different power needing sensors and actuators for which the boost and power switch need to be enabled.
	 */
	typedef enum
	{
		PWR_DS18B20	= 0,
		PWR_BME280	= 1,
        PWR_DS3231  = 2,
		PWR_HX711	= 3,
		PWR_AUDIO	= 4,
		PWR_ATECC	= 5,
		PWR_BUZZER	= 6,
		PWR_RFM		= 7,
        PWR_MX      = 8,
		PWR_ALL		= 9,
	}power_t;


	bool powerApp_getEnabled	(power_t index);
	void powerApp_Enable		(bool enable, power_t index);


#endif	/* POWER_APPLICATION_LAYER_H */

