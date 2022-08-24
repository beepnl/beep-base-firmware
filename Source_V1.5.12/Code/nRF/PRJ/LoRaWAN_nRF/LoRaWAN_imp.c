/*!
 * \file      LoRaWAN_imp.c
 *
 * \brief     LoRaMac classA device implementation
 *
 * \copyright Revised BSD License, see section \ref LICENSE.
 *
 */

/*! \file classA/nRF52840/main.c */

#include <stdio.h>
#include "utilities.h"
#include "board.h"
#include "gpio.h"
#include "gpio-board.h"
#include "LoRaMac.h"
#include "Commissioning.h"
#include "LoRaWAN_imp.h"
#include "NvmCtxMgmt.h"

#include "nrf.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "LmHandlerMsgDisplay.h"

#include "nrf_drv_clock.h"
#include "app_timer.h"
#include "app_error.h"
#include "rng_nrf.h"
#include "nvm_fs.h"
#include "utilities.h"
#include "SQ.h"
#include "CayenneLpp.h"




static uint8_t DevEui[]		= LORAWAN_DEVICE_EUI;
static uint8_t JoinEui[]	= LORAWAN_JOIN_EUI;
#if( ABP_ACTIVATION_LRWAN_VERSION == ABP_ACTIVATION_LRWAN_VERSION_V10x )
static uint8_t GenAppKey[]	= LORAWAN_GEN_APP_KEY;
#else
static uint8_t AppKey[]		= LORAWAN_APP_KEY;
#endif
static uint8_t NwkKey[]		= LORAWAN_NWK_KEY;

#if( OVER_THE_AIR_ACTIVATION == 0 )

static uint8_t FNwkSIntKey[]	= LORAWAN_F_NWK_S_INT_KEY;
static uint8_t SNwkSIntKey[]	= LORAWAN_S_NWK_S_INT_KEY;
static uint8_t NwkSEncKey[]		= LORAWAN_NWK_S_ENC_KEY;
static uint8_t AppSKey[]		= LORAWAN_APP_S_KEY;

/*!
 * Device address
 */
static uint32_t DevAddr = LORAWAN_DEVICE_ADDRESS;

#endif

SFI_STATUS sfiMsgtTrigger = SFI_UNKNOWN;

/*!
 * Application port
 */
static uint8_t AppPort = LORAWAN_APP_PORT;

/*!
 * User application data size
 */
static uint8_t AppDataSize = 1;
static uint8_t AppDataSizeBackup = 1;

/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE                           242

/*!
 * User application data
 */
static uint8_t AppDataBuffer[LORAWAN_APP_DATA_MAX_SIZE];

/*!
 * Indicates if the node is sending confirmed or unconfirmed messages
 */
static uint8_t IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;

/*!
 * Defines the application data transmission duty cycle
 */
static uint32_t TxDutyCycleTime;

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t TxNextPacketTimer;

/*!
 * Specifies the state of the application LED
 */
static bool AppLedStateOn = false;

/*!
 * Timer to handle the state of LED1
 */
static TimerEvent_t Led1Timer;

/*!
 * Timer to handle the state of LED2
 */
static TimerEvent_t Led2Timer;

/*!
 * Indicates if a new packet can be sent
 */
static bool NextTx = false;

/*!
 * Indicates if LoRaMacProcess call is pending.
 * 
 * \warning If variable is equal to 0 then the MCU can be set in low power mode
 */
static uint8_t IsMacProcessPending = 0;

/*!
 * Device states
 */
static enum eDeviceState
{
    DEVICE_STATE_RESTORE,
    DEVICE_STATE_START,
    DEVICE_STATE_JOIN,
    DEVICE_STATE_SEND,
    DEVICE_STATE_CYCLE,
    DEVICE_STATE_SLEEP
}DeviceState;

/*!
 * LoRaWAN compliance tests support data
 */
struct ComplianceTest_s
{
    bool Running;
    uint8_t State;
    bool IsTxConfirmed;
    uint8_t AppPort;
    uint8_t AppDataSize;
    uint8_t *AppDataBuffer;
    uint16_t DownLinkCounter;
    bool LinkCheck;
    uint8_t DemodMargin;
    uint8_t NbGateways;
}ComplianceTest;


/*!
 * Application data structure
 */
typedef struct LoRaMacHandlerAppData_s
{
    LmHandlerMsgTypes_t MsgType;
    uint8_t Port;
    uint8_t BufferSize;
    uint8_t *Buffer;
}LoRaMacHandlerAppData_t;

LoRaMacHandlerAppData_t AppData =
{
    .MsgType = LORAMAC_HANDLER_UNCONFIRMED_MSG,
    .Buffer = NULL,
    .BufferSize = 0,
    .Port = 0
};

/*!
 * LED GPIO pins objects
 */
Gpio_t Led1; // Tx


/*!
 * Prints the provided buffer in HEX
 * 
 * \param buffer Buffer to be printed
 * \param size   Buffer size to be printed
 */
extern void PrintHexBuffer( uint8_t *buffer, uint8_t size );

void setTrigger(SFI_STATUS triggerValue)
{
	sfiMsgtTrigger = triggerValue;
}


/*!
 * Executes the network Join request
 */
