
#include <stdbool.h>
#include <stdint.h>
#include "SQ.h"
#include "app_timer.h"
#include "nrf_log.h"
#include "nrf_gpiote.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "boards.h"

APP_TIMER_DEF(vibTimer);


// Angle sensor
SQ_SEN_645B sen =
{
	.timeCount		= 0,
	.timeThreshold	= 25,
};

sq_event_handler_t sq_handler;


uint32_t SQ_SEN_645B_pinRead (void)
{
	return nrf_gpio_pin_read(SQ_SEN_645B_PIN);
}

static inline void SQ_Event_Call(SQ_EVENTS eventType)
{
	if(sq_handler != NULL)
	{
		sq_handler(eventType);
	}
}




static inline void SQ_SEN_645B_Tick(void)
{
	static uint32_t count = 0;
	// Get the current output
	sen.stateNew = SQ_SEN_645B_pinRead();

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
        #if SQ_DEBUG
			NRF_LOG_INFO("%u - SQ-SEN-645B new state: %u", count++, sen.stateCurrently);
		#endif
        SQ_Event_Call((sen.stateCurrently) ? SQ_SEN_645B_HIGH : SQ_SEN_645B_LOW);
	}
	#if 0
	else
	{
		NRF_LOG_INFO("%u - SQ-SEN-645B pin state: %u, time: %u", count++, sen.stateNew, sen.timeCount);
	}
	#endif
}


static void SQ_MIN_200_timerCallback( void * p_context )
{
	UNUSED_VARIABLE(p_context);
    SQ_SEN_645B_Tick();
}






void SetGPIOinterrupt(uint32_t pin, nrfx_gpiote_evt_handler_t evt_handler)
{
    uint32_t err_code;

    /*  Pin change interrupt event configuration. For low power consumption the accuracy must be set to false and in the interrupt must be checked whether the pin is high or low. */
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.hi_accuracy = false;

    err_code = nrf_drv_gpiote_in_init(pin, &in_config, evt_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(pin, true);
}


void SQ_init(sq_event_handler_t handler)
{
	uint32_t ret;

	if(handler != NULL)
	{
		sq_handler = handler;
	}

    // Create an app timer.
    ret = app_timer_create(&vibTimer, APP_TIMER_MODE_REPEATED, SQ_MIN_200_timerCallback);
    APP_ERROR_CHECK(ret);

	// Check the number of vibration pulses
    ret = app_timer_start(vibTimer, APP_TIMER_TICKS(1000), NULL);
    APP_ERROR_CHECK(ret);

	// Set the current pin input value as the current state.
	sen.stateCurrently	= SQ_SEN_645B_pinRead();
}




bool SQ_getOrientation(void)
{
	return (sen.stateCurrently) ? true : false;
}


