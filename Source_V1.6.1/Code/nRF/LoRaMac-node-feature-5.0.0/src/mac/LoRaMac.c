/*!
 * \file      LoRaMac.c
 *
 * \brief     LoRa MAC layer implementation
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
 *               ___ _____ _   ___ _  _____ ___  ___  ___ ___
 *              / __|_   _/_\ / __| |/ / __/ _ \| _ \/ __| __|
 *              \__ \ | |/ _ \ (__| ' <| _| (_) |   / (__| _|
 *              |___/ |_/_/ \_\___|_|\_\_| \___/|_|_\\___|___|
 *              embedded.connectivity.solutions===============
 *
 * \endcode
 *
 * \author    Miguel Luis ( Semtech )
 *
 * \author    Gregory Cristian ( Semtech )
 *
 * \author    Daniel Jaeckle ( STACKFORCE )
 *
 * \author    Johannes Bruder ( STACKFORCE )
 */
#include "utilities.h"
#include "region/Region.h"
#include "LoRaMacClassB.h"
#include "LoRaMacCrypto.h"
#include "secure-element.h"
#include "LoRaMacTest.h"
#include "LoRaMacTypes.h"
#include "LoRaMacConfirmQueue.h"
#include "LoRaMacHeaderTypes.h"
#include "LoRaMacMessageTypes.h"
#include "LoRaMacParser.h"
#include "LoRaMacFCntHandler.h"
#include "LoRaMacCommands.h"
#include "LoRaMacAdr.h"
#include "LoRaMacTypes.h"
#include "rtc-board.h"

#define NRF_LOG_MODULE_NAME LORAMAC
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "Region.h"
#include "rng_nrf.h"

/*!
 * Maximum PHY layer payload size
 */
#define LORAMAC_PHY_MAXPAYLOAD                      255

/*!
 * Maximum MAC commands buffer size
 */
#define LORA_MAC_COMMAND_MAX_LENGTH                 128

/*!
 * Maximum length of the fOpts field
 */
#define LORA_MAC_COMMAND_MAX_FOPTS_LENGTH           15

/*!
 * LoRaMac duty cycle for the back-off procedure during the first hour.
 */
#define BACKOFF_DC_1_HOUR                           100

/*!
 * LoRaMac duty cycle for the back-off procedure during the next 10 hours.
 */
#define BACKOFF_DC_10_HOURS                         1000

/*!
 * LoRaMac duty cycle for the back-off procedure during the next 24 hours.
 */
#define BACKOFF_DC_24_HOURS                         10000

/*!
 * LoRaMac internal states
 */
enum eLoRaMacState
{
    LORAMAC_IDLE          = 0x00000000,
    LORAMAC_STOPPED       = 0x00000001,
    LORAMAC_TX_RUNNING    = 0x00000002,
    LORAMAC_RX            = 0x00000004,
    LORAMAC_ACK_RETRY     = 0x00000010,
    LORAMAC_TX_DELAYED    = 0x00000020,
    LORAMAC_TX_CONFIG     = 0x00000040,
    LORAMAC_RX_ABORT      = 0x00000080,
};

/*
 * Request permission state
 */
typedef enum eLoRaMacRequestHandling
{
    LORAMAC_REQUEST_HANDLING_OFF = 0,
    LORAMAC_REQUEST_HANDLING_ON = !LORAMAC_REQUEST_HANDLING_OFF
}LoRaMacRequestHandling_t;

typedef struct sLoRaMacNvmCtx
{
    /*
     * LoRaMac region.
     */
    LoRaMacRegion_t Region;
    /*
     * LoRaMac default parameters
     */
    LoRaMacParams_t MacParamsDefaults;
    /*
     * Network ID ( 3 bytes )
     */
    uint32_t NetID;
    /*
     * Mote Address
     */
    uint32_t DevAddr;
    /*!
    * Multicast channel list
    */
    MulticastCtx_t MulticastChannelList[LORAMAC_MAX_MC_CTX];
    /*
     * Actual device class
     */
    DeviceClass_t DeviceClass;
    /*
     * Indicates if the node is connected to
     * a private or public network
     */
    bool PublicNetwork;
    /*
     * LoRaMac ADR control status
     */
    bool AdrCtrlOn;
    /*
     * Counts the number of missed ADR acknowledgements
     */
    uint32_t AdrAckCounter;

    /*
     * LoRaMac parameters
     */
    LoRaMacParams_t MacParams;
    /*
     * Maximum duty cycle
     * \remark Possibility to shutdown the device.
     */
    uint8_t MaxDCycle;
    /*
    * Enables/Disables duty cycle management (Test only)
    */
    bool DutyCycleOn;
    /*
     * Current channel index
     */
    uint8_t LastTxChannel;
    /*
     * Holds the current rx window slot
     */
    bool RepeaterSupport;
    /*
     * Buffer containing the MAC layer commands
     */
    uint8_t MacCommandsBuffer[LORA_MAC_COMMAND_MAX_LENGTH];
    /*
     * If the server has sent a FRAME_TYPE_DATA_CONFIRMED_DOWN this variable indicates
     * if the ACK bit must be set for the next transmission
     */
    bool SrvAckRequested;
    /*
     * Aggregated duty cycle management
     */
    uint16_t AggregatedDCycle;
    /*
    * Aggregated duty cycle management
    */
    TimerTime_t LastTxDoneTime;
    TimerTime_t AggregatedTimeOff;
    /*
    * Stores the time at LoRaMac initialization.
    *
    * \remark Used for the BACKOFF_DC computation.
    */
    SysTime_t InitializationTime;
    /*
     * Current LoRaWAN Version
     */
    Version_t Version;
    /*
     * End-Device network activation
     */
    ActivationType_t NetworkActivation;
    /*
     * Number of uplinks without Rejoin request type 0.
     * ( If requested by the server through RejoinParamSetupReq MAC command )
     * When it's set to 0, Rejoin0UplinksCounter won't be incremented
     */
    uint16_t Rejoin0UplinksLimit;
    /*
     * Counts the number if uplinks to know when the next Rejoin request type 0 is required.
     * ( If requested by the server through RejoinParamSetupReq MAC command )
     */
    uint16_t Rejoin0UplinksCounter;
    /*
     * The total number of times the device will retry the Rejoin Request.
     * ( If requested by the server through ForceRejoinReq MAC command )
     */
    uint8_t ForceRejoinMaxRetries;
    /*
     * Counter of Rejoin Request of retries.
     * ( If requested by the server through ForceRejoinReq MAC command )
     */
    uint8_t ForceRejoinRetriesCounter;
    /*
     * Rejoin Request Type
     * ( If requested by the server through ForceRejoinReq MAC command )
     */
    uint8_t ForceRejoinType;
    /*!
     * Time in seconds between cyclic transmission of Type 0 Rejoin requests.
     */
    uint32_t Rejoin0CycleInSec;
    /*!
     * Time in seconds between cyclic transmission of Type 1 Rejoin requests.
     */
    uint32_t Rejoin1CycleInSec;
    /*!
     * Time in seconds between cyclic transmission of Type 2 Rejoin requests.
     */
    uint32_t Rejoin2CycleInSec;
    /*
     * Indicates if a Rejoin request was sent and no join-accept or any downlink has been received yet.
     */
    bool IsRejoinRequestPending;
    /*
     * Counts the number of uplinks containing a RekeyInd MAC command to know
     * when the end device should reverted to join state because it didn't received a RekeyConf.
     */
    uint16_t RekeyIndUplinksCounter;
    /*!
     * Last received Message integrity Code (MIC)
     */
    uint32_t LastRxMic;
}LoRaMacNvmCtx_t;

typedef struct sLoRaMacCtx
{
    /*
     * Device IEEE EUI
     */
    uint8_t* DevEui;
    /*
    * Join IEEE EUI
    */
    uint8_t* JoinEui;
    /*
    * Length of packet in PktBuffer
    */
    uint16_t PktBufferLen;
    /*
    * Buffer containing the data to be sent or received.
    */
    uint8_t PktBuffer[LORAMAC_PHY_MAXPAYLOAD];
    /*!
    * Current processed transmit message
    */
    LoRaMacMessage_t TxMsg;
    /*!
    * Buffer containing the data received by the application.
    */
    uint8_t AppData[LORAMAC_PHY_MAXPAYLOAD];
    /*
    * Size of buffer containing the application data.
    */
    uint8_t AppDataSize;
    /*
    * Buffer containing the upper layer data.
    */
    uint8_t RxPayload[LORAMAC_PHY_MAXPAYLOAD];
    SysTime_t LastTxSysTime;
    /*
    * LoRaMac internal state
    */
    uint32_t MacState;
    /*
    * LoRaMac upper layer event functions
    */
    LoRaMacPrimitives_t* MacPrimitives;
    /*
    * LoRaMac upper layer callback functions
    */
    LoRaMacCallback_t* MacCallbacks;
    /*
    * Radio events function pointer
    */
    RadioEvents_t RadioEvents;
    /*
    * LoRaMac duty cycle delayed Tx timer
    */
    TimerEvent_t TxDelayedTimer;
    /*
    * LoRaMac reception windows timers
    */
    TimerEvent_t RxWindowTimer1;
    TimerEvent_t RxWindowTimer2;
    /*
    * LoRaMac reception windows delay
    * \remark normal frame: RxWindowXDelay = ReceiveDelayX - RADIO_WAKEUP_TIME
    *         join frame  : RxWindowXDelay = JoinAcceptDelayX - RADIO_WAKEUP_TIME
    */
    uint32_t RxWindow1Delay;
    uint32_t RxWindow2Delay;
    /*
    * LoRaMac Rx windows configuration
    */
    RxConfigParams_t RxWindow1Config;
    RxConfigParams_t RxWindow2Config;
    RxConfigParams_t RxWindowCConfig;
    /*
     * Limit of uplinks without any donwlink response before the ADRACKReq bit will be set.
     */
    uint16_t AdrAckLimit;
    /*
     * Limit of uplinks without any donwlink response after a the first frame with set ADRACKReq bit
     * before the trying to regain the connectivity.
     */
    uint16_t AdrAckDelay;
    /*
    * Acknowledge timeout timer. Used for packet retransmissions.
    */
    TimerEvent_t AckTimeoutTimer;
    /*
     * Uplink messages repetitions counter
     */
    uint8_t ChannelsNbTransCounter;
    /*
     * Number of trials to get a frame acknowledged
     */
    uint8_t AckTimeoutRetries;
    /*
     * Number of trials to get a frame acknowledged
     */
    uint8_t AckTimeoutRetriesCounter;
    /*
     * Indicates if the AckTimeout timer has expired or not
     */
    bool AckTimeoutRetry;
    /*
     * If the node has sent a FRAME_TYPE_DATA_CONFIRMED_UP this variable indicates
     * if the nodes needs to manage the server acknowledgement.
     */
    bool NodeAckRequested;
    /*
     * Current channel index
     */
    uint8_t Channel;
    /*
    * Last transmission time on air
    */
    TimerTime_t TxTimeOnAir;
    /*
    * Structure to hold an MCPS indication data.
    */
    McpsIndication_t McpsIndication;
    /*
    * Structure to hold MCPS confirm data.
    */
    McpsConfirm_t McpsConfirm;
    /*
    * Structure to hold MLME confirm data.
    */
    MlmeConfirm_t MlmeConfirm;
    /*
    * Structure to hold MLME indication data.
    */
    MlmeIndication_t MlmeIndication;
    /*
    * Holds the current rx window slot
    */
    LoRaMacRxSlot_t RxSlot;
    /*
    * LoRaMac tx/rx operation state
    */
    LoRaMacFlags_t MacFlags;
    /*
    * Data structure indicating if a request is allowed or not.
    */
    LoRaMacRequestHandling_t AllowRequests;
    /*
    * Cycle timer for Type 0 Rejoin requests
    */
    TimerEvent_t Rejoin0CycleTimer;
    /*
    * Cycle timer for Type 1 Rejoin requests
    */
    TimerEvent_t Rejoin1CycleTimer;
    /*
    * Cycle timer for Type 2 Rejoin requests
    */
    TimerEvent_t Rejoin2CycleTimer;
    /*
    * Cycle timer for Rejoin requests trigged by ForceRejoinReq MAC command
    */
    TimerEvent_t ForceRejoinReqCycleTimer;
    /*
    * Time of Type 0 Rejoin requests cycles
    */
    TimerTime_t Rejoin0CycleTime;
    /*
    * Time of Type 1 Rejoin requests cycles
    */
    TimerTime_t Rejoin1CycleTime;
    /*
    * Time of Type 2 Rejoin requests cycles
    */
    TimerTime_t Rejoin2CycleTime;
    /*
    * Non-volatile module context structure
    */
    LoRaMacNvmCtx_t* pNvmCtx;
}LoRaMacCtx_t;

/*
 * Module context.
 */
static LoRaMacCtx_t MacCtx;

/*
 * Non-volatile module context.
 */
static LoRaMacNvmCtx_t NvmMacCtx;



/*
 * List of module contexts.
 */
LoRaMacCtxs_t Contexts;

/*!
 * Defines the LoRaMac radio events status
 */
typedef union uLoRaMacRadioEvents
{
    uint32_t Value;
    struct sEvents
    {
        uint32_t RxTimeout : 1;
        uint32_t RxError   : 1;
        uint32_t TxTimeout : 1;
        uint32_t RxDone    : 1;
        uint32_t TxDone    : 1;
    }Events;
}LoRaMacRadioEvents_t;

/*!
 * LoRaMac radio events status
 */
LoRaMacRadioEvents_t LoRaMacRadioEvents = { .Value = 0 };

/*!
 * \brief Function to be executed on Radio Tx Done event
 */
static void OnRadioTxDone( void );

/*!
 * \brief This function prepares the MAC to abort the execution of function
 *        OnRadioRxDone in case of a reception error.
 */
static void PrepareRxDoneAbort( void );

/*!
 * \brief Function to be executed on Radio Rx Done event
 */
static void OnRadioRxDone( uint8_t* payload, uint16_t size, int16_t rssi, int8_t snr );

/*!
 * \brief Function executed on Radio Tx Timeout event
 */
static void OnRadioTxTimeout( void );

/*!
 * \brief Function executed on Radio Rx error event
 */
static void OnRadioRxError( void );

/*!
 * \brief Function executed on Radio Rx Timeout event
 */
static void OnRadioRxTimeout( void );

/*!
 * \brief Function executed on duty cycle delayed Tx  timer event
 */
static void OnTxDelayedTimerEvent( void* context );

/*!
 * \brief Function executed on first Rx window timer event
 */
static void OnRxWindow1TimerEvent( void* context );

/*!
 * \brief Function executed on second Rx window timer event
 */
static void OnRxWindow2TimerEvent( void* context );

/*!
 * \brief Function executed on Rejoin Type 0 cycle timer event
 */
static void OnRejoin0CycleTimerEvent( void* context );

/*!
 * \brief Function executed on Rejoin Type 0 cycle timer event
 */
static void OnRejoin1CycleTimerEvent( void* context );

/*!
 * \brief Function executed on Rejoin Type 0 cycle timer event
 */
static void OnRejoin2CycleTimerEvent( void* context );

/*!
 * \brief Function executed on Rejoin Type 0 or 2 cycle timer event
 *        which was requested by a ForceRejoinReq MAC command.
 */
static void OnForceRejoinReqCycleTimerEvent( void* context );

/*!
 * \brief Function executed on AckTimeout timer event
 */
static void OnAckTimeoutTimerEvent( void* context );

/*!
 * \brief Configures the events to trigger an MLME-Indication with
 *        a MLME type of MLME_SCHEDULE_UPLINK.
 */
static void SetMlmeScheduleUplinkIndication( void );

/*!
 * \brief Switches the device class
 *
 * \param [IN] deviceClass Device class to switch to
 */
static LoRaMacStatus_t SwitchClass( DeviceClass_t deviceClass );

/*!
 * \brief Gets the maximum application payload length in the absence of the optional FOpt field.
 *
 * \param [IN] datarate        Current datarate
 *
 * \retval                    Max length
 */
static uint8_t GetMaxAppPayloadWithoutFOptsLength( int8_t datarate );

/*!
 * \brief Validates if the payload fits into the frame, taking the datarate
 *        into account.
 *
 * \details Refer to chapter 4.3.2 of the LoRaWAN specification, v1.0
 *
 * \param lenN Length of the application payload. The length depends on the
 *             datarate and is region specific
 *
 * \param datarate Current datarate
 *
 * \param fOptsLen Length of the fOpts field
 *
 * \retval [false: payload does not fit into the frame, true: payload fits into
 *          the frame]
 */
static bool ValidatePayloadLength( uint8_t lenN, int8_t datarate, uint8_t fOptsLen );

/*!
 * \brief Decodes MAC commands in the fOpts field and in the payload
 *
 * \param [IN] payload      A pointer to the payload
 * \param [IN] macIndex     The index of the payload where the MAC commands start
 * \param [IN] commandsSize The size of the MAC commands
 * \param [IN] snr          The SNR value  of the frame
 * \param [IN] rxSlot       The RX slot where the frame was received
 */
static void ProcessMacCommands( uint8_t* payload, uint8_t macIndex, uint8_t commandsSize, int8_t snr, LoRaMacRxSlot_t rxSlot );

/*!
 * \brief LoRaMAC layer generic send frame
 *
 * \param [IN] macHdr      MAC header field
 * \param [IN] fPort       MAC payload port
 * \param [IN] fBuffer     MAC data buffer to be sent
 * \param [IN] fBufferSize MAC data buffer size
 * \retval status          Status of the operation.
 */
LoRaMacStatus_t Send( LoRaMacHeader_t* macHdr, uint8_t fPort, void* fBuffer, uint16_t fBufferSize );

/*!
 * \brief LoRaMAC layer send join/rejoin request
 *
 * \param [IN] joinReqType Type of join-request or rejoin
 *
 * \retval status          Status of the operation.
 */
LoRaMacStatus_t SendReJoinReq( JoinReqIdentifier_t joinReqType );

/*!
 * \brief LoRaMAC layer frame buffer initialization
 *
 * \param [IN] macHdr      MAC header field
 * \param [IN] fCtrl       MAC frame control field
 * \param [IN] fOpts       MAC commands buffer
 * \param [IN] fPort       MAC payload port
 * \param [IN] fBuffer     MAC data buffer to be sent
 * \param [IN] fBufferSize MAC data buffer size
 * \retval status          Status of the operation.
 */
LoRaMacStatus_t PrepareFrame( LoRaMacHeader_t* macHdr, LoRaMacFrameCtrl_t* fCtrl, uint8_t fPort, void* fBuffer, uint16_t fBufferSize );

/*
 * \brief Schedules the frame according to the duty cycle
 *
 * \param [IN] allowDelayedTx When set to true, the a frame will be delayed,
 *                            the duty cycle restriction is active
 * \retval Status of the operation
 */
static LoRaMacStatus_t ScheduleTx( bool allowDelayedTx );

/*
 * \brief Secures the current processed frame ( TxMsg )
 * \param[IN]     txDr      Data rate used for the transmission
 * \param[IN]     txCh      Index of the channel used for the transmission
 * \retval status           Status of the operation
 */
static LoRaMacStatus_t SecureFrame( uint8_t txDr, uint8_t txCh );

/*
 * \brief Calculates the back-off time for the band of a channel.
 *
 * \param [IN] channel     The last Tx channel index
 */
static void CalculateBackOff( uint8_t channel );

/*
 * \brief Function to remove pending MAC commands
 *
 * \param [IN] rxSlot     The RX slot on which the frame was received
 * \param [IN] fCtrl      The frame control field of the received frame
 * \param [IN] request    The request type
 */
static void RemoveMacCommands( LoRaMacRxSlot_t rxSlot, LoRaMacFrameCtrl_t fCtrl, Mcps_t request );

/*!
 * \brief LoRaMAC layer prepared frame buffer transmission with channel specification
 *
 * \remark PrepareFrame must be called at least once before calling this
 *         function.
 *
 * \param [IN] channel     Channel to transmit on
 * \retval status          Status of the operation.
 */
LoRaMacStatus_t SendFrameOnChannel( uint8_t channel );

/*!
 * \brief Sets the radio in continuous transmission mode
 *
 * \remark Uses the radio parameters set on the previous transmission.
 *
 * \param [IN] timeout     Time in seconds while the radio is kept in continuous wave mode
 * \retval status          Status of the operation.
 */
LoRaMacStatus_t SetTxContinuousWave( uint16_t timeout );

/*!
 * \brief Sets the radio in continuous transmission mode
 *
 * \remark Uses the radio parameters set on the previous transmission.
 *
 * \param [IN] timeout     Time in seconds while the radio is kept in continuous wave mode
 * \param [IN] frequency   RF frequency to be set.
 * \param [IN] power       RF output power to be set.
 * \retval status          Status of the operation.
 */
LoRaMacStatus_t SetTxContinuousWave1( uint16_t timeout, uint32_t frequency, uint8_t power );

/*!
 * \brief Converts a second based Rejoin Cycle base in the from timer module required format.
 *
 * \param [IN]  rejoinCycleTime     The time in second
 * \param [out] timeInMiliSec       The time in second
 * \retval status          Status of the operation.
 */
static bool ConvertRejoinCycleTime( uint32_t rejoinCycleTime, uint32_t* timeInMiliSec );

/*!
 * \brief Resets MAC specific parameters to default
 */
static void ResetMacParameters( void );

/*!
 * \brief Checks if it's required to send a Rejoin (Type 0) request.
 *
 * \retval [false: Rejoin not required, true: Rejoin required]
 */
static bool IsReJoin0Required( void );

/*!
 * \brief Initializes and opens the reception window
 *
 * \param [IN] rxTimer  Window timer to be topped.
 * \param [IN] rxConfig Window parameters to be setup
 */
static void RxWindowSetup( TimerEvent_t* rxTimer, RxConfigParams_t* rxConfig );

/*!
 * \brief Opens up a continuous RX C window. This is used for
 *        class c devices.
 */
static void OpenContinuousRxCWindow( void );

/*!
 * \brief   Returns a pointer to the internal contexts structure.
 *
 * \retval  void Points to a structure containing all contexts
 */
LoRaMacCtxs_t* GetCtxs( void );

/*!
 * \brief   Restoring of internal module contexts
 *
 * \details This function allows to restore module contexts by a given pointer.
 *
 *
 * \retval  LoRaMacStatus_t Status of the operation. Possible returns are:
 *          returns are:
 *          \ref LORAMAC_STATUS_OK,
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID,
 */
LoRaMacStatus_t RestoreCtxs( LoRaMacCtxs_t* contexts );

/*!
 * \brief   Determines the frame type
 *
 * \param [IN] macMsg Data message object
 *
 * \param [OUT] fType Frame type
 *
 * \retval  LoRaMacStatus_t Status of the operation. Possible returns are:
 *          returns are:
 *          \ref LORAMAC_STATUS_OK,
 *          \ref LORAMAC_STATUS_PARAMETER_INVALID,
 */
LoRaMacStatus_t DetermineFrameType( LoRaMacMessageData_t* macMsg, FType_t* fType );

/*!
 * \brief Checks if the retransmission should be stopped in case of a unconfirmed uplink
 *
 * \retval Returns true if it should be stopped.
 */
static bool CheckRetransUnconfirmedUplink( void );

/*!
 * \brief Checks if the retransmission should be stopped in case of a confirmed uplink
 *
 * \retval Returns true it should be stopped.
 */
static bool CheckRetransConfirmedUplink( void );

/*!
 * \brief Stops the uplink retransmission
 *
 * \retval Returns true if successful.
 */
static bool StopRetransmission( void );

/*!
 * \brief Calls the callback to indicate that a context changed
 */
static void CallNvmCtxCallback( LoRaMacNvmCtxModule_t module );

/*!
 * \brief MAC NVM Context has been changed
 */