static void JoinNetwork( void )
{
    LoRaMacStatus_t status;
    MlmeReq_t mlmeReq;
    mlmeReq.Type				= MLME_JOIN;
    mlmeReq.Req.Join.DevEui		= DevEui;
    mlmeReq.Req.Join.JoinEui	= JoinEui;
    mlmeReq.Req.Join.Datarate	= LORAWAN_DEFAULT_DATARATE;

    // Starts the join procedure
    status = LoRaMacMlmeRequest( &mlmeReq );

	#if 0
		NRF_LOG_INFO( "###### ===== MLME-Request - MLME_JOIN ==== ######" );
		NRF_LOG_INFO( "JOIN STATUS      : %s", getMacStatusStringPointer(status) );
	#endif

    if( status == LORAMAC_STATUS_OK )
    {
        NRF_LOG_INFO( "###### ===== JOINING ==== ######" );
        DeviceState = DEVICE_STATE_SLEEP;
    }
    else
    {
		//NRF_LOG_INFO( "JOIN STATUS      : %s = DEVICE_STATE_CYCLE", getMacStatusStringPointer(status) );
        DeviceState = DEVICE_STATE_CYCLE;
    }
}

/*!
 * \brief   Prepares the payload of the frame
 */
static void PrepareTxFrame( uint8_t port )
{
    switch( port )
    {
    case 2:
        {
            AppDataSizeBackup = AppDataSize = 1;
            AppDataBuffer[0] = AppLedStateOn;
        }
        break;
    case 224:
        if( ComplianceTest.LinkCheck == true )
        {
            ComplianceTest.LinkCheck = false;
            AppDataSize				= 3;
            AppDataBuffer[0]		= 5;
            AppDataBuffer[1]		= ComplianceTest.DemodMargin;
            AppDataBuffer[2]		= ComplianceTest.NbGateways;
            ComplianceTest.State	= 1;
        }
        else
        {
            switch( ComplianceTest.State )
            {
            case 4:
                ComplianceTest.State = 1;
                break;
            case 1:
                AppDataSize = 2;
                AppDataBuffer[0] = ComplianceTest.DownLinkCounter >> 8;
                AppDataBuffer[1] = ComplianceTest.DownLinkCounter;
                break;
            }
        }
        break;
    default:
        break;
    }
}

/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
static bool SendFrame( void )
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;

    if( LoRaMacQueryTxPossible( AppDataSize, &txInfo ) != LORAMAC_STATUS_OK )
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer = NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate = LORAWAN_DEFAULT_DATARATE;
    }
    else
    {
        if( IsTxConfirmed == false )
        {
            mcpsReq.Type = MCPS_UNCONFIRMED;
            mcpsReq.Req.Unconfirmed.fPort		= AppPort;
            mcpsReq.Req.Unconfirmed.fBuffer		= AppDataBuffer;
            mcpsReq.Req.Unconfirmed.fBufferSize	= AppDataSize;
            mcpsReq.Req.Unconfirmed.Datarate	= LORAWAN_DEFAULT_DATARATE;
        }
        else
        {
            mcpsReq.Type = MCPS_CONFIRMED;
            mcpsReq.Req.Confirmed.fPort			= AppPort;
            mcpsReq.Req.Confirmed.fBuffer		= AppDataBuffer;
            mcpsReq.Req.Confirmed.fBufferSize	= AppDataSize;
            mcpsReq.Req.Confirmed.Datarate		= LORAWAN_DEFAULT_DATARATE;
        }
    }

    // Update global variable
    AppData.MsgType		= ( mcpsReq.Type == MCPS_CONFIRMED ) ? LORAMAC_HANDLER_CONFIRMED_MSG : LORAMAC_HANDLER_UNCONFIRMED_MSG;
    AppData.Port		= mcpsReq.Req.Unconfirmed.fPort;
    AppData.Buffer		= mcpsReq.Req.Unconfirmed.fBuffer;
    AppData.BufferSize	= mcpsReq.Req.Unconfirmed.fBufferSize;

    LoRaMacStatus_t status;
    status = LoRaMacMcpsRequest( &mcpsReq );
    NRF_LOG_INFO( "###### ===== SendFrame ==== ######" );
    NRF_LOG_INFO( "STATUS      : %s", getMacStatusStringPointer(status) );

    if( status == LORAMAC_STATUS_OK )
    {
        return false;
    }
    return true;
}

const char* SFIstatusToString [] =
{
	"SFI_SENSOR_OFF",
	"SFI_SENSOR_ON",
	"SFI_KEEP_ALIVE",
	"SFI_MOVING",
	"SFI_STANDSTILL",
	"SFI_HORIZONTAL",
	"SFI_VERTICAL",
};

/*!
 * \brief   Prepares the payload of the frame
 *
 * \retval  [0: frame could be send, 1: error]
 */
