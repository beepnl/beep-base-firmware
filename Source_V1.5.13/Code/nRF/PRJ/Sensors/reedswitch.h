/*!
 * \file      reedswitch.h
 *
 * \brief     reedswitch code
 *
 */
#ifndef __REEDSWITCH_H__
#define __REEDSWITCH_H__

	#include <stdbool.h>
	#include <stdint.h>
	#include "beep_protocol.h"

    #define REEDSWITCH_RESET_COUNT                  45
    #define REEDSWITCH_ON_STATE_TOGGLE_COUNT        3
    #define REEDSWITCH_OFF_STATE_TOGGLE_COUNT       10

    typedef enum{
        REED_TICK,
        REED_SHORT_ACTIVATION,
        REED_RESET,
        REED_TOGGLE_STATE,
    }REED_EVENT_TYPE_e;

    typedef struct
    {
        REED_EVENT_TYPE_e   eventType;
        uint32_t            tickCount;
    }REED_EVENT_s;

    typedef void (*reedswitch_callback_fp) (REED_EVENT_s * event_p);

    bool    reedSwitchActive        (void);
    void    reedSwitchTimerEnable   (const bool enable);
    void    reedSwitchEnable        (const bool enable);
    bool    reedSwitchIsEnabled     (void);
    void    reedswitchInit          (reedswitch_callback_fp init_cb);
    bool    readswitchTimerActive   (void);

#endif // __REEDSWITCH_H__