static void EventMacNvmCtxChanged( void );

/*!
 * \brief Region NVM Context has been changed
 */
static void EventRegionNvmCtxChanged( void );

/*!
 * \brief Crypto NVM Context has been changed
 */
static void EventCryptoNvmCtxChanged( void );

/*!
 * \brief Secure Element NVM Context has been changed
 */
static void EventSecureElementNvmCtxChanged( void );

/*!
 * \brief MAC commands module nvm context has been changed
 */
static void EventCommandsNvmCtxChanged( void );

/*!
 * \brief Class B module nvm context has been changed
 */
static void EventClassBNvmCtxChanged( void );

/*!
 * \brief Confirm Queue module nvm context has been changed
 */
static void EventConfirmQueueNvmCtxChanged( void );

/*!
 * \brief FCnt Handler module nvm context has been changed
 */
static void EventFCntHandlerNvmCtxChanged( void );

/*!
 * \brief Verifies if a request is pending currently
 *
 *\retval 1: Request pending, 0: request not pending
 */
static uint8_t IsRequestPending( void );

/*!
 * \brief Enabled the possibility to perform requests
 *
 * \param [IN] requestState Request permission state
 */
static void LoRaMacEnableRequests( LoRaMacRequestHandling_t requestState );

/*!
 * \brief This function verifies if a RX abort occurred
 */
static void LoRaMacCheckForRxAbort( void );

/*!
 * \brief This function verifies if a TX timeout occurred
 *
 *\retval 1: TX timeout, 0: no TX timeout
 */
static uint8_t LoRaMacCheckForTxTimeout( void );

/*!
 * \brief This function verifies if a beacon acquisition MLME
 *        request was pending
 *
 * \retval 1: Request pending, 0: no request pending
 */
static uint8_t LoRaMacCheckForBeaconAcquisition( void );

/*!
 * \brief This function handles join request
 */
static void LoRaMacHandleJoinRequest( void );

/*!
 * \brief This function handles mcps request
 */
static void LoRaMacHandleMcpsRequest( void );

/*!
 * \brief This function handles callback events for requests
 */
static void LoRaMacHandleRequestEvents( void );

/*!
 * \brief This function handles callback events for indications
 */
static void LoRaMacHandleIndicationEvents( void );

/*!
 * Structure used to store the radio Tx event data
 */
struct
{
    TimerTime_t CurTime;
	uint32_t	timestamp;
}TxDoneParams;

/*!
 * Structure used to store the radio Rx event data
 */
struct
{
    TimerTime_t LastRxDone;
    uint8_t *Payload;
    uint16_t Size;
    int16_t Rssi;
    int8_t Snr;
}RxDoneParams;

static void OnRadioTxDone( void )
{
    TxDoneParams.CurTime	= TimerGetCurrentTime( );
    TxDoneParams.timestamp	= TimerGetTimestamp( );	// Remember a timestamp from the RTC for timing the RX1 and RX2 window delays more precisely.
    MacCtx.LastTxSysTime	= SysTimeGet( );

	#if 0
		NRF_LOG_INFO("Tx Done Event timestamp: %u", TxDoneParams.timestamp);
	#endif

    LoRaMacRadioEvents.Events.TxDone = 1; 

    if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->MacProcessNotify != NULL ) )
    {
        MacCtx.MacCallbacks->MacProcessNotify( );
    }
}

static void OnRadioRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    RxDoneParams.LastRxDone = TimerGetCurrentTime( );
    RxDoneParams.Payload = payload;
    RxDoneParams.Size = size;
    RxDoneParams.Rssi = rssi;
    RxDoneParams.Snr = snr;

    LoRaMacRadioEvents.Events.RxDone = 1;

    if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->MacProcessNotify != NULL ) )
    {
        MacCtx.MacCallbacks->MacProcessNotify( );
    }
}

static void OnRadioTxTimeout( void )
{
    LoRaMacRadioEvents.Events.TxTimeout = 1;

    if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->MacProcessNotify != NULL ) )
    {
        MacCtx.MacCallbacks->MacProcessNotify( );
    }
}

static void OnRadioRxError( void )
{
    LoRaMacRadioEvents.Events.RxError = 1;

    if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->MacProcessNotify != NULL ) )
    {
        MacCtx.MacCallbacks->MacProcessNotify( );
    }
}

static void OnRadioRxTimeout( void )
{
    LoRaMacRadioEvents.Events.RxTimeout = 1;
	#if 0
		NRF_LOG_INFO("Rx%u Radio Timeout", 1 + (uint32_t) MacCtx.RxSlot); // "Rx1 Radio Timeout" "Rx2 Radio Timeout"
	#endif

    if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->MacProcessNotify != NULL ) )
    {
        MacCtx.MacCallbacks->MacProcessNotify( );
    }
}

static void UpdateRxSlotIdleState( void )
{
    if( MacCtx.pNvmCtx->DeviceClass != CLASS_C )
    {
        MacCtx.RxSlot = RX_SLOT_NONE;
    }
    else
    {
        MacCtx.RxSlot = RX_SLOT_WIN_CLASS_C;
    }
}

static void ProcessRadioTxDone( void )
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    SetBandTxDoneParams_t txDone;

    if( MacCtx.pNvmCtx->DeviceClass != CLASS_C )
    {
        Radio.Sleep( );
    }
    // Setup timers
    TimerSetValue( &MacCtx.RxWindowTimer1, MacCtx.RxWindow1Delay );
    TimerStart( &MacCtx.RxWindowTimer1);
    TimerSetValue( &MacCtx.RxWindowTimer2, MacCtx.RxWindow2Delay );
    TimerStart( &MacCtx.RxWindowTimer2);

//    TimerSetValue( &MacCtx.RxWindowTimer1, MacCtx.RxWindow1Delay );
//    TimerStartFromTimestamp( &MacCtx.RxWindowTimer1, TxDoneParams.timestamp);
//    TimerSetValue( &MacCtx.RxWindowTimer2, MacCtx.RxWindow2Delay );
//    TimerStartFromTimestamp( &MacCtx.RxWindowTimer2, TxDoneParams.timestamp );

	#if LORAMAC_LOG_ENABLED
		NRF_LOG_INFO("RX1: %u Ticks/%u ms, RX2: %u Ticks/%u ms", MacCtx.RxWindowTimer1.delay_ticks, MacCtx.RxWindow1Delay, MacCtx.RxWindowTimer2.delay_ticks, MacCtx.RxWindow2Delay);
	#endif

    if( ( MacCtx.pNvmCtx->DeviceClass == CLASS_C ) || ( MacCtx.NodeAckRequested == true ) )
    {
        getPhy.Attribute = PHY_ACK_TIMEOUT;
        phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
        TimerSetValue( &MacCtx.AckTimeoutTimer, MacCtx.RxWindow2Delay + phyParam.Value );
        TimerStart( &MacCtx.AckTimeoutTimer );
    }

    // Store last Tx channel
    MacCtx.pNvmCtx->LastTxChannel = MacCtx.Channel;
    // Update last tx done time for the current channel
    txDone.Channel = MacCtx.Channel;
    if( MacCtx.pNvmCtx->NetworkActivation == ACTIVATION_TYPE_NONE )
    {
        txDone.Joined  = false;
    }
    else
    {
        txDone.Joined  = true;
    }
    txDone.LastTxDoneTime = TxDoneParams.CurTime;
    RegionSetBandTxDone( MacCtx.pNvmCtx->Region, &txDone );
    // Update Aggregated last tx done time
    MacCtx.pNvmCtx->LastTxDoneTime = TxDoneParams.CurTime;

    if( MacCtx.NodeAckRequested == false )
    {
        MacCtx.McpsConfirm.Status = LORAMAC_EVENT_INFO_STATUS_OK;
    }
    MacCtx.ChannelsNbTransCounter++;
    MacCtx.McpsConfirm.NbTrans = MacCtx.ChannelsNbTransCounter;
}

static void PrepareRxDoneAbort( void )
{
    MacCtx.MacState |= LORAMAC_RX_ABORT;

    if( MacCtx.NodeAckRequested == true )
    {
        OnAckTimeoutTimerEvent( NULL );
    }

    MacCtx.MacFlags.Bits.McpsInd = 1;
    MacCtx.MacFlags.Bits.MacDone = 1;

    UpdateRxSlotIdleState( );
}

static void ProcessRadioRxDone( void )
{
    LoRaMacHeader_t macHdr;
    ApplyCFListParams_t applyCFList;
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    LoRaMacCryptoStatus_t macCryptoStatus = LORAMAC_CRYPTO_ERROR;

    LoRaMacMessageData_t macMsgData;
    LoRaMacMessageJoinAccept_t macMsgJoinAccept;
    uint8_t *payload = RxDoneParams.Payload;
    uint16_t size = RxDoneParams.Size;
    int16_t rssi = RxDoneParams.Rssi;
    int8_t snr = RxDoneParams.Snr;

    uint8_t pktHeaderLen = 0;

    uint32_t downLinkCounter = 0;
    uint32_t address = MacCtx.pNvmCtx->DevAddr;
    uint8_t multicast = 0;
    AddressIdentifier_t addrID = UNICAST_DEV_ADDR;
    LoRaMacFCntHandlerStatus_t fCntHandlerStatus;
    FCntIdentifier_t fCntID;
    uint8_t macCmdPayload[2] = { 0 };

    MacCtx.McpsConfirm.AckReceived = false;
    MacCtx.McpsIndication.Rssi = rssi;
    MacCtx.McpsIndication.Snr = snr;
    MacCtx.McpsIndication.RxSlot = MacCtx.RxSlot;
    MacCtx.McpsIndication.Port = 0;
    MacCtx.McpsIndication.Multicast = 0;
    MacCtx.McpsIndication.FramePending = 0;
    MacCtx.McpsIndication.Buffer = NULL;
    MacCtx.McpsIndication.BufferSize = 0;
    MacCtx.McpsIndication.RxData = false;
    MacCtx.McpsIndication.AckReceived = false;
    MacCtx.McpsIndication.DownLinkCounter = 0;
    MacCtx.McpsIndication.McpsIndication = MCPS_UNCONFIRMED;
    MacCtx.McpsIndication.DevAddress = 0;
    MacCtx.McpsIndication.DeviceTimeAnsReceived = false;

    Radio.Sleep( );
    TimerStop( &MacCtx.RxWindowTimer2 );

    // This function must be called even if we are not in class b mode yet.
    if( LoRaMacClassBRxBeacon( payload, size ) == true )
    {
        MacCtx.MlmeIndication.BeaconInfo.Rssi = rssi;
        MacCtx.MlmeIndication.BeaconInfo.Snr = snr;
        return;
    }
    // Check if we expect a ping or a multicast slot.
    if( MacCtx.pNvmCtx->DeviceClass == CLASS_B )
    {
        if( LoRaMacClassBIsPingExpected( ) == true )
        {
            LoRaMacClassBSetPingSlotState( PINGSLOT_STATE_CALC_PING_OFFSET );
            LoRaMacClassBPingSlotTimerEvent( NULL );
            MacCtx.McpsIndication.RxSlot = RX_SLOT_WIN_CLASS_B_PING_SLOT;
        }
        else if( LoRaMacClassBIsMulticastExpected( ) == true )
        {
            LoRaMacClassBSetMulticastSlotState( PINGSLOT_STATE_CALC_PING_OFFSET );
            LoRaMacClassBMulticastSlotTimerEvent( NULL );
            MacCtx.McpsIndication.RxSlot = RX_SLOT_WIN_CLASS_B_MULTICAST_SLOT;
        }
    }

    macHdr.Value = payload[pktHeaderLen++];

    // Accept frames of LoRaWAN Major Version 1 only
    if( macHdr.Bits.Major != 0 )
    {
        MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
        PrepareRxDoneAbort( );
        return;
    }

    switch( macHdr.Bits.MType )
    {
        case FRAME_TYPE_JOIN_ACCEPT:
            macMsgJoinAccept.Buffer = payload;
            macMsgJoinAccept.BufSize = size;

            // Abort in case if the device isn't joined yet and no rejoin request is ongoing.
            if( ( MacCtx.pNvmCtx->NetworkActivation != ACTIVATION_TYPE_NONE ) && ( MacCtx.pNvmCtx->IsRejoinRequestPending == false ) )
            {
                MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                PrepareRxDoneAbort( );
                return;
            }
            macCryptoStatus = LoRaMacCryptoHandleJoinAccept( JOIN_REQ, MacCtx.JoinEui, &macMsgJoinAccept );

            if( LORAMAC_CRYPTO_SUCCESS == macCryptoStatus )
            {
                // Network ID
                MacCtx.pNvmCtx->NetID  = ( ( uint32_t ) macMsgJoinAccept.NetID[0] << 0 );
                MacCtx.pNvmCtx->NetID |= ( ( uint32_t ) macMsgJoinAccept.NetID[1] << 8 );
                MacCtx.pNvmCtx->NetID |= ( ( uint32_t ) macMsgJoinAccept.NetID[2] << 16 );

                // Device Address
                MacCtx.pNvmCtx->DevAddr = macMsgJoinAccept.DevAddr;

                // DLSettings
                MacCtx.pNvmCtx->MacParams.Rx1DrOffset			= macMsgJoinAccept.DLSettings.Bits.RX1DRoffset;
                MacCtx.pNvmCtx->MacParams.Rx2Channel.Datarate	= macMsgJoinAccept.DLSettings.Bits.RX2DataRate;
                MacCtx.pNvmCtx->MacParams.RxCChannel.Datarate	= macMsgJoinAccept.DLSettings.Bits.RX2DataRate;

                // RxDelay
                MacCtx.pNvmCtx->MacParams.ReceiveDelay1 = macMsgJoinAccept.RxDelay;
                if( MacCtx.pNvmCtx->MacParams.ReceiveDelay1 == 0 )
                {
                    MacCtx.pNvmCtx->MacParams.ReceiveDelay1 = 1;
                }
                MacCtx.pNvmCtx->MacParams.ReceiveDelay1 *= 1000;
                MacCtx.pNvmCtx->MacParams.ReceiveDelay2 = MacCtx.pNvmCtx->MacParams.ReceiveDelay1 + 1000;

                // Is Networkserver's LoRaWAN Version before 1.1.0 ?
                if( macMsgJoinAccept.DLSettings.Bits.OptNeg == 0 )
                {
                    MacCtx.pNvmCtx->Version.Fields.Minor = 0;
                }
                else
                {
                    MacCtx.pNvmCtx->Version.Fields.Minor = 1;
                }

                // Apply CF list
                applyCFList.Payload = macMsgJoinAccept.CFList;
                // Size of the regular payload is 12. Plus 1 byte MHDR and 4 bytes MIC
                applyCFList.Size = size - 17;

                RegionApplyCFList( MacCtx.pNvmCtx->Region, &applyCFList );

                MacCtx.pNvmCtx->NetworkActivation = ACTIVATION_TYPE_OTAA;

                // Add a RekeyInd MAC command to confirm the security key update.
                if( MacCtx.pNvmCtx->Version.Fields.Minor >= 1 )
                {
                    MacCtx.pNvmCtx->RekeyIndUplinksCounter = 0;
                    macCmdPayload[0] = MacCtx.pNvmCtx->Version.Fields.Minor;
                    LoRaMacCommandsAddCmd( MOTE_MAC_REKEY_IND, macCmdPayload, 1 );
                }

                // MLME handling
                if( LoRaMacConfirmQueueIsCmdActive( MLME_JOIN ) == true )
                {
                    LoRaMacConfirmQueueSetStatus( LORAMAC_EVENT_INFO_STATUS_OK, MLME_JOIN );
                }

                // Rejoin handling
                if( MacCtx.pNvmCtx->IsRejoinRequestPending == true )
                {
                    MacCtx.pNvmCtx->IsRejoinRequestPending = false;

                    // Stop in any case the ForceRejoinReqCycleTimer
                    TimerStop( &MacCtx.ForceRejoinReqCycleTimer );
                }
            }
            else
            {
                // MLME handling
                if( LoRaMacConfirmQueueIsCmdActive( MLME_JOIN ) == true )
                {
                    LoRaMacConfirmQueueSetStatus( LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL, MLME_JOIN );
                }
            }
            break;
        case FRAME_TYPE_DATA_CONFIRMED_DOWN:
            MacCtx.McpsIndication.McpsIndication = MCPS_CONFIRMED;
            // Intentional fall through
        case FRAME_TYPE_DATA_UNCONFIRMED_DOWN:
            // Check if the received payload size is valid
            getPhy.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;
            getPhy.Datarate = MacCtx.McpsIndication.RxDatarate;
            getPhy.Attribute = PHY_MAX_PAYLOAD;

            // Get the maximum payload length
            if( MacCtx.pNvmCtx->RepeaterSupport == true )
            {
                getPhy.Attribute = PHY_MAX_PAYLOAD_REPEATER;
            }
            phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
            if( MAX( 0, ( int16_t )( ( int16_t ) size - ( int16_t ) LORA_MAC_FRMPAYLOAD_OVERHEAD ) ) > ( int16_t )phyParam.Value )
            {
                MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                PrepareRxDoneAbort( );
                return;
            }
            macMsgData.Buffer = payload;
            macMsgData.BufSize = size;
            macMsgData.FRMPayload = MacCtx.RxPayload;
            macMsgData.FRMPayloadSize = LORAMAC_PHY_MAXPAYLOAD;

            if( LORAMAC_PARSER_SUCCESS != LoRaMacParserData( &macMsgData ) )
            {
                MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                PrepareRxDoneAbort( );
                return;
            }

            // Store device address
            MacCtx.McpsIndication.DevAddress = macMsgData.FHDR.DevAddr;

            FType_t fType;
            if( LORAMAC_STATUS_OK != DetermineFrameType( &macMsgData, &fType ) )
            {
                MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                PrepareRxDoneAbort( );
                return;
            }

            //Check if it is a multicast message
            multicast = 0;
            downLinkCounter = 0;
            for( uint8_t i = 0; i < LORAMAC_MAX_MC_CTX; i++ )
            {
                if( ( MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.Address == macMsgData.FHDR.DevAddr ) &&
                    ( MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.IsEnabled == true ) )
                {
                    multicast = 1;
                    addrID = MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.GroupID;
                    downLinkCounter = *( MacCtx.pNvmCtx->MulticastChannelList[i].DownLinkCounter );
                    address = MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.Address;
                    if( MacCtx.pNvmCtx->DeviceClass == CLASS_C )
                    {
                        MacCtx.McpsIndication.RxSlot = RX_SLOT_WIN_CLASS_C_MULTICAST;
                    }
                    break;
                }
            }

            // Get maximum allowed counter difference
            getPhy.Attribute = PHY_MAX_FCNT_GAP;
            phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );

            // Get downlink frame counter value
            fCntHandlerStatus = LoRaMacGetFCntDown( addrID, fType, &macMsgData, MacCtx.pNvmCtx->Version, phyParam.Value, &fCntID, &downLinkCounter );
            if( fCntHandlerStatus != LORAMAC_FCNT_HANDLER_SUCCESS )
            {
                if( fCntHandlerStatus == LORAMAC_FCNT_HANDLER_CHECK_FAIL )
                {
                    // Catch the case of repeated downlink frame counter
                    MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_REPEATED;
                    if( ( MacCtx.pNvmCtx->Version.Fields.Minor == 0 ) && ( macHdr.Bits.MType == FRAME_TYPE_DATA_CONFIRMED_DOWN ) && ( MacCtx.pNvmCtx->LastRxMic == macMsgData.MIC ) )
                    {
                        MacCtx.pNvmCtx->SrvAckRequested = true;
                    }
                }
                else if( fCntHandlerStatus == LORAMAC_FCNT_HANDLER_MAX_GAP_FAIL )
                {
                    // Lost too many frames
                    MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_DOWNLINK_TOO_MANY_FRAMES_LOSS;
                }
                else
                {
                    // Other errors
                    MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                }
                MacCtx.McpsIndication.DownLinkCounter = downLinkCounter;
                PrepareRxDoneAbort( );
                return;
            }

            macCryptoStatus = LoRaMacCryptoUnsecureMessage( addrID, address, fCntID, downLinkCounter, &macMsgData );
            if( macCryptoStatus != LORAMAC_CRYPTO_SUCCESS )
            {
                if( macCryptoStatus == LORAMAC_CRYPTO_FAIL_ADDRESS )
                {
                    // We are not the destination of this frame.
                    MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ADDRESS_FAIL;
                }
                else
                {
                    // MIC calculation fail
                    MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_MIC_FAIL;
                }
                PrepareRxDoneAbort( );
                return;
            }

            // Frame is valid
            MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_OK;
            MacCtx.McpsIndication.Multicast = multicast;
            MacCtx.McpsIndication.FramePending = macMsgData.FHDR.FCtrl.Bits.FPending;
            MacCtx.McpsIndication.Buffer = NULL;
            MacCtx.McpsIndication.BufferSize = 0;
            MacCtx.McpsIndication.DownLinkCounter = downLinkCounter;
            MacCtx.McpsIndication.AckReceived = macMsgData.FHDR.FCtrl.Bits.Ack;

            MacCtx.McpsConfirm.Status = LORAMAC_EVENT_INFO_STATUS_OK;
            MacCtx.McpsConfirm.AckReceived = macMsgData.FHDR.FCtrl.Bits.Ack;

            MacCtx.pNvmCtx->AdrAckCounter = 0;

            // MCPS Indication and ack requested handling
            if( multicast == 1 )
            {
                MacCtx.McpsIndication.McpsIndication = MCPS_MULTICAST;
            }
            else
            {
                if( macHdr.Bits.MType == FRAME_TYPE_DATA_CONFIRMED_DOWN )
                {
                    MacCtx.pNvmCtx->SrvAckRequested = true;
                    if( MacCtx.pNvmCtx->Version.Fields.Minor == 0 )
                    {
                        MacCtx.pNvmCtx->LastRxMic = macMsgData.MIC;
                    }
                    MacCtx.McpsIndication.McpsIndication = MCPS_CONFIRMED;
                }
                else
                {
                    MacCtx.pNvmCtx->SrvAckRequested = false;
                    MacCtx.McpsIndication.McpsIndication = MCPS_UNCONFIRMED;
                }
            }

            // Update downlink counter in mac context / multicast context.
            if( LORAMAC_FCNT_HANDLER_SUCCESS != LoRaMacSetFCntDown( fCntID, downLinkCounter ) )
            {
                MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                PrepareRxDoneAbort( );
                return;
            }

            RemoveMacCommands( MacCtx.McpsIndication.RxSlot, macMsgData.FHDR.FCtrl, MacCtx.McpsConfirm.McpsRequest );

            switch( fType )
            {
                case FRAME_TYPE_A:
                {  /* +----------+------+-------+--------------+
                    * | FOptsLen | Fopt | FPort |  FRMPayload  |
                    * +----------+------+-------+--------------+
                    * |    > 0   |   X  |  > 0  |       X      |
                    * +----------+------+-------+--------------+
                    */

                    // Decode MAC commands in FOpts field
                    ProcessMacCommands( macMsgData.FHDR.FOpts, 0, macMsgData.FHDR.FCtrl.Bits.FOptsLen, snr, MacCtx.McpsIndication.RxSlot );
                    MacCtx.McpsIndication.Port = macMsgData.FPort;
                    MacCtx.McpsIndication.Buffer = macMsgData.FRMPayload;
                    MacCtx.McpsIndication.BufferSize = macMsgData.FRMPayloadSize;
                    MacCtx.McpsIndication.RxData = true;
                    break;
                }
                case FRAME_TYPE_B:
                {  /* +----------+------+-------+--------------+
                    * | FOptsLen | Fopt | FPort |  FRMPayload  |
                    * +----------+------+-------+--------------+
                    * |    > 0   |   X  |   -   |       -      |
                    * +----------+------+-------+--------------+
                    */

                    // Decode MAC commands in FOpts field
                    ProcessMacCommands( macMsgData.FHDR.FOpts, 0, macMsgData.FHDR.FCtrl.Bits.FOptsLen, snr, MacCtx.McpsIndication.RxSlot );
                    MacCtx.McpsIndication.Port = macMsgData.FPort;
                    break;
                }
                case FRAME_TYPE_C:
                {  /* +----------+------+-------+--------------+
                    * | FOptsLen | Fopt | FPort |  FRMPayload  |
                    * +----------+------+-------+--------------+
                    * |    = 0   |   -  |  = 0  | MAC commands |
                    * +----------+------+-------+--------------+
                    */

                    // Decode MAC commands in FRMPayload
                    ProcessMacCommands( macMsgData.FRMPayload, 0, macMsgData.FRMPayloadSize, snr, MacCtx.McpsIndication.RxSlot );
                    MacCtx.McpsIndication.Port = macMsgData.FPort;
                    break;
                }
                case FRAME_TYPE_D:
                {  /* +----------+------+-------+--------------+
                    * | FOptsLen | Fopt | FPort |  FRMPayload  |
                    * +----------+------+-------+--------------+
                    * |    = 0   |   -  |  > 0  |       X      |
                    * +----------+------+-------+--------------+
                    */

                    // No MAC commands just application payload
                    MacCtx.McpsIndication.Port = macMsgData.FPort;
                    MacCtx.McpsIndication.Buffer = macMsgData.FRMPayload;
                    MacCtx.McpsIndication.BufferSize = macMsgData.FRMPayloadSize;
                    MacCtx.McpsIndication.RxData = true;
                    break;
                }
                default:
                    MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
                    PrepareRxDoneAbort( );
                    break;
            }

            // Rejoin handling
            if( MacCtx.pNvmCtx->IsRejoinRequestPending == true )
            {
                MacCtx.pNvmCtx->IsRejoinRequestPending = false;

                // Stop in any case the ForceRejoinReqCycleTimer
                TimerStop( &MacCtx.ForceRejoinReqCycleTimer );

                // If the rejoin was trigged by MLME, set confirmation status
                if( MacCtx.MacFlags.Bits.MlmeReq == 1 )
                {
                    MacCtx.MlmeConfirm.Status = LORAMAC_EVENT_INFO_STATUS_OK;
                }
            }

            // Provide always an indication, skip the callback to the user application,
            // in case of a confirmed downlink retransmission.
            MacCtx.MacFlags.Bits.McpsInd = 1;

            break;
        case FRAME_TYPE_PROPRIETARY:
            memcpy1( MacCtx.RxPayload, &payload[pktHeaderLen], size - pktHeaderLen );

            MacCtx.McpsIndication.McpsIndication = MCPS_PROPRIETARY;
            MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_OK;
            MacCtx.McpsIndication.Buffer = MacCtx.RxPayload;
            MacCtx.McpsIndication.BufferSize = size - pktHeaderLen;

            MacCtx.MacFlags.Bits.McpsInd = 1;
            break;
        default:
            MacCtx.McpsIndication.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
            PrepareRxDoneAbort( );
            break;
    }

    // Verify if we need to disable the AckTimeoutTimer
    if( MacCtx.NodeAckRequested == true )
    {
        if( MacCtx.McpsConfirm.AckReceived == true )
        {
            OnAckTimeoutTimerEvent( NULL );
        }
    }
    else
    {
        if( MacCtx.pNvmCtx->DeviceClass == CLASS_C )
        {
            OnAckTimeoutTimerEvent( NULL );
        }
    }
    MacCtx.MacFlags.Bits.MacDone = 1;

    UpdateRxSlotIdleState( );
}

