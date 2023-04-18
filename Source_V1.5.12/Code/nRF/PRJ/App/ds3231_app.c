#define NRF_LOG_MODULE_NAME DS3231_APP
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"
#include "sdk_errors.h"	
#include "app_error.h"
#include "app_timer.h"
#include "beepbaseboard.h"
#include "beep_protocol.h"
#include "ds3231_app.h"
#include "gpio-board.h"
#include "power_app.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "log_time.h"
#include "time.h"
#include "app_timer.h"
#include "nvm_fs.h"

#define DS3231_ADDR 0x68U

nrf_drv_twi_t m_ds3231 = NRF_DRV_TWI_INSTANCE(1);
static volatile bool m_xfer_done = false;

static volatile time_t m_time;
static volatile uint32_t lastRTCcounterValue;
static volatile uint32_t remainderRTCcounter;
static volatile time_t logtime;

void ds3231_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
        switch (p_event->type)
        {
            case NRF_DRV_TWI_EVT_DONE:
            {
                if (p_event->xfer_desc.type == (NRF_DRV_TWI_XFER_TX || NRF_DRV_TWI_XFER_RX ||
                                                NRF_DRV_TWI_XFER_TXTX || NRF_DRV_TWI_XFER_TXRX))
                {
                }
                m_xfer_done = true;
                break;        
            }
            default:
            {
              m_xfer_done = false;
              break;
            }
        }       
}

void ds3231_init(void)
{ 
        nrf_drv_twi_config_t ds3231_config = {
           .scl                = SCL_EXT,
           .sda                = SDA_EXT,
           .frequency          = NRF_TWI_FREQ_100K,
           .interrupt_priority = APP_IRQ_PRIORITY_LOW,
           .clear_bus_init     = true
        };
        
          #ifdef DS3231_LOG_ENABLED
            NRF_LOG_INFO("### ENABLE POWER --> TWI INSTANCE 1 ###");
            NRF_LOG_FLUSH();
          #endif

          powerApp_Enable(true, PWR_BME280);
          ret_code_t err_code = nrf_drv_twi_init(&m_ds3231, &ds3231_config, ds3231_handler, NULL); 
        
          nrf_drv_twi_enable(&m_ds3231);
          nrf_delay_ms(5);

        APP_ERROR_CHECK(err_code);
}

void ds3231_uninit(void)
{ 
        nrf_drv_twi_uninit(&m_ds3231);
        nrf_drv_twi_disable(&m_ds3231);
        // todo: disable pullup on pin too?

        #ifdef DS3231_LOG_ENABLED
        NRF_LOG_INFO("### DS3231 TWI INSTANCE --> DE-INIT AND DISABLE ###");
        NRF_LOG_FLUSH();
        #endif
}

nrfx_drv_state_t ds3231_state(void)
{
        return nrf_drv_twi_enabled(&m_ds3231);
}

ret_code_t ds3231_writeByte(const uint8_t address, const uint8_t reg, uint8_t data, uint8_t length)
{
        m_xfer_done = false;
        ret_code_t err_code;

        uint8_t buf[2] = {0};
        buf[0] = reg;
        buf[1] = data;     

        err_code = nrf_drv_twi_tx(&m_ds3231, 0x68, buf, 2, 1);
        
        APP_ERROR_CHECK(err_code);
        return err_code;
}