static bool SFI_SendFrame( SFI_STATUS trigger )
{
    McpsReq_t mcpsReq;
    LoRaMacTxInfo_t txInfo;
	uint8_t sqState = 0;
    uint8_t SFIdataSize = 6;


    if( LoRaMacQueryTxPossible( SFIdataSize, &txInfo ) != LORAMAC_STATUS_OK )
    {
        // Send empty frame in order to flush MAC commands
        mcpsReq.Type = MCPS_UNCONFIRMED;
        mcpsReq.Req.Unconfirmed.fBuffer		= NULL;
        mcpsReq.Req.Unconfirmed.fBufferSize = 0;
        mcpsReq.Req.Unconfirmed.Datarate	= LORAWAN_DEFAULT_DATARATE;
    }
    else
    {
		CayenneLppInit();

		if(SQ_getOrientation())
		{
			sqState |= (1 << 0);
		}
		
        CayenneLppAddDigitalInput(0, trigger);		//	Message trigger status
        CayenneLppAddDigitalInput(1, sqState);		//	Set the status of the sensor


		mcpsReq.Type = MCPS_UNCONFIRMED;
		mcpsReq.Req.Unconfirmed.fPort		= AppPort;
		mcpsReq.Req.Unconfirmed.fBuffer		= CayenneLppGetBuffer();
		mcpsReq.Req.Unconfirmed.fBufferSize	= CayenneLppGetSize();
		mcpsReq.Req.Unconfirmed.Datarate	= LORAWAN_DEFAULT_DATARATE;
    }

    // Update global variable
    AppData.MsgType		= ( mcpsReq.Type == MCPS_CONFIRMED ) ? LORAMAC_HANDLER_CONFIRMED_MSG : LORAMAC_HANDLER_UNCONFIRMED_MSG;
    AppData.Port		= mcpsReq.Req.Unconfirmed.fPort;
    AppData.Buffer		= mcpsReq.Req.Unconfirmed.fBuffer;
    AppData.BufferSize	= mcpsReq.Req.Unconfirmed.fBufferSize;

    LoRaMacStatus_t status;
    status = LoRaMacMcpsRequest( &mcpsReq );
    NRF_LOG_INFO( "###### ===== SFI SendFrame ==== ######" );
    NRF_LOG_INFO( "Trigger: %s, STATUS      : %s", SFIstatusToString[sfiMsgtTrigger], getMacStatusStringPointer(status) );
    sfiMsgtTrigger = SFI_UNKNOWN; // Clear the message trigger

    if( status == LORAMAC_STATUS_OK )
    {
        return false;
    }
    return true;
}

/*!
 * \brief Function executed on TxNextPacket Timeout event
 */
static void OnTxNextPacketTimerEvent( void* context )
{
    MibRequestConfirm_t mibReq;
    LoRaMacStatus_t status;

    TimerStop( &TxNextPacketTimer );

    mibReq.Type = MIB_NETWORK_ACTIVATION;
    status = LoRaMacMibGetRequestConfirm( &mibReq );

    if( status == LORAMAC_STATUS_OK )
    {
        if( mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE )
        {
            // Network not joined yet. Try to join again
            JoinNetwork( );
        }
        else
        {
			static uint8_t sendMsgCounter = 0;
            sendMsgCounter++;
			if(sendMsgCounter > 10)
			{
				sendMsgCounter = 0;
                setTrigger(SFI_KEEP_ALIVE);
			}
            DeviceState = DEVICE_STATE_CYCLE;
        }
    }
}

/*!
 * \brief Function executed on Led 1 Timeout event
 */
static void OnLed1TimerEvent( void* context )
{
    TimerStop( &Led1Timer );
    // Switch LED 1 OFF
    GpioWrite( &Led1, 0 );
}

/*!
 * \brief Function executed on Led 2 Timeout event
 */
static void OnLed2TimerEvent( void* context )
{
    TimerStop( &Led2Timer );
    // Switch LED 1 OFF
    GpioWrite( &Led1, 0 );
}

/*!
 * \brief   MCPS-Confirm event function
 *
 * \param   [IN] mcpsConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void McpsConfirm( McpsConfirm_t *mcpsConfirm )
{
    NRF_LOG_INFO( "###### ===== MCPS-Confirm ==== ######" );
    NRF_LOG_INFO( "STATUS      : %s", getEventInfoStatusStringPointer(mcpsConfirm->Status) );
    if( mcpsConfirm->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
    }
    else
    {
        switch( mcpsConfirm->McpsRequest )
        {
            case MCPS_UNCONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                break;
            }
            case MCPS_CONFIRMED:
            {
                // Check Datarate
                // Check TxPower
                // Check AckReceived
                // Check NbTrans
                break;
            }
            case MCPS_PROPRIETARY:
            {
                break;
            }
            default:
                break;
        }

        // Switch LED 1 ON
        GpioWrite( &Led1, 0 );
        TimerStart( &Led1Timer );
    }
    MibRequestConfirm_t mibGet;
    MibRequestConfirm_t mibReq;

    mibReq.Type = MIB_DEVICE_CLASS;
    LoRaMacMibGetRequestConfirm( &mibReq );

    NRF_LOG_INFO( "###### ===== UPLINK FRAME %lu ==== ######", mcpsConfirm->UpLinkCounter );
    NRF_LOG_INFO( "" );

    NRF_LOG_INFO( "CLASS       : %c", "ABC"[mibReq.Param.Class] );
    NRF_LOG_INFO( "TX PORT     : %d", AppData.Port );

    if( AppData.BufferSize != 0 )
    {
        NRF_LOG_INFO( "TX DATA     : " );
        if( AppData.MsgType == LORAMAC_HANDLER_CONFIRMED_MSG )
        {
            NRF_LOG_INFO( "CONFIRMED - %s", ( mcpsConfirm->AckReceived != 0 ) ? "ACK" : "NACK" );
        }
        else
        {
            NRF_LOG_INFO( "UNCONFIRMED" );
        }
        PrintHexBuffer( AppData.Buffer, AppData.BufferSize );
    }

    NRF_LOG_INFO( "" );
    NRF_LOG_INFO( "DATA RATE   : DR_%d", mcpsConfirm->Datarate );

    mibGet.Type  = MIB_CHANNELS;
    if( LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK )
    {
        NRF_LOG_INFO( "U/L FREQ    : %lu", mibGet.Param.ChannelList[mcpsConfirm->Channel].Frequency );
    }

    NRF_LOG_INFO( "TX POWER    : %d", mcpsConfirm->TxPower );

    mibGet.Type  = MIB_CHANNELS_MASK;
    if( LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK )
    {
        NRF_LOG_INFO("CHANNEL MASK: ");
		#if defined( REGION_AS923 ) || defined( REGION_CN779 ) || \
			defined( REGION_EU868 ) || defined( REGION_IN865 ) || \
			defined( REGION_KR920 ) || defined( REGION_EU433 ) || \
			defined( REGION_RU864 )

			for( uint8_t i = 0; i < 1; i++)

		#elif defined( REGION_AU915 ) || defined( REGION_US915 ) || defined( REGION_CN470 )
			for( uint8_t i = 0; i < 5; i++)
		#else
			#error "Please define a region in the compiler options."
		#endif
        {
            NRF_LOG_INFO("%04X ", mibGet.Param.ChannelsMask[i] );
        }
        NRF_LOG_INFO("");
    }

    NRF_LOG_INFO( "" );
}

/*!
 * \brief   MCPS-Indication event function
 *
 * \param   [IN] mcpsIndication - Pointer to the indication structure,
 *               containing indication attributes.
 */