static void ProcessRadioTxTimeout( void )
{
    if( MacCtx.pNvmCtx->DeviceClass != CLASS_C )
    {
        Radio.Sleep( );
    }
    UpdateRxSlotIdleState( );

    MacCtx.McpsConfirm.Status = LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT;
    LoRaMacConfirmQueueSetStatusCmn( LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT );
    MacCtx.MacFlags.Bits.MacDone = 1;
}

static void HandleRadioRxErrorTimeout( LoRaMacEventInfoStatus_t rx1EventInfoStatus, LoRaMacEventInfoStatus_t rx2EventInfoStatus )
{
    bool classBRx = false;

    if( MacCtx.pNvmCtx->DeviceClass != CLASS_C )
    {
        Radio.Sleep( );
    }

    if( LoRaMacClassBIsBeaconExpected( ) == true )
    {
        LoRaMacClassBSetBeaconState( BEACON_STATE_TIMEOUT );
        LoRaMacClassBBeaconTimerEvent( NULL );
        classBRx = true;
    }
    if( MacCtx.pNvmCtx->DeviceClass == CLASS_B )
    {
        if( LoRaMacClassBIsPingExpected( ) == true )
        {
            LoRaMacClassBSetPingSlotState( PINGSLOT_STATE_CALC_PING_OFFSET );
            LoRaMacClassBPingSlotTimerEvent( NULL );
            classBRx = true;
        }
        if( LoRaMacClassBIsMulticastExpected( ) == true )
        {
            LoRaMacClassBSetMulticastSlotState( PINGSLOT_STATE_CALC_PING_OFFSET );
            LoRaMacClassBMulticastSlotTimerEvent( NULL );
            classBRx = true;
        }
    }

    if( classBRx == false )
    {
		#if LORAMAC_LOG_ENABLED
			NRF_LOG_INFO("Handle Rx%u Timeout", 1 + (uint32_t) MacCtx.RxSlot);
		#endif
        if( MacCtx.RxSlot == RX_SLOT_WIN_1 )
        {
            if( MacCtx.NodeAckRequested == true )
            {
                MacCtx.McpsConfirm.Status = rx1EventInfoStatus;
            }
            LoRaMacConfirmQueueSetStatusCmn( rx1EventInfoStatus );
        }
        else
        {
            if( MacCtx.NodeAckRequested == true )
            {
                MacCtx.McpsConfirm.Status = rx2EventInfoStatus;
            }
            LoRaMacConfirmQueueSetStatusCmn( rx2EventInfoStatus );

            if( MacCtx.pNvmCtx->DeviceClass != CLASS_C )
            {
                MacCtx.MacFlags.Bits.MacDone = 1;
            }
        }
    }

    UpdateRxSlotIdleState( );
}

static void ProcessRadioRxError( void )
{
    HandleRadioRxErrorTimeout( LORAMAC_EVENT_INFO_STATUS_RX1_ERROR, LORAMAC_EVENT_INFO_STATUS_RX2_ERROR );
}

static void ProcessRadioRxTimeout( void )
{
    HandleRadioRxErrorTimeout( LORAMAC_EVENT_INFO_STATUS_RX1_TIMEOUT, LORAMAC_EVENT_INFO_STATUS_RX2_TIMEOUT );
}

static void LoRaMacHandleIrqEvents( void )
{
    LoRaMacRadioEvents_t events;

    CRITICAL_SECTION_BEGIN( );
    events = LoRaMacRadioEvents;
    LoRaMacRadioEvents.Value = 0;
    CRITICAL_SECTION_END( );

    if( events.Value != 0 )
    {
        if( events.Events.TxDone == 1 )
        {
            ProcessRadioTxDone( );
        }
        if( events.Events.RxDone == 1 )
        {
			NRF_LOG_INFO("Rx Done");
            ProcessRadioRxDone( );
        }
        if( events.Events.TxTimeout == 1 )
        {
			NRF_LOG_INFO("Tx Timeout");
            ProcessRadioTxTimeout( );
        }
        if( events.Events.RxError == 1 )
        {
			NRF_LOG_INFO("Rx Error");
            ProcessRadioRxError( );
        }
        if( events.Events.RxTimeout == 1 )
        {
	    
            ProcessRadioRxTimeout( );
        }
    }
}

bool LoRaMacIsBusy( void )
{
    if( ( MacCtx.MacState == LORAMAC_IDLE ) &&
        ( MacCtx.AllowRequests == LORAMAC_REQUEST_HANDLING_ON ) )
    {
        return false;
    }
    return true;
}


static void LoRaMacEnableRequests( LoRaMacRequestHandling_t requestState )
{
    MacCtx.AllowRequests = requestState;
}

static void LoRaMacHandleRequestEvents( void )
{
    // Handle events
    LoRaMacFlags_t reqEvents = MacCtx.MacFlags;

    if( MacCtx.MacState == LORAMAC_IDLE )
    {
        // Update event bits
        if( MacCtx.MacFlags.Bits.McpsReq == 1 )
        {
            MacCtx.MacFlags.Bits.McpsReq = 0;
        }

        if( MacCtx.MacFlags.Bits.MlmeReq == 1 )
        {
            MacCtx.MacFlags.Bits.MlmeReq = 0;
        }

        // Allow requests again
        LoRaMacEnableRequests( LORAMAC_REQUEST_HANDLING_ON );

        // Handle callbacks
        if( reqEvents.Bits.McpsReq == 1 )
        {
            MacCtx.MacPrimitives->MacMcpsConfirm( &MacCtx.McpsConfirm );
        }

        if( reqEvents.Bits.MlmeReq == 1 )
        {
            LoRaMacConfirmQueueHandleCb( &MacCtx.MlmeConfirm );
            if( LoRaMacConfirmQueueGetCnt( ) > 0 )
            {
                MacCtx.MacFlags.Bits.MlmeReq = 1;
            }
        }

        // Start beaconing again
        LoRaMacClassBResumeBeaconing( );

        // Procedure done. Reset variables.
        MacCtx.MacFlags.Bits.MacDone = 0;
    }
}

static void LoRaMacHandleScheduleUplinkEvent( void )
{
    // Handle events
    if( MacCtx.MacState == LORAMAC_IDLE )
    {
        // Verify if sticky MAC commands are pending or not
        bool isStickyMacCommandPending = false;
        LoRaMacCommandsStickyCmdsPending( &isStickyMacCommandPending );
        if( isStickyMacCommandPending == true )
        {// Setup MLME indication
            SetMlmeScheduleUplinkIndication( );
        }
    }
}

static void LoRaMacHandleIndicationEvents( void )
{
    // Handle MLME indication
    if( MacCtx.MacFlags.Bits.MlmeInd == 1 )
    {
        MacCtx.MacFlags.Bits.MlmeInd = 0;
        MacCtx.MacPrimitives->MacMlmeIndication( &MacCtx.MlmeIndication );
    }

    if( MacCtx.MacFlags.Bits.MlmeSchedUplinkInd == 1 )
    {
        MlmeIndication_t schduleUplinkIndication;
        schduleUplinkIndication.MlmeIndication = MLME_SCHEDULE_UPLINK;
        schduleUplinkIndication.Status = LORAMAC_EVENT_INFO_STATUS_OK;

        MacCtx.MacPrimitives->MacMlmeIndication( &schduleUplinkIndication );
        MacCtx.MacFlags.Bits.MlmeSchedUplinkInd = 0;
    }

    // Handle MCPS indication
    if( MacCtx.MacFlags.Bits.McpsInd == 1 )
    {
        MacCtx.MacFlags.Bits.McpsInd = 0;
        MacCtx.MacPrimitives->MacMcpsIndication( &MacCtx.McpsIndication );
    }
}

static void LoRaMacHandleMcpsRequest( void )
{
    // Handle MCPS uplinks
    if( MacCtx.MacFlags.Bits.McpsReq == 1 )
    {
        bool stopRetransmission = false;
        bool waitForRetransmission = false;

        if( ( MacCtx.McpsConfirm.McpsRequest == MCPS_UNCONFIRMED ) ||
            ( MacCtx.McpsConfirm.McpsRequest == MCPS_PROPRIETARY ) )
        {
            stopRetransmission = CheckRetransUnconfirmedUplink( );
        }
        else if( MacCtx.McpsConfirm.McpsRequest == MCPS_CONFIRMED )
        {
            if( MacCtx.AckTimeoutRetry == true )
            {
                stopRetransmission = CheckRetransConfirmedUplink( );
            }
            else
            {
                waitForRetransmission = true;
            }
        }

        if( stopRetransmission == true )
        {// Stop retransmission
            TimerStop( &MacCtx.TxDelayedTimer );
            MacCtx.MacState &= ~LORAMAC_TX_DELAYED;
            StopRetransmission( );

            if( IsReJoin0Required( ) == true )
            {
                SendReJoinReq( REJOIN_REQ_0 );
            }
        }
        else if( waitForRetransmission == false )
        {// Arrange further retransmission
            MacCtx.MacFlags.Bits.MacDone = 0;
            // Reset the state of the AckTimeout
            MacCtx.AckTimeoutRetry = false;
            // Sends the same frame again
            OnTxDelayedTimerEvent( NULL );
        }
    }
}

static void LoRaMacHandleJoinRequest( void )
{
    // Handle join request
    if( ( MacCtx.MacFlags.Bits.MlmeReq == 1 ) && ( LoRaMacConfirmQueueIsCmdActive( MLME_JOIN ) == true ) )
    {
        if( LoRaMacConfirmQueueGetStatus( MLME_JOIN ) == LORAMAC_EVENT_INFO_STATUS_OK )
        {// Node joined successfully
            LoRaMacResetFCnts( );
            MacCtx.ChannelsNbTransCounter = 0;
        }
        MacCtx.MacState &= ~LORAMAC_TX_RUNNING;
    }
}

static uint8_t LoRaMacCheckForTxTimeout( void )
{
    if( ( LoRaMacConfirmQueueGetStatusCmn( ) == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT ) ||
        ( MacCtx.McpsConfirm.Status == LORAMAC_EVENT_INFO_STATUS_TX_TIMEOUT ) )
    {
        // Stop transmit cycle due to tx timeout
        MacCtx.MacState &= ~LORAMAC_TX_RUNNING;
        MacCtx.McpsConfirm.NbTrans = MacCtx.ChannelsNbTransCounter;
        MacCtx.McpsConfirm.AckReceived = false;
        MacCtx.McpsConfirm.TxTimeOnAir = 0;
        return 0x01;
    }
    return 0x00;
}

static uint8_t LoRaMacCheckForBeaconAcquisition( void )
{
    if( ( LoRaMacConfirmQueueIsCmdActive( MLME_BEACON_ACQUISITION ) == true ) &&
        ( MacCtx.MacFlags.Bits.McpsReq == 0 ) )
    {
        if( MacCtx.MacFlags.Bits.MlmeReq == 1 )
        {
            MacCtx.MacState &= ~LORAMAC_TX_RUNNING;
            return 0x01;
        }
    }
    return 0x00;
}

static void LoRaMacCheckForRxAbort( void )
{
    // A error occurs during receiving
    if( ( MacCtx.MacState & LORAMAC_RX_ABORT ) == LORAMAC_RX_ABORT )
    {
        MacCtx.MacState &= ~LORAMAC_RX_ABORT;
        MacCtx.MacState &= ~LORAMAC_TX_RUNNING;
    }
}


void LoRaMacProcess( void )
{
    uint8_t noTx = false;

    LoRaMacHandleIrqEvents( );
    LoRaMacClassBProcess( );

    // MAC proceeded a state and is ready to check
    if( MacCtx.MacFlags.Bits.MacDone == 1 )
    {
        LoRaMacEnableRequests( LORAMAC_REQUEST_HANDLING_OFF );
        LoRaMacCheckForRxAbort( );

        // An error occurs during transmitting
        if( IsRequestPending( ) > 0 )
        {
            noTx |= LoRaMacCheckForTxTimeout( );
            noTx |= LoRaMacCheckForBeaconAcquisition( );
        }

        if( noTx == 0x00 )
        {
            LoRaMacHandleJoinRequest( );
            LoRaMacHandleMcpsRequest( );
        }
        LoRaMacHandleRequestEvents( );
        LoRaMacHandleScheduleUplinkEvent( );
        LoRaMacEnableRequests( LORAMAC_REQUEST_HANDLING_ON );
    }
    LoRaMacHandleIndicationEvents( );
    if( MacCtx.RxSlot == RX_SLOT_WIN_CLASS_C )
    {
        OpenContinuousRxCWindow( );
    }
}

static void OnTxDelayedTimerEvent( void* context )
{
    TimerStop( &MacCtx.TxDelayedTimer );
    MacCtx.MacState &= ~LORAMAC_TX_DELAYED;

    // Schedule frame, allow delayed frame transmissions
    switch( ScheduleTx( true ) )
    {
        case LORAMAC_STATUS_OK:
        case LORAMAC_STATUS_DUTYCYCLE_RESTRICTED:
        {
            break;
        }
        default:
        {
            // Stop retransmission attempt
            MacCtx.McpsConfirm.Datarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
            MacCtx.McpsConfirm.NbTrans = MacCtx.ChannelsNbTransCounter;
            MacCtx.McpsConfirm.Status = LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR;
            LoRaMacConfirmQueueSetStatusCmn( LORAMAC_EVENT_INFO_STATUS_TX_DR_PAYLOAD_SIZE_ERROR );
            StopRetransmission( );
            break;
        }
    }
}

static void OnRxWindow1TimerEvent( void* context )
{
    MacCtx.RxWindow1Config.Channel				= MacCtx.Channel;
    MacCtx.RxWindow1Config.DrOffset				= MacCtx.pNvmCtx->MacParams.Rx1DrOffset;
    MacCtx.RxWindow1Config.DownlinkDwellTime	= MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;
    MacCtx.RxWindow1Config.RepeaterSupport		= MacCtx.pNvmCtx->RepeaterSupport;
    MacCtx.RxWindow1Config.RxContinuous			= false;
    MacCtx.RxWindow1Config.RxSlot				= RX_SLOT_WIN_1;

    RxWindowSetup( &MacCtx.RxWindowTimer1, &MacCtx.RxWindow1Config );
}

static void OnRxWindow2TimerEvent( void* context )
{
    // Check if we are processing Rx1 window.
    // If yes, we don't setup the Rx2 window.
    if( MacCtx.RxSlot == RX_SLOT_WIN_1 )
    {
		NRF_LOG_INFO("RX2 window discarded, because still processing RX1");
        return;
    }
    MacCtx.RxWindow2Config.Channel				= MacCtx.Channel;
    MacCtx.RxWindow2Config.Frequency			= MacCtx.pNvmCtx->MacParams.Rx2Channel.Frequency;
    MacCtx.RxWindow2Config.DownlinkDwellTime	= MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;
    MacCtx.RxWindow2Config.RepeaterSupport		= MacCtx.pNvmCtx->RepeaterSupport;
    MacCtx.RxWindow2Config.RxContinuous			= false;
    MacCtx.RxWindow2Config.RxSlot				= RX_SLOT_WIN_2;

    RxWindowSetup( &MacCtx.RxWindowTimer2, &MacCtx.RxWindow2Config );
}

static void OnAckTimeoutTimerEvent( void* context )
{
    TimerStop( &MacCtx.AckTimeoutTimer );

    if( MacCtx.NodeAckRequested == true )
    {
        MacCtx.AckTimeoutRetry = true;
    }
    if( MacCtx.pNvmCtx->DeviceClass == CLASS_C )
    {
        MacCtx.MacFlags.Bits.MacDone = 1;
    }
    if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->MacProcessNotify != NULL ) )
    {
        MacCtx.MacCallbacks->MacProcessNotify( );
    }
}

static LoRaMacStatus_t SwitchClass( DeviceClass_t deviceClass )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_PARAMETER_INVALID;

    switch( MacCtx.pNvmCtx->DeviceClass )
    {
        case CLASS_A:
        {
            if( deviceClass == CLASS_A )
            {
                // Revert back RxC parameters
                MacCtx.pNvmCtx->MacParams.RxCChannel = MacCtx.pNvmCtx->MacParams.Rx2Channel;
            }
            if( deviceClass == CLASS_B )
            {
                status = LoRaMacClassBSwitchClass( deviceClass );
                if( status == LORAMAC_STATUS_OK )
                {
                    MacCtx.pNvmCtx->DeviceClass = deviceClass;
                }
            }

            if( deviceClass == CLASS_C )
            {
                MacCtx.pNvmCtx->DeviceClass = deviceClass;

                MacCtx.RxWindowCConfig = MacCtx.RxWindow2Config;
                MacCtx.RxWindowCConfig.RxSlot = RX_SLOT_WIN_CLASS_C;

                for( int8_t i = 0; i < LORAMAC_MAX_MC_CTX; i++ )
                {
                    if( MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.IsEnabled == true )
                    // TODO: Check multicast channel device class.
                    {
                        MacCtx.pNvmCtx->MacParams.RxCChannel.Frequency = MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.RxParams.ClassC.Frequency;
                        MacCtx.pNvmCtx->MacParams.RxCChannel.Datarate = MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.RxParams.ClassC.Datarate;

                        MacCtx.RxWindowCConfig.Channel = MacCtx.Channel;
                        MacCtx.RxWindowCConfig.Frequency = MacCtx.pNvmCtx->MacParams.RxCChannel.Frequency;
                        MacCtx.RxWindowCConfig.DownlinkDwellTime = MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;
                        MacCtx.RxWindowCConfig.RepeaterSupport = MacCtx.pNvmCtx->RepeaterSupport;
                        MacCtx.RxWindowCConfig.RxSlot = RX_SLOT_WIN_CLASS_C_MULTICAST;
                        MacCtx.RxWindowCConfig.RxContinuous = true;
                        break;
                    }
                }

                // Set the NodeAckRequested indicator to default
                MacCtx.NodeAckRequested = false;
                // Set the radio into sleep mode in case we are still in RX mode
                Radio.Sleep( );
                // Compute Rx2 windows parameters in case the RX2 datarate has changed
                RegionComputeRxWindowParameters( MacCtx.pNvmCtx->Region,
                                                 MacCtx.pNvmCtx->MacParams.RxCChannel.Datarate,
                                                 MacCtx.pNvmCtx->MacParams.MinRxSymbols,
                                                 MacCtx.pNvmCtx->MacParams.SystemMaxRxError,
                                                 &MacCtx.RxWindowCConfig );
                OpenContinuousRxCWindow( );

                // Add a DeviceModeInd MAC Command to indicate the network a device mode change.
                if( MacCtx.pNvmCtx->Version.Fields.Minor == 1 )
                {
                    LoRaMacCommandsAddCmd( MOTE_MAC_DEVICE_MODE_IND, ( uint8_t* )&MacCtx.pNvmCtx->DeviceClass, 1 );
                }

                status = LORAMAC_STATUS_OK;
            }
            break;
        }
        case CLASS_B:
        {
            status = LoRaMacClassBSwitchClass( deviceClass );
            if( status == LORAMAC_STATUS_OK )
            {
                MacCtx.pNvmCtx->DeviceClass = deviceClass;
            }
            break;
        }
        case CLASS_C:
        {
            if( deviceClass == CLASS_A )
            {
                MacCtx.pNvmCtx->DeviceClass = deviceClass;

                // Set the radio into sleep to setup a defined state
                Radio.Sleep( );

                status = LORAMAC_STATUS_OK;

                // Add a DeviceModeInd MAC Command to indicate the network a device mode change.
                if( MacCtx.pNvmCtx->Version.Fields.Minor == 1 )
                {
                    LoRaMacCommandsAddCmd( MOTE_MAC_DEVICE_MODE_IND, ( uint8_t* )&MacCtx.pNvmCtx->DeviceClass, 1 );
                }
            }
            break;
        }
    }

    return status;
}

static uint8_t GetMaxAppPayloadWithoutFOptsLength( int8_t datarate )
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;

    // Setup PHY request
    getPhy.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;
    getPhy.Datarate = datarate;
    getPhy.Attribute = PHY_MAX_PAYLOAD;

    // Get the maximum payload length
    if( MacCtx.pNvmCtx->RepeaterSupport == true )
    {
        getPhy.Attribute = PHY_MAX_PAYLOAD_REPEATER;
    }
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );

    return phyParam.Value;
}