void ds3231_setTime(struct tm *ds3231_dateTime_s)
{
        m_xfer_done = false;
        ret_code_t err_code;
        struct tm logtime;

        time_t timeValue = get_logtime_value();
        logtime = *localtime(&timeValue); 
        
        ds3231_dateTime_s->tm_sec = dec2bcd(logtime.tm_sec); 
        ds3231_dateTime_s->tm_min = dec2bcd(logtime.tm_min);
        ds3231_dateTime_s->tm_hour = dec2bcd(logtime.tm_hour & 0x3f);  // always enable 24-hour mode (bit 3)
        ds3231_dateTime_s->tm_wday = dec2bcd(logtime.tm_wday);
        ds3231_dateTime_s->tm_mday = dec2bcd(logtime.tm_mday);
        ds3231_dateTime_s->tm_mon = dec2bcd(logtime.tm_mon);
        ds3231_dateTime_s->tm_year = dec2bcd(logtime.tm_year);

        uint8_t tx_buf[8];
        tx_buf[0] = 0; // register address 

        memcpy(&tx_buf[1], ds3231_dateTime_s, ((sizeof(tx_buf))-1));

        #ifdef DS3231_LOG_ENABLED
                NRF_LOG_INFO("### tx_buf[1] = %d ###", tx_buf[1]);
                NRF_LOG_FLUSH();

                NRF_LOG_INFO("### ds3231_dateTime_s = %d ###", &ds3231_dateTime_s->tm_sec);
               // NRF_LOG_FLUSH();

                NRF_LOG_INFO("### get_logtime_value = %s ###", get_logtime_string(timeValue));
               // NRF_LOG_FLUSH();

                NRF_LOG_INFO("### logtime_sec = %d ###", logtime.tm_sec );
              //  NRF_LOG_FLUSH();

        #endif 



        nrf_drv_twi_tx(&m_ds3231, 0x68, tx_buf, sizeof(tx_buf), 0);        
            while(!m_xfer_done)
                {
                __WFE();
                };
/*
        #ifdef DS3231_LOG_ENABLED 
          NRF_LOG_INFO("### ds3231_dateTime_s ###");
          NRF_LOG_HEXDUMP_INFO(ds3231_dateTime_s, sizeof(ds3231_dateTime_s));
          nrf_delay_ms(1);
          NRF_LOG_FLUSH();

          NRF_LOG_INFO("### tx_buf ###");
          NRF_LOG_HEXDUMP_INFO(tx_buf, sizeof(tx_buf));
          NRF_LOG_FLUSH();
        #endif
*/
}

void ds3231_getStatus()
{
        uint8_t rx_buf;
        uint8_t tx_buf[2];

        uint8_t reg;
        uint8_t statusflag;

        // go to STATUS REGISTER 0x0F
        reg = 0x0F;
        statusflag = 12;
        tx_buf[0] = reg;
        tx_buf[1] = statusflag; // 0xC
        
        // 0xC (set OSF bit in STATUS REGISTER to 0 to start clock when BAT-powered)   
        nrf_drv_twi_tx(&m_ds3231, 0x68, tx_buf, sizeof(tx_buf), 1);
            while(!m_xfer_done)
                {
                __WFE();
                };
        nrf_delay_ms(5);

        // read STATUS REGISTER
        nrf_drv_twi_tx(&m_ds3231, 0x68, &reg, 1, 1);
            while(!m_xfer_done)
                {
                __WFE();
                };

        nrf_drv_twi_rx(&m_ds3231, 0x68, &rx_buf, sizeof(rx_buf));
            while(!m_xfer_done)
                {
                __WFE();
                };          

          #ifdef DS3231_LOG_ENABLED 
            NRF_LOG_INFO("### STATUS REGISTER = %x ###", rx_buf);
           // NRF_LOG_FLUSH();      
          #endif

        // also read CONTROL REGISTER for oscillator on/off status 
        reg = 0x0E;

        nrf_drv_twi_tx(&m_ds3231, 0x68, &reg, 1, 0);
            while(!m_xfer_done)
                {
                __WFE();
                };

        nrf_delay_ms(5);

        nrf_drv_twi_rx(&m_ds3231, 0x68, &rx_buf, sizeof(rx_buf));
            while(!m_xfer_done)
                {
                __WFE();
                };          

          #ifdef DS3231_LOG_ENABLED   
            NRF_LOG_INFO("### CONTROL REGISTER = %x ###", rx_buf);
            // NRF_LOG_FLUSH();      
          #endif
}