static void McpsIndication( McpsIndication_t *mcpsIndication )
{
    NRF_LOG_INFO( "\r\n###### ===== MCPS-Indication ==== ######" );
    NRF_LOG_INFO( "STATUS      : %s", getEventInfoStatusStringPointer(mcpsIndication->Status) );
    if( mcpsIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
        return;
    }

    switch( mcpsIndication->McpsIndication )
    {
        case MCPS_UNCONFIRMED:
        {
            break;
        }
        case MCPS_CONFIRMED:
        {
            break;
        }
        case MCPS_PROPRIETARY:
        {
            break;
        }
        case MCPS_MULTICAST:
        {
            break;
        }
        default:
            break;
    }

    // Check Multicast
    // Check Port
    // Check Datarate
    // Check FramePending
    if( mcpsIndication->FramePending == true )
    {
        // The server signals that it has pending data to be sent.
        // We schedule an uplink as soon as possible to flush the server.
        OnTxNextPacketTimerEvent( NULL );
    }
    // Check Buffer
    // Check BufferSize
    // Check Rssi
    // Check Snr
    // Check RxSlot

    if( ComplianceTest.Running == true )
    {
        ComplianceTest.DownLinkCounter++;
    }

    if( mcpsIndication->RxData == true )
    {
        switch( mcpsIndication->Port )
        {
        case 1: // The application LED can be controlled on port 1 or 2
        case 2:
            if( mcpsIndication->BufferSize == 1 )
            {
                AppLedStateOn = mcpsIndication->Buffer[0] & 0x01;
            }
            break;
        case 224:
            if( ComplianceTest.Running == false )
            {
                // Check compliance test enable command (i)
                if( ( mcpsIndication->BufferSize == 4 ) &&
                    ( mcpsIndication->Buffer[0] == 0x01 ) &&
                    ( mcpsIndication->Buffer[1] == 0x01 ) &&
                    ( mcpsIndication->Buffer[2] == 0x01 ) &&
                    ( mcpsIndication->Buffer[3] == 0x01 ) )
                {
                    IsTxConfirmed = false;
                    AppPort = 224;
                    AppDataSizeBackup = AppDataSize;
                    AppDataSize = 2;
                    ComplianceTest.DownLinkCounter = 0;
                    ComplianceTest.LinkCheck = false;
                    ComplianceTest.DemodMargin = 0;
                    ComplianceTest.NbGateways = 0;
                    ComplianceTest.Running = true;
                    ComplianceTest.State = 1;

                    MibRequestConfirm_t mibReq;
                    mibReq.Type = MIB_ADR;
                    mibReq.Param.AdrEnable = true;
                    LoRaMacMibSetRequestConfirm( &mibReq );

		    #if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
			LoRaMacTestSetDutyCycleOn( false );
		    #endif
                }
            }
            else
            {
                ComplianceTest.State = mcpsIndication->Buffer[0];
                switch( ComplianceTest.State )
                {
                case 0: // Check compliance test disable command (ii)
                    IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
                    AppPort = LORAWAN_APP_PORT;
                    AppDataSize = AppDataSizeBackup;
                    ComplianceTest.DownLinkCounter = 0;
                    ComplianceTest.Running = false;

                    MibRequestConfirm_t mibReq;
                    mibReq.Type = MIB_ADR;
                    mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                    LoRaMacMibSetRequestConfirm( &mibReq );

		    #if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
			LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
		    #endif
                    break;
                case 1: // (iii, iv)
                    AppDataSize = 2;
                    break;
                case 2: // Enable confirmed messages (v)
                    IsTxConfirmed = true;
                    ComplianceTest.State = 1;
                    break;
                case 3:  // Disable confirmed messages (vi)
                    IsTxConfirmed = false;
                    ComplianceTest.State = 1;
                    break;
                case 4: // (vii)
                    AppDataSize = mcpsIndication->BufferSize;

                    AppDataBuffer[0] = 4;
                    for( uint8_t i = 1; i < MIN( AppDataSize, LORAWAN_APP_DATA_MAX_SIZE ); i++ )
                    {
                        AppDataBuffer[i] = mcpsIndication->Buffer[i] + 1;
                    }
                    break;
                case 5: // (viii)
                    {
                        MlmeReq_t mlmeReq;
                        mlmeReq.Type = MLME_LINK_CHECK;
                        LoRaMacStatus_t status = LoRaMacMlmeRequest( &mlmeReq );
                        NRF_LOG_INFO( "###### ===== MLME-Request - MLME_LINK_CHECK ==== ######" );
                        NRF_LOG_INFO( "STATUS      : %s", getMacStatusStringPointer(status) );
                    }
                    break;
                case 6: // (ix)
                    {
                        // Disable TestMode and revert back to normal operation
                        IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
                        AppPort = LORAWAN_APP_PORT;
                        AppDataSize = AppDataSizeBackup;
                        ComplianceTest.DownLinkCounter = 0;
                        ComplianceTest.Running = false;

                        MibRequestConfirm_t mibReq;
                        mibReq.Type = MIB_ADR;
                        mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                        LoRaMacMibSetRequestConfirm( &mibReq );

			#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
			    LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );
			#endif

                        JoinNetwork( );
                    }
                    break;
                case 7: // (x)
                    {
                        if( mcpsIndication->BufferSize == 3 )
                        {
                            MlmeReq_t mlmeReq;
                            mlmeReq.Type = MLME_TXCW;
                            mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                            LoRaMacStatus_t status = LoRaMacMlmeRequest( &mlmeReq );
                            NRF_LOG_INFO( "###### ===== MLME-Request - MLME_TXCW ==== ######" );
                            NRF_LOG_INFO( "STATUS      : %s", getMacStatusStringPointer(status) );
                        }
                        else if( mcpsIndication->BufferSize == 7 )
                        {
                            MlmeReq_t mlmeReq;
                            mlmeReq.Type = MLME_TXCW_1;
                            mlmeReq.Req.TxCw.Timeout = ( uint16_t )( ( mcpsIndication->Buffer[1] << 8 ) | mcpsIndication->Buffer[2] );
                            mlmeReq.Req.TxCw.Frequency = ( uint32_t )( ( mcpsIndication->Buffer[3] << 16 ) | ( mcpsIndication->Buffer[4] << 8 ) | mcpsIndication->Buffer[5] ) * 100;
                            mlmeReq.Req.TxCw.Power = mcpsIndication->Buffer[6];
                            LoRaMacStatus_t status = LoRaMacMlmeRequest( &mlmeReq );
                            NRF_LOG_INFO( "###### ===== MLME-Request - MLME_TXCW1 ==== ######" );
                            NRF_LOG_INFO( "STATUS      : %s", getMacStatusStringPointer(status) );
                        }
                        ComplianceTest.State = 1;
                    }
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }

    // Switch LED 1 ON for each received downlink
    GpioWrite( &Led1, 1 );
    TimerStart( &Led2Timer );

    const char *slotStrings[] = { "1", "2", "C", "C Multicast", "B Ping-Slot", "B Multicast Ping-Slot" };

    NRF_LOG_INFO( "###### ===== DOWNLINK FRAME %lu ==== ######", mcpsIndication->DownLinkCounter );

    NRF_LOG_INFO( "RX WINDOW   : %s", slotStrings[mcpsIndication->RxSlot] );
    
    NRF_LOG_INFO( "RX PORT     : %d", mcpsIndication->Port );

    if( mcpsIndication->BufferSize != 0 )
    {
        NRF_LOG_INFO( "RX DATA     : " );
        PrintHexBuffer( mcpsIndication->Buffer, mcpsIndication->BufferSize );
    }

    NRF_LOG_INFO( "" );
    NRF_LOG_INFO( "DATA RATE   : DR_%d", mcpsIndication->RxDatarate );
    NRF_LOG_INFO( "RX RSSI     : %d", mcpsIndication->Rssi );
    NRF_LOG_INFO( "RX SNR      : %d", mcpsIndication->Snr );

    NRF_LOG_INFO( "" );
}

/*!
 * \brief   MLME-Confirm event function
 *
 * \param   [IN] mlmeConfirm - Pointer to the confirm structure,
 *               containing confirm attributes.
 */
static void MlmeConfirm( MlmeConfirm_t *mlmeConfirm )
{
    NRF_LOG_INFO( "###### ===== MLME-Confirm ==== ######" );
    NRF_LOG_INFO( "STATUS      : %s", getEventInfoStatusStringPointer(mlmeConfirm->Status) );
    if( mlmeConfirm->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
    }
    switch( mlmeConfirm->MlmeRequest )
    {
        case MLME_JOIN:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                MibRequestConfirm_t mibGet;
                NRF_LOG_INFO( "###### ===== OTAA JOINED ==== ######" );

                mibGet.Type = MIB_DEV_ADDR;
                if(LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK)
				{
					NRF_LOG_INFO( "DevAddr     : %08lX", mibGet.Param.DevAddr );
				}

                mibGet.Type = MIB_CHANNELS_DATARATE;
                if(LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK)
				{
					NRF_LOG_INFO( "DATA RATE   : DR_%d", mibGet.Param.ChannelsDatarate );
				}

				// These keys can' be read as they are read only
                mibGet.Type = MIB_NWK_S_ENC_KEY;
                if(LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK)
				{
					printStringWithHexArray("MIB_NWK_S_ENC_KEY:", 16, mibGet.Param.NwkSEncKey);
				}

				mibGet.Type = MIB_S_NWK_S_INT_KEY;
                if(LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK)
				{
					printStringWithHexArray("MIB_S_NWK_S_INT_KEY:", 16, mibGet.Param.SNwkSIntKey);
				}

                mibGet.Type = MIB_APP_S_KEY;
                if(LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK)
				{
					printStringWithHexArray("MIB_APP_S_KEY:", 16, mibGet.Param.AppSKey);
				}

                mibGet.Type = MIB_NWK_S_ENC_KEY;
                if(LoRaMacMibGetRequestConfirm( &mibGet ) == LORAMAC_STATUS_OK)
				{
					printStringWithHexArray("MIB_APP_S_KEY:", 16, mibGet.Param.NwkSEncKey);
				}

                // Status is OK, node has joined the network
                DeviceState = DEVICE_STATE_SEND;

				// Transmit a startup message
                setTrigger(SFI_SENSOR_ON);
            }
            else
            {
                // Join was not successful. Try to join again
                JoinNetwork( );
            }
            break;
        }
        case MLME_LINK_CHECK:
        {
            if( mlmeConfirm->Status == LORAMAC_EVENT_INFO_STATUS_OK )
            {
                // Check DemodMargin
                // Check NbGateways
                if( ComplianceTest.Running == true )
                {
                    ComplianceTest.LinkCheck = true;
                    ComplianceTest.DemodMargin = mlmeConfirm->DemodMargin;
                    ComplianceTest.NbGateways = mlmeConfirm->NbGateways;
                }
            }
            break;
        }
        default:
            break;
    }
}