static bool ValidatePayloadLength( uint8_t lenN, int8_t datarate, uint8_t fOptsLen )
{
    uint16_t maxN = 0;
    uint16_t payloadSize = 0;

    maxN = GetMaxAppPayloadWithoutFOptsLength( datarate );

    // Calculate the resulting payload size
    payloadSize = ( lenN + fOptsLen );

    // Validation of the application payload size
    if( ( payloadSize <= maxN ) && ( payloadSize <= LORAMAC_PHY_MAXPAYLOAD ) )
    {
        return true;
    }
    return false;
}

static void SetMlmeScheduleUplinkIndication( void )
{
    MacCtx.MacFlags.Bits.MlmeSchedUplinkInd = 1;
}

static void ProcessMacCommands( uint8_t *payload, uint8_t macIndex, uint8_t commandsSize, int8_t snr, LoRaMacRxSlot_t rxSlot )
{
    uint8_t status = 0;
    bool adrBlockFound = false;
    bool adrInvalidBlockFound = false;
    uint8_t macCmdPayload[2] = { 0x00, 0x00 };
    MacCommand_t* macCmd;

    while( macIndex < commandsSize )
    {
        if( payload[macIndex] != SRV_MAC_LINK_ADR_REQ )
        {
            // Reset the status if the command is not a SRV_MAC_LINK_ADR_REQ
            adrInvalidBlockFound = false;
        }

        // Decode Frame MAC commands
        switch( payload[macIndex++] )
        {
            case SRV_MAC_RESET_CONF:
            {
                uint8_t serverMinorVersion = payload[macIndex++];

                // Compare own LoRaWAN Version with server's
                if( MacCtx.pNvmCtx->Version.Fields.Minor == serverMinorVersion )
                {
                    // If they equal remove the sticky ResetInd MAC-Command.
                    LoRaMacCommandsGetCmd( MOTE_MAC_RESET_IND, &macCmd);
                    LoRaMacCommandsRemoveCmd( macCmd );
                }
                break;
            }
            case SRV_MAC_LINK_CHECK_ANS:
            {
                if( LoRaMacConfirmQueueIsCmdActive( MLME_LINK_CHECK ) == true )
                {
                    LoRaMacConfirmQueueSetStatus( LORAMAC_EVENT_INFO_STATUS_OK, MLME_LINK_CHECK );
                    MacCtx.MlmeConfirm.DemodMargin = payload[macIndex++];
                    MacCtx.MlmeConfirm.NbGateways = payload[macIndex++];
                }
                break;
            }
            case SRV_MAC_LINK_ADR_REQ:
            {
                LinkAdrReqParams_t linkAdrReq;
                int8_t linkAdrDatarate = DR_0;
                int8_t linkAdrTxPower = TX_POWER_0;
                uint8_t linkAdrNbRep = 0;
                uint8_t linkAdrNbBytesParsed = 0;

                if( adrBlockFound == false )
                {
                    adrBlockFound = true;

                    // Fill parameter structure
                    linkAdrReq.Payload = &payload[macIndex - 1];
                    linkAdrReq.PayloadSize = commandsSize - ( macIndex - 1 );
                    linkAdrReq.AdrEnabled = MacCtx.pNvmCtx->AdrCtrlOn;
                    linkAdrReq.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;
                    linkAdrReq.CurrentDatarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
                    linkAdrReq.CurrentTxPower = MacCtx.pNvmCtx->MacParams.ChannelsTxPower;
                    linkAdrReq.CurrentNbRep = MacCtx.pNvmCtx->MacParams.ChannelsNbTrans;
                    linkAdrReq.Version = MacCtx.pNvmCtx->Version;

                    // Process the ADR requests
                    status = RegionLinkAdrReq( MacCtx.pNvmCtx->Region, &linkAdrReq, &linkAdrDatarate,
                                               &linkAdrTxPower, &linkAdrNbRep, &linkAdrNbBytesParsed );

                    if( ( status & 0x07 ) == 0x07 )
                    {
                        MacCtx.pNvmCtx->MacParams.ChannelsDatarate = linkAdrDatarate;
                        MacCtx.pNvmCtx->MacParams.ChannelsTxPower = linkAdrTxPower;
                        MacCtx.pNvmCtx->MacParams.ChannelsNbTrans = linkAdrNbRep;
                    }

                    // Add one single answer to the buffer
                    LoRaMacCommandsAddCmd( MOTE_MAC_LINK_ADR_ANS, &status, 1 );

                    // Update MAC index
                    macIndex += linkAdrNbBytesParsed - 1;
                }
                else if( adrInvalidBlockFound == false )
                {
                    adrInvalidBlockFound = true;
                    // Set all status bits to zero, if there is more than one
                    // atomic block of LinkAdrReq.
                    status = 0;
                    LoRaMacCommandsAddCmd( MOTE_MAC_LINK_ADR_ANS, &status, 1 );
                    macIndex += 4;
                }
                else
                {
                    macIndex += 4;
                }
                break;
            }
            case SRV_MAC_DUTY_CYCLE_REQ:
            {
                MacCtx.pNvmCtx->MaxDCycle = payload[macIndex++] & 0x0F;
                MacCtx.pNvmCtx->AggregatedDCycle = 1 << MacCtx.pNvmCtx->MaxDCycle;
                LoRaMacCommandsAddCmd( MOTE_MAC_DUTY_CYCLE_ANS, macCmdPayload, 0 );
                break;
            }
            case SRV_MAC_RX_PARAM_SETUP_REQ:
            {
                RxParamSetupReqParams_t rxParamSetupReq;
                status = 0x07;

                rxParamSetupReq.DrOffset = ( payload[macIndex] >> 4 ) & 0x07;
                rxParamSetupReq.Datarate = payload[macIndex] & 0x0F;
                macIndex++;

                rxParamSetupReq.Frequency = ( uint32_t ) payload[macIndex++];
                rxParamSetupReq.Frequency |= ( uint32_t ) payload[macIndex++] << 8;
                rxParamSetupReq.Frequency |= ( uint32_t ) payload[macIndex++] << 16;
                rxParamSetupReq.Frequency *= 100;

                // Perform request on region
                status = RegionRxParamSetupReq( MacCtx.pNvmCtx->Region, &rxParamSetupReq );

                if( ( status & 0x07 ) == 0x07 )
                {
                    MacCtx.pNvmCtx->MacParams.Rx2Channel.Datarate = rxParamSetupReq.Datarate;
                    MacCtx.pNvmCtx->MacParams.RxCChannel.Datarate = rxParamSetupReq.Datarate;
                    MacCtx.pNvmCtx->MacParams.Rx2Channel.Frequency = rxParamSetupReq.Frequency;
                    MacCtx.pNvmCtx->MacParams.RxCChannel.Frequency = rxParamSetupReq.Frequency;
                    MacCtx.pNvmCtx->MacParams.Rx1DrOffset = rxParamSetupReq.DrOffset;
                }
                macCmdPayload[0] = status;
                LoRaMacCommandsAddCmd( MOTE_MAC_RX_PARAM_SETUP_ANS, macCmdPayload, 1 );
                // Setup indication to inform the application
                SetMlmeScheduleUplinkIndication( );
                break;
            }
            case SRV_MAC_DEV_STATUS_REQ:
            {
                uint8_t batteryLevel = BAT_LEVEL_NO_MEASURE;
                if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->GetBatteryLevel != NULL ) )
                {
                    batteryLevel = MacCtx.MacCallbacks->GetBatteryLevel( );
                }
                macCmdPayload[0] = batteryLevel;
                macCmdPayload[1] = ( uint8_t )( snr & 0x3F );
                LoRaMacCommandsAddCmd( MOTE_MAC_DEV_STATUS_ANS, macCmdPayload, 2 );
                break;
            }
            case SRV_MAC_NEW_CHANNEL_REQ:
            {
                NewChannelReqParams_t newChannelReq;
                ChannelParams_t chParam;
                status = 0x03;

                newChannelReq.ChannelId = payload[macIndex++];
                newChannelReq.NewChannel = &chParam;

                chParam.Frequency = ( uint32_t ) payload[macIndex++];
                chParam.Frequency |= ( uint32_t ) payload[macIndex++] << 8;
                chParam.Frequency |= ( uint32_t ) payload[macIndex++] << 16;
                chParam.Frequency *= 100;
                chParam.Rx1Frequency = 0;
                chParam.DrRange.Value = payload[macIndex++];

                status = RegionNewChannelReq( MacCtx.pNvmCtx->Region, &newChannelReq );

                macCmdPayload[0] = status;
                LoRaMacCommandsAddCmd( MOTE_MAC_NEW_CHANNEL_ANS, macCmdPayload, 1 );
                break;
            }
            case SRV_MAC_RX_TIMING_SETUP_REQ:
            {
                uint8_t delay = payload[macIndex++] & 0x0F;

                if( delay == 0 )
                {
                    delay++;
                }
                MacCtx.pNvmCtx->MacParams.ReceiveDelay1 = delay * 1000;
                MacCtx.pNvmCtx->MacParams.ReceiveDelay2 = MacCtx.pNvmCtx->MacParams.ReceiveDelay1 + 1000;
                LoRaMacCommandsAddCmd( MOTE_MAC_RX_TIMING_SETUP_ANS, macCmdPayload, 0 );
                // Setup indication to inform the application
                SetMlmeScheduleUplinkIndication( );
                break;
            }
            case SRV_MAC_TX_PARAM_SETUP_REQ:
            {
                TxParamSetupReqParams_t txParamSetupReq;
                GetPhyParams_t getPhy;
                PhyParam_t phyParam;
                uint8_t eirpDwellTime = payload[macIndex++];

                txParamSetupReq.UplinkDwellTime = 0;
                txParamSetupReq.DownlinkDwellTime = 0;

                if( ( eirpDwellTime & 0x20 ) == 0x20 )
                {
                    txParamSetupReq.DownlinkDwellTime = 1;
                }
                if( ( eirpDwellTime & 0x10 ) == 0x10 )
                {
                    txParamSetupReq.UplinkDwellTime = 1;
                }
                txParamSetupReq.MaxEirp = eirpDwellTime & 0x0F;

                // Check the status for correctness
                if( RegionTxParamSetupReq( MacCtx.pNvmCtx->Region, &txParamSetupReq ) != -1 )
                {
                    // Accept command
                    MacCtx.pNvmCtx->MacParams.UplinkDwellTime = txParamSetupReq.UplinkDwellTime;
                    MacCtx.pNvmCtx->MacParams.DownlinkDwellTime = txParamSetupReq.DownlinkDwellTime;
                    MacCtx.pNvmCtx->MacParams.MaxEirp = LoRaMacMaxEirpTable[txParamSetupReq.MaxEirp];
                    // Update the datarate in case of the new configuration limits it
                    getPhy.Attribute = PHY_MIN_TX_DR;
                    getPhy.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;
                    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
                    MacCtx.pNvmCtx->MacParams.ChannelsDatarate = MAX( MacCtx.pNvmCtx->MacParams.ChannelsDatarate, ( int8_t )phyParam.Value );

                    // Add command response
                    LoRaMacCommandsAddCmd( MOTE_MAC_TX_PARAM_SETUP_ANS, macCmdPayload, 0 );
                }
                break;
            }
            case SRV_MAC_REKEY_CONF:
            {
                uint8_t serverMinorVersion = payload[macIndex++];

                // Compare own LoRaWAN Version with server's
                if( MacCtx.pNvmCtx->Version.Fields.Minor == serverMinorVersion )
                {
                    // If they equal remove the sticky RekeyInd MAC-Command.
                    LoRaMacCommandsGetCmd( MOTE_MAC_REKEY_IND, &macCmd);
                    LoRaMacCommandsRemoveCmd( macCmd );
                }
                break;
            }
            case SRV_MAC_DL_CHANNEL_REQ:
            {
                DlChannelReqParams_t dlChannelReq;
                status = 0x03;

                dlChannelReq.ChannelId = payload[macIndex++];
                dlChannelReq.Rx1Frequency = ( uint32_t ) payload[macIndex++];
                dlChannelReq.Rx1Frequency |= ( uint32_t ) payload[macIndex++] << 8;
                dlChannelReq.Rx1Frequency |= ( uint32_t ) payload[macIndex++] << 16;
                dlChannelReq.Rx1Frequency *= 100;

                status = RegionDlChannelReq( MacCtx.pNvmCtx->Region, &dlChannelReq );
                macCmdPayload[0] = status;
                LoRaMacCommandsAddCmd( MOTE_MAC_DL_CHANNEL_ANS, macCmdPayload, 1 );
                // Setup indication to inform the application
                SetMlmeScheduleUplinkIndication( );
                break;
            }
            case SRV_MAC_ADR_PARAM_SETUP_REQ:
            {
                /* ADRParamSetupReq Payload:  ADRparam
                 * +----------------+---------------+
                 * | 7:4 Limit_exp  | 3:0 Delay_exp |
                 * +----------------+---------------+
                 */

                uint8_t delayExp = 0x0F & payload[macIndex];
                uint8_t limitExp = 0x0F & ( payload[macIndex] >> 4 );
                macIndex++;

                // ADR_ACK_ DELAY = 2^Delay_exp
                MacCtx.AdrAckDelay = 0x01 << delayExp;

                // ADR_ACK_LIMIT = 2^Limit_exp
                MacCtx.AdrAckLimit = 0x01 << limitExp;

                LoRaMacCommandsAddCmd( MOTE_MAC_ADR_PARAM_SETUP_ANS, macCmdPayload, 0 );
                break;
            }
            case SRV_MAC_FORCE_REJOIN_REQ:
            {
                /* ForceRejoinReq Payload:
                 * +--------------+------------------+-------+----------------+--------+
                 * | 13:11 Period | 10:8 Max_Retries | 7 RFU | 6:4 RejoinType | 3:0 DR |
                 * +--------------+------------------+-------+----------------+--------+
                 */

                // Parse payload
                uint8_t period = ( 0x38 & payload[macIndex] ) >> 3;
                MacCtx.pNvmCtx->ForceRejoinMaxRetries = 0x07 & payload[macIndex];
                macIndex++;
                MacCtx.pNvmCtx->ForceRejoinType = ( 0x70 & payload[macIndex] ) >> 4;
                MacCtx.pNvmCtx->MacParams.ChannelsDatarate = 0x0F & payload[macIndex];
                macIndex ++;

                // Calc delay between retransmissions: 32 seconds x 2^Period + Rand32
                uint32_t rejoinCycleInSec = 32 * ( 0x01 << period ) + randr( 0, 32 );

                uint32_t forceRejoinReqCycleTime = 0;
                ConvertRejoinCycleTime( rejoinCycleInSec, &forceRejoinReqCycleTime );
                TimerSetValue( &MacCtx.ForceRejoinReqCycleTimer, forceRejoinReqCycleTime );

                if( ( MacCtx.pNvmCtx->ForceRejoinType == 0 ) || ( MacCtx.pNvmCtx->ForceRejoinType == 1 ) )
                {
                    SendReJoinReq( REJOIN_REQ_0 );
                }
                else
                {
                    SendReJoinReq( REJOIN_REQ_2 );
                }
                TimerStart( &MacCtx.ForceRejoinReqCycleTimer );
                break;
            }
            case SRV_MAC_REJOIN_PARAM_REQ:
            {
                /* RejoinParamSetupReq Payload:
                 * +----------------+---------------+
                 * | 7:4 MaxTimeN   | 3:0 MaxCountN |
                 * +----------------+---------------+
                 */

                uint8_t timeOK = 0;
                uint8_t maxCountN = 0x0F & payload[macIndex];
                uint8_t maxTimeN = 0x0F & ( payload[macIndex] >> 4 );
                macIndex ++;

                // Calc delay between retransmissions: 2^(maxTimeN+10)
                MacCtx.pNvmCtx->Rejoin0CycleInSec = 0x01 << ( maxTimeN + 10 );

                // Calc number if uplinks without rejoin request: 2^(maxCountN+4)
                MacCtx.pNvmCtx->Rejoin0UplinksLimit = 0x01 << ( maxCountN + 4 );

                if( ConvertRejoinCycleTime( MacCtx.pNvmCtx->Rejoin0CycleInSec, &MacCtx.Rejoin0CycleTime ) == true )
                {
                    timeOK = 0x01;
                    TimerStop( &MacCtx.Rejoin0CycleTimer );
                    TimerSetValue( &MacCtx.Rejoin0CycleTimer, MacCtx.Rejoin0CycleTime );
                    TimerStart( &MacCtx.Rejoin0CycleTimer );
                }
                macCmdPayload[0] = timeOK;
                LoRaMacCommandsAddCmd( MOTE_MAC_REJOIN_PARAM_ANS, macCmdPayload, 1 );
                break;
             }
            case SRV_MAC_DEVICE_MODE_CONF:
            {
                // 1 byte payload which we do not handle.
                macIndex++;
                LoRaMacCommandsGetCmd( MOTE_MAC_DEVICE_MODE_IND, &macCmd );
                LoRaMacCommandsRemoveCmd( macCmd );
                break;
            }
            case SRV_MAC_DEVICE_TIME_ANS:
            {
                SysTime_t gpsEpochTime = { 0 };
                SysTime_t sysTime = { 0 };
                SysTime_t sysTimeCurrent = { 0 };

                gpsEpochTime.Seconds = ( uint32_t )payload[macIndex++];
                gpsEpochTime.Seconds |= ( uint32_t )payload[macIndex++] << 8;
                gpsEpochTime.Seconds |= ( uint32_t )payload[macIndex++] << 16;
                gpsEpochTime.Seconds |= ( uint32_t )payload[macIndex++] << 24;
                gpsEpochTime.SubSeconds = payload[macIndex++];

                // Convert the fractional second received in ms
                // round( pow( 0.5, 8.0 ) * 1000 ) = 3.90625
                gpsEpochTime.SubSeconds = ( int16_t )( ( ( int32_t )gpsEpochTime.SubSeconds * 1000 ) >> 8 );

                // Copy received GPS Epoch time into system time
                sysTime = gpsEpochTime;
                // Add Unix to Gps epcoh offset. The system time is based on Unix time.
                sysTime.Seconds += UNIX_GPS_EPOCH_OFFSET;

                // Compensate time difference between Tx Done time and now
                sysTimeCurrent = SysTimeGet( );
                sysTime = SysTimeAdd( sysTimeCurrent, SysTimeSub( sysTime, MacCtx.LastTxSysTime ) );

                // Apply the new system time.
                SysTimeSet( sysTime );
                LoRaMacClassBDeviceTimeAns( );
                MacCtx.McpsIndication.DeviceTimeAnsReceived = true;
                break;
            }
            case SRV_MAC_PING_SLOT_INFO_ANS:
            {
                // According to the specification, it is not allowed to process this answer in
                // a ping or multicast slot
                if( ( MacCtx.RxSlot != RX_SLOT_WIN_CLASS_B_PING_SLOT ) && ( MacCtx.RxSlot != RX_SLOT_WIN_CLASS_B_MULTICAST_SLOT ) )
                {
                    LoRaMacClassBPingSlotInfoAns( );
                }
                break;
            }
            case SRV_MAC_PING_SLOT_CHANNEL_REQ:
            {
                uint8_t status = 0x03;
                uint32_t frequency = 0;
                uint8_t datarate;

                frequency = ( uint32_t )payload[macIndex++];
                frequency |= ( uint32_t )payload[macIndex++] << 8;
                frequency |= ( uint32_t )payload[macIndex++] << 16;
                frequency *= 100;
                datarate = payload[macIndex++] & 0x0F;

                status = LoRaMacClassBPingSlotChannelReq( datarate, frequency );
                macCmdPayload[0] = status;
                LoRaMacCommandsAddCmd( MOTE_MAC_PING_SLOT_FREQ_ANS, macCmdPayload, 1 );
                break;
            }
            case SRV_MAC_BEACON_TIMING_ANS:
            {
                uint16_t beaconTimingDelay = 0;
                uint8_t beaconTimingChannel = 0;

                beaconTimingDelay = ( uint16_t )payload[macIndex++];
                beaconTimingDelay |= ( uint16_t )payload[macIndex++] << 8;
                beaconTimingChannel = payload[macIndex++];

                LoRaMacClassBBeaconTimingAns( beaconTimingDelay, beaconTimingChannel, RxDoneParams.LastRxDone );
                break;
            }
            case SRV_MAC_BEACON_FREQ_REQ:
                {
                    uint32_t frequency = 0;

                    frequency = ( uint32_t )payload[macIndex++];
                    frequency |= ( uint32_t )payload[macIndex++] << 8;
                    frequency |= ( uint32_t )payload[macIndex++] << 16;
                    frequency *= 100;

                    if( LoRaMacClassBBeaconFreqReq( frequency ) == true )
                    {
                        macCmdPayload[0] = 1;
                    }
                    else
                    {
                        macCmdPayload[0] = 0;
                    }
                    LoRaMacCommandsAddCmd( MOTE_MAC_BEACON_FREQ_ANS, macCmdPayload, 1 );
                }
                break;
            default:
                // Unknown command. ABORT MAC commands processing
                return;
        }
    }
}

LoRaMacStatus_t Send( LoRaMacHeader_t* macHdr, uint8_t fPort, void* fBuffer, uint16_t fBufferSize )
{
    LoRaMacFrameCtrl_t fCtrl;
    LoRaMacStatus_t status = LORAMAC_STATUS_PARAMETER_INVALID;
    int8_t datarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
    int8_t txPower = MacCtx.pNvmCtx->MacParams.ChannelsTxPower;
    uint32_t adrAckCounter = MacCtx.pNvmCtx->AdrAckCounter;
    CalcNextAdrParams_t adrNext;

    // Check if we are joined
    if( MacCtx.pNvmCtx->NetworkActivation == ACTIVATION_TYPE_NONE )
    {
        return LORAMAC_STATUS_NO_NETWORK_JOINED;
    }
    if( MacCtx.pNvmCtx->MaxDCycle == 0 )
    {
        MacCtx.pNvmCtx->AggregatedTimeOff = 0;
    }

    fCtrl.Value = 0;
    fCtrl.Bits.FOptsLen      = 0;
    fCtrl.Bits.Adr           = MacCtx.pNvmCtx->AdrCtrlOn;

    // Check class b
    if( MacCtx.pNvmCtx->DeviceClass == CLASS_B )
    {
        fCtrl.Bits.FPending      = 1;
    }
    else
    {
        fCtrl.Bits.FPending      = 0;
    }

    // Check server ack
    if( MacCtx.pNvmCtx->SrvAckRequested == true )
    {
        fCtrl.Bits.Ack = 1;
    }

    // ADR next request
    adrNext.Version = MacCtx.pNvmCtx->Version;
    adrNext.UpdateChanMask = true;
    adrNext.AdrEnabled = fCtrl.Bits.Adr;
    adrNext.AdrAckCounter = MacCtx.pNvmCtx->AdrAckCounter;
    adrNext.AdrAckLimit = MacCtx.AdrAckLimit;
    adrNext.AdrAckDelay = MacCtx.AdrAckDelay;
    adrNext.Datarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
    adrNext.TxPower = MacCtx.pNvmCtx->MacParams.ChannelsTxPower;
    adrNext.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;
    adrNext.Region = MacCtx.pNvmCtx->Region;

    fCtrl.Bits.AdrAckReq = LoRaMacAdrCalcNext( &adrNext, &MacCtx.pNvmCtx->MacParams.ChannelsDatarate,
                                               &MacCtx.pNvmCtx->MacParams.ChannelsTxPower, &adrAckCounter );

    // Prepare the frame
    status = PrepareFrame( macHdr, &fCtrl, fPort, fBuffer, fBufferSize );

    // Validate status
    if( ( status == LORAMAC_STATUS_OK ) || ( status == LORAMAC_STATUS_SKIPPED_APP_DATA ) )
    {
        // Schedule frame, do not allow delayed transmissions
        status = ScheduleTx( false );
    }

    // Post processing
    if( status != LORAMAC_STATUS_OK )
    {
        // Bad case - restore
        // Store local variables
        MacCtx.pNvmCtx->MacParams.ChannelsDatarate = datarate;
        MacCtx.pNvmCtx->MacParams.ChannelsTxPower = txPower;
    }
    else
    {
        // Good case
        MacCtx.pNvmCtx->SrvAckRequested = false;
        MacCtx.pNvmCtx->AdrAckCounter = adrAckCounter;
        // Remove all none sticky MAC commands
        if( LoRaMacCommandsRemoveNoneStickyCmds( ) != LORAMAC_COMMANDS_SUCCESS )
        {
            return LORAMAC_STATUS_MAC_COMMAD_ERROR;
        }
    }
    return status;
}

