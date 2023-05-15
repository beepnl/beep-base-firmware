#define NRF_LOG_MODULE_NAME DS3231_APP
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"
#include "sdk_errors.h"	
#include "app_error.h"
#include <stdbool.h>
#include "beepbaseboard.h"
#include "beep_protocol.h"
#include "ds3231_app.h"
#include "gpio-board.h"
#include "power_app.h"
#include "nrf_delay.h"
#include "nrf.h"
#include "log_time.h"
#include "time.h"
#include "nvm_fs.h"
#include "Buzzer.h"

#define DS3231_LOG_ENABLED 0

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
           .frequency          = NRF_TWI_FREQ_400K,
           .interrupt_priority = APP_IRQ_PRIORITY_LOW,
           .clear_bus_init     = true,
           .hold_bus_uninit = true,
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

        nrf_gpio_cfg_output_state(SCL_EXT, 0);
        nrf_gpio_cfg_output_state(SDA_EXT, 0);

          #ifdef DS3231_LOG_ENABLED
            NRF_LOG_INFO("### DS3231 DE-INIT AND DISABLE ###");
            NRF_LOG_FLUSH();
          #endif
}

nrfx_drv_state_t ds3231_status(void)
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

        err_code = nrf_drv_twi_tx(&m_ds3231, 0x68, buf, 2, 0);
        
        APP_ERROR_CHECK(err_code);
        return err_code;
}


void ds3231_setTime(struct tm *ds3231_dateTime_s)
{
        m_xfer_done = false;
        ret_code_t err_code;

        if(ds3231_status() != NRFX_DRV_STATE_POWERED_ON)
          {
              #ifdef DEBUG
                Buzzer_sound(50, 600, 1000, 200, 4);
              #endif
            ds3231_init();        
          }

        ds3231_writeByte(0x68, 0, 0, 2);
        nrf_delay_ms(5);

        //memset(ds3231_dateTime_s, 0, sizeof(struct tm));

        uint8_t tx_buf[8];
        tx_buf[0] = 0; // register address 
   
        tx_buf[1] = dec2bcd(ds3231_dateTime_s->tm_sec);// & 0x7F; 
        tx_buf[2] = dec2bcd(ds3231_dateTime_s->tm_min);// & 0x7F;
        tx_buf[3] = dec2bcd(ds3231_dateTime_s->tm_hour);// & 0x3F;  // always enable 24-hour mode (bit 3)
        tx_buf[4] = dec2bcd(ds3231_dateTime_s->tm_wday);// & 0x07; // ignored by mktime(), memset to 0?
        tx_buf[5] = dec2bcd(ds3231_dateTime_s->tm_mday);// & 0x3F;
        tx_buf[6] = dec2bcd(ds3231_dateTime_s->tm_mon);// & 0x1F;
        tx_buf[7] = dec2bcd(ds3231_dateTime_s->tm_year);// % 100);

        time_t newtime = mktime(ds3231_dateTime_s);
              #ifdef DEBUG
                Buzzer_sound(50, 500, 400, 400, 7);
              #endif
 
        nrf_drv_twi_tx(&m_ds3231, 0x68, tx_buf, sizeof(tx_buf), 0);        
            while(!m_xfer_done)
                {
                __WFE();
                };
}

void ds3231_start_clock_osc()
{
        uint8_t rx_buf;
        uint8_t tx_buf[2];
        uint8_t reg;
        uint8_t statusflag;

        //  set OSF bit to 0 in status reg
        //  starts clock when DS3231 has VBAT 
        reg = 0x0F; 
        statusflag = 0x0C; 
        tx_buf[0] = reg;
        tx_buf[1] = statusflag; 
        
        nrf_drv_twi_tx(&m_ds3231, 0x68, tx_buf, sizeof(tx_buf), 0);
            while(!m_xfer_done)
                {
                __WFE();  
                };    
        nrf_delay_ms(5);

        // read back status reg
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
        nrf_delay_ms(5);     

          #ifdef DS3231_LOG_ENABLED
              NRF_LOG_INFO("### STATUS REGISTER = %d ###", rx_buf);                               
            if(rx_buf == 12)
              {
              NRF_LOG_INFO("### OSF BIT = 1 ### \n ### DS3231 clock oscillator is STOPPED ###");
              };
          #endif
 
        reg = 0x0E; // also read control reg for oscillator on/off status 
        rx_buf = 0; // empty rx buffer

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
        nrf_delay_ms(5);

          #ifdef DS3231_LOG_ENABLED   
            NRF_LOG_INFO("### CONTROL REGISTER = %d ###", rx_buf);
                          NRF_LOG_FLUSH();      
          #endif
}