/*!
 * \brief   MLME-Indication event function
 *
 * \param   [IN] mlmeIndication - Pointer to the indication structure.
 */
static void MlmeIndication( MlmeIndication_t *mlmeIndication )
{
    if( mlmeIndication->Status != LORAMAC_EVENT_INFO_STATUS_BEACON_LOCKED )
    {
        NRF_LOG_INFO( "###### ===== MLME-Indication ==== ######" );
        NRF_LOG_INFO( "STATUS      : %s", getEventInfoStatusStringPointer(mlmeIndication->Status) );
    }
    if( mlmeIndication->Status != LORAMAC_EVENT_INFO_STATUS_OK )
    {
    }
    switch( mlmeIndication->MlmeIndication )
    {
        case MLME_SCHEDULE_UPLINK:
        {// The MAC signals that we shall provide an uplink as soon as possible
            OnTxNextPacketTimerEvent( NULL );
            break;
        }
        default:
            break;
    }
}

void OnMacProcessNotify( void )
{
    IsMacProcessPending = 1;
}

void sq_event_handler(SQ_EVENTS event)
{
	switch(event)
	{
		case SQ_MIN_200_MOVING:
			NRF_LOG_INFO("SQ_MIN_200_MOVING");
			break;
		case SQ_MIN_200_STANDSTILL:
			NRF_LOG_INFO("SQ_MIN_200_STANDSTILL");
            setTrigger(SFI_STANDSTILL);
			break;
		case SQ_SEN_645B_HIGH:
			NRF_LOG_INFO("SQ_SEN_645B_HIGH");
            setTrigger(SFI_HORIZONTAL);
			break;
		case SQ_SEN_645B_LOW:
			NRF_LOG_INFO("SQ_SEN_645B_LOW");
            setTrigger(SFI_VERTICAL);
			break;
		default:
			break;
	}
}


