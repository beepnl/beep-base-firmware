#ifndef LOG_TIME_H
#define	LOG_TIME_H

	#include <stdint.h>
	#include <stdbool.h>
    #include "time.h"

    #define LF_CLK_FREQ 32768UL

    typedef void (*callback_log_time_t)(time_t);

    time_t get_logtime_value(void);

    char * get_logtime_string(time_t timeValue);

    void logtime_set_long(time_t newtime);

    void logtime_init(callback_log_time_t callback, const time_t lastKnownTime);

#endif	/* LOG_TIME */

