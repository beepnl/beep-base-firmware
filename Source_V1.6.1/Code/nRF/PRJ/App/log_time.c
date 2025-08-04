#define NRF_LOG_MODULE_NAME TIME
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "log_time.h"
#include "nrf.h"
#include "app_timer.h"
#include "math.h"
#include <stdlib.h>

static volatile time_t m_time;
static volatile uint32_t lastRTCcounterValue;
static volatile uint32_t remainderRTCcounter;

APP_TIMER_DEF(minute_app_timer);

callback_log_time_t main_cb = NULL;
external_logtime_getter_t rtc_time = NULL;

logtime_source_t logtime_source(void)
{
    if (rtc_time != NULL)
    {
        return LOGTIME_EXTERNAL;
    }

    return LOGTIME_INTERNAL;
}

static time_t get_local_logtime_value(const bool rememberResult)
{
    time_t return_time;
    
    const uint32_t currentRTCcounterValue = app_timer_cnt_get();
    const uint32_t diff = app_timer_cnt_diff_compute(currentRTCcounterValue, lastRTCcounterValue);

    // Add the difference to the previous remainder and calculate the remainder numer of seconds passed 
    uint32_t remainder = remainderRTCcounter;
    const uint32_t sum = remainder + diff;
    remainder = sum % LF_CLK_FREQ;

    // Add the whole number of seconds to the time variable
    const uint32_t addSec = (sum - remainder) / LF_CLK_FREQ;
    return_time = m_time + addSec;

    // When the time overflows, substract the maximum positive value.
    if(return_time < 0)
    {
        return_time = (INT32_MAX - m_time) + addSec;
    }

    #ifdef LOGTIME_DEBUG
        // NRF_LOG_INFO("Internal RTC: curr=%u, last=%u, remain=%u, addSec=%u, retTime=%u, mTime=%u", currentRTCcounterValue, lastRTCcounterValue, remainder, addSec, return_time, m_time);
    #endif

    if(rememberResult)
    {
        remainderRTCcounter = remainder;
        m_time              = return_time;
        lastRTCcounterValue = currentRTCcounterValue;
    }

    return return_time;
}

time_t get_logtime_value(void)
{
    // Branch on availability of external time getter function
    if (logtime_source() == LOGTIME_EXTERNAL)
    {
        return rtc_time();
    }

    // Return internal time value if RTC not available
    return get_local_logtime_value(false);
}


char * get_logtime_string(time_t timeValue)
{
    static char cal_string[80];
    static struct tm m_tm_return_time;

    m_tm_return_time = *localtime(&timeValue);
    strftime(cal_string, 80, "%x - %X", &m_tm_return_time);
    return cal_string;
}

static void update_local_logtime_value(time_t newtime)
{
    uint32_t ret;

    // Stop the timer and restart the minute timer after setting the new values.
    ret = app_timer_stop(minute_app_timer);

    remainderRTCcounter = 0;
    m_time = newtime;
    lastRTCcounterValue = app_timer_cnt_get();

    ret = app_timer_start(minute_app_timer, APP_TIMER_TICKS(60UL * 1000UL), NULL);
    APP_ERROR_CHECK(ret);

    #ifdef LOGTIME_DEBUG
        // NRF_LOG_INFO("Set Internal RTC: %s, last_cnt: %u, %u/0x%04X", get_logtime_string(newtime), lastRTCcounterValue, newtime, newtime);
    #endif
}


void logtime_set_long(time_t newtime)
{
    uint32_t ret;

    if (logtime_source() == LOGTIME_EXTERNAL)
    {
        ds3231_set_long(newtime);
        update_local_logtime_value(newtime); // Keep the internal time in sync with the external RTC
    }
    else
    {
        // Update the internal timer with new value
        update_local_logtime_value(newtime);
    }

    // NRF_LOG_INFO("Set Logtime: %s, %u/0x%04X", get_logtime_string(newtime), newtime, newtime);
}


// void logtime_set_time(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
// {
//     time_t newtime;
//     struct tm time_struct;
//     uint32_t ret;

//     time_struct.tm_year = year - 1900;
//     time_struct.tm_mon = month;
//     time_struct.tm_mday = day;
//     time_struct.tm_hour = hour;
//     time_struct.tm_min = minute;
//     time_struct.tm_sec = second;   
//     newtime = mktime(&time_struct);
     
//     // Apply the new time.            
//     logtime_set_long(newtime);
// }    



static void logtimeAppTimerCallback(void * p_context)
{
    time_t now;

    // Branch on availability of external time getter function
   if (logtime_source() == LOGTIME_EXTERNAL)
   {
        // Update the time value with the external rtc time.
        now = rtc_time();
        update_local_logtime_value(now); // Keep the internal time in sync with the external RTC
   }
   else
   {
        // Get the current time in seconds
        now = get_local_logtime_value(true);
   }

    // Return the current time back to the main.
    if(main_cb != NULL)
    {
        main_cb(now);
    }

    // NRF_LOG_INFO("Time [%s]: %s, %u/0x%04X", (logtime_source() == LOGTIME_EXTERNAL) ? "external RTC" : "internal RTC", get_logtime_string(now), now, now);
}


void logtime_init(callback_log_time_t callback, const time_t lastKnownTime, external_logtime_getter_t getter_func)
{    
    uint32_t ret = 0;
    main_cb = callback;

    // Create an apptimer to call every minute to the callback and calculate the passed time
    ret = app_timer_create(&minute_app_timer, APP_TIMER_MODE_REPEATED, logtimeAppTimerCallback);
    APP_ERROR_CHECK(ret);

    // Branch on availability of external time getter function
    if (getter_func != NULL)
    {
        // Store the getter function
        rtc_time = getter_func;
        ds3231_enable_osc();
        time_t externalTime = getter_func();
        // NRF_LOG_INFO("Initialized: [External RTC] %s, %u/0x%04X", get_logtime_string(externalTime), externalTime, externalTime);
        // Start the minute timer with the time value from external rtc.
        update_local_logtime_value(getter_func());
    }
    else
    {
        // NRF_LOG_INFO("Initialized: [Last Known] %s, %u/0x%04X", get_logtime_string(lastKnownTime), lastKnownTime, lastKnownTime);
        // Start the minute timer with the last known time value.
        update_local_logtime_value(lastKnownTime);
    }
}