/*!
 * \file      gpio-board.c
 *
 * \brief     Target board GPIO driver implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 * \code
 *                ______                              _
 *               / _____)             _              | |
 *              ( (____  _____ ____ _| |_ _____  ____| |__
 *               \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 *               _____) ) ____| | | || |_| ____( (___| | | |
 *              (______/|_____)_|_|_| \__)_____)\____)_| |_|
 *              (C)2013-2017 Semtech
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Marten Lootsma(TWTG) on behalf of Microchip/Atmel (c)2017
 */
#include "gpio-board.h"
#include "boards.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"


void GpioMcuInit( void )
{
    // Configure the LEDs as outputs and set the pins high
	#ifdef BSP_LED_0
		nrf_gpio_cfg_output(BSP_LED_0); nrf_gpio_pin_set(BSP_LED_0);
    #endif

    #ifdef BSP_LED_1
		nrf_gpio_cfg_output(BSP_LED_1); nrf_gpio_pin_set(BSP_LED_1);
    #endif

    #ifdef BSP_LED_2
		nrf_gpio_cfg_output(BSP_LED_2); nrf_gpio_pin_set(BSP_LED_2);
    #endif

    #ifdef BSP_LED_3
		nrf_gpio_cfg_output(BSP_LED_3); nrf_gpio_pin_set(BSP_LED_3);
	#endif

    // Configure the IO of the RFM module: NSS and RST pins as output and high, DIO as inputs.
    nrf_gpio_cfg_input(RFM_A_DIO_0_PIN, GPIO_PIN_CNF_PULL_Disabled);
    nrf_gpio_cfg_input(RFM_A_DIO_1_PIN, GPIO_PIN_CNF_PULL_Disabled);

    #ifdef RFM_A_DIO_2_PIN
		nrf_gpio_cfg_input(RFM_A_DIO_2_PIN, GPIO_PIN_CNF_PULL_Disabled);
	#endif

	#ifdef RFM_A_DIO_3_PIN
		nrf_gpio_cfg_input(RFM_A_DIO_3_PIN, GPIO_PIN_CNF_PULL_Disabled);
	#endif

    #ifdef RFM_A_DIO_4_PIN
		nrf_gpio_cfg_input(RFM_A_DIO_4_PIN, GPIO_PIN_CNF_PULL_Disabled);
	#endif

    nrf_gpio_cfg_input(RFM_A_DIO_5_PIN, GPIO_PIN_CNF_PULL_Disabled);

    nrf_gpio_cfg_output(RFM_A_NSS_PIN);
    nrf_gpio_cfg_output(RFM_A_RST_PIN);

    nrf_gpio_pin_set(RFM_A_NSS_PIN);
    nrf_gpio_pin_set(RFM_A_RST_PIN);


    nrf_gpio_cfg_output(OWI_PIN);
    nrf_gpio_pin_clear(OWI_PIN);

	// SQ movement sensors
	#ifdef SQ_MIN_200_PIN
        nrf_gpio_cfg_input(SQ_MIN_200_PIN, GPIO_PIN_CNF_PULL_Disabled);
	#endif

	#if SQ_SEN_645B_PIN
		nrf_gpio_cfg_input(SQ_SEN_645B_PIN, GPIO_PIN_CNF_PULL_Disabled);
	#endif
}

void GpioIntInit(void)
{
    uint32_t err_code;
    if(nrf_drv_gpiote_is_init())
    {
		return;
    }

    err_code = nrf_drv_gpiote_init();
    APP_ERROR_CHECK(err_code);
}

void GpioMcuSetContext( Gpio_t *obj, void* context )
{
//    obj->Context = context;
}

void GpioMcuSetInterrupt( uint32_t pin, nrfx_gpiote_evt_handler_t evt_handler)
{
    uint32_t err_code;

    /*  Pin change interrupt event configuration. For low power consumption the accuracy must be set to false and in the interrupt must be checked whether the pin is high or low. */
    nrf_drv_gpiote_in_config_t in_config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
    in_config.hi_accuracy = false;

    err_code = nrf_drv_gpiote_in_init(pin, &in_config, evt_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_gpiote_in_event_enable(pin, true);
}



void GpioMcuRemoveInterrupt( Gpio_t *obj )
{
    //ext_irq_register( obj->pin, NULL );
}

void GpioMcuWrite( Gpio_t *obj, uint32_t value )
{
//
//    if( obj == NULL )
//    {
//        //assert_param( FAIL );
//        while( 1 );
//    }
//    // Check if pin is not connected
//    if( obj->pin == NC )
//    {
//        return;
//    }
//    gpio_set_pin_level( obj->pin, value );
}

void GpioMcuToggle( Gpio_t *obj )
{
//    if( obj == NULL )
//    {
//        //assert_param( FAIL );
//        while( 1 );
//    }
//
//    // Check if pin is not connected
//    if( obj->pin == NC )
//    {
//        return;
//    }
//    gpio_toggle_pin_level( obj->pin );
}

uint32_t GpioMcuRead( Gpio_t *obj )
{
//    if( obj == NULL )
//    {
//        //assert_param( FAIL );
//        while( 1 );
//    }
//    // Check if pin is not connected
//    if( obj->pin == NC )
//    {
//        return 0;
//    }
//    return ( uint32_t )gpio_get_pin_level( obj->pin );
}
