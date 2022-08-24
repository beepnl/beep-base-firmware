/*!
 * \file      board.c
 *
 * \brief     Target board general functions implementation
 *
 * \author    
 */
#include "bsp.h"
#include "boards.h"
#include "utilities.h"
#include "rtc-board.h"
#include "sx1276-board.h"
#include "spi-board.h"
#include "board.h"
#include "boards.h"
#include "gpio-board.h"
#include "nvm_fs.h"

#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_soc.h"
#include "sdk_config.h"

#include "nrf_drv_clock.h"
#include "app_timer.h"
#include "app_error.h"
#include "rng_nrf.h"
#include "nrf_gpio.h"
#include "timestamp_timer.h"


_APP_TIMER_DEF(Blinky);


/*
 * MCU objects
 */
//Gpio_t Led1;
//Uart_t Uart1;

/*!
 * Flag to indicate if the MCU is Initialized
 */
static bool McuInitialized = false;



static void BoardBlink_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
	#ifdef BSP_LED_0
    nrf_gpio_pin_toggle(BSP_LED_0);
	#endif

	#if 0
	static uint32_t blinkCounter = 0;
	NRF_LOG_INFO("%u blink", blinkCounter++);
	#endif
}


void BoardBlinkSetup(void)
{
    uint32_t retVal;

    // Create blinky timer.
    retVal = app_timer_create(&Blinky, APP_TIMER_MODE_REPEATED, BoardBlink_handler);
    APP_ERROR_CHECK(retVal);

    // Start blinky timer.
    retVal = app_timer_start(Blinky, APP_TIMER_TICKS(1000), NULL);
    APP_ERROR_CHECK(retVal);
}

void BoardInitPeriph( void )
{
    uint32_t ret;

    #if NRF_LOG_ENABLED
		ret = NRF_LOG_INIT(get_timestamp_counter);
    #else
		ret = NRF_LOG_INIT(NULL);
    #endif

    APP_ERROR_CHECK(ret);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    GpioIntInit();

    RtcInit();

    #if NRF_LOG_ENABLED
		timestamp_init();
    #endif

    //Configure all leds and buttons on board.
	extern void bsp_evt_handler(bsp_event_t evt);
    ret = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_evt_handler);

    init_rng();

    SpiInit( NULL, 0, RFM_A_MOSI_PIN, RFM_A_MISO_PIN, RFM_A_SCK_PIN, RFM_A_NSS_PIN );

    // Test function to periodically generate an interrrupt to force the MCU to wake-up 
//    BoardBlinkSetup();

    nvm_fds_init();

    SX1276IoDbgInit( );
    SX1276IoTcxoInit( );
}

void BoardInitMcu( void )
{

}

void BoardResetMcu( void )
{

}

void BoardDeInitMcu( void )
{
	uint32_t err_code;

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}

uint32_t BoardGetRandomSeed( void )
{
    return 0;
}

void BoardGetUniqueId( uint8_t *id )
{
    // We don't have an ID, so use the one from Commissioning.h
}

uint8_t BoardGetBatteryLevel( void )
{
    return 0; //  Battery level [0: node is connected to an external power source ...
}

uint8_t GetBoardPowerSource( void )
{
    return USB_POWER;
}

void BoardLowPowerHandler( void )
{
    // Handle the logging before going to sleep
    if(!NRF_LOG_PROCESS())
    {
		nrf_pwr_mgmt_run();
    }
}

#if !defined ( __CC_ARM )

/*
 * Function to be used by stdout for printf etc
 */
int _write( int fd, const void *buf, size_t count )
{

}

/*
 * Function to be used by stdin for scanf etc
 */
int _read( int fd, const void *buf, size_t count )
{

}

#else

// Keil compiler
int fputc( int c, FILE *stream )
{
    while( UartPutChar( &Uart1, ( uint8_t )c ) != 0 );
    return c;
}

int fgetc( FILE *stream )
{
    uint8_t c = 0;
    while( UartGetChar( &Uart1, &c ) != 0 );
    // Echo back the character
    while( UartPutChar( &Uart1, c ) != 0 );
    return ( int )c;
}

#endif

#ifdef USE_FULL_ASSERT
/*
 * Function Name  : assert_failed
 * Description    : Reports the name of the source file and the source line number
 *                  where the assert_param error has occurred.
 * Input          : - file: pointer to the source file name
 *                  - line: assert_param error line source number
 * Output         : None
 * Return         : None
 */
void assert_failed( uint8_t* file, uint32_t line )
{
    /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %u\r\n", file, line) */

    printf( "Wrong parameters value: file %s on line %u\r\n", ( const char* )file, line );
    /* Infinite loop */
    while( 1 )
    {
    }
}
#endif
