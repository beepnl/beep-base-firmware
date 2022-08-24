/*!
 * \file      SQ-min-200.h
 *
 * \brief     SQ-MIN-200 vibration sensor and SQ-SEN-645B implementation
 *
 */
#ifndef __SQMIN200_H__
#define __SQMIN200_H__

	#include <stdbool.h>
	#include <stdint.h>

	#define SQ_DEBUG 0

	typedef struct
	{
		uint32_t	pulseTotalCount;			//	The total number of pulses measured
		uint32_t	pulseCount;					//	The number of pulses measured in a second
		uint32_t	pulseThreshold;				//	The number of pulses per second that need to be sensed to determine that the sensor is moving.
		uint32_t	secondsOfMovement;			//	The number of seconds movement has been detected within a period.
		uint32_t	movementTimeThresholdStart;	//	The time in seconds for which the sensor has to move within a period to count is active
        uint32_t	movementTimeThresholdStop;	//	The time in seconds for which the sensor has to move within a period to count is inactive
		uint32_t	period;						//	The period is seconds.
		uint32_t	time;						//	The current count within the period.
		bool		moving;						//	current state of the sensor
	}SQ_MIN_200s;

	typedef struct
	{
		uint32_t	stateCurrently;
		uint32_t	stateNew;
		uint32_t	timeCount;
		uint32_t	timeThreshold;	
	}SQ_SEN_645B;

	typedef enum
	{
		SQ_MIN_200_MOVING,
		SQ_MIN_200_STANDSTILL,
		SQ_SEN_645B_HIGH,
		SQ_SEN_645B_LOW,
	}SQ_EVENTS;

    typedef void (*sq_event_handler_t)(SQ_EVENTS event);


    void SQ_init			(sq_event_handler_t handler);
    bool SQ_getOrientation	(void);
	bool SQ_moving			(void);


#endif // __SQMIN200_H__