LoRaMacStatus_t SendReJoinReq( JoinReqIdentifier_t joinReqType )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;
    LoRaMacHeader_t macHdr;
    macHdr.Value = 0;
    bool allowDelayedTx = true;

    // Setup join/rejoin message
    switch( joinReqType )
    {
        case REJOIN_REQ_1:
        {
            MacCtx.pNvmCtx->IsRejoinRequestPending = true;

            MacCtx.TxMsg.Type = LORAMAC_MSG_TYPE_RE_JOIN_1;
            MacCtx.TxMsg.Message.ReJoin1.Buffer = MacCtx.PktBuffer;
            MacCtx.TxMsg.Message.ReJoin1.BufSize = LORAMAC_PHY_MAXPAYLOAD;

            macHdr.Bits.MType = FRAME_TYPE_REJOIN;
            MacCtx.TxMsg.Message.ReJoin1.MHDR.Value = macHdr.Value;

            MacCtx.TxMsg.Message.ReJoin1.ReJoinType = 1;

            memcpy1( MacCtx.TxMsg.Message.ReJoin1.JoinEUI, MacCtx.JoinEui, LORAMAC_JOIN_EUI_FIELD_SIZE );
            memcpy1( MacCtx.TxMsg.Message.ReJoin1.DevEUI, MacCtx.DevEui, LORAMAC_DEV_EUI_FIELD_SIZE );

            if( LORAMAC_FCNT_HANDLER_SUCCESS != LoRaMacGetRJcount( RJ_COUNT_1, &MacCtx.TxMsg.Message.ReJoin1.RJcount1 ) )
            {
                return LORAMAC_STATUS_FCNT_HANDLER_ERROR;
            }

            break;
        }
        case REJOIN_REQ_0:
        case REJOIN_REQ_2:
        {
            if( joinReqType == REJOIN_REQ_0 )
            {
                MacCtx.TxMsg.Message.ReJoin0or2.ReJoinType = 0;
            }
            else
            {
                MacCtx.TxMsg.Message.ReJoin0or2.ReJoinType = 2;
            }

            MacCtx.pNvmCtx->IsRejoinRequestPending = true;

            MacCtx.TxMsg.Type = LORAMAC_MSG_TYPE_RE_JOIN_0_2;
            MacCtx.TxMsg.Message.ReJoin0or2.Buffer = MacCtx.PktBuffer;
            MacCtx.TxMsg.Message.ReJoin0or2.BufSize = LORAMAC_PHY_MAXPAYLOAD;

            macHdr.Bits.MType = FRAME_TYPE_REJOIN;
            MacCtx.TxMsg.Message.ReJoin0or2.MHDR.Value = macHdr.Value;

            MacCtx.TxMsg.Message.ReJoin0or2.NetID[0] = MacCtx.pNvmCtx->NetID & 0xFF;
            MacCtx.TxMsg.Message.ReJoin0or2.NetID[1] = ( MacCtx.pNvmCtx->NetID >> 8 ) & 0xFF;
            MacCtx.TxMsg.Message.ReJoin0or2.NetID[2] = ( MacCtx.pNvmCtx->NetID >> 16 ) & 0xFF;

            memcpy1( MacCtx.TxMsg.Message.ReJoin0or2.DevEUI, MacCtx.DevEui, LORAMAC_DEV_EUI_FIELD_SIZE );

            if( LORAMAC_FCNT_HANDLER_SUCCESS != LoRaMacGetRJcount( RJ_COUNT_0, &MacCtx.TxMsg.Message.ReJoin0or2.RJcount0 ) )
            {
                return LORAMAC_STATUS_FCNT_HANDLER_ERROR;
            }

            break;
        }
        case JOIN_REQ:
        {
            MacCtx.TxMsg.Type = LORAMAC_MSG_TYPE_JOIN_REQUEST;
            MacCtx.TxMsg.Message.JoinReq.Buffer = MacCtx.PktBuffer;
            MacCtx.TxMsg.Message.JoinReq.BufSize = LORAMAC_PHY_MAXPAYLOAD;

            macHdr.Bits.MType = FRAME_TYPE_JOIN_REQ;
            MacCtx.TxMsg.Message.JoinReq.MHDR.Value = macHdr.Value;

            memcpy1( MacCtx.TxMsg.Message.JoinReq.JoinEUI, MacCtx.JoinEui, LORAMAC_JOIN_EUI_FIELD_SIZE );
            memcpy1( MacCtx.TxMsg.Message.JoinReq.DevEUI, MacCtx.DevEui, LORAMAC_DEV_EUI_FIELD_SIZE );
    
            allowDelayedTx = false;

            break;
        }
        default:
            status = LORAMAC_STATUS_SERVICE_UNKNOWN;
            break;
    }

    // Schedule frame
    status = ScheduleTx( allowDelayedTx );
    return status;
}

static LoRaMacStatus_t ScheduleTx( bool allowDelayedTx )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_PARAMETER_INVALID;
    TimerTime_t dutyCycleTimeOff = 0;
    NextChanParams_t nextChan;
    size_t macCmdsSize = 0;

    // Update back-off
    CalculateBackOff( MacCtx.pNvmCtx->LastTxChannel );

    nextChan.AggrTimeOff		= MacCtx.pNvmCtx->AggregatedTimeOff;
    nextChan.Datarate			= MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
    nextChan.DutyCycleEnabled	= MacCtx.pNvmCtx->DutyCycleOn;

    if( MacCtx.pNvmCtx->NetworkActivation == ACTIVATION_TYPE_NONE )
    {
        nextChan.Joined = false;
    }
    else
    {
        nextChan.Joined = true;
    }
    nextChan.LastAggrTx = MacCtx.pNvmCtx->LastTxDoneTime;

    // Select channel
    status = RegionNextChannel( MacCtx.pNvmCtx->Region, &nextChan, &MacCtx.Channel, &dutyCycleTimeOff, &MacCtx.pNvmCtx->AggregatedTimeOff );

    if( status != LORAMAC_STATUS_OK )
    {
        if( ( status == LORAMAC_STATUS_DUTYCYCLE_RESTRICTED ) &&
            ( allowDelayedTx == true ) )
        {
            // Allow delayed transmissions. We have to allow it in case
            // the MAC must retransmit a frame with the frame repetitions
            if( dutyCycleTimeOff != 0 )
            {// Send later - prepare timer
                MacCtx.MacState |= LORAMAC_TX_DELAYED;
                TimerSetValue( &MacCtx.TxDelayedTimer, dutyCycleTimeOff );
                TimerStart( &MacCtx.TxDelayedTimer );
            }
            return LORAMAC_STATUS_OK;
        }
        else
        {// State where the MAC cannot send a frame
            return status;
        }
    }

    // Compute Rx1 windows parameters
    RegionComputeRxWindowParameters( MacCtx.pNvmCtx->Region,
                                     RegionApplyDrOffset( MacCtx.pNvmCtx->Region, MacCtx.pNvmCtx->MacParams.DownlinkDwellTime, MacCtx.pNvmCtx->MacParams.ChannelsDatarate, MacCtx.pNvmCtx->MacParams.Rx1DrOffset ),
                                     MacCtx.pNvmCtx->MacParams.MinRxSymbols,
                                     MacCtx.pNvmCtx->MacParams.SystemMaxRxError,
                                     &MacCtx.RxWindow1Config );
    // Compute Rx2 windows parameters
    RegionComputeRxWindowParameters( MacCtx.pNvmCtx->Region,
                                     MacCtx.pNvmCtx->MacParams.Rx2Channel.Datarate,
                                     MacCtx.pNvmCtx->MacParams.MinRxSymbols,
                                     MacCtx.pNvmCtx->MacParams.SystemMaxRxError,
                                     &MacCtx.RxWindow2Config );

    if( MacCtx.pNvmCtx->NetworkActivation == ACTIVATION_TYPE_NONE )
    {
        MacCtx.RxWindow1Delay = MacCtx.pNvmCtx->MacParams.JoinAcceptDelay1 + MacCtx.RxWindow1Config.WindowOffset;
        MacCtx.RxWindow2Delay = MacCtx.pNvmCtx->MacParams.JoinAcceptDelay2 + MacCtx.RxWindow2Config.WindowOffset;
    }
    else
    {
        if( LoRaMacCommandsGetSizeSerializedCmds( &macCmdsSize ) != LORAMAC_COMMANDS_SUCCESS )
        {
            return LORAMAC_STATUS_MAC_COMMAD_ERROR;
        }

        if( ValidatePayloadLength( MacCtx.AppDataSize, MacCtx.pNvmCtx->MacParams.ChannelsDatarate, macCmdsSize ) == false )
        {
            return LORAMAC_STATUS_LENGTH_ERROR;
        }
        MacCtx.RxWindow1Delay = MacCtx.pNvmCtx->MacParams.ReceiveDelay1 + MacCtx.RxWindow1Config.WindowOffset;
        MacCtx.RxWindow2Delay = MacCtx.pNvmCtx->MacParams.ReceiveDelay2 + MacCtx.RxWindow2Config.WindowOffset;
    }

    // Secure frame
    LoRaMacStatus_t retval = SecureFrame( MacCtx.pNvmCtx->MacParams.ChannelsDatarate, MacCtx.Channel );
    if( retval != LORAMAC_STATUS_OK )
    {
        return retval;
    }

    // Try to send now
    return SendFrameOnChannel( MacCtx.Channel );
}

static LoRaMacStatus_t SecureFrame( uint8_t txDr, uint8_t txCh )
{
    LoRaMacCryptoStatus_t macCryptoStatus = LORAMAC_CRYPTO_ERROR;
    uint32_t fCntUp = 0;

    switch( MacCtx.TxMsg.Type )
    {
        case LORAMAC_MSG_TYPE_JOIN_REQUEST:
            macCryptoStatus = LoRaMacCryptoPrepareJoinRequest( &MacCtx.TxMsg.Message.JoinReq );
            if( LORAMAC_CRYPTO_SUCCESS != macCryptoStatus )
            {
                return LORAMAC_STATUS_CRYPTO_ERROR;
            }
            MacCtx.PktBufferLen = MacCtx.TxMsg.Message.JoinReq.BufSize;
            break;
        case LORAMAC_MSG_TYPE_RE_JOIN_1:
            macCryptoStatus = LoRaMacCryptoPrepareReJoinType1( &MacCtx.TxMsg.Message.ReJoin1 );
            if( LORAMAC_CRYPTO_SUCCESS != macCryptoStatus )
            {
                return LORAMAC_STATUS_CRYPTO_ERROR;
            }
            MacCtx.PktBufferLen = MacCtx.TxMsg.Message.ReJoin1.BufSize;
            break;
        case LORAMAC_MSG_TYPE_RE_JOIN_0_2:
            macCryptoStatus = LoRaMacCryptoPrepareReJoinType0or2( &MacCtx.TxMsg.Message.ReJoin0or2 );
            if( LORAMAC_CRYPTO_SUCCESS != macCryptoStatus )
            {
                return LORAMAC_STATUS_CRYPTO_ERROR;
            }
            MacCtx.PktBufferLen = MacCtx.TxMsg.Message.ReJoin0or2.BufSize;
            break;
        case LORAMAC_MSG_TYPE_DATA:

            if( LORAMAC_FCNT_HANDLER_SUCCESS != LoRaMacGetFCntUp( &fCntUp ) )
            {
                return LORAMAC_STATUS_FCNT_HANDLER_ERROR;
            }

            macCryptoStatus = LoRaMacCryptoSecureMessage( fCntUp, txDr, txCh, &MacCtx.TxMsg.Message.Data );
            if( LORAMAC_CRYPTO_SUCCESS != macCryptoStatus )
            {
                return LORAMAC_STATUS_CRYPTO_ERROR;
            }
            MacCtx.PktBufferLen = MacCtx.TxMsg.Message.Data.BufSize;
            break;
        case LORAMAC_MSG_TYPE_JOIN_ACCEPT:
        case LORAMAC_MSG_TYPE_UNDEF:
        default:
            return LORAMAC_STATUS_PARAMETER_INVALID;
    }
    return LORAMAC_STATUS_OK;
}

static void CalculateBackOff( uint8_t channel )
{
    CalcBackOffParams_t calcBackOff;

    if( MacCtx.pNvmCtx->NetworkActivation == ACTIVATION_TYPE_NONE )
    {
        calcBackOff.Joined = false;
    }
    else
    {
        calcBackOff.Joined = true;
    }
    calcBackOff.DutyCycleEnabled	= MacCtx.pNvmCtx->DutyCycleOn;
    calcBackOff.Channel				= channel;
    calcBackOff.ElapsedTime			= SysTimeSub( SysTimeGetMcuTime( ), MacCtx.pNvmCtx->InitializationTime );
    calcBackOff.TxTimeOnAir			= MacCtx.TxTimeOnAir;
    calcBackOff.LastTxIsJoinRequest = false;

    if( ( MacCtx.MacFlags.Bits.MlmeReq == 1 ) && ( LoRaMacConfirmQueueIsCmdActive( MLME_JOIN ) == true ) )
    {
        calcBackOff.LastTxIsJoinRequest = true;
    }

    // Update regional back-off
    RegionCalcBackOff( MacCtx.pNvmCtx->Region, &calcBackOff );

    // Update aggregated time-off. This must be an assignment and no incremental
    // update as we do only calculate the time-off based on the last transmission
    MacCtx.pNvmCtx->AggregatedTimeOff = ( MacCtx.TxTimeOnAir * MacCtx.pNvmCtx->AggregatedDCycle - MacCtx.TxTimeOnAir );
	#if 0
		NRF_LOG_INFO("AggregatedTimeOff: %u", MacCtx.pNvmCtx->AggregatedTimeOff);
	#endif
}

static void RemoveMacCommands( LoRaMacRxSlot_t rxSlot, LoRaMacFrameCtrl_t fCtrl, Mcps_t request )
{
    if( rxSlot == RX_SLOT_WIN_1 || rxSlot == RX_SLOT_WIN_2  )
    {
        // Remove all sticky MAC commands answers since we can assume
        // that they have been received by the server.
        if( request == MCPS_CONFIRMED )
        {
            if( fCtrl.Bits.Ack == 1 )
            {  // For confirmed uplinks only if we have received an ACK.
                LoRaMacCommandsRemoveStickyAnsCmds( );
            }
        }
        else
        {
            LoRaMacCommandsRemoveStickyAnsCmds( );
        }
    }
}


static void ResetMacParameters( void )
{
    MacCtx.pNvmCtx->NetworkActivation = ACTIVATION_TYPE_NONE;

    // ADR counter
    MacCtx.pNvmCtx->AdrAckCounter = 0;

    // Initialize the uplink and downlink counters default value
    LoRaMacResetFCnts( );

    MacCtx.ChannelsNbTransCounter = 0;
    MacCtx.AckTimeoutRetry = false;

    MacCtx.pNvmCtx->MaxDCycle = 0;
    MacCtx.pNvmCtx->AggregatedDCycle = 1;

    MacCtx.pNvmCtx->MacParams.ChannelsTxPower = MacCtx.pNvmCtx->MacParamsDefaults.ChannelsTxPower;
    MacCtx.pNvmCtx->MacParams.ChannelsDatarate = MacCtx.pNvmCtx->MacParamsDefaults.ChannelsDatarate;
    MacCtx.pNvmCtx->MacParams.Rx1DrOffset = MacCtx.pNvmCtx->MacParamsDefaults.Rx1DrOffset;
    MacCtx.pNvmCtx->MacParams.Rx2Channel = MacCtx.pNvmCtx->MacParamsDefaults.Rx2Channel;
    MacCtx.pNvmCtx->MacParams.RxCChannel = MacCtx.pNvmCtx->MacParamsDefaults.RxCChannel;
    MacCtx.pNvmCtx->MacParams.UplinkDwellTime = MacCtx.pNvmCtx->MacParamsDefaults.UplinkDwellTime;
    MacCtx.pNvmCtx->MacParams.DownlinkDwellTime = MacCtx.pNvmCtx->MacParamsDefaults.DownlinkDwellTime;
    MacCtx.pNvmCtx->MacParams.MaxEirp = MacCtx.pNvmCtx->MacParamsDefaults.MaxEirp;
    MacCtx.pNvmCtx->MacParams.AntennaGain = MacCtx.pNvmCtx->MacParamsDefaults.AntennaGain;

    MacCtx.NodeAckRequested = false;
    MacCtx.pNvmCtx->SrvAckRequested = false;

    // Reset to application defaults
    InitDefaultsParams_t params;
    params.Type = INIT_TYPE_RESTORE_DEFAULT_CHANNELS;
    params.NvmCtx = NULL;
    RegionInitDefaults( MacCtx.pNvmCtx->Region, &params );

    // Initialize channel index.
    MacCtx.Channel = 0;
    MacCtx.pNvmCtx->LastTxChannel = MacCtx.Channel;
}

static bool IsReJoin0Required( )
{

    if( ( MacCtx.pNvmCtx->Rejoin0UplinksLimit == MacCtx.pNvmCtx->Rejoin0UplinksCounter ) && ( MacCtx.pNvmCtx->Version.Fields.Minor == 1 ) && ( MacCtx.pNvmCtx->Rejoin0UplinksLimit != 0 ) )
    {
        MacCtx.pNvmCtx->Rejoin0UplinksCounter = 0;
        return true;
    }
    return false;
}

/*!
 * \brief Initializes and opens the reception window
 *
 * \param [IN] rxTimer  Window timer to be topped.
 * \param [IN] rxConfig Window parameters to be setup
 */
static void RxWindowSetup( TimerEvent_t* rxTimer, RxConfigParams_t* rxConfig )
{
    TimerStop( rxTimer );

    #if LORAMAC_LOG_ENABLED
		uint32_t Tdiff = RtcTick2Ms(RtcCalcAbsDiff(RtcGetTimerValue(), TxDoneParams.timestamp));
		NRF_LOG_INFO("Rx%u tDelay=%u, Ch: %u, Freq: %u, Datarate: %i, Timeout: %u",
			1 + (uint32_t)rxConfig->RxSlot, 
			Tdiff,
			rxConfig->Channel,
			rxConfig->Frequency,
			rxConfig->Datarate,
			rxConfig->WindowTimeout);
    #endif

    // Ensure the radio is Idle
    Radio.Standby( );

    if( RegionRxConfig( MacCtx.pNvmCtx->Region, rxConfig, ( int8_t* )&MacCtx.McpsIndication.RxDatarate ) == true )
    {
        Radio.Rx( MacCtx.pNvmCtx->MacParams.MaxRxWindow );
        MacCtx.RxSlot = rxConfig->RxSlot;
    }
}

static void OpenContinuousRxCWindow( void )
{
    MacCtx.RxWindowCConfig.RxSlot = RX_SLOT_WIN_CLASS_C;
    // Setup continuous listening
    MacCtx.RxWindowCConfig.RxContinuous = true;

    // At this point the Radio should be idle.
    // Thus, there is no need to set the radio in standby mode.
    if( RegionRxConfig( MacCtx.pNvmCtx->Region, &MacCtx.RxWindowCConfig, ( int8_t* )&MacCtx.McpsIndication.RxDatarate ) == true )
    {
        Radio.Rx( 0 ); // Continuous mode
        MacCtx.RxSlot = MacCtx.RxWindowCConfig.RxSlot;
    }
}