/**
 * Main application entry point.
 */
int main_lorawan_implementation( void )
{
    uint32_t				ret;
    LoRaMacStatus_t			retValLoRaMAC;
    LoRaMacPrimitives_t	    macPrimitives;
    LoRaMacCallback_t	    macCallbacks;
    MibRequestConfirm_t	    mibReq;
    LoRaMacStatus_t			status;
    NvmCtxMgmtStatus_t		retNVM;

    GpioMcuInit();
    BoardInitPeriph( );

    macPrimitives.MacMcpsConfirm		= McpsConfirm;
    macPrimitives.MacMcpsIndication		= McpsIndication;
    macPrimitives.MacMlmeConfirm		= MlmeConfirm;
    macPrimitives.MacMlmeIndication		= MlmeIndication;
    macCallbacks.GetBatteryLevel		= BoardGetBatteryLevel;
    macCallbacks.GetTemperatureLevel	= NULL;
    macCallbacks.NvmContextChange		= NvmCtxMgmtEvent;
    macCallbacks.MacProcessNotify		= OnMacProcessNotify;

    retValLoRaMAC = LoRaMacInitialization( &macPrimitives, &macCallbacks, ACTIVE_REGION );
    if(retValLoRaMAC != LORAMAC_STATUS_OK)
    {
		NRF_LOG_INFO("LoRaMacInitialization failed: %u", retValLoRaMAC);
    }

    DeviceState = DEVICE_STATE_RESTORE;

    NRF_LOG_INFO( "###### ===== ClassA demo application v1.0.RC1 ==== ######" );

    SQ_init(sq_event_handler);

    // Force the device to write the new bootcount to flash.
    nvm_fds_changed();

    while( 1 )
    {
		NRF_LOG_PROCESS();

        nvm_fds_check_pending();

        // Tick the RTC to execute callback in context of the main loop (in stead of the IRQ)
        TimerProcess( );

        // Process Radio IRQ
        if( Radio.IrqProcess != NULL )
        {
            Radio.IrqProcess( );
        }

        // Processes the LoRaMac events
        LoRaMacProcess( );

        switch( DeviceState )
        {
            case DEVICE_STATE_RESTORE:
            {
                // Try to restore from NVM and query the mac if possible.
                retNVM = NvmCtxMgmtRestore( );
                if( retNVM == NVMCTXMGMT_STATUS_SUCCESS )
                {
                    NRF_LOG_INFO( "###### ===== NVM CTXS RESTORED ==== ######" );
                }
                else
                {
					#if( OVER_THE_AIR_ACTIVATION == 0 )
						// Tell the MAC layer which network server version are we connecting too.
						mibReq.Type = MIB_ABP_LORAWAN_VERSION;
						mibReq.Param.AbpLrWanVersion.Value = ABP_ACTIVATION_LRWAN_VERSION;
						LoRaMacMibSetRequestConfirm( &mibReq );
					#endif

					#if( ABP_ACTIVATION_LRWAN_VERSION == ABP_ACTIVATION_LRWAN_VERSION_V10x )
						mibReq.Type = MIB_GEN_APP_KEY;
						mibReq.Param.GenAppKey = GenAppKey;
						LoRaMacMibSetRequestConfirm( &mibReq );
					#else
						mibReq.Type = MIB_APP_KEY;
						mibReq.Param.AppKey = AppKey;
						LoRaMacMibSetRequestConfirm( &mibReq );
					#endif

                    mibReq.Type = MIB_NWK_KEY;
                    mibReq.Param.NwkKey = NwkKey;
                    LoRaMacMibSetRequestConfirm( &mibReq );

                    // Initialize LoRaMac device unique ID if not already defined in Commissioning.h
					#ifndef LORAWAN_DEVICE_EUI
						if( ( DevEui[0] == 0 ) && ( DevEui[1] == 0 ) &&
							( DevEui[2] == 0 ) && ( DevEui[3] == 0 ) &&
							( DevEui[4] == 0 ) && ( DevEui[5] == 0 ) &&
							( DevEui[6] == 0 ) && ( DevEui[7] == 0 ) )
						{
							BoardGetUniqueId( DevEui );
						}
					#endif

					#if( OVER_THE_AIR_ACTIVATION == 0 )
						// Choose a random device address if not already defined in Commissioning.h
						if( DevAddr == 0 )
						{
							// Random seed initialization
							srand1( BoardGetRandomSeed( ) );

							// Choose a random device address
							DevAddr = randr( 0, 0x01FFFFFF );
						}

						mibReq.Type = MIB_NET_ID;
						mibReq.Param.NetID = LORAWAN_NETWORK_ID;
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_DEV_ADDR;
						mibReq.Param.DevAddr = DevAddr;
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_F_NWK_S_INT_KEY;
						mibReq.Param.FNwkSIntKey = FNwkSIntKey;
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_S_NWK_S_INT_KEY;
						mibReq.Param.SNwkSIntKey = SNwkSIntKey;
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_NWK_S_ENC_KEY;
						mibReq.Param.NwkSEncKey = NwkSEncKey;
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_APP_S_KEY;
						mibReq.Param.AppSKey = AppSKey;
						LoRaMacMibSetRequestConfirm( &mibReq );
					#endif
                }
                DeviceState = DEVICE_STATE_START;
                break;
            }

            case DEVICE_STATE_START:
            {
                TimerInit( &TxNextPacketTimer, OnTxNextPacketTimerEvent );

                TimerInit( &Led1Timer, OnLed1TimerEvent );
                TimerSetValue( &Led1Timer, 25 );

                TimerInit( &Led2Timer, OnLed2TimerEvent );
                TimerSetValue( &Led2Timer, 25 );

                mibReq.Type = MIB_PUBLIC_NETWORK;
                mibReq.Param.EnablePublicNetwork = LORAWAN_PUBLIC_NETWORK;
                LoRaMacMibSetRequestConfirm( &mibReq );

                mibReq.Type = MIB_ADR;
                mibReq.Param.AdrEnable = LORAWAN_ADR_ON;
                LoRaMacMibSetRequestConfirm( &mibReq );

				#if defined( REGION_EU868 ) || defined( REGION_RU864 ) || defined( REGION_CN779 ) || defined( REGION_EU433 )
					LoRaMacTestSetDutyCycleOn( LORAWAN_DUTYCYCLE_ON );

					#if( USE_TTN_NETWORK == 1 )
						mibReq.Type = MIB_RX2_DEFAULT_CHANNEL;
						mibReq.Param.Rx2DefaultChannel = ( RxChannelParams_t ){ 869525000, DR_3 };
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_RX2_CHANNEL;
						mibReq.Param.Rx2Channel = ( RxChannelParams_t ){ 869525000, DR_3 };
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_RXC_DEFAULT_CHANNEL;
						mibReq.Param.RxCDefaultChannel = ( RxChannelParams_t ){ 869525000, DR_3 };
						LoRaMacMibSetRequestConfirm( &mibReq );

						mibReq.Type = MIB_RXC_CHANNEL;
						mibReq.Param.RxCChannel = ( RxChannelParams_t ){ 869525000, DR_3 };
						LoRaMacMibSetRequestConfirm( &mibReq );
					#endif
				#endif

                mibReq.Type = MIB_SYSTEM_MAX_RX_ERROR;
                mibReq.Param.SystemMaxRxError = 20;
                LoRaMacMibSetRequestConfirm( &mibReq );

                LoRaMacStart( );

                mibReq.Type = MIB_NETWORK_ACTIVATION;
                status = LoRaMacMibGetRequestConfirm( &mibReq );

                if( status == LORAMAC_STATUS_OK )
                {
                    if( mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE )
                    {
                        DeviceState = DEVICE_STATE_JOIN;
                    }
                    else
                    {
                        DeviceState = DEVICE_STATE_SEND;
                        NextTx = true;
                        setTrigger(SFI_SENSOR_ON);
                    }
                }
                break;
            }
            case DEVICE_STATE_JOIN:
            {
				NRF_LOG_INFO("DEVICE_STATE_JOIN");
				printStringWithHexArray("DevEui      : ", 8, DevEui);
				printStringWithHexArray("AppEui      : ", 8, JoinEui);
				printStringWithHexArray("AppKey      : ", 16, NwkKey);

				#if( OVER_THE_AIR_ACTIVATION == 0 )
					NRF_LOG_INFO( "###### ===== ABP JOINED ==== ######\r\n" );
					NRF_LOG_INFO( "\r\nABP\r\n\r\n" );
					NRF_LOG_INFO( "DevAddr     : %08lX\r\n", DevAddr );
					printStringWithHexArray("NwkSKey      : ", 16, FNwkSIntKey);
					printStringWithHexArray("AppSKey      : ", 16, AppSKey);

					mibReq.Type = MIB_NETWORK_ACTIVATION;
					mibReq.Param.NetworkActivation = ACTIVATION_TYPE_ABP;
					LoRaMacMibSetRequestConfirm( &mibReq );

					DeviceState = DEVICE_STATE_SEND;
				#else
					JoinNetwork( );
				#endif
                break;
            }
            case DEVICE_STATE_SEND:
            {
                DeviceState = DEVICE_STATE_CYCLE;
                break;
            }
            case DEVICE_STATE_CYCLE:
            {

                DeviceState = DEVICE_STATE_SLEEP;
                if( ComplianceTest.Running == true )
                {
                    // Schedule next packet transmission
                    TxDutyCycleTime = 5000; // 5000 ms
                }
                else
                {
					mibReq.Type = MIB_NETWORK_ACTIVATION;
					status = LoRaMacMibGetRequestConfirm( &mibReq );

					if( status == LORAMAC_STATUS_OK )
					{
						if( mibReq.Param.NetworkActivation == ACTIVATION_TYPE_NONE )
						{
							// Network not joined yet. Try to join again
                            TxDutyCycleTime = APP_JOIN_DUTYCYCLE;
						}
                        else
						{
							// If joined schedule the next packet transmission
							TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
						}
					}
					else
					{
						// Error case
						TxDutyCycleTime = APP_TX_DUTYCYCLE;
					}
                }

                // Schedule next packet transmission
                TimerStop( &TxNextPacketTimer );
                TimerSetValue( &TxNextPacketTimer, TxDutyCycleTime );
                TimerStart( &TxNextPacketTimer );
                break;
            }
            case DEVICE_STATE_SLEEP:
            {
                if( NvmCtxMgmtStore( ) == NVMCTXMGMT_STATUS_SUCCESS )
                {
                    NRF_LOG_INFO( "###### ===== CTXS STORED ==== ######" );
                }

                if( IsMacProcessPending == 1 )
                {
                    // Clear flag and prevent MCU to go into low power modes.
                    IsMacProcessPending = 0;
                }
                else
                {
					#if 1
					if(sfiMsgtTrigger != SFI_UNKNOWN)
					{
						if(!SFI_SendFrame(sfiMsgtTrigger))
						{
							// Clear the trigger when the message is send succesfully.
							sfiMsgtTrigger = SFI_UNKNOWN; 

                            TxDutyCycleTime = APP_TX_DUTYCYCLE + randr( -APP_TX_DUTYCYCLE_RND, APP_TX_DUTYCYCLE_RND );
                            TimerStop( &TxNextPacketTimer );
                            TimerSetValue( &TxNextPacketTimer, APP_TX_DUTYCYCLE );
							TimerStart( &TxNextPacketTimer );
						}
					}
					#endif


                    // The MCU wakes up through events
                    BoardLowPowerHandler( );
                }
                break;
            }
            default:
            {
                DeviceState = DEVICE_STATE_START;
                break;
            }
        }
    }
}
