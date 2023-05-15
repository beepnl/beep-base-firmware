#ifndef __NVM_FS_H__
#define __NVM_FS_H__

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "beep_protocol.h"
#include "time.h"

/* File ID and Key used for the configuration record. */
#define CONFIG_FILE (0xF010)
#define CONFIG_REC_KEY (0x7010)
#define LORAWAN_NVM_SIZE 1600
#ifdef DEBUG
#define DEFAULT_LORAWAN_MSG_INTERVAL_MINUTES 2
#else
#define DEFAULT_LORAWAN_MSG_INTERVAL_MINUTES 15
#endif

#define DEFAULT_AUDIO_CHANNEL AIN_IN2RP
#define DEFAULT_AUDIO_COARSE_GAIN (20 * 2)
#define DEFAULT_AUDIO_VOLUME 0
#define DEFAULT_AUDIO_INPUT_DIV false
#define DEFAULT_FFT_DIVIDER 10
#define DEFAULT_FFT_START 0
#define DEFAULT_FFT_STOP UINT8_MAX

#define DEFAULT_DS_TEMP_MAX INT16_MAX   //  (80 * 100)
#define DEFAULT_DS_TEMP_MIN INT16_MIN   //  (-25 * 100)
#define DEFAULT_DS_TEMP_DIFF UINT16_MAX //  (10 * 100)

#define DEFAULT_SUPPLY_MAX UINT16_MAX  //  3300
#define DEFAULT_SUPPLY_MIN 0           //  1800
#define DEFAULT_SUPPLY_DIFF UINT16_MAX //  500

#define DEFAULT_HX_MAX INT32_MAX   //  16000
#define DEFAULT_HX_MIN INT32_MIN   //  -16000
#define DEFAULT_HX_DIFF UINT32_MAX //  1000U

// BME defaults
#define DEFAULT_BME_TEMP_MAX INT16_MAX   //  (80 * 100)
#define DEFAULT_BME_TEMP_MIN INT16_MIN   //  (-25 * 100)
#define DEFAULT_BME_TEMP_DIFF UINT16_MAX //  (10 * 100)

#define DEFAULT_BME_HUMIDITY_MAX UINT16_MAX  //  (90 * 100)
#define DEFAULT_BME_HUMIDITY_MIN 0           //  (10 * 100)
#define DEFAULT_BME_HUMIDITY_DIFF UINT16_MAX //  (5  * 100)

#define DEFAULT_BME_PRESSURE_MAX UINT16_MAX  //  10000
#define DEFAULT_BME_PRESSURE_MIN 0           //  200
#define DEFAULT_BME_PRESSURE_DIFF UINT16_MAX //  200

#define ONOFF_STATE_BITMASK (0x01)
#define ONOFF_STATE_ENABLED (0x01)
#define ONOFF_STATE_DISABLED (0x00)

#define ONOFF_CONTROL_REEDSWITCH_MASK 0x01
#define ONOFF_CONTROL_TILTSWITCH_MASK 0x02
#define ONOFF_CONTROL_MASK (ONOFF_CONTROL_TILTSWITCH_MASK | ONOFF_CONTROL_REEDSWITCH_MASK)
#define ONOFF_CONTROL_DEFAULT_VALUE ONOFF_CONTROL_REEDSWITCH_MASK

#define TIME_DEFAULT 0
#define TIME_SAVE_INTERVAL_MIN (48 * 60)

/*
#define DEFAULT_DS3231_INITIALIZED false
#define DEFAULT_DS3231_BLE_INITIALIZED false
*/

typedef enum {
  UICR_HARDWARE_REVISION_LOCATION = 0, // Stores the Hardware revision in two uint16_t: Major (LSB), Minor (MSB)
  UICR_HARDWARE_SERIAL_NUMBER = 1,
  UICR_ATECC_ID_1 = 2,
  UICR_ATECC_ID_2 = 3,
  UICR_ATECC_ID_3 = 4,
} UICR_LOCATIONS;

/* A structure definition to save data in flash. */
typedef struct
{
  uint32_t Major_version;
  uint32_t Minor_version;
  uint32_t Sub_version;
  uint32_t boot_count;

  /* LoRaWAN Non-volatile storage array */
  uint8_t LoRaWANnvm[LORAWAN_NVM_SIZE];
  uint8_t devEUI[8];
  uint8_t appEUI[8];
  uint8_t appKey[16];
  uint8_t lorawanStatus;
  uint8_t ds18B20Status;
  uint16_t ds18B20Interval;
  uint8_t lorawanRatio;
  uint16_t mainSampleInterval_min;
  uint8_t pinCodeLenght;
  uint8_t pinCode[PIN_CODE_LENGHT_MAX + 1]; // add an extra byte, which should always be 0
  uint8_t hx711ConvChannels;
  uint8_t hx711nSamples;
  uint8_t attecSerialNumber[ATECC_ID_LENGHT];

  /*
   *   Added on 6-12-2019, firmware version: 1.2.3
   */
  uint8_t audioChannel;
  uint8_t audioCoarseGain_dB2;
  int8_t audioVolume_dB2;
  bool min6dBInputDivider;
  uint8_t fft_count;
  uint8_t fft_start;
  uint8_t fft_stop;
  DS_ALARM_s alarm_DS;
  SUPPLY_ALARM_s alarm_Supply;
  HX711_ALARM_s alarm_HX;
  BME_ALARM_s alarm_BME;

  /*
   *   Added on 23-12-2019, firmware version: 1.3.1
   */
  uint8_t prev_main_state;

  /*
   *  Added on 19-05-2020, firmware version: 1.4.0
   *  Changed to on_state for 1.5.0
   */
  uint8_t on_state; // Was previously .sq_state in firmware version 1.4.0
  time_t lastTime;  // The last stored time value

  /*
   *  Added on 07-10-2020, firmware version: 1.5.1
   */
  uint8_t onoff_control;

  /*
   *  Added on 26-04-2023, firmware version: 1.5.13
   */
   /*
  bool ds3231_initialized;
  bool ds3231_ble_initialized;
  */
} flash_struct_s;

typedef enum {
  PREVIOUS_MAIN_STATE,
} FLASH_INTERNAL_PARAM;

typedef struct
{
  FLASH_INTERNAL_PARAM param;

  union {
    uint8_t previous_state;
  } value;
} INTERNAL_PARAM;

void nvm_fds_changed(void);
void nvm_fds_check_pending(void);
bool nvm_fds_check_busy(void);
void nvm_fds_init(void);

void nvm_fds_eeprom_wrapper_read(uint16_t addr, uint8_t *buffer, uint16_t size);
void nvm_fds_eeprom_wrapper_write(uint16_t addr, uint8_t *buffer, uint16_t size);

uint32_t nvm_fds_eeprom_get(BEEP_protocol_s *prot);
uint32_t nvm_fds_eeprom_set(BEEP_protocol_s *prot);

uint32_t nvm_fds_internal_param_get(INTERNAL_PARAM *internal);
uint32_t nvm_fds_internal_param_set(INTERNAL_PARAM *internal);

bool nvm_toggle_on_state(void);
bool nvm_get_on_state(void);

time_t nvm_getLastTime(void);
void nvm_setLastTime(time_t newTime);

bool nvm_reedSwitchEnabled(void);
bool nvm_tiltSwitchEnabled(void);
/*
bool nvm_ds3231_is_initialized(void);
void nvm_ds3231_set_initialized(void);
bool nvm_ds3231_is_ble_initialized(void);
void nvm_ds3231_set_ble_initialized(void);
*/
void uicr_getHWid(void);

#endif /* __NVM_FS_H__ */