
#include <stdbool.h>
#include <stdint.h>
#define NRF_LOG_MODULE_NAME REED
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "app_timer.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "boards.h"
#include "nvm_fs.h"
#include "reedswitch.h"
#include "nrf_delay.h"

APP_TIMER_DEF(reedswitchtimer);
reedswitch_callback_fp mainCallback_fp = NULL;
static uint32_t reedSwitchTicks = 0;
volatile bool timerOn = false;
static volatile bool reedEnabled = false;

static void callMainCallback(const REED_EVENT_TYPE_e eventType)
{
    if(mainCallback_fp != NULL)
    {
        REED_EVENT_s param;
        memset(&param, 0, sizeof(param));
        param.eventType = eventType;
        param.tickCount = reedSwitchTicks;

        // Pass the parameters to the main
        mainCallback_fp(&param);
    }
}

/*@Brief: Returns the current state of the reedswitch: true when active, false when not active
 */
bool reedSwitchActive(void)
{
    uint32_t input = nrf_gpio_pin_read(BUTTON_1);
    return (input ? false : true);
}



/*@Brief: start or stop the timer.
 */
void reedSwitchTimerEnable(const bool enable)
{
    uint32_t ret;
    reedSwitchTicks = 0;
    if(enable)
    {
        if(!timerOn){
            ret = app_timer_start(reedswitchtimer, APP_TIMER_TICKS(1000), NULL);
            timerOn = true;
        }
    }
    else
    {
        ret = app_timer_stop(reedswitchtimer);
        timerOn = false;
    }
}

/*@Brief: App timer callback handler in which the current state of the reedswitch is determined and the actual orientation is determined.
 *        If the tiltswitch detects the current orientation for a longer period of time an orientation change is calledback to the main.
 */
static void reedSwitchCallback(void * p_context)
{
    // Check the pin state, when high increment. If low clear the timer.
    if(!nrf_gpio_pin_read(BUTTON_1))
    {
        reedSwitchTicks++;
        // NRF_LOG_INFO("Reedswitch tick count: %u", reedSwitchTicks);

        // When the reset count is reached, disable the reedswitch timer and call the callback to the main.
        if(reedSwitchTicks >= REEDSWITCH_RESET_COUNT)
        {
            reedSwitchTimerEnable(false);
            callMainCallback(REED_RESET);
        }
        else if(reedEnabled)
        {
            // On every increment perform a callback to trigger sound effects from the main application
            callMainCallback(REED_TICK);
        }
    }
    else
    {
        // NRF_LOG_INFO("Reedswitch STOP tick count: %u", reedSwitchTicks);
        const bool current_state = nvm_get_on_state();

        if((current_state && reedSwitchTicks >= REEDSWITCH_OFF_STATE_TOGGLE_COUNT) || (!current_state && reedSwitchTicks >=REEDSWITCH_ON_STATE_TOGGLE_COUNT))
        {
            reedSwitchTimerEnable(false);
            if(reedEnabled)
            {
                callMainCallback(REED_TOGGLE_STATE);
            }
        }
        else
        {
            callMainCallback(REED_SHORT_ACTIVATION);
        }
        reedSwitchTimerEnable(false);
    }
}


/*@Brief: Enable of disable the reedswitch functionality
 */
void reedSwitchEnable(const bool enable)
{
    if(enable == reedEnabled)
    {
        return;
    }

    // Apply the new state here or the timer cannot be started, due to the reedEnabled boolean being checked
    reedEnabled = enable;

    if(enable)
    {
        const bool reedswitchActive = reedSwitchActive();

        // If the reedswitch is currently activated start the reedswitch timer
        reedSwitchTimerEnable(reedswitchActive);
    }
    else
    {
        // disable the reedswitch functionality if currently active
        reedSwitchTimerEnable(false);  
    }
    
}

bool reedSwitchIsEnabled(void)
{
    return reedEnabled;
}


void reedswitchInit(reedswitch_callback_fp init_cb)
{
    uint32_t ret;
    ret = app_timer_create(&reedswitchtimer, APP_TIMER_MODE_REPEATED, reedSwitchCallback);
    APP_ERROR_CHECK(ret);

    mainCallback_fp = init_cb;
}

bool readswitchTimerActive(void)
{
    return timerOn;
}