#define NRF_LOG_MODULE_NAME DS3231_APP
#include "nrf_log.h"
NRF_LOG_MODULE_REGISTER();
#include "nrf_log_ctrl.h"
#include "sdk_errors.h"	
#include "app_error.h"
#include "app_timer.h"
#include "beepbaseboard.h"
#include "ds3231_app.h"
#include "gpio-board.h"
#include "power_app.h"
#include "nrf_delay.h"
#include "nrf.h"

#define DS3231_ADDR 0x68U

nrf_drv_twi_t m_ds3231 = NRF_DRV_TWI_INSTANCE(1);
static volatile bool m_xfer_done = false;

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
           .clear_bus_init     = false,
           .hold_bus_uninit   = false
        };

        powerApp_Enable(true, PWR_BME280);
        nrf_gpio_pin_set(SDA_EXT); // pull SDA high
        nrf_delay_ms(5); 
        NRF_LOG_INFO("### ENABLE POWER --> TWI INSTANCE 1 ###");
        NRF_LOG_FLUSH();

        ret_code_t err_code = nrf_drv_twi_init(&m_ds3231, &ds3231_config, ds3231_handler, NULL); 

        nrf_drv_twi_enable(&m_ds3231);
        nrf_delay_ms(5);

        NRF_LOG_INFO("### DS3231 TWI INSTANCE INIT ###");
        NRF_LOG_FLUSH();

        APP_ERROR_CHECK(err_code);
}

void ds3231_uninit(void)
{ 
        nrf_drv_twi_uninit(&m_ds3231);
        nrf_drv_twi_disable(&m_ds3231);

        NRF_LOG_INFO("### DS3231 TWI INSTANCE --> DE-INIT AND DISABLE ###");
        NRF_LOG_FLUSH();
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

        NRF_LOG_INFO("### DS3231 WRITE BYTE [%x , %d] TO ADDRESS %x ###", reg, data, address);
        NRF_LOG_FLUSH();
          err_code = nrf_drv_twi_tx(&m_ds3231, address, buf, 1, 1);
        
        APP_ERROR_CHECK(err_code);
        return err_code;
}

void ds3231_readTime(void)
{
        ret_code_t err_code;
        uint8_t r_buf[32] = {0};
        uint8_t register_read = 0xD1;

          ds3231_writeByte(0x68, 0x00, 0x01, 2);
          
            while(!m_xfer_done)
                {
                __WFE();
                };

          nrf_drv_twi_rx(&m_ds3231, 0xD1, r_buf, sizeof(r_buf));

            while(!m_xfer_done)
                {
                __WFE();
                };

          NRF_LOG_INFO("### DS3231 BUFFER: %d, %d, %d ###", r_buf[0],r_buf[1],r_buf[2]);
          NRF_LOG_FLUSH();
          NRF_LOG_PROCESS();

          nrf_delay_ms(10);

          NRF_LOG_INFO("### 10ms DELAY ---> DS3231 BUFFER: ###");
          NRF_LOG_HEXDUMP_INFO(r_buf, 32);
          NRF_LOG_FLUSH();
}

void ds3231_start(void)
{
        NRF_LOG_INFO("### START DS3231 READ ###");
        NRF_LOG_FLUSH();
        ds3231_writeByte(0x68, 0x00, 0x00, 1); 
        //ds3231_writeByte(0x68, 0xD1, 0, 1);
}

void twi_scanner(void)
{
        ret_code_t err_code;

        uint8_t address;
        uint8_t sample_data;
        bool detected_device = false;

        NRF_LOG_INFO("### START TWI BUS SCAN ###");
        NRF_LOG_FLUSH();

       // do {
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
        //} while (m_xfer_done = false);

        if (!detected_device)
        {
            NRF_LOG_INFO("### TWI SCAN FOUND NO DEVICES ###");
            NRF_LOG_FLUSH();
        }

        APP_ERROR_CHECK(err_code);
}