LoRaMacStatus_t PrepareFrame( LoRaMacHeader_t* macHdr, LoRaMacFrameCtrl_t* fCtrl, uint8_t fPort, void* fBuffer, uint16_t fBufferSize )
{
    MacCtx.PktBufferLen = 0;
    MacCtx.NodeAckRequested = false;
    uint32_t fCntUp = 0;
    size_t macCmdsSize = 0;
    uint8_t availableSize = 0;

    if( fBuffer == NULL )
    {
        fBufferSize = 0;
    }

    memcpy1( MacCtx.AppData, ( uint8_t* ) fBuffer, fBufferSize );
    MacCtx.AppDataSize = fBufferSize;
    MacCtx.PktBuffer[0] = macHdr->Value;

    switch( macHdr->Bits.MType )
    {
        case FRAME_TYPE_DATA_CONFIRMED_UP:
            MacCtx.NodeAckRequested = true;
            // Intentional fall through
        case FRAME_TYPE_DATA_UNCONFIRMED_UP:
            MacCtx.TxMsg.Type = LORAMAC_MSG_TYPE_DATA;
            MacCtx.TxMsg.Message.Data.Buffer = MacCtx.PktBuffer;
            MacCtx.TxMsg.Message.Data.BufSize = LORAMAC_PHY_MAXPAYLOAD;
            MacCtx.TxMsg.Message.Data.MHDR.Value = macHdr->Value;
            MacCtx.TxMsg.Message.Data.FPort = fPort;
            MacCtx.TxMsg.Message.Data.FHDR.DevAddr = MacCtx.pNvmCtx->DevAddr;
            MacCtx.TxMsg.Message.Data.FHDR.FCtrl.Value = fCtrl->Value;
            MacCtx.TxMsg.Message.Data.FRMPayloadSize = MacCtx.AppDataSize;
            MacCtx.TxMsg.Message.Data.FRMPayload = MacCtx.AppData;

            if( LORAMAC_FCNT_HANDLER_SUCCESS != LoRaMacGetFCntUp( &fCntUp ) )
            {
                return LORAMAC_STATUS_FCNT_HANDLER_ERROR;
            }
            MacCtx.TxMsg.Message.Data.FHDR.FCnt = ( uint16_t ) fCntUp;

            // Reset confirm parameters
            MacCtx.McpsConfirm.NbTrans = 0;
            MacCtx.McpsConfirm.AckReceived = false;
            MacCtx.McpsConfirm.UpLinkCounter = fCntUp;

            // Handle the MAC commands if there are any available
            if( LoRaMacCommandsGetSizeSerializedCmds( &macCmdsSize ) != LORAMAC_COMMANDS_SUCCESS )
            {
                return LORAMAC_STATUS_MAC_COMMAD_ERROR;
            }

            if( macCmdsSize > 0 )
            {
                availableSize = GetMaxAppPayloadWithoutFOptsLength( MacCtx.pNvmCtx->MacParams.ChannelsDatarate );

                // There is application payload available and the MAC commands fit into FOpts field.
                if( ( MacCtx.AppDataSize > 0 ) && ( macCmdsSize <= LORA_MAC_COMMAND_MAX_FOPTS_LENGTH ) )
                {
                    if( LoRaMacCommandsSerializeCmds( LORA_MAC_COMMAND_MAX_FOPTS_LENGTH, &macCmdsSize, MacCtx.TxMsg.Message.Data.FHDR.FOpts ) != LORAMAC_COMMANDS_SUCCESS )
                    {
                        return LORAMAC_STATUS_MAC_COMMAD_ERROR;
                    }
                    fCtrl->Bits.FOptsLen = macCmdsSize;
                    // Update FCtrl field with new value of FOptionsLength
                    MacCtx.TxMsg.Message.Data.FHDR.FCtrl.Value = fCtrl->Value;
                }
                // There is application payload available but the MAC commands does NOT fit into FOpts field.
                else if( ( MacCtx.AppDataSize > 0 ) && ( macCmdsSize > LORA_MAC_COMMAND_MAX_FOPTS_LENGTH ) )
                {

                    if( LoRaMacCommandsSerializeCmds( availableSize, &macCmdsSize, MacCtx.pNvmCtx->MacCommandsBuffer ) != LORAMAC_COMMANDS_SUCCESS )
                    {
                        return LORAMAC_STATUS_MAC_COMMAD_ERROR;
                    }
                    return LORAMAC_STATUS_SKIPPED_APP_DATA;
                }
                // No application payload available therefore add all mac commands to the FRMPayload.
                else
                {
                    if( LoRaMacCommandsSerializeCmds( availableSize, &macCmdsSize, MacCtx.pNvmCtx->MacCommandsBuffer ) != LORAMAC_COMMANDS_SUCCESS )
                    {
                        return LORAMAC_STATUS_MAC_COMMAD_ERROR;
                    }
                    // Force FPort to be zero
                    MacCtx.TxMsg.Message.Data.FPort = 0;

                    MacCtx.TxMsg.Message.Data.FRMPayload = MacCtx.pNvmCtx->MacCommandsBuffer;
                    MacCtx.TxMsg.Message.Data.FRMPayloadSize = macCmdsSize;
                }
            }

            break;
        case FRAME_TYPE_PROPRIETARY:
            if( ( fBuffer != NULL ) && ( MacCtx.AppDataSize > 0 ) )
            {
                memcpy1( MacCtx.PktBuffer + LORAMAC_MHDR_FIELD_SIZE, ( uint8_t* ) fBuffer, MacCtx.AppDataSize );
                MacCtx.PktBufferLen = LORAMAC_MHDR_FIELD_SIZE + MacCtx.AppDataSize;
            }
            break;
        default:
            return LORAMAC_STATUS_SERVICE_UNKNOWN;
    }

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t SendFrameOnChannel( uint8_t channel )
{
    TxConfigParams_t txConfig;
    int8_t txPower = 0;

    txConfig.Channel = channel;
    txConfig.Datarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
    txConfig.TxPower = MacCtx.pNvmCtx->MacParams.ChannelsTxPower;
    txConfig.MaxEirp = MacCtx.pNvmCtx->MacParams.MaxEirp;
    txConfig.AntennaGain = MacCtx.pNvmCtx->MacParams.AntennaGain;
    txConfig.PktLen = MacCtx.PktBufferLen;


    if( LoRaMacClassBIsBeaconExpected( ) == true )
    {
        return LORAMAC_STATUS_BUSY_BEACON_RESERVED_TIME;
    }

    if( MacCtx.pNvmCtx->DeviceClass == CLASS_B )
    {
        if( LoRaMacClassBIsPingExpected( ) == true )
        {
            return LORAMAC_STATUS_BUSY_PING_SLOT_WINDOW_TIME;
        }
        else if( LoRaMacClassBIsMulticastExpected( ) == true )
        {
            return LORAMAC_STATUS_BUSY_PING_SLOT_WINDOW_TIME;
        }
        else
        {
            LoRaMacClassBStopRxSlots( );
        }
    }
    RegionTxConfig( MacCtx.pNvmCtx->Region, &txConfig, &txPower, &MacCtx.TxTimeOnAir );

    MacCtx.McpsConfirm.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    MacCtx.McpsConfirm.Datarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
    MacCtx.McpsConfirm.TxPower = txPower;
    MacCtx.McpsConfirm.Channel = channel;

    // Store the time on air
    MacCtx.McpsConfirm.TxTimeOnAir = MacCtx.TxTimeOnAir;
    MacCtx.MlmeConfirm.TxTimeOnAir = MacCtx.TxTimeOnAir;

    if( LoRaMacClassBIsBeaconModeActive( ) == true )
    {
        // Currently, the Time-On-Air can only be computed when the radio is configured with
        // the TX configuration
        TimerTime_t collisionTime = LoRaMacClassBIsUplinkCollision( MacCtx.TxTimeOnAir );

        if( collisionTime > 0 )
        {
            return LORAMAC_STATUS_BUSY_UPLINK_COLLISION;
        }
    }

    LoRaMacClassBHaltBeaconing( );

    MacCtx.MacState |= LORAMAC_TX_RUNNING;

    #if LORAMAC_LOG_ENABLED
		NRF_LOG_INFO("Tx Ch: %u, Datarate: %i, Pwr: %i", RegionFreq(MacCtx.pNvmCtx->Region, channel), txConfig.Datarate, MacCtx.McpsConfirm.TxPower);
    #endif

    // Send now
    Radio.Send( MacCtx.PktBuffer, MacCtx.PktBufferLen );

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t SetTxContinuousWave( uint16_t timeout )
{
    ContinuousWaveParams_t continuousWave;

    continuousWave.Channel = MacCtx.Channel;
    continuousWave.Datarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
    continuousWave.TxPower = MacCtx.pNvmCtx->MacParams.ChannelsTxPower;
    continuousWave.MaxEirp = MacCtx.pNvmCtx->MacParams.MaxEirp;
    continuousWave.AntennaGain = MacCtx.pNvmCtx->MacParams.AntennaGain;
    continuousWave.Timeout = timeout;

    RegionSetContinuousWave( MacCtx.pNvmCtx->Region, &continuousWave );

    MacCtx.MacState |= LORAMAC_TX_RUNNING;

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t SetTxContinuousWave1( uint16_t timeout, uint32_t frequency, uint8_t power )
{
    Radio.SetTxContinuousWave( frequency, power, timeout );

    MacCtx.MacState |= LORAMAC_TX_RUNNING;

    return LORAMAC_STATUS_OK;
}

LoRaMacCtxs_t* GetCtxs( void )
{
    Contexts.MacNvmCtx = &NvmMacCtx;
    Contexts.MacNvmCtxSize = sizeof( NvmMacCtx );
    Contexts.CryptoNvmCtx = LoRaMacCryptoGetNvmCtx( &Contexts.CryptoNvmCtxSize );
    GetNvmCtxParams_t params ={ 0 };
    Contexts.RegionNvmCtx = RegionGetNvmCtx( MacCtx.pNvmCtx->Region, &params );
    Contexts.RegionNvmCtxSize = params.nvmCtxSize;
    Contexts.SecureElementNvmCtx = SecureElementGetNvmCtx( &Contexts.SecureElementNvmCtxSize );
    Contexts.CommandsNvmCtx = LoRaMacCommandsGetNvmCtx( &Contexts.CommandsNvmCtxSize );
    Contexts.ClassBNvmCtx = LoRaMacClassBGetNvmCtx( &Contexts.ClassBNvmCtxSize );
    Contexts.ConfirmQueueNvmCtx = LoRaMacConfirmQueueGetNvmCtx( &Contexts.ConfirmQueueNvmCtxSize );
    Contexts.FCntHandlerNvmCtx = LoRaMacFCntHandlerGetNvmCtx( &Contexts.FCntHandlerNvmCtxSize );
    return &Contexts;
}

LoRaMacStatus_t RestoreCtxs( LoRaMacCtxs_t* contexts )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;
    MacCommand_t* macCmd = NULL;
    uint8_t cmdPayload = 0;

    if( MacCtx.MacState != LORAMAC_STOPPED )
    {
        return LORAMAC_STATUS_BUSY;
    }
    if( contexts == NULL )
    {
        status = LORAMAC_STATUS_PARAMETER_INVALID;
    }

    if( contexts->MacNvmCtx != NULL )
    {
        memcpy1( ( uint8_t* ) &NvmMacCtx, ( uint8_t* ) contexts->MacNvmCtx, contexts->MacNvmCtxSize );
    }
    else
    {
        status = LORAMAC_STATUS_PARAMETER_INVALID;
    }

    InitDefaultsParams_t params;
    params.Type = INIT_TYPE_RESTORE_CTX;
    params.NvmCtx = contexts->RegionNvmCtx;
    RegionInitDefaults( MacCtx.pNvmCtx->Region, &params );

    if( SecureElementRestoreNvmCtx( contexts->SecureElementNvmCtx ) != SECURE_ELEMENT_SUCCESS )
    {
        status = LORAMAC_STATUS_CRYPTO_ERROR;
    }

    if( LoRaMacCryptoRestoreNvmCtx( contexts->CryptoNvmCtx ) != LORAMAC_CRYPTO_SUCCESS )
    {
        status = LORAMAC_STATUS_CRYPTO_ERROR;
    }

    if( LoRaMacFCntHandlerRestoreNvmCtx( contexts->FCntHandlerNvmCtx ) != LORAMAC_FCNT_HANDLER_SUCCESS )
    {
        status = LORAMAC_STATUS_FCNT_HANDLER_ERROR;
    }

    if( LoRaMacCommandsRestoreNvmCtx( contexts->CommandsNvmCtx ) != LORAMAC_COMMANDS_SUCCESS )
    {
        status = LORAMAC_STATUS_MAC_COMMAD_ERROR;
    }

    if( LoRaMacClassBRestoreNvmCtx( contexts->ClassBNvmCtx ) != true )
    {
        status = LORAMAC_STATUS_CLASS_B_ERROR;
    }

    if( LoRaMacConfirmQueueRestoreNvmCtx( contexts->ConfirmQueueNvmCtx ) != true )
    {
        status = LORAMAC_STATUS_CONFIRM_QUEUE_ERROR;
    }

    if( status != LORAMAC_STATUS_OK)
    {
        if( LoRaMacCommandsGetCmd( MOTE_MAC_RESET_IND, &macCmd ) != LORAMAC_COMMANDS_SUCCESS )
        {
            cmdPayload = 0x0F & MacCtx.pNvmCtx->Version.Fields.Minor;
            LoRaMacCommandsAddCmd( MOTE_MAC_RESET_IND, &cmdPayload, 1 );
        }
    }
    return status;
}

LoRaMacStatus_t DetermineFrameType( LoRaMacMessageData_t* macMsg, FType_t* fType )
{
    if( ( macMsg == NULL ) || ( fType == NULL ) )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    /* The LoRaWAN specification allows several possible configurations how data up/down frames are built up.
     * In sake of clearness the following naming is applied. Please keep in mind that this is
     * implementation specific since there is no definition in the LoRaWAN specification included.
     *
     * X -> Field is available
     * - -> Field is not available
     *
     * +-------+  +----------+------+-------+--------------+
     * | FType |  | FOptsLen | Fopt | FPort |  FRMPayload  |
     * +-------+  +----------+------+-------+--------------+
     * |   A   |  |    > 0   |   X  |  > 0  |       X      |
     * +-------+  +----------+------+-------+--------------+
     * |   B   |  |   >= 0   |  X/- |   -   |       -      |
     * +-------+  +----------+------+-------+--------------+
     * |   C   |  |    = 0   |   -  |  = 0  | MAC commands |
     * +-------+  +----------+------+-------+--------------+
     * |   D   |  |    = 0   |   -  |  > 0  |       X      |
     * +-------+  +----------+------+-------+--------------+
     */

    if( ( macMsg->FHDR.FCtrl.Bits.FOptsLen > 0 ) && ( macMsg->FPort > 0 ) )
    {
        *fType = FRAME_TYPE_A;
    }
    else if( macMsg->FRMPayloadSize == 0 )
    {
        *fType = FRAME_TYPE_B;
    }
    else if( ( macMsg->FHDR.FCtrl.Bits.FOptsLen == 0 ) && ( macMsg->FPort == 0 ) )
    {
        *fType = FRAME_TYPE_C;
    }
    else if( ( macMsg->FHDR.FCtrl.Bits.FOptsLen == 0 ) && ( macMsg->FPort > 0 ) )
    {
        *fType = FRAME_TYPE_D;
    }
    else
    {
        // Should never happen.
        return LORAMAC_STATUS_ERROR;
    }

    return LORAMAC_STATUS_OK;
}

static bool CheckRetransUnconfirmedUplink( void )
{
    // Unconfirmed uplink, when all retransmissions are done.
    if( MacCtx.ChannelsNbTransCounter >=
        MacCtx.pNvmCtx->MacParams.ChannelsNbTrans )
    {
        return true;
    }
    else if( MacCtx.MacFlags.Bits.McpsInd == 1 )
    {
        // For Class A stop in each case
        if( MacCtx.pNvmCtx->DeviceClass == CLASS_A )
        {
            return true;
        }
        else
        {// For Class B & C stop only if the frame was received in RX1 window
            if( MacCtx.RxSlot == RX_SLOT_WIN_1 )
            {
                return true;
            }
        }
    }
    return false;
}

static bool CheckRetransConfirmedUplink( void )
{
    // Confirmed uplink, when all retransmissions ( tries to get a ack ) are done.
    if( MacCtx.ChannelsNbTransCounter >=
        MacCtx.pNvmCtx->MacParams.ChannelsNbTrans )
    {
        return true;
    }
    else if( MacCtx.MacFlags.Bits.McpsInd == 1 )
    {
        if( MacCtx.McpsConfirm.AckReceived == true )
        {
            return true;
        }
    }
    return false;
}


static bool StopRetransmission( void )
{
    // Increase the current value by 1
    uint32_t fCntUp = 0;
    if( LORAMAC_FCNT_HANDLER_SUCCESS != LoRaMacGetFCntUp( &fCntUp ) )
    {
        return false;
    }
    if( LORAMAC_FCNT_HANDLER_SUCCESS != LoRaMacSetFCntUp( ( fCntUp ) ) )
    {
        return false;
    }

    // Increase Rejoin Uplinks counter
    if( MacCtx.pNvmCtx->Rejoin0UplinksLimit != 0 )
    {
        MacCtx.pNvmCtx->Rejoin0UplinksCounter++;
    }

    if( MacCtx.pNvmCtx->Version.Fields.Minor >= 1 )
    {
        MacCommand_t* macCmd;
        if( LoRaMacCommandsGetCmd( MOTE_MAC_REKEY_IND, &macCmd ) == LORAMAC_COMMANDS_SUCCESS )
        {
            // Increase the Rekey Uplinks counter
            MacCtx.pNvmCtx->RekeyIndUplinksCounter++;

            /*
             * If the device has not received a RekeyConf within
             * the first ADR_ACK_LIMIT uplinks it SHALL revert to the Join state.
             */
            if( MacCtx.pNvmCtx->RekeyIndUplinksCounter == MacCtx.AdrAckLimit )
            {
                MacCtx.pNvmCtx->NetworkActivation = ACTIVATION_TYPE_NONE;
                MacCtx.MacFlags.Bits.MlmeInd = 1;
                MacCtx.MlmeIndication.MlmeIndication = MLME_REVERT_JOIN;
            }
        }
    }

    if( MacCtx.MacFlags.Bits.McpsInd == 0 )
    {   // Maximum repetitions without downlink. Increase ADR Ack counter.
        // Only process the case when the MAC did not receive a downlink.
        if( MacCtx.pNvmCtx->AdrCtrlOn == true )
        {
            MacCtx.pNvmCtx->AdrAckCounter++;
        }
    }

    MacCtx.ChannelsNbTransCounter = 0;
    MacCtx.NodeAckRequested = false;
    MacCtx.AckTimeoutRetry = false;
    MacCtx.MacState &= ~LORAMAC_TX_RUNNING;

    return true;
}

static void CallNvmCtxCallback( LoRaMacNvmCtxModule_t module )
{
    if( ( MacCtx.MacCallbacks != NULL ) && ( MacCtx.MacCallbacks->NvmContextChange != NULL ) )
    {
        MacCtx.MacCallbacks->NvmContextChange( module );
    }
}

static void EventMacNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_MAC );
}

static void EventRegionNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_REGION );
}

static void EventCryptoNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_CRYPTO );
}

static void EventSecureElementNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_SECURE_ELEMENT );
}

static void EventCommandsNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_COMMANDS );
}

static void EventClassBNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_CLASS_B );
}

static void EventConfirmQueueNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_CONFIRM_QUEUE );
}

static void EventFCntHandlerNvmCtxChanged( void )
{
    CallNvmCtxCallback( LORAMAC_NVMCTXMODULE_FCNT_HANDLER );
}

static uint8_t IsRequestPending( void )
{
    if( ( MacCtx.MacFlags.Bits.MlmeReq == 1 ) ||
        ( MacCtx.MacFlags.Bits.McpsReq == 1 ) )
    {
        return 1;
    }
    return 0;
}


