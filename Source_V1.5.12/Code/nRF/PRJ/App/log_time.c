#define NRF_LOG_MODULE_NAME TIME
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "log_time.h"
#include "nrf.h"
#include "app_timer.h"
#include "math.h"
#include "ds3231_app.h"
#include <stdlib.h>
#include <stdbool.h>

static volatile time_t m_time;
static volatile uint32_t lastRTCcounterValue;
static volatile uint32_t remainderRTCcounter;

APP_TIMER_DEF(minute_app_timer);

callback_log_time_t main_cb = NULL;

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

    #if 0
        NRF_LOG_INFO("curr=%u, last=%u, remain=%u, addSec=%u, retTime=%u, mTime=%u", currentRTCcounterValue, lastRTCcounterValue, remainder, addSec, return_time, m_time);
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
       if(ds3231_enabled == 1)
      {
      return ds3231_getTime(); 
      }
      if(ds3231_enabled == 0)
      {
      return get_local_logtime_value(false);
      }
}

char * get_logtime_string(time_t timeValue)
{
    static char cal_string[80];
    static struct tm m_tm_return_time;

    m_tm_return_time = *localtime(&timeValue);
    strftime(cal_string, 80, "%x - %X", &m_tm_return_time);
    return cal_string;
}


void logtime_set_long(time_t newtime)
{
    uint32_t ret;

    // Stop the timer and restart the minute timer after setting the new values.
    ret = app_timer_stop(minute_app_timer);

    remainderRTCcounter = 0;
    m_time = newtime;
    lastRTCcounterValue = app_timer_cnt_get();

    ret = app_timer_start(minute_app_timer, APP_TIMER_TICKS(60UL * 1000UL), NULL);
    APP_ERROR_CHECK(ret);

    NRF_LOG_INFO("Set Long: %s, %u/0x%04X", get_logtime_string(newtime), newtime, newtime);
}


void logtime_set_time(uint32_t year, uint32_t month, uint32_t day, uint32_t hour, uint32_t minute, uint32_t second)
{
    time_t newtime;
    struct tm time_struct;
    uint32_t ret;

    time_struct.tm_year = year - 1900;
    time_struct.tm_mon = month;
    time_struct.tm_mday = day;
    time_struct.tm_hour = hour;
    time_struct.tm_min = minute;
    time_struct.tm_sec = second;   
    newtime = mktime(&time_struct);
    
    if(ds3231_enabled)
    {
      // update RTC with new time
      ds3231_setTime(time_struct);
    }
     
    // Apply the new time.            
    logtime_set_long(newtime);
}    


static void logtimeAppTimerCallback(void * p_context)
{
  time_t return_time;
  
  if (ds3231_enabled == 0)
    {
      // Get the current time in seconds
      return_time = get_local_logtime_value(true);

      // Return the current time back to the main.
      if(main_cb != NULL)
      {
          main_cb(return_time);
      }

      NRF_LOG_INFO("Time: %s, %u/0x%04X", get_logtime_string(m_time), m_time, m_time);
    }

  if(ds3231_enabled == 1)
    {

    // Get the current time in seconds 
    return_time = get_logtime_value();
    
      #ifdef DEBUG
        NRF_LOG_INFO("### APP_TIMER_CALLBACK ds3231 time: %s, %u/0x%04X", get_logtime_string(return_time), return_time, return_time);
      #endif

    // Return the current time back to the main.
    if(main_cb != NULL)
    {
        main_cb(return_time);
    }
    NRF_LOG_INFO("Time: %s, %u/0x%04X", get_logtime_string(return_time), return_time, return_time);
    }
}


void logtime_init(callback_log_time_t callback, const time_t lastKnownTime)
{    
    
    uint32_t ret = 0;
    main_cb = callback;

    // Create an apptimer to call every minute to the callback and calculate the passed time
    ret = app_timer_create(&minute_app_timer, APP_TIMER_MODE_REPEATED, logtimeAppTimerCallback);
    APP_ERROR_CHECK(ret);

    // Start the minute timer with the last known time value.
    logtime_set_long(lastKnownTime);
    NRF_LOG_INFO("### callback INIT time: %s, %u/0x%04X", get_logtime_string(lastKnownTime), lastKnownTime, lastKnownTime);
}