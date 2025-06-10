#include <stdbool.h>
#include <stdint.h>
#include "app_timer.h"
#include "tiltswitch.h"
#define NRF_LOG_MODULE_NAME TILT
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "boards.h"
#include "nvm_fs.h"

APP_TIMER_DEF(tiltTimer);
static volatile bool orientationEnabled = false;

// Angle sensor
TILT_s sen =
{
	.timeCount          = 0,
	#ifdef DEBUG
		.timeThreshold	= 5,
	#else
		.timeThreshold	= 5,
	#endif
};

tilt_callback_t main_handler;

bool tilt_startup(void)
{
	return sen.startUp;
}


uint32_t tilt_pinRead (void)
{
	return nrf_gpio_pin_read(SQ_SEN_645B_PIN);
}


/*@Brief: Callback wrapper
 */
static inline void tilt_Event_Callback(TILT_ORIENTATIONS_e eventType)
{
	if(main_handler != NULL)
	{
		main_handler(eventType);
	}
}


static inline void tilt_Tick(void)
{
	static uint32_t count = 0;
	// Get the current output
	sen.stateNew = tilt_pinRead();

	if(sen.startUp)
	{
		// On start-up determine the current orientation over a longer time to prevent snap shots.
		if(sen.stateNew)
		{
			if((sen.countHorizontal < UINT8_MAX))
			{
				sen.countHorizontal++;
			}
		}
		else
		{
			if((sen.countVertical < UINT8_MAX))
			{
				sen.countVertical++;
			}
		}

		sen.timeCount++;
		if(sen.timeCount >= sen.timeCount)
		{
			sen.timeCount = 0;
			if(sen.countHorizontal > sen.countVertical)
			{
				sen.stateCurrently = 1;
			}
			else
			{
				sen.stateCurrently = 0;
			}

			// After the start-up period and the current orientation has been detected, call the main application
			sen.startUp = false;
			// NRF_LOG_INFO("Startup orientation is %s", (sen.stateCurrently) ? "Horizontal" : "Vertical");

            tilt_Event_Callback((sen.stateCurrently) ? TILT_HORIZONTAL : TILT_VERTICAL);
		}
	}
	else
	{
		if(sen.stateNew != sen.stateCurrently)
		{
			sen.timeCount++;
		}
		else if(sen.timeCount > 0)
		{
			sen.timeCount--;
		}

		if(sen.timeCount >= sen.timeThreshold)
		{
			sen.timeCount		= 0;
			sen.stateCurrently	= sen.stateNew;
			#if TILT_DEBUG
				// NRF_LOG_INFO("%u - SQ-SEN-645B new state: %u", count++, sen.stateCurrently);
			#endif
			tilt_Event_Callback((sen.stateCurrently) ? TILT_HORIZONTAL : TILT_VERTICAL);
		}
		#if 0
		else
		{
			// NRF_LOG_INFO("%u - SQ-SEN-645B pin state: %u, time: %u", count++, sen.stateNew, sen.timeCount);
		}
		#endif
	}
}


static void tilt_timerCallback( void * p_context )
{
	UNUSED_VARIABLE(p_context);
    tilt_Tick();
}

/*@Brief: tilt switch initialization which creates an apptimer regardless whether the functionality is used or not.
 */
void tilt_init(tilt_callback_t handler)
{
	uint32_t ret;

	main_handler = handler;
    // Create an app timer.
    ret = app_timer_create(&tiltTimer, APP_TIMER_MODE_REPEATED, tilt_timerCallback);
    APP_ERROR_CHECK(ret);
}


/*@Brief: Function to enable or disable the tilt switch functionality
 */
void tilt_enableOrientation(const bool enable)
{
    uint32_t ret;

    // Check for a change in state.
    if(orientationEnabled == enable)
    {
        return;
    }

    if(enable)
    {
        // Clear all parameters
        sen.startUp			= true;
        sen.timeCount		= 0;
        sen.countHorizontal	= 0;
        sen.countVertical	= 0;

        // Enable the tilt swith
        nrf_gpio_cfg_input(SQ_SEN_645B_PIN, NRF_GPIO_PIN_NOPULL);

        // Start the app timer on 1s interval to check the tilt switch periodically
        ret = app_timer_start(tiltTimer, APP_TIMER_TICKS(1000), NULL);
        APP_ERROR_CHECK(ret);

        // Set the current pin input value as the current state.
        const bool on = nvm_get_on_state();
        sen.stateCurrently	= on ? 0 : 1;
    }
    else
    {
        // Disable the timer
        ret = app_timer_stop(tiltTimer);

        // Disable the tilt switch
        nrf_gpio_input_disconnect(SQ_SEN_645B_PIN);
    }
    
    orientationEnabled = enable;
}

bool tilt_isEnabled(void)
{
    return orientationEnabled;
}


bool tilt_getOrientation(void)
{
    if(!orientationEnabled)
    {
        return true;
    }
    else
    {
        return (sen.stateCurrently) ? true : false;
    }
}