time_t ds3231_getTime()
{
        /*if(ds3231_status() != NRFX_DRV_STATE_POWERED_ON)
          {
            ds3231_init();
          }*/

        uint8_t reg = 0;
        uint8_t rx_buf[7] = {0};

        struct tm ds3231_dateTime; 
        memset(&ds3231_dateTime, 0, sizeof(struct tm));

        nrf_drv_twi_tx(&m_ds3231, 0x68, &reg, 1, 0);
            while(!m_xfer_done)
                {
                __WFE();
                };
        nrf_delay_ms(5);

        nrf_drv_twi_rx(&m_ds3231, 0x68, rx_buf, sizeof(rx_buf));
            while(!m_xfer_done)
                {
                __WFE();
                };     
                
        nrf_delay_ms(50); 

        ds3231_dateTime.tm_sec = bcd2dec(rx_buf[0]);
        ds3231_dateTime.tm_min = bcd2dec(rx_buf[1]);
        ds3231_dateTime.tm_hour = bcd2dec(rx_buf[2] & 0x3F); // use only the first six bits, for 24hr mode
        ds3231_dateTime.tm_wday = bcd2dec(rx_buf[3]) - 1; // is ignored by mktime()
        ds3231_dateTime.tm_mday = bcd2dec(rx_buf[4]);
        ds3231_dateTime.tm_mon = bcd2dec(rx_buf[5] & 0x1f) - 1;
        ds3231_dateTime.tm_year = bcd2dec(rx_buf[6]);
         
           /*
           if ((ds3231_dateTime->tm_sec > 59) || (ds3231_dateTime->tm_min > 59) || (ds3231_dateTime->tm_hour > 23) ||
               (ds3231_dateTime->tm_mday < 1) || (ds3231_dateTime->tm_mday > 31) || (ds3231_dateTime->tm_mon > 11) || (ds3231_dateTime->tm_year > 199) ||
               (ds3231_dateTime->tm_wday > 6))                   
           */

           #ifdef DS3231_LOG_ENABLED
             nrf_delay_ms(5);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd sec %d ###\n", rx_buf[0]); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd min %d ###\n", rx_buf[1]); nrf_delay_ms(1); 
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd hr %d ###\n", rx_buf[2]); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd wday %d ###\n", rx_buf[3]); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd mday %d ###\n", rx_buf[4]); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd mon %d ###\n", rx_buf[5]); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd year %d ###\n", rx_buf[6]); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 

             NRF_LOG_INFO("### DS3231 bcd sec %d ###\n", ds3231_dateTime.tm_sec); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd min %d ###\n", ds3231_dateTime.tm_min); nrf_delay_ms(1); 
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd hr %d ###\n", ds3231_dateTime.tm_hour); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd wday %d ###\n", ds3231_dateTime.tm_wday); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd mday %d ###\n", ds3231_dateTime.tm_mday); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd mon %d ###\n", ds3231_dateTime.tm_mon); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 bcd year %d ###\n", ds3231_dateTime.tm_year); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             // NRF_LOG_INFO("### DS3231 bcd2dec sec %d ###", rx_buf[0], rx_buf[1], rx_buf[2], rx_buf[3], rx_buf[4], rx_buf[5], rx_buf[6]);
             nrf_delay_ms(5);
           #endif 
 
        time_t ds3231_dateTime_timestamp;
        ds3231_dateTime_timestamp = 0;
        ds3231_dateTime_timestamp = mktime(&ds3231_dateTime); // ds3231 time struct to timestamp
               
          #ifdef DS3231_LOG_ENABLED  
            nrf_delay_ms(5);                      
            NRF_LOG_INFO("### DS3231 time_t: %s, %u/0x%04X", get_logtime_string(ds3231_dateTime_timestamp), ds3231_dateTime_timestamp, ds3231_dateTime_timestamp);
            NRF_LOG_FLUSH();
          #endif   

        return ds3231_dateTime_timestamp;      
}

uint8_t dec2bcd(uint8_t dec)
{
        uint8_t bcd;
        bcd = ((dec / 10 * 16) + (dec % 10));
        return bcd;
}

uint8_t bcd2dec(uint8_t bcd)
{
        uint8_t dec;
        dec = (bcd / 16 * 10) + (bcd % 16);
        return dec;
}
   
bool ds3231_detected()
{
       if(ds3231_status() == !NRFX_DRV_STATE_POWERED_ON)
         {
           ds3231_init();
         }

        ret_code_t err_code;

        uint8_t address;
        uint8_t tx = 0;
        uint8_t rx = 0;

        #ifdef DS3231_LOG_ENABLED
          NRF_LOG_INFO("### START SCAN ###");
          NRF_LOG_FLUSH();
        #endif

              nrf_drv_twi_tx(&m_ds3231, 0x68, &tx, 1, 0);
                  while(!m_xfer_done)
                      {
                      __WFE();
                      };
              nrf_delay_ms(5);

              err_code = nrf_drv_twi_rx(&m_ds3231, 0x68, &rx, sizeof(rx));
                  while(!m_xfer_done)
                      {
                      __WFE();
                      };
              nrf_delay_ms(5);

              if (err_code == NRF_SUCCESS)
              {
                  #ifdef DS3231_LOG_ENABLED
                    NRF_LOG_INFO("### TWI SCAN FOUND DS3231 RTC AT 0x68 ###");
                    NRF_LOG_FLUSH();
                  #endif    

                  #ifdef DEBUG 
                   // Buzzer_sound(50, 400, 500, 200, 2);
                  #endif
                return true;
              }
              else
              {
                  #ifdef DS3231_LOG_ENABLED
                    NRF_LOG_INFO("### TWI SCAN FOUND NO DEVICES ###");
                    NRF_LOG_FLUSH();
                  #endif          
                         
                ds3231_uninit();
                return false;
              }
}