LoRaMacStatus_t LoRaMacInitialization( LoRaMacPrimitives_t* primitives, LoRaMacCallback_t* callbacks, LoRaMacRegion_t region )
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    LoRaMacClassBCallback_t classBCallbacks;
    LoRaMacClassBParams_t classBParams;

    if( ( primitives == NULL ) ||
        ( callbacks == NULL ) )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    if( ( primitives->MacMcpsConfirm == NULL ) ||
        ( primitives->MacMcpsIndication == NULL ) ||
        ( primitives->MacMlmeConfirm == NULL ) ||
        ( primitives->MacMlmeIndication == NULL ) )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }
    // Verify if the region is supported
    if( RegionIsActive( region ) == false )
    {
        return LORAMAC_STATUS_REGION_NOT_SUPPORTED;
    }

    // Confirm queue reset
    LoRaMacConfirmQueueInit( primitives, EventConfirmQueueNvmCtxChanged );

    // Initialize the module context with zeros
    memset1( ( uint8_t* ) &NvmMacCtx, 0x00, sizeof( LoRaMacNvmCtx_t ) );
    memset1( ( uint8_t* ) &MacCtx, 0x00, sizeof( LoRaMacCtx_t ) );
    MacCtx.pNvmCtx = &NvmMacCtx;

    // Set non zero variables to its default value
    MacCtx.pNvmCtx->Region = region;
    MacCtx.pNvmCtx->DeviceClass = CLASS_A;
    MacCtx.pNvmCtx->RepeaterSupport = false;

    Version_t lrWanVersion;
    lrWanVersion.Fields.Major    = 1;
    lrWanVersion.Fields.Minor    = 1;
    lrWanVersion.Fields.Revision = 1;
    lrWanVersion.Fields.Rfu      = 0;
    MacCtx.pNvmCtx->Version = lrWanVersion;

    // Reset to defaults
    getPhy.Attribute = PHY_DUTY_CYCLE;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->DutyCycleOn = ( bool ) phyParam.Value;

    getPhy.Attribute = PHY_DEF_TX_POWER;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.ChannelsTxPower = phyParam.Value;

    getPhy.Attribute = PHY_DEF_TX_DR;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.ChannelsDatarate = phyParam.Value;

    getPhy.Attribute = PHY_MAX_RX_WINDOW;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.MaxRxWindow = phyParam.Value;

    getPhy.Attribute = PHY_RECEIVE_DELAY1;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.ReceiveDelay1 = phyParam.Value;

    getPhy.Attribute = PHY_RECEIVE_DELAY2;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.ReceiveDelay2 = phyParam.Value;

    getPhy.Attribute = PHY_JOIN_ACCEPT_DELAY1;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.JoinAcceptDelay1 = phyParam.Value;

    getPhy.Attribute = PHY_JOIN_ACCEPT_DELAY2;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.JoinAcceptDelay2 = phyParam.Value;

    getPhy.Attribute = PHY_DEF_DR1_OFFSET;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.Rx1DrOffset = phyParam.Value;

    getPhy.Attribute = PHY_DEF_RX2_FREQUENCY;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.Rx2Channel.Frequency = phyParam.Value;
    MacCtx.pNvmCtx->MacParamsDefaults.RxCChannel.Frequency = phyParam.Value;

    getPhy.Attribute = PHY_DEF_RX2_DR;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.Rx2Channel.Datarate = phyParam.Value;
    MacCtx.pNvmCtx->MacParamsDefaults.RxCChannel.Datarate = phyParam.Value;

    getPhy.Attribute = PHY_DEF_UPLINK_DWELL_TIME;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.UplinkDwellTime = phyParam.Value;

    getPhy.Attribute = PHY_DEF_DOWNLINK_DWELL_TIME;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.DownlinkDwellTime = phyParam.Value;

    getPhy.Attribute = PHY_DEF_MAX_EIRP;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.MaxEirp = phyParam.fValue;

    getPhy.Attribute = PHY_DEF_ANTENNA_GAIN;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.pNvmCtx->MacParamsDefaults.AntennaGain = phyParam.fValue;

    getPhy.Attribute = PHY_DEF_ADR_ACK_LIMIT;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.AdrAckLimit = phyParam.Value;

    getPhy.Attribute = PHY_DEF_ADR_ACK_DELAY;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    MacCtx.AdrAckDelay = phyParam.Value;

    // Init parameters which are not set in function ResetMacParameters
    MacCtx.pNvmCtx->MacParamsDefaults.ChannelsNbTrans = 1;
    MacCtx.pNvmCtx->MacParamsDefaults.SystemMaxRxError = 10;
    MacCtx.pNvmCtx->MacParamsDefaults.MinRxSymbols = 6;

    MacCtx.pNvmCtx->MacParams.SystemMaxRxError	= MacCtx.pNvmCtx->MacParamsDefaults.SystemMaxRxError;
    MacCtx.pNvmCtx->MacParams.MinRxSymbols	= MacCtx.pNvmCtx->MacParamsDefaults.MinRxSymbols;
    MacCtx.pNvmCtx->MacParams.MaxRxWindow	= MacCtx.pNvmCtx->MacParamsDefaults.MaxRxWindow;
    MacCtx.pNvmCtx->MacParams.ReceiveDelay1	= MacCtx.pNvmCtx->MacParamsDefaults.ReceiveDelay1;
    MacCtx.pNvmCtx->MacParams.ReceiveDelay2	= MacCtx.pNvmCtx->MacParamsDefaults.ReceiveDelay2;
    MacCtx.pNvmCtx->MacParams.JoinAcceptDelay1	= MacCtx.pNvmCtx->MacParamsDefaults.JoinAcceptDelay1;
    MacCtx.pNvmCtx->MacParams.JoinAcceptDelay2	= MacCtx.pNvmCtx->MacParamsDefaults.JoinAcceptDelay2;
    MacCtx.pNvmCtx->MacParams.ChannelsNbTrans	= MacCtx.pNvmCtx->MacParamsDefaults.ChannelsNbTrans;

    ResetMacParameters( );

    MacCtx.pNvmCtx->PublicNetwork = true;

    MacCtx.MacPrimitives = primitives;
    MacCtx.MacCallbacks = callbacks;
    MacCtx.MacFlags.Value = 0;
    MacCtx.MacState = LORAMAC_STOPPED;

    // Reset duty cycle times
    MacCtx.pNvmCtx->LastTxDoneTime = 0;
    MacCtx.pNvmCtx->AggregatedTimeOff = 0;

    // Initialize timers
    TimerInit( &MacCtx.TxDelayedTimer,	    OnTxDelayedTimerEvent );
    TimerInit( &MacCtx.RxWindowTimer1,	    OnRxWindow1TimerEvent );
    TimerInit( &MacCtx.RxWindowTimer2,	    OnRxWindow2TimerEvent );
    TimerInit( &MacCtx.AckTimeoutTimer,	    OnAckTimeoutTimerEvent );
    TimerInit( &MacCtx.Rejoin0CycleTimer,   OnRejoin0CycleTimerEvent );
    TimerInit( &MacCtx.Rejoin1CycleTimer,   OnRejoin1CycleTimerEvent );
    TimerInit( &MacCtx.Rejoin2CycleTimer,   OnRejoin2CycleTimerEvent );
    TimerInit( &MacCtx.ForceRejoinReqCycleTimer, OnForceRejoinReqCycleTimerEvent );

    // Store the current initialization time
    MacCtx.pNvmCtx->InitializationTime = SysTimeGetMcuTime( );

    // Initialize Radio driver
    MacCtx.RadioEvents.TxDone	    = OnRadioTxDone;
    MacCtx.RadioEvents.RxDone	    = OnRadioRxDone;
    MacCtx.RadioEvents.RxError	    = OnRadioRxError;
    MacCtx.RadioEvents.TxTimeout    = OnRadioTxTimeout;
    MacCtx.RadioEvents.RxTimeout    = OnRadioRxTimeout;
    Radio.Init( &MacCtx.RadioEvents );

    InitDefaultsParams_t params;
    params.Type = INIT_TYPE_INIT;
    params.NvmCtx = NULL;
    RegionInitDefaults( MacCtx.pNvmCtx->Region, &params );

    // Initialize the Secure Element driver
    if( SecureElementInit( EventSecureElementNvmCtxChanged ) != SECURE_ELEMENT_SUCCESS )
    {
        return LORAMAC_STATUS_CRYPTO_ERROR;
    }

    // Initialize Crypto module
    if( LoRaMacCryptoInit( EventCryptoNvmCtxChanged ) != LORAMAC_CRYPTO_SUCCESS )
    {
        return LORAMAC_STATUS_CRYPTO_ERROR;
    }

    // Initialize MAC commands module
    if( LoRaMacCommandsInit( EventCommandsNvmCtxChanged ) != LORAMAC_COMMANDS_SUCCESS )
    {
        return LORAMAC_STATUS_MAC_COMMAD_ERROR;
    }

    // Initialize FCnt Handler module
    if( LoRaMacFCntHandlerInit( EventFCntHandlerNvmCtxChanged ) != LORAMAC_FCNT_HANDLER_SUCCESS )
    {
        return LORAMAC_STATUS_FCNT_HANDLER_ERROR;
    }

    // Set multicast downlink counter reference
    LoRaMacFCntHandlerSetMulticastReference( MacCtx.pNvmCtx->MulticastChannelList );

    // Random seed initialization
    srand1( Radio.Random( ) );

    Radio.SetPublicNetwork( MacCtx.pNvmCtx->PublicNetwork );
    Radio.Sleep( );

    // Initialize class b
    // Apply callback
    classBCallbacks.GetTemperatureLevel = NULL;
    classBCallbacks.MacProcessNotify = NULL;
    if( callbacks != NULL )
    {
        classBCallbacks.GetTemperatureLevel = callbacks->GetTemperatureLevel;
        classBCallbacks.MacProcessNotify = callbacks->MacProcessNotify;
    }

    // Must all be static. Don't use local references.
    classBParams.MlmeIndication = &MacCtx.MlmeIndication;
    classBParams.McpsIndication = &MacCtx.McpsIndication;
    classBParams.MlmeConfirm = &MacCtx.MlmeConfirm;
    classBParams.LoRaMacFlags = &MacCtx.MacFlags;
    classBParams.LoRaMacDevAddr = &MacCtx.pNvmCtx->DevAddr;
    classBParams.LoRaMacRegion = &MacCtx.pNvmCtx->Region;
    classBParams.LoRaMacParams = &MacCtx.pNvmCtx->MacParams;
    classBParams.MulticastChannels = &MacCtx.pNvmCtx->MulticastChannelList[0];

    LoRaMacClassBInit( &classBParams, &classBCallbacks, &EventClassBNvmCtxChanged );

    LoRaMacEnableRequests( LORAMAC_REQUEST_HANDLING_ON );

    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMacStart( void )
{
    MacCtx.MacState = LORAMAC_IDLE;
    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMacStop( void )
{
    if( LoRaMacIsBusy( ) == false )
    {
		NRF_LOG_INFO("LORAMAC_STOPPED");
        MacCtx.MacState = LORAMAC_STOPPED;
        return LORAMAC_STATUS_OK;
    }
    else if(  MacCtx.MacState == LORAMAC_STOPPED )
    {
        return LORAMAC_STATUS_OK;
    }
    return LORAMAC_STATUS_BUSY;
}

LoRaMacStatus_t LoRaMacQueryTxPossible( uint8_t size, LoRaMacTxInfo_t* txInfo )
{
    CalcNextAdrParams_t adrNext;
    uint32_t adrAckCounter = MacCtx.pNvmCtx->AdrAckCounter;
    int8_t datarate = MacCtx.pNvmCtx->MacParamsDefaults.ChannelsDatarate;
    int8_t txPower = MacCtx.pNvmCtx->MacParamsDefaults.ChannelsTxPower;
    size_t macCmdsSize = 0;

    if( txInfo == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    // Setup ADR request
    adrNext.Version = MacCtx.pNvmCtx->Version;
    adrNext.UpdateChanMask = false;
    adrNext.AdrEnabled = MacCtx.pNvmCtx->AdrCtrlOn;
    adrNext.AdrAckCounter = MacCtx.pNvmCtx->AdrAckCounter;
    adrNext.AdrAckLimit = MacCtx.AdrAckLimit;
    adrNext.AdrAckDelay = MacCtx.AdrAckDelay;
    adrNext.Datarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
    adrNext.TxPower = MacCtx.pNvmCtx->MacParams.ChannelsTxPower;
    adrNext.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;
    adrNext.Region = MacCtx.pNvmCtx->Region;

    // We call the function for information purposes only. We don't want to
    // apply the datarate, the tx power and the ADR ack counter.
    LoRaMacAdrCalcNext( &adrNext, &datarate, &txPower, &adrAckCounter );

    txInfo->CurrentPossiblePayloadSize = GetMaxAppPayloadWithoutFOptsLength( datarate );

    if( LoRaMacCommandsGetSizeSerializedCmds( &macCmdsSize ) != LORAMAC_COMMANDS_SUCCESS )
    {
        return LORAMAC_STATUS_MAC_COMMAD_ERROR;
    }

    // Verify if the MAC commands fit into the FOpts and into the maximum payload.
    if( ( LORA_MAC_COMMAND_MAX_FOPTS_LENGTH >= macCmdsSize ) && ( txInfo->CurrentPossiblePayloadSize >= macCmdsSize ) )
    {
        txInfo->MaxPossibleApplicationDataSize = txInfo->CurrentPossiblePayloadSize - macCmdsSize;

        // Verify if the application data together with MAC command fit into the maximum payload.
        if( txInfo->CurrentPossiblePayloadSize >= ( macCmdsSize + size ) )
        {
            return LORAMAC_STATUS_OK;
        }
        else
        {
           return LORAMAC_STATUS_LENGTH_ERROR;
        }
    }
    else
    {
        txInfo->MaxPossibleApplicationDataSize = 0;
        return LORAMAC_STATUS_LENGTH_ERROR;
    }
}



LoRaMacStatus_t LoRaMacMibGetRequestConfirm( MibRequestConfirm_t* mibGet )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;

    if( mibGet == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    switch( mibGet->Type )
    {
        case MIB_DEVICE_CLASS:
        {
            mibGet->Param.Class = MacCtx.pNvmCtx->DeviceClass;
            break;
        }
        case MIB_NETWORK_ACTIVATION:
        {
            mibGet->Param.NetworkActivation = MacCtx.pNvmCtx->NetworkActivation;
            break;
        }
        case MIB_ADR:
        {
            mibGet->Param.AdrEnable = MacCtx.pNvmCtx->AdrCtrlOn;
            break;
        }
        case MIB_NET_ID:
        {
            mibGet->Param.NetID = MacCtx.pNvmCtx->NetID;
            break;
        }
        case MIB_DEV_ADDR:
        {
            mibGet->Param.DevAddr = MacCtx.pNvmCtx->DevAddr;
            break;
        }

		// Network key
        case MIB_GEN_APP_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( GEN_APP_KEY, mibGet->Param.GenAppKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_APP_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( APP_KEY, mibGet->Param.AppKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_NWK_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( NWK_KEY, mibGet->Param.NwkKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_J_S_INT_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( J_S_INT_KEY, mibGet->Param.JSIntKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_J_S_ENC_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( J_S_ENC_KEY, mibGet->Param.JSEncKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_F_NWK_S_INT_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( F_NWK_S_INT_KEY, mibGet->Param.FNwkSIntKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_S_NWK_S_INT_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( S_NWK_S_INT_KEY, mibGet->Param.SNwkSIntKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_NWK_S_ENC_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( NWK_S_ENC_KEY, mibGet->Param.NwkSEncKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_APP_S_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( APP_S_KEY, mibGet->Param.AppSKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_KE_KEY:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_KE_KEY, mibGet->Param.McKEKey ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_KEY_0:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_KEY_0, mibGet->Param.McKey0 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_APP_S_KEY_0:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_APP_S_KEY_0, mibGet->Param.McAppSKey0 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_NWK_S_KEY_0:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_NWK_S_KEY_0, mibGet->Param.McNwkSKey0 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_KEY_1:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_KEY_1, mibGet->Param.McKey1 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_APP_S_KEY_1:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_APP_S_KEY_1, mibGet->Param.McAppSKey1 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_NWK_S_KEY_1:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_NWK_S_KEY_1, mibGet->Param.McNwkSKey1 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_KEY_2:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_KEY_2, mibGet->Param.McKey2 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_APP_S_KEY_2:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_APP_S_KEY_2, mibGet->Param.McAppSKey2 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_NWK_S_KEY_2:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_NWK_S_KEY_2, mibGet->Param.McNwkSKey2 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_KEY_3:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_KEY_3, mibGet->Param.McKey3 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_APP_S_KEY_3:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_APP_S_KEY_3, mibGet->Param.McAppSKey3 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_MC_NWK_S_KEY_3:
        {
			if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoGetKey( MC_NWK_S_KEY_3, mibGet->Param.McNwkSKey3 ) )
			{
				return LORAMAC_STATUS_CRYPTO_ERROR;
			}
            break;
        }
        case MIB_PUBLIC_NETWORK:
        {
            mibGet->Param.EnablePublicNetwork = MacCtx.pNvmCtx->PublicNetwork;
            break;
        }
        case MIB_REPEATER_SUPPORT:
        {
            mibGet->Param.EnableRepeaterSupport = MacCtx.pNvmCtx->RepeaterSupport;
            break;
        }
        case MIB_CHANNELS:
        {
            getPhy.Attribute = PHY_CHANNELS;
            phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );

            mibGet->Param.ChannelList = phyParam.Channels;
            break;
        }
        case MIB_RX2_CHANNEL:
        {
            mibGet->Param.Rx2Channel = MacCtx.pNvmCtx->MacParams.Rx2Channel;
            break;
        }
        case MIB_RX2_DEFAULT_CHANNEL:
        {
            mibGet->Param.Rx2Channel = MacCtx.pNvmCtx->MacParamsDefaults.Rx2Channel;
            break;
        }
        case MIB_RXC_CHANNEL:
        {
            mibGet->Param.RxCChannel = MacCtx.pNvmCtx->MacParams.RxCChannel;
            break;
        }
        case MIB_RXC_DEFAULT_CHANNEL:
        {
            mibGet->Param.RxCChannel = MacCtx.pNvmCtx->MacParamsDefaults.RxCChannel;
            break;
        }
        case MIB_CHANNELS_DEFAULT_MASK:
        {
            getPhy.Attribute = PHY_CHANNELS_DEFAULT_MASK;
            phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );

            mibGet->Param.ChannelsDefaultMask = phyParam.ChannelsMask;
            break;
        }
        case MIB_CHANNELS_MASK:
        {
            getPhy.Attribute = PHY_CHANNELS_MASK;
            phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );

            mibGet->Param.ChannelsMask = phyParam.ChannelsMask;
            break;
        }
        case MIB_CHANNELS_NB_TRANS:
        {
            mibGet->Param.ChannelsNbTrans = MacCtx.pNvmCtx->MacParams.ChannelsNbTrans;
            break;
        }
        case MIB_MAX_RX_WINDOW_DURATION:
        {
            mibGet->Param.MaxRxWindow = MacCtx.pNvmCtx->MacParams.MaxRxWindow;
            break;
        }
        case MIB_RECEIVE_DELAY_1:
        {
            mibGet->Param.ReceiveDelay1 = MacCtx.pNvmCtx->MacParams.ReceiveDelay1;
            break;
        }
        case MIB_RECEIVE_DELAY_2:
        {
            mibGet->Param.ReceiveDelay2 = MacCtx.pNvmCtx->MacParams.ReceiveDelay2;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_1:
        {
            mibGet->Param.JoinAcceptDelay1 = MacCtx.pNvmCtx->MacParams.JoinAcceptDelay1;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_2:
        {
            mibGet->Param.JoinAcceptDelay2 = MacCtx.pNvmCtx->MacParams.JoinAcceptDelay2;
            break;
        }
        case MIB_CHANNELS_DEFAULT_DATARATE:
        {
            mibGet->Param.ChannelsDefaultDatarate = MacCtx.pNvmCtx->MacParamsDefaults.ChannelsDatarate;
            break;
        }
        case MIB_CHANNELS_DATARATE:
        {
            mibGet->Param.ChannelsDatarate = MacCtx.pNvmCtx->MacParams.ChannelsDatarate;
            break;
        }
        case MIB_CHANNELS_DEFAULT_TX_POWER:
        {
            mibGet->Param.ChannelsDefaultTxPower = MacCtx.pNvmCtx->MacParamsDefaults.ChannelsTxPower;
            break;
        }
        case MIB_CHANNELS_TX_POWER:
        {
            mibGet->Param.ChannelsTxPower = MacCtx.pNvmCtx->MacParams.ChannelsTxPower;
            break;
        }
        case MIB_SYSTEM_MAX_RX_ERROR:
        {
            mibGet->Param.SystemMaxRxError = MacCtx.pNvmCtx->MacParams.SystemMaxRxError;
            break;
        }
        case MIB_MIN_RX_SYMBOLS:
        {
            mibGet->Param.MinRxSymbols = MacCtx.pNvmCtx->MacParams.MinRxSymbols;
            break;
        }
        case MIB_ANTENNA_GAIN:
        {
            mibGet->Param.AntennaGain = MacCtx.pNvmCtx->MacParams.AntennaGain;
            break;
        }
        case MIB_NVM_CTXS:
        {
            mibGet->Param.Contexts = GetCtxs( );
            break;
        }
        case MIB_DEFAULT_ANTENNA_GAIN:
        {
            mibGet->Param.DefaultAntennaGain = MacCtx.pNvmCtx->MacParamsDefaults.AntennaGain;
            break;
        }
        case MIB_REJOIN_0_CYCLE:
        {
            mibGet->Param.Rejoin0CycleInSec = MacCtx.pNvmCtx->Rejoin0CycleInSec;
            break;
        }
        case MIB_REJOIN_1_CYCLE:
        {
            mibGet->Param.Rejoin1CycleInSec = MacCtx.pNvmCtx->Rejoin1CycleInSec;
            break;
        }
        case MIB_REJOIN_2_CYCLE:
        {
            mibGet->Param.Rejoin2CycleInSec = MacCtx.pNvmCtx->Rejoin2CycleInSec;
            break;
        }
        default:
        {
            status = LoRaMacClassBMibGetRequestConfirm( mibGet );
            break;
        }
    }
    return status;
}

LoRaMacStatus_t LoRaMacMibSetRequestConfirm( MibRequestConfirm_t* mibSet )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_OK;
    ChanMaskSetParams_t chanMaskSet;
    VerifyParams_t verify;

    if( mibSet == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }
    if( ( MacCtx.MacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        return LORAMAC_STATUS_BUSY;
    }

    switch( mibSet->Type )
    {
        case MIB_DEVICE_CLASS:
        {
            status = SwitchClass( mibSet->Param.Class );
            break;
        }
        case MIB_NETWORK_ACTIVATION:
        {
            if( mibSet->Param.NetworkActivation != ACTIVATION_TYPE_OTAA  )
            {
                MacCtx.pNvmCtx->NetworkActivation = mibSet->Param.NetworkActivation;
            }
            else
            {   // Do not allow to set ACTIVATION_TYPE_OTAA since the MAC will set it automatically after a successful join process.
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_ADR:
        {
            MacCtx.pNvmCtx->AdrCtrlOn = mibSet->Param.AdrEnable;
            break;
        }
        case MIB_NET_ID:
        {
            MacCtx.pNvmCtx->NetID = mibSet->Param.NetID;
            break;
        }
        case MIB_DEV_ADDR:
        {
            MacCtx.pNvmCtx->DevAddr = mibSet->Param.DevAddr;
            break;
        }
        case MIB_GEN_APP_KEY:
        {
            if( mibSet->Param.GenAppKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( GEN_APP_KEY, mibSet->Param.GenAppKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_APP_KEY:
        {
            if( mibSet->Param.AppKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( APP_KEY, mibSet->Param.AppKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_NWK_KEY:
        {
            if( mibSet->Param.NwkKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( NWK_KEY, mibSet->Param.NwkKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_J_S_INT_KEY:
        {
            if( mibSet->Param.JSIntKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( J_S_INT_KEY, mibSet->Param.JSIntKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_J_S_ENC_KEY:
        {
            if( mibSet->Param.JSEncKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( J_S_ENC_KEY, mibSet->Param.JSEncKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_F_NWK_S_INT_KEY:
        {
            if( mibSet->Param.FNwkSIntKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( F_NWK_S_INT_KEY, mibSet->Param.FNwkSIntKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_S_NWK_S_INT_KEY:
        {
            if( mibSet->Param.SNwkSIntKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( S_NWK_S_INT_KEY, mibSet->Param.SNwkSIntKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_NWK_S_ENC_KEY:
        {
            if( mibSet->Param.NwkSEncKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( NWK_S_ENC_KEY, mibSet->Param.NwkSEncKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_APP_S_KEY:
        {
            if( mibSet->Param.AppSKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( APP_S_KEY, mibSet->Param.AppSKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_KE_KEY:
        {
            if( mibSet->Param.McKEKey != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_KE_KEY, mibSet->Param.McKEKey ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_KEY_0:
        {
            if( mibSet->Param.McKey0 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_KEY_0, mibSet->Param.McKey0 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_APP_S_KEY_0:
        {
            if( mibSet->Param.McAppSKey0 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_APP_S_KEY_0, mibSet->Param.McAppSKey0 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_NWK_S_KEY_0:
        {
            if( mibSet->Param.McNwkSKey0 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_NWK_S_KEY_0, mibSet->Param.McNwkSKey0 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_KEY_1:
        {
            if( mibSet->Param.McKey1 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_KEY_1, mibSet->Param.McKey1 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_APP_S_KEY_1:
        {
            if( mibSet->Param.McAppSKey1 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_APP_S_KEY_1, mibSet->Param.McAppSKey1 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_NWK_S_KEY_1:
        {
            if( mibSet->Param.McNwkSKey1 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_NWK_S_KEY_1, mibSet->Param.McNwkSKey1 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_KEY_2:
        {
            if( mibSet->Param.McKey2 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_KEY_2, mibSet->Param.McKey2 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_APP_S_KEY_2:
        {
            if( mibSet->Param.McAppSKey2 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_APP_S_KEY_2, mibSet->Param.McAppSKey2 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_NWK_S_KEY_2:
        {
            if( mibSet->Param.McNwkSKey2 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_NWK_S_KEY_2, mibSet->Param.McNwkSKey2 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_KEY_3:
        {
            if( mibSet->Param.McKey3 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_KEY_3, mibSet->Param.McKey3 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_APP_S_KEY_3:
        {
            if( mibSet->Param.McAppSKey3 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_APP_S_KEY_3, mibSet->Param.McAppSKey3 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MC_NWK_S_KEY_3:
        {
            if( mibSet->Param.McNwkSKey3 != NULL )
            {
                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetKey( MC_NWK_S_KEY_3, mibSet->Param.McNwkSKey3 ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_PUBLIC_NETWORK:
        {
            MacCtx.pNvmCtx->PublicNetwork = mibSet->Param.EnablePublicNetwork;
            Radio.SetPublicNetwork( MacCtx.pNvmCtx->PublicNetwork );
            break;
        }
        case MIB_REPEATER_SUPPORT:
        {
            MacCtx.pNvmCtx->RepeaterSupport = mibSet->Param.EnableRepeaterSupport;
            break;
        }
        case MIB_RX2_CHANNEL:
        {
            verify.DatarateParams.Datarate = mibSet->Param.Rx2Channel.Datarate;
            verify.DatarateParams.DownlinkDwellTime = MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_RX_DR ) == true )
            {
                MacCtx.pNvmCtx->MacParams.Rx2Channel = mibSet->Param.Rx2Channel;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_RX2_DEFAULT_CHANNEL:
        {
            verify.DatarateParams.Datarate = mibSet->Param.Rx2Channel.Datarate;
            verify.DatarateParams.DownlinkDwellTime = MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_RX_DR ) == true )
            {
                MacCtx.pNvmCtx->MacParamsDefaults.Rx2Channel = mibSet->Param.Rx2DefaultChannel;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_RXC_CHANNEL:
        {
            verify.DatarateParams.Datarate = mibSet->Param.RxCChannel.Datarate;
            verify.DatarateParams.DownlinkDwellTime = MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_RX_DR ) == true )
            {
                MacCtx.pNvmCtx->MacParams.RxCChannel = mibSet->Param.RxCChannel;

                if( ( MacCtx.pNvmCtx->DeviceClass == CLASS_C ) && ( MacCtx.pNvmCtx->NetworkActivation != ACTIVATION_TYPE_NONE ) )
                {
                    // We can only compute the RX window parameters directly, if we are already
                    // in class c mode and joined. We cannot setup an RX window in case of any other
                    // class type.
                    // Set the radio into sleep mode in case we are still in RX mode
                    Radio.Sleep( );
                    // Compute RxC windows parameters
                    RegionComputeRxWindowParameters( MacCtx.pNvmCtx->Region,
                                                     MacCtx.pNvmCtx->MacParams.RxCChannel.Datarate,
                                                     MacCtx.pNvmCtx->MacParams.MinRxSymbols,
                                                     MacCtx.pNvmCtx->MacParams.SystemMaxRxError,
                                                     &MacCtx.RxWindowCConfig );
                    OpenContinuousRxCWindow( );
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_RXC_DEFAULT_CHANNEL:
        {
            verify.DatarateParams.Datarate = mibSet->Param.RxCChannel.Datarate;
            verify.DatarateParams.DownlinkDwellTime = MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_RX_DR ) == true )
            {
                MacCtx.pNvmCtx->MacParamsDefaults.RxCChannel = mibSet->Param.RxCDefaultChannel;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DEFAULT_MASK:
        {
            chanMaskSet.ChannelsMaskIn = mibSet->Param.ChannelsMask;
            chanMaskSet.ChannelsMaskType = CHANNELS_DEFAULT_MASK;

            if( RegionChanMaskSet( MacCtx.pNvmCtx->Region, &chanMaskSet ) == false )
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_MASK:
        {
            chanMaskSet.ChannelsMaskIn = mibSet->Param.ChannelsMask;
            chanMaskSet.ChannelsMaskType = CHANNELS_MASK;

            if( RegionChanMaskSet( MacCtx.pNvmCtx->Region, &chanMaskSet ) == false )
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_NB_TRANS:
        {
            if( ( mibSet->Param.ChannelsNbTrans >= 1 ) &&
                ( mibSet->Param.ChannelsNbTrans <= 15 ) )
            {
                MacCtx.pNvmCtx->MacParams.ChannelsNbTrans = mibSet->Param.ChannelsNbTrans;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_MAX_RX_WINDOW_DURATION:
        {
            MacCtx.pNvmCtx->MacParams.MaxRxWindow = mibSet->Param.MaxRxWindow;
            break;
        }
        case MIB_RECEIVE_DELAY_1:
        {
            MacCtx.pNvmCtx->MacParams.ReceiveDelay1 = mibSet->Param.ReceiveDelay1;
            break;
        }
        case MIB_RECEIVE_DELAY_2:
        {
            MacCtx.pNvmCtx->MacParams.ReceiveDelay2 = mibSet->Param.ReceiveDelay2;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_1:
        {
            MacCtx.pNvmCtx->MacParams.JoinAcceptDelay1 = mibSet->Param.JoinAcceptDelay1;
            break;
        }
        case MIB_JOIN_ACCEPT_DELAY_2:
        {
            MacCtx.pNvmCtx->MacParams.JoinAcceptDelay2 = mibSet->Param.JoinAcceptDelay2;
            break;
        }
        case MIB_CHANNELS_DEFAULT_DATARATE:
        {
            verify.DatarateParams.Datarate = mibSet->Param.ChannelsDefaultDatarate;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_DEF_TX_DR ) == true )
            {
                MacCtx.pNvmCtx->MacParamsDefaults.ChannelsDatarate = verify.DatarateParams.Datarate;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DATARATE:
        {
            verify.DatarateParams.Datarate = mibSet->Param.ChannelsDatarate;
            verify.DatarateParams.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_TX_DR ) == true )
            {
                MacCtx.pNvmCtx->MacParams.ChannelsDatarate = verify.DatarateParams.Datarate;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_DEFAULT_TX_POWER:
        {
            verify.TxPower = mibSet->Param.ChannelsDefaultTxPower;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_DEF_TX_POWER ) == true )
            {
                MacCtx.pNvmCtx->MacParamsDefaults.ChannelsTxPower = verify.TxPower;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_CHANNELS_TX_POWER:
        {
            verify.TxPower = mibSet->Param.ChannelsTxPower;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_TX_POWER ) == true )
            {
                MacCtx.pNvmCtx->MacParams.ChannelsTxPower = verify.TxPower;
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_SYSTEM_MAX_RX_ERROR:
        {
            MacCtx.pNvmCtx->MacParams.SystemMaxRxError = MacCtx.pNvmCtx->MacParamsDefaults.SystemMaxRxError = mibSet->Param.SystemMaxRxError;
            break;
        }
        case MIB_MIN_RX_SYMBOLS:
        {
            MacCtx.pNvmCtx->MacParams.MinRxSymbols = MacCtx.pNvmCtx->MacParamsDefaults.MinRxSymbols = mibSet->Param.MinRxSymbols;
            break;
        }
        case MIB_ANTENNA_GAIN:
        {
            MacCtx.pNvmCtx->MacParams.AntennaGain = mibSet->Param.AntennaGain;
            break;
        }
        case MIB_DEFAULT_ANTENNA_GAIN:
        {
            MacCtx.pNvmCtx->MacParamsDefaults.AntennaGain = mibSet->Param.DefaultAntennaGain;
            break;
        }
        case MIB_NVM_CTXS:
        {
            if( mibSet->Param.Contexts != 0 )
            {
                status = RestoreCtxs( mibSet->Param.Contexts );
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_ABP_LORAWAN_VERSION:
        {
            if( mibSet->Param.AbpLrWanVersion.Fields.Minor <= 1 )
            {
                MacCtx.pNvmCtx->Version = mibSet->Param.AbpLrWanVersion;

                if( LORAMAC_CRYPTO_SUCCESS != LoRaMacCryptoSetLrWanVersion( mibSet->Param.AbpLrWanVersion ) )
                {
                    return LORAMAC_STATUS_CRYPTO_ERROR;
                }
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_REJOIN_0_CYCLE:
        {
            if( ConvertRejoinCycleTime( MacCtx.pNvmCtx->Rejoin0CycleInSec, &MacCtx.Rejoin0CycleTime ) == true )
            {
                TimerSetValue( &MacCtx.Rejoin0CycleTimer, MacCtx.Rejoin0CycleTime );
                TimerStart( &MacCtx.Rejoin0CycleTimer );
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        case MIB_REJOIN_1_CYCLE:
        {
           if( ConvertRejoinCycleTime( MacCtx.pNvmCtx->Rejoin1CycleInSec, &MacCtx.Rejoin1CycleTime ) == true )
            {
                TimerSetValue( &MacCtx.Rejoin1CycleTimer, MacCtx.Rejoin1CycleTime );
                TimerStart( &MacCtx.Rejoin1CycleTimer );
            }
            else
            {
                status = LORAMAC_STATUS_PARAMETER_INVALID;
            }
            break;
        }
        default:
        {
            status = LoRaMacMibClassBSetRequestConfirm( mibSet );
            break;
        }
    }
    EventRegionNvmCtxChanged( );
    EventMacNvmCtxChanged( );
    return status;
}

LoRaMacStatus_t LoRaMacChannelAdd( uint8_t id, ChannelParams_t params )
{
    ChannelAddParams_t channelAdd;

    // Validate if the MAC is in a correct state
    if( ( MacCtx.MacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        if( ( MacCtx.MacState & LORAMAC_TX_CONFIG ) != LORAMAC_TX_CONFIG )
        {
            return LORAMAC_STATUS_BUSY;
        }
    }

    channelAdd.NewChannel = &params;
    channelAdd.ChannelId = id;

    EventRegionNvmCtxChanged( );
    return RegionChannelAdd( MacCtx.pNvmCtx->Region, &channelAdd );
}

LoRaMacStatus_t LoRaMacChannelRemove( uint8_t id )
{
    ChannelRemoveParams_t channelRemove;

    if( ( MacCtx.MacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        if( ( MacCtx.MacState & LORAMAC_TX_CONFIG ) != LORAMAC_TX_CONFIG )
        {
            return LORAMAC_STATUS_BUSY;
        }
    }

    channelRemove.ChannelId = id;

    if( RegionChannelsRemove( MacCtx.pNvmCtx->Region, &channelRemove ) == false )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    EventRegionNvmCtxChanged( );
    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMacMcChannelSetup( McChannelParams_t *channel )
{
    if( ( MacCtx.MacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        return LORAMAC_STATUS_BUSY;
    }

    if( channel->GroupID >= LORAMAC_MAX_MC_CTX )
    {
        return LORAMAC_STATUS_MC_GROUP_UNDEFINED;
    }

    MacCtx.pNvmCtx->MulticastChannelList[channel->GroupID].ChannelParams = *channel;

    const KeyIdentifier_t mcKeys[LORAMAC_MAX_MC_CTX] = { MC_KEY_0, MC_KEY_1, MC_KEY_2, MC_KEY_3 };
    if( LoRaMacCryptoSetKey( mcKeys[channel->GroupID], channel->McKeyE ) != LORAMAC_CRYPTO_SUCCESS )
    {
        return LORAMAC_STATUS_CRYPTO_ERROR;
    }

    if( LoRaMacCryptoDeriveMcSessionKeyPair( channel->GroupID, channel->Address ) != LORAMAC_CRYPTO_SUCCESS )
    {
        return LORAMAC_STATUS_CRYPTO_ERROR;
    }

    if( channel->Class == CLASS_B )
    {
        // Calculate class b parameters
        LoRaMacClassBSetMulticastPeriodicity( &MacCtx.pNvmCtx->MulticastChannelList[channel->GroupID] );
    }

    EventMacNvmCtxChanged( );
    EventRegionNvmCtxChanged( );
    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMacMcChannelDelete( AddressIdentifier_t groupID )
{
    if( ( MacCtx.MacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        return LORAMAC_STATUS_BUSY;
    }

    if( ( groupID >= LORAMAC_MAX_MC_CTX ) || 
        ( MacCtx.pNvmCtx->MulticastChannelList[groupID].ChannelParams.IsEnabled == false ) )
    {
        return LORAMAC_STATUS_MC_GROUP_UNDEFINED;
    }

    McChannelParams_t channel;

    // Set all channel fields with 0
    memset1( ( uint8_t* )&channel, 0, sizeof( McChannelParams_t ) );

    MacCtx.pNvmCtx->MulticastChannelList[groupID].ChannelParams = channel;

    EventMacNvmCtxChanged( );
    EventRegionNvmCtxChanged( );
    return LORAMAC_STATUS_OK;
}

uint8_t LoRaMacMcChannelGetGroupId( uint32_t mcAddress )
{
    for( uint8_t i = 0; i < LORAMAC_MAX_MC_CTX; i++ )
    {
        if( mcAddress == MacCtx.pNvmCtx->MulticastChannelList[i].ChannelParams.Address )
        {
            return i;
        }
    }
    return 0xFF;
}

LoRaMacStatus_t LoRaMacMcChannelSetupRxParams( AddressIdentifier_t groupID, McRxParams_t *rxParams, uint8_t *status )
{
   *status = 0x1C + ( groupID & 0x03 );

    if( ( MacCtx.MacState & LORAMAC_TX_RUNNING ) == LORAMAC_TX_RUNNING )
    {
        return LORAMAC_STATUS_BUSY;
    }

    DeviceClass_t devClass = MacCtx.pNvmCtx->MulticastChannelList[groupID].ChannelParams.Class;
    if( ( devClass == CLASS_A ) || ( devClass > CLASS_C ) )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

    if( ( groupID >= LORAMAC_MAX_MC_CTX ) || 
        ( MacCtx.pNvmCtx->MulticastChannelList[groupID].ChannelParams.IsEnabled == false ) )
    {
        return LORAMAC_STATUS_MC_GROUP_UNDEFINED;
    }
    *status &= 0x0F; // groupID OK

    VerifyParams_t verify;
    // Check datarate
    if( devClass == CLASS_B )
    {
        verify.DatarateParams.Datarate = rxParams->ClassB.Datarate;
    }
    else
    {
        verify.DatarateParams.Datarate = rxParams->ClassC.Datarate;
    }
    verify.DatarateParams.DownlinkDwellTime = MacCtx.pNvmCtx->MacParams.DownlinkDwellTime;

    if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_RX_DR ) == true )
    {
        *status &= 0xFB; // datarate OK
    }

    // Check frequency
    if( devClass == CLASS_B )
    {
        verify.Frequency = rxParams->ClassB.Frequency;
    }
    else
    {
        verify.Frequency = rxParams->ClassC.Frequency;
    }
    if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_FREQUENCY ) == true )
    {
        *status &= 0xF7; // frequency OK
    }

    if( *status == ( groupID & 0x03 ) )
    {
        // Apply parameters
        MacCtx.pNvmCtx->MulticastChannelList[groupID].ChannelParams.RxParams = *rxParams;
    }

    EventMacNvmCtxChanged( );
    EventRegionNvmCtxChanged( );
    return LORAMAC_STATUS_OK;
}

LoRaMacStatus_t LoRaMacMlmeRequest( MlmeReq_t* mlmeRequest )
{
    LoRaMacStatus_t status = LORAMAC_STATUS_SERVICE_UNKNOWN;
    MlmeConfirmQueue_t queueElement;
    uint8_t macCmdPayload[2] = { 0x00, 0x00 };

    if( mlmeRequest == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }
    if( LoRaMacIsBusy( ) == true )
    {
        return LORAMAC_STATUS_BUSY;
    }
    if( LoRaMacConfirmQueueIsFull( ) == true )
    {
        return LORAMAC_STATUS_ERROR; //LORAMAC_STATUS_BUSY;
    }

    if( LoRaMacConfirmQueueGetCnt( ) == 0 )
    {
        memset1( ( uint8_t* ) &MacCtx.MlmeConfirm, 0, sizeof( MacCtx.MlmeConfirm ) );
    }
    MacCtx.MlmeConfirm.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;

    MacCtx.MacFlags.Bits.MlmeReq = 1;
    queueElement.Request = mlmeRequest->Type;
    queueElement.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;
    queueElement.RestrictCommonReadyToHandle = false;

    switch( mlmeRequest->Type )
    {
        case MLME_JOIN:
        {
            if( ( MacCtx.MacState & LORAMAC_TX_DELAYED ) == LORAMAC_TX_DELAYED )
            {
                return LORAMAC_STATUS_BUSY;
            }

            if( ( mlmeRequest->Req.Join.DevEui == NULL ) ||
                ( mlmeRequest->Req.Join.JoinEui == NULL ) )
            {
                return LORAMAC_STATUS_PARAMETER_INVALID;
            }

            MacCtx.pNvmCtx->NetworkActivation = ACTIVATION_TYPE_NONE;

            ResetMacParameters( );

            MacCtx.DevEui = mlmeRequest->Req.Join.DevEui;
            MacCtx.JoinEui = mlmeRequest->Req.Join.JoinEui;

            MacCtx.pNvmCtx->MacParams.ChannelsDatarate = RegionAlternateDr( MacCtx.pNvmCtx->Region, mlmeRequest->Req.Join.Datarate, ALTERNATE_DR );

            queueElement.Status = LORAMAC_EVENT_INFO_STATUS_JOIN_FAIL;

            status = SendReJoinReq( JOIN_REQ );

            if( status != LORAMAC_STATUS_OK )
            {
                // Revert back the previous datarate ( mainly used for US915 like regions )
                MacCtx.pNvmCtx->MacParams.ChannelsDatarate = RegionAlternateDr( MacCtx.pNvmCtx->Region, mlmeRequest->Req.Join.Datarate, ALTERNATE_DR_RESTORE );
            }
            break;
        }
        case MLME_REJOIN_0:
        {
            MacCtx.MacFlags.Bits.MlmeReq = 1;
            MacCtx.MlmeConfirm.MlmeRequest = mlmeRequest->Type;

            status = SendReJoinReq( REJOIN_REQ_0 );

            break;
        }
        case MLME_REJOIN_1:
        {
            MacCtx.MacFlags.Bits.MlmeReq = 1;
            MacCtx.MlmeConfirm.MlmeRequest = mlmeRequest->Type;

            status = SendReJoinReq( REJOIN_REQ_1 );

            break;
        }
        case MLME_LINK_CHECK:
        {
            // LoRaMac will send this command piggy-pack
            status = LORAMAC_STATUS_OK;
            if( LoRaMacCommandsAddCmd( MOTE_MAC_LINK_CHECK_REQ, macCmdPayload, 0 ) != LORAMAC_COMMANDS_SUCCESS )
            {
                status = LORAMAC_STATUS_MAC_COMMAD_ERROR;
            }
            break;
        }
        case MLME_TXCW:
        {
            status = SetTxContinuousWave( mlmeRequest->Req.TxCw.Timeout );
            break;
        }
        case MLME_TXCW_1:
        {

            status = SetTxContinuousWave1( mlmeRequest->Req.TxCw.Timeout, mlmeRequest->Req.TxCw.Frequency, mlmeRequest->Req.TxCw.Power );
            break;
        }
        case MLME_DEVICE_TIME:
        {
            // LoRaMac will send this command piggy-pack
            status = LORAMAC_STATUS_OK;
            if( LoRaMacCommandsAddCmd( MOTE_MAC_DEVICE_TIME_REQ, macCmdPayload, 0 ) != LORAMAC_COMMANDS_SUCCESS )
            {
                status = LORAMAC_STATUS_MAC_COMMAD_ERROR;
            }
            break;
        }
        case MLME_PING_SLOT_INFO:
        {
            uint8_t value = mlmeRequest->Req.PingSlotInfo.PingSlot.Value;

            // LoRaMac will send this command piggy-pack
            LoRaMacClassBSetPingSlotInfo( mlmeRequest->Req.PingSlotInfo.PingSlot.Fields.Periodicity );
            macCmdPayload[0] = value;
            status = LORAMAC_STATUS_OK;
            if( LoRaMacCommandsAddCmd( MOTE_MAC_PING_SLOT_INFO_REQ, macCmdPayload, 1 ) != LORAMAC_COMMANDS_SUCCESS )
            {
                status = LORAMAC_STATUS_MAC_COMMAD_ERROR;
            }
            break;
        }
        case MLME_BEACON_TIMING:
        {
            // LoRaMac will send this command piggy-pack
            status = LORAMAC_STATUS_OK;
            if( LoRaMacCommandsAddCmd( MOTE_MAC_BEACON_TIMING_REQ, macCmdPayload, 0 ) != LORAMAC_COMMANDS_SUCCESS )
            {
                status = LORAMAC_STATUS_MAC_COMMAD_ERROR;
            }
            break;
        }
        case MLME_BEACON_ACQUISITION:
        {
            // Apply the request
            queueElement.RestrictCommonReadyToHandle = true;

            if( LoRaMacClassBIsAcquisitionInProgress( ) == false )
            {
                // Start class B algorithm
                LoRaMacClassBSetBeaconState( BEACON_STATE_ACQUISITION );
                LoRaMacClassBBeaconTimerEvent( NULL );

                status = LORAMAC_STATUS_OK;
            }
            else
            {
                status = LORAMAC_STATUS_BUSY;
            }
            break;
        }
        default:
            break;
    }

    if( status != LORAMAC_STATUS_OK )
    {
        if( LoRaMacConfirmQueueGetCnt( ) == 0 )
        {
            MacCtx.NodeAckRequested = false;
            MacCtx.MacFlags.Bits.MlmeReq = 0;
        }
    }
    else
    {
        LoRaMacConfirmQueueAdd( &queueElement );
        EventMacNvmCtxChanged( );
    }
    return status;
}

LoRaMacStatus_t LoRaMacMcpsRequest( McpsReq_t* mcpsRequest )
{
    GetPhyParams_t getPhy;
    PhyParam_t phyParam;
    LoRaMacStatus_t status = LORAMAC_STATUS_SERVICE_UNKNOWN;
    LoRaMacHeader_t macHdr;
    VerifyParams_t verify;
    uint8_t fPort = 0;
    void* fBuffer;
    uint16_t fBufferSize;
    int8_t datarate = DR_0;
    bool readyToSend = false;

    if( mcpsRequest == NULL )
    {
        return LORAMAC_STATUS_PARAMETER_INVALID;
    }

	// Disable this check for data messages
    if( LoRaMacIsBusy( ) == true && mcpsRequest->Type != MCPS_UNCONFIRMED )
    {
        return LORAMAC_STATUS_BUSY;
    }

    macHdr.Value = 0;
    memset1( ( uint8_t* ) &MacCtx.McpsConfirm, 0, sizeof( MacCtx.McpsConfirm ) );
    MacCtx.McpsConfirm.Status = LORAMAC_EVENT_INFO_STATUS_ERROR;

    switch( mcpsRequest->Type )
    {
        case MCPS_UNCONFIRMED:
        {
            readyToSend = true;

            macHdr.Bits.MType = FRAME_TYPE_DATA_UNCONFIRMED_UP;
            fPort = mcpsRequest->Req.Unconfirmed.fPort;
            fBuffer = mcpsRequest->Req.Unconfirmed.fBuffer;
            fBufferSize = mcpsRequest->Req.Unconfirmed.fBufferSize;
            datarate = mcpsRequest->Req.Unconfirmed.Datarate;
            break;
        }
        case MCPS_CONFIRMED:
        {
            readyToSend = true;

            macHdr.Bits.MType = FRAME_TYPE_DATA_CONFIRMED_UP;
            fPort = mcpsRequest->Req.Confirmed.fPort;
            fBuffer = mcpsRequest->Req.Confirmed.fBuffer;
            fBufferSize = mcpsRequest->Req.Confirmed.fBufferSize;
            datarate = mcpsRequest->Req.Confirmed.Datarate;
            break;
        }
        case MCPS_PROPRIETARY:
        {
            readyToSend = true;

            macHdr.Bits.MType = FRAME_TYPE_PROPRIETARY;
            fBuffer = mcpsRequest->Req.Proprietary.fBuffer;
            fBufferSize = mcpsRequest->Req.Proprietary.fBufferSize;
            datarate = mcpsRequest->Req.Proprietary.Datarate;
            break;
        }
        default:
            break;
    }

    // Get the minimum possible datarate
    getPhy.Attribute = PHY_MIN_TX_DR;
    getPhy.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;
    phyParam = RegionGetPhyParam( MacCtx.pNvmCtx->Region, &getPhy );
    // Apply the minimum possible datarate.
    // Some regions have limitations for the minimum datarate.
    datarate = MAX( datarate, ( int8_t )phyParam.Value );

    if( readyToSend == true )
    {
        if( MacCtx.pNvmCtx->AdrCtrlOn == false )
        {
            verify.DatarateParams.Datarate = datarate;
            verify.DatarateParams.UplinkDwellTime = MacCtx.pNvmCtx->MacParams.UplinkDwellTime;

            if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_TX_DR ) == true )
            {
                MacCtx.pNvmCtx->MacParams.ChannelsDatarate = verify.DatarateParams.Datarate;
            }
            else
            {
                return LORAMAC_STATUS_PARAMETER_INVALID;
            }
        }

        status = Send( &macHdr, fPort, fBuffer, fBufferSize );
        if( status == LORAMAC_STATUS_OK )
        {
            MacCtx.McpsConfirm.McpsRequest = mcpsRequest->Type;
            MacCtx.MacFlags.Bits.McpsReq = 1;
        }
        else
        {
            MacCtx.NodeAckRequested = false;
        }
    }

    EventMacNvmCtxChanged( );
    return status;
}

static bool ConvertRejoinCycleTime( uint32_t rejoinCycleTime, uint32_t* timeInMiliSec )
{
    // Our timer implementation do not allow longer times than 4294967295 ms
    if( rejoinCycleTime > 4294967 )
    {
        *timeInMiliSec = rejoinCycleTime * 1000;
        return true;
    }
    else
    {
        return false;
    }
}

static void OnRejoin0CycleTimerEvent( void* context )
{
    TimerStop( &MacCtx.Rejoin0CycleTimer );
    ConvertRejoinCycleTime( MacCtx.pNvmCtx->Rejoin0CycleInSec, &MacCtx.Rejoin0CycleTime );

    SendReJoinReq( REJOIN_REQ_0 );

    TimerSetValue( &MacCtx.Rejoin0CycleTimer, MacCtx.Rejoin0CycleTime );
    TimerStart( &MacCtx.Rejoin0CycleTimer );
}

static void OnRejoin1CycleTimerEvent( void* context )
{
    TimerStop( &MacCtx.Rejoin1CycleTimer );
    ConvertRejoinCycleTime( MacCtx.pNvmCtx->Rejoin1CycleInSec, &MacCtx.Rejoin1CycleTime );

    SendReJoinReq( REJOIN_REQ_1 );

    TimerSetValue( &MacCtx.Rejoin1CycleTimer, MacCtx.Rejoin1CycleTime );
    TimerStart( &MacCtx.Rejoin1CycleTimer );
}

static void OnRejoin2CycleTimerEvent( void* context )
{
    TimerStop( &MacCtx.Rejoin2CycleTimer );
    ConvertRejoinCycleTime( MacCtx.pNvmCtx->Rejoin2CycleInSec, &MacCtx.Rejoin2CycleTime );

    SendReJoinReq( REJOIN_REQ_2 );

    TimerSetValue( &MacCtx.Rejoin2CycleTimer, MacCtx.Rejoin2CycleTime );
    TimerStart( &MacCtx.Rejoin2CycleTimer );
}

static void OnForceRejoinReqCycleTimerEvent( void* context )
{
    if( MacCtx.pNvmCtx->ForceRejoinRetriesCounter == MacCtx.pNvmCtx->ForceRejoinMaxRetries )
    {
        TimerStop( &MacCtx.ForceRejoinReqCycleTimer );
        MacCtx.pNvmCtx->ForceRejoinRetriesCounter = 0;
    }

    MacCtx.pNvmCtx->ForceRejoinRetriesCounter ++;

    if( ( MacCtx.pNvmCtx->ForceRejoinType == 0 ) || ( MacCtx.pNvmCtx->ForceRejoinType == 1 ) )
    {
        SendReJoinReq( REJOIN_REQ_0 );
    }
    else
    {
        SendReJoinReq( REJOIN_REQ_2 );
    }
}

void LoRaMacTestSetDutyCycleOn( bool enable )
{
    VerifyParams_t verify;

    verify.DutyCycle = enable;

//    if( RegionVerify( MacCtx.pNvmCtx->Region, &verify, PHY_DUTY_CYCLE ) == true )
//    {
        MacCtx.pNvmCtx->DutyCycleOn = enable;
//    }
}
