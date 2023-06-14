/*!
 * \file      tiltswitch.h
 *
 * \brief     tiltswitch code
 *
 */
#ifndef __TILTSWITCH_H__
#define __TILTSWITCH_H__

#include <stdbool.h>
	#include <stdint.h>
	#include "beep_protocol.h"

	#define TILT_DEBUG 1


	typedef struct
	{
		bool		startUp;
		uint32_t	stateCurrently;
		uint32_t	stateNew;
		uint32_t	timeCount;
		uint32_t	timeThreshold;
		uint8_t		countVertical;
		uint8_t		countHorizontal;
	}TILT_s;


    typedef void (*tilt_callback_t) (TILT_ORIENTATIONS_e newOrientation);


    void tilt_init                  (tilt_callback_t handler);
    bool tilt_getOrientation        (void);
	bool tilt_startup               (void);
    void tilt_enableOrientation     (const bool enable);
    bool tilt_isEnabled             (void);

#endif // __TILTSWITCH_H__
