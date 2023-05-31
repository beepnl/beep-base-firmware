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
#include "main.h"

//#define TWI_BLOCKING_MODE 

nrf_drv_twi_t m_ds3231 = NRF_DRV_TWI_INSTANCE(1);

static volatile bool m_xfer_done = false;
static volatile bool m_xfer_nack = false; 

static volatile uint32_t lastRTCcounterValue;
static volatile uint32_t remainderRTCcounter;
static volatile time_t logtime;

uint8_t ds3231_enabled;


void ds3231_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
        switch (p_event->type)
        {
            case NRF_DRV_TWI_EVT_DONE:
            {
                m_xfer_nack = false;
                if (p_event->xfer_desc.type == (NRF_DRV_TWI_XFER_TX || NRF_DRV_TWI_XFER_RX ||
                                                NRF_DRV_TWI_XFER_TXTX || NRF_DRV_TWI_XFER_TXRX))
                  {
                    m_xfer_done = true;
                    break;
                  }       
            }         
            case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            {
              m_xfer_nack = true;
              m_xfer_done = true;
              break;
            }
            case NRF_DRV_TWI_EVT_DATA_NACK:
            {
              m_xfer_nack = true;
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
           .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
           .clear_bus_init     = true,
           .hold_bus_uninit = true,
        };
        
          #ifdef DS3231_LOG_ENABLED
            NRF_LOG_INFO("### ENABLE POWER --> TWI INSTANCE 1 ###");
            NRF_LOG_FLUSH();
          #endif

        powerApp_Enable(true, PWR_BME280);
        ret_code_t err_code = nrf_drv_twi_init(&m_ds3231, &ds3231_config, NULL, NULL); 
        
        nrf_drv_twi_enable(&m_ds3231); 
         
        ds3231_enabled = 1;

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


void ds3231_setTime(struct tm ds3231_dateTime_s)
{
        m_xfer_done = false;
        ret_code_t err_code;

        if(ds3231_status() == 0) // && !(NRFX_CHECK(NRFX_TWI1_ENABLED)))
          {
            ds3231_init();        
          }

        uint8_t tx_buf[8];
        tx_buf[0] = 0; // register address 
   
        tx_buf[1] = dec2bcd(ds3231_dateTime_s.tm_sec);// & 0x7F; 
        tx_buf[2] = dec2bcd(ds3231_dateTime_s.tm_min);// & 0x7F;
        tx_buf[3] = dec2bcd(ds3231_dateTime_s.tm_hour) & 0x3F;  // always enable 24-hour mode (bit 3)
        tx_buf[4] = dec2bcd(ds3231_dateTime_s.tm_wday);// & 0x07; 
        tx_buf[5] = dec2bcd(ds3231_dateTime_s.tm_mday) & 0x3F;
        tx_buf[6] = dec2bcd(ds3231_dateTime_s.tm_mon); // & 0x1F;
        tx_buf[7] = dec2bcd(ds3231_dateTime_s.tm_year);

        nrf_drv_twi_tx(&m_ds3231, 0x68, tx_buf, sizeof(tx_buf), 0);    

        ds3231_start_clock_osc(); // in case the tm struct has overwritten the OSF bit or EOSC bit
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
       // statusflag = 0x0C; 
        statusflag = 0;
        tx_buf[0] = reg;
        tx_buf[1] = statusflag; 
        rx_buf = 0;
        
        nrf_drv_twi_tx(&m_ds3231, 0x68, tx_buf, sizeof(tx_buf), 0);
        #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done)
                {
                __WFE();  
                };    
                #endif
         

        // read back status reg 0X0F to check the Oscillator Stop Flag (OSF) bit
        nrf_drv_twi_tx(&m_ds3231, 0x68, &reg, 1, 0);
        #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done)
                {
                __WFE();
                };
                #endif
         

        nrf_drv_twi_rx(&m_ds3231, 0x68, &rx_buf, sizeof(rx_buf));
        #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done) 
                {
                __WFE();
                };     
                #endif

          #ifdef DS3231_LOG_ENABLED
              NRF_LOG_INFO("### STATUS REGISTER = %d ###", rx_buf);        
              NRF_LOG_FLUSH();                       
              if(rx_buf == 12)
              {
                NRF_LOG_INFO("### OSF BIT = 1 ### \n ###");
                NRF_LOG_FLUSH();
              };
          #endif
 
        reg = 0x0E; // also read control reg for oscillator on/off status 
        rx_buf = 0; // empty rx buffer

        nrf_drv_twi_tx(&m_ds3231, 0x68, tx_buf, sizeof(tx_buf), 0);
        #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done)
                {
                __WFE();
                };  
                #endif 
         
                   
        nrf_drv_twi_rx(&m_ds3231, 0x68, &rx_buf, sizeof(rx_buf));
        #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done)
                {
                __WFE();
                };        
                #endif  
         

          #ifdef DS3231_LOG_ENABLED   
            NRF_LOG_INFO("### CONTROL REGISTER = %d ###", rx_buf);
            NRF_LOG_FLUSH();      
          #endif
          
          // clear control register to make sure OSF is not set so that battery-backup is active
          nrf_drv_twi_tx(&m_ds3231, 0x68, &reg, 1, 0);
          #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done)
                {
                __WFE();
                };  
                #endif 
}

time_t ds3231_getTime()
{
         if(ds3231_status() == 0) // && !(NRFX_CHECK(NRFX_TWI1_ENABLED)))
          {
            #ifdef DS3231_LOG_ENABLED   
              NRF_LOG_INFO("### DS3231 INIT STATUS = %d %s -- INITIALIZING ###", ds3231_status(),ds3231_status());
              NRF_LOG_FLUSH();      
            #endif

            ds3231_init();
          }

          if(!powerApp_getEnabled(PWR_BME280))
          {
            #ifdef DS3231_LOG_ENABLED   
              NRF_LOG_INFO("### PWR_BME280 NOT ENABLED -- ENABLING ###");
              NRF_LOG_FLUSH();      
            #endif

            powerApp_Enable(1, BME280);
          }

        uint8_t reg = 0;
        uint8_t rx_buf[7] = {0};

        struct tm ds3231_dateTime; 
        memset(&ds3231_dateTime, 0, sizeof(struct tm));

        nrf_drv_twi_tx(&m_ds3231, 0x68, &reg, 1, 0);
        #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done)
                {
                __WFE();
                };
                #endif
         
        nrf_drv_twi_rx(&m_ds3231, 0x68, rx_buf, sizeof(rx_buf));
        #ifdef TWI_BLOCKING_MODE
            while(!m_xfer_done)
                {
                __WFE();
                };     
                 #endif              

        ds3231_dateTime.tm_sec = bcd2dec(rx_buf[0]);
        ds3231_dateTime.tm_min = bcd2dec(rx_buf[1]);
        ds3231_dateTime.tm_hour = bcd2dec(rx_buf[2] & 0x3F); // 24hr mode
        ds3231_dateTime.tm_wday = bcd2dec(rx_buf[3]) - 1; 
        ds3231_dateTime.tm_mday = bcd2dec(rx_buf[4]);
        ds3231_dateTime.tm_mon = bcd2dec(rx_buf[5] & 0x1f);
        ds3231_dateTime.tm_year = bcd2dec(rx_buf[6]);

           #ifdef DEBUG
           #ifdef DS3231_LOG_ENABLED
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
             NRF_LOG_FINAL_FLUSH();  nrf_delay_ms(1); 

             NRF_LOG_INFO("### DS3231 dec sec %d ###\n", ds3231_dateTime.tm_sec); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 dec min %d ###\n",ds3231_dateTime.tm_min); nrf_delay_ms(1);
             NRF_LOG_FLUSH(); nrf_delay_ms(1); 
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 dec hr %d ###\n", ds3231_dateTime.tm_hour); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 dec wday %d ###\n", ds3231_dateTime.tm_wday); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 dec mday %d ###\n", ds3231_dateTime.tm_mday); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 dec mon %d ###\n", ds3231_dateTime.tm_mon); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
             NRF_LOG_INFO("### DS3231 dec year %d ###\n", ds3231_dateTime.tm_year); nrf_delay_ms(1);
             NRF_LOG_FLUSH();  nrf_delay_ms(1); 
           #endif 
           #endif
 
        time_t ds3231_dateTime_timestamp;
        ds3231_dateTime.tm_isdst = 0;
        ds3231_dateTime_timestamp = mktime(&ds3231_dateTime); 
               
          #ifdef DS3231_LOG_ENABLED                           
            NRF_LOG_INFO("### DS3231 time_t: %s, %u/0x%04X", get_logtime_string(ds3231_dateTime_timestamp), ds3231_dateTime_timestamp, ds3231_dateTime_timestamp);
            NRF_LOG_FLUSH();
          #endif   

        // set the timestamp in flash 
        nvm_setLastTime(ds3231_dateTime_timestamp);
        nvm_fds_changed();

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
        dec = ((bcd / 16 * 10) + (bcd % 16));
        return dec;
}
   
bool ds3231_detected()
{
        if(ds3231_status() == 0) // !(NRFX_CHECK(NRFX_TWI1_ENABLED)))
         {
           ds3231_init();
         }

        ret_code_t err_code;
        uint8_t tx;
        uint8_t rx;

          #ifdef DS3231_LOG_ENABLED
            NRF_LOG_INFO("### START SCAN ###");
            NRF_LOG_FLUSH();
          #endif

              err_code = nrf_drv_twi_rx(&m_ds3231, 0x68, &rx, sizeof(rx));

                #ifndef TWI_BLOCKING_MODE
                  if(err_code == 0) // == NRF_SUCCESS
                  {
                    ds3231_enabled = 1;
                  }
                  else
                  {
                    ds3231_enabled = 0;
                  }
                #endif
              
              if(ds3231_enabled == 0)
              {
                  #ifdef DS3231_LOG_ENABLED
                    NRF_LOG_INFO("### TWI SCAN FOUND NO DEVICES ###");
                    NRF_LOG_FLUSH();
                  #endif          

                powerApp_Enable(false, PWR_BME280);
                return false;
              }
              else
              {
                  #ifdef DS3231_LOG_ENABLED
                    NRF_LOG_INFO("### TWI SCAN FOUND DS3231 RTC AT 0x68 -- ERR_CODE %d###", err_code);
                    NRF_LOG_FLUSH();
                  #endif    

                ds3231_enabled = 1;       
                          ds3231_start_clock_osc();
                return true;
              }
}