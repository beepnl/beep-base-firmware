#ifndef LOG_TIME_H
#define	LOG_TIME_H

	#include <stdint.h>
	#include <stdbool.h>
    #include "time.h"

    #define LF_CLK_FREQ 32768UL

    // #define LOGTIME_DEBUG

    typedef enum {
        LOGTIME_INTERNAL,
        LOGTIME_EXTERNAL
    } logtime_source_t;

    typedef void (*callback_log_time_t)(time_t);
    typedef time_t (*external_logtime_getter_t)();

    logtime_source_t logtime_source(void);

    time_t get_logtime_value(void);

    char * get_logtime_string(time_t timeValue);

    void logtime_set_long(time_t newtime);

    void logtime_init(callback_log_time_t callback, const time_t lastKnownTime, external_logtime_getter_t getter_func);

#endif	/* LOG_TIME */

