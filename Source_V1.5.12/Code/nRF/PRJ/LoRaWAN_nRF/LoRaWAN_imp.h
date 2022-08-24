/*!
 * \file      LoRaWAN_imp.h
 *
 * \brief     LoRaWAN implementation
 *
 */
#ifndef __LORAWAN_IMP_H__
#define __LORAWAN_IMP_H__

typedef enum
{
	SFI_SENSOR_OFF	= 0,
	SFI_SENSOR_ON	= 1,
	SFI_KEEP_ALIVE	= 2,
	SFI_MOVING		= 3,
	SFI_STANDSTILL	= 4,
	SFI_HORIZONTAL	= 5,
	SFI_VERTICAL	= 6,
	SFI_BUTTON		= 7,
	SFI_UNKNOWN		= 0xFF,
}SFI_STATUS;



/*!
 * Indicate if the TTN network parameters have to be used.
 */
#define USE_TTN_NETWORK                             0

/*!
 * Defines the application data transmission duty cycle. 60s, value in [ms].
 */
#define APP_TX_DUTYCYCLE                            (1 * 60 * 1000) // 6.000.000
#define APP_JOIN_DUTYCYCLE                          (30 * 1000)

/*!
 * Defines a random delay for application data transmission duty cycle. 10s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        10000

/*!
 * Default datarate
 */
#define LORAWAN_DEFAULT_DATARATE                    DR_0

/*!
 * LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON                    false

/*!
 * LoRaWAN Adaptive Data Rate
 *
 * \remark Please note that when ADR is enabled the end-device should be static
 */
#define LORAWAN_ADR_ON                              1

#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )

#include "LoRaMacTest.h"

/*!
 * LoRaWAN ETSI duty cycle control enable/disable
 *
 * \remark Please note that ETSI mandates duty cycled transmissions. Use only for test purposes
 */
#define LORAWAN_DUTYCYCLE_ON                        true

#endif

/*!
 * LoRaWAN application port
 */
#define LORAWAN_APP_PORT                            2


int main_lorawan_implementation( void );

void setTrigger(SFI_STATUS triggerValue);


#endif // __LORAWAN_IMP_H__