void ds3231_getTime(struct tm *ds3231_dateTime)
{
        uint8_t reg = 0;
        uint8_t rx_buf[7] = {0};

        time_t ds3231_timestamp;
        ds3231_timestamp = app_timer_cnt_get();

        #ifdef DS3231_LOG_ENABLED
            NRF_LOG_INFO("### app_timer_cnt_get = %lu ###", ds3231_timestamp);
            NRF_LOG_FLUSH();
        #endif
                   
        nrf_drv_twi_tx(&m_ds3231, 0x68, &reg, 1, 0);
            while(!m_xfer_done)
                {
                __WFE();
                };

            nrf_delay_ms(1);

        nrf_drv_twi_rx(&m_ds3231, 0x68, rx_buf, sizeof(rx_buf));
            while(!m_xfer_done)
                {
                __WFE();
                };      

            nrf_delay_ms(5);  
        
        ds3231_dateTime->tm_sec = bcd2dec(rx_buf[0]);
        ds3231_dateTime->tm_min = bcd2dec(rx_buf[1]);
        ds3231_dateTime->tm_hour = bcd2dec(rx_buf[2]);
        ds3231_dateTime->tm_wday = bcd2dec(rx_buf[3]);
        ds3231_dateTime->tm_mday = bcd2dec(rx_buf[4]);
        ds3231_dateTime->tm_mon = bcd2dec(rx_buf[5]);
        ds3231_dateTime->tm_year = bcd2dec(rx_buf[6]);

        // struct to timestamp
        time_t ds3231_dateTime_timestamp;
        ds3231_dateTime_timestamp = mktime(ds3231_dateTime);

          #ifdef DS3231_LOG_ENABLED 
            nrf_delay_ms(5);
            for(int i = 0; i < 7; ++i) 
            {
              NRF_LOG_INFO("rx_buf[ %d ] = %d ", i, rx_buf[i]);
            };
            NRF_LOG_FLUSH();
             
            NRF_LOG_INFO("### ds3231_dateTime -> tm_sec = %lu ###", ds3231_dateTime->tm_sec);
            NRF_LOG_INFO("### ds3231_timestamp = %s ###", get_logtime_string(ds3231_dateTime_timestamp)); 

            /*
            NRF_LOG_INFO("### sec = %d, min = %d, hour = %d, day = %d, date = %d, month = %d, year = %d ###", 
                         ds3231_dateTime->second, ds3231_dateTime->minute, ds3231_dateTime->hour, ds3231_dateTime->day,
                         ds3231_dateTime->date, ds3231_dateTime->month, ds3231_dateTime->year);
            */

          #endif
}

uint8_t dec2bcd(uint8_t dec)
{
        uint8_t bcd;
        bcd = ((dec / 10 * 16) + (dec % 10));
        #ifdef DS3231_LOG_ENABLED
          NRF_LOG_INFO("### DEC %d = %d in BCD ###", dec, bcd); 
        #endif
        return bcd;
}

uint8_t bcd2dec(uint8_t bcd)
{
        uint8_t dec;
        dec = ((bcd / 16 * 10) + (bcd % 16));
        #ifdef DS3231_LOG_ENABLED
          NRF_LOG_INFO("### BCD %d = %d in DEC ###", bcd, dec); 
        #endif
        return dec;
}
   
void twi_scanner(void)
{
        ret_code_t err_code;

        uint8_t address;
        uint8_t sample_data;
        bool detected_device = false;

        NRF_LOG_INFO("### START TWI BUS SCAN ###");
        NRF_LOG_FLUSH();

          for (address = 1; address <= 127; address++)
          {
              err_code = nrf_drv_twi_rx(&m_ds3231, address, &sample_data, sizeof(sample_data));
              if (err_code == NRF_SUCCESS)
              {
                  detected_device = true;
                  NRF_LOG_INFO("### TWI SCAN FOUND DEVICE AT 0x%x ###", address);
            
              }
          NRF_LOG_FLUSH();
          }

        if (!detected_device)
        {
            NRF_LOG_INFO("### TWI SCAN FOUND NO DEVICES ###");
            NRF_LOG_FLUSH();
        }

        APP_ERROR_CHECK(err_code);
}