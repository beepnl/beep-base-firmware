var hardware_drivers_desc =
[
    [ "Clock", "hardware_driver_clock.html", [
      [ "Driver configuration", "hardware_driver_clock.html#hardware_driver_clock_configuration", null ],
      [ "Controlling the HFCLK", "hardware_driver_clock.html#hardware_driver_clock_hfclk", null ],
      [ "Controlling the LFCLK (without a SoftDevice only)", "hardware_driver_clock.html#hardware_driver_clock_lfclk", null ],
      [ "Controlling the calibration of the LFCLK (without a SoftDevice and using RC as source clock)", "hardware_driver_clock.html#hardware_driver_clock_calibration", null ]
    ] ],
    [ "COMP", "hardware_driver_comp.html", [
      [ "Driver configuration", "hardware_driver_comp.html#hardware_driver_comp_config", null ],
      [ "Using the COMP driver", "hardware_driver_comp.html#hardware_driver_comp_use", [
        [ "Capacitive sensing", "hardware_driver_comp.html#hardware_driver_comp_csense", null ]
      ] ]
    ] ],
    [ "FPU", "hardware_driver_fpu.html", [
      [ "Using FPU with the CMSIS DSP library", "hardware_driver_fpu.html#fpu_cmsis_dsp", null ],
      [ "Using FPU in polling mode with error handling", "hardware_driver_fpu.html#fpu_polling_using", null ],
      [ "Using FPU in interrupt mode with error handling", "hardware_driver_fpu.html#fpu_interrupt_using", null ]
    ] ],
    [ "GPIOTE", "hardware_driver_gpiote.html", [
      [ "TASK/EVENT channels allocation", "hardware_driver_gpiote.html#gpiote_allocation", null ],
      [ "Driver configuration", "hardware_driver_gpiote.html#gpiote_configuration", null ],
      [ "Driver initialization", "hardware_driver_gpiote.html#gpiote_initialization", null ],
      [ "Controlling output pins", "hardware_driver_gpiote.html#gpiote_out", [
        [ "Initialization", "hardware_driver_gpiote.html#gpiote_out_initialization", null ],
        [ "Manually controlled output pins", "hardware_driver_gpiote.html#gpiote_out_simple", null ],
        [ "Task-controlled output pins", "hardware_driver_gpiote.html#gpiote_out_task", null ]
      ] ],
      [ "Controlling input pins", "hardware_driver_gpiote.html#gpiote_in", [
        [ "Initialization", "hardware_driver_gpiote.html#gpiote_in_initialization", null ],
        [ "Usage", "hardware_driver_gpiote.html#gpiote_in_usage", null ],
        [ "Working with a SoftDevice", "hardware_driver_gpiote.html#gpiote_in_sd", null ]
      ] ]
    ] ],
    [ "I2S", "hardware_driver_i2s.html", [
      [ "Driver configuration", "hardware_driver_i2s.html#i2s_driver_configuration", null ],
      [ "Using the I2S driver", "hardware_driver_i2s.html#i2s_driver_using", null ]
    ] ],
    [ "LPCOMP", "hardware_driver_lpcomp.html", null ],
    [ "PDM", "hardware_driver_pdm.html", [
      [ "Driver configuration", "hardware_driver_pdm.html#pdm_configuration", null ],
      [ "Using the PDM driver", "hardware_driver_pdm.html#pdm_using", null ]
    ] ],
    [ "PPI", "hardware_driver_ppi.html", null ],
    [ "PWM", "hardware_driver_pwm.html", [
      [ "Driver configuration", "hardware_driver_pwm.html#pwm_driver_configuration", null ],
      [ "Using the PWM driver", "hardware_driver_pwm.html#pwm_driver_using", null ]
    ] ],
    [ "QDEC", "hardware_driver_qdec.html", null ],
    [ "QSPI", "hardware_driver_qspi.html", [
      [ "Driver configuration", "hardware_driver_qspi.html#qspi_driver_configuration", null ],
      [ "Using the QSPI driver with the memory device for asynchronous transfers", "hardware_driver_qspi.html#qspi_driver_using", [
        [ "Defining a data handling function", "hardware_driver_qspi.html#qspi_driver_define_function", null ],
        [ "Initializing and configuring the driver", "hardware_driver_qspi.html#qspi_driver_initialize_driver", null ],
        [ "Configuring memory", "hardware_driver_qspi.html#qspi_driver_config_memory", null ],
        [ "Transferring data", "hardware_driver_qspi.html#qspi_driver_transfer_data", null ],
        [ "Erasing data", "hardware_driver_qspi.html#qspi_driver_erase_data", null ]
      ] ]
    ] ],
    [ "RNG", "hardware_driver_rng.html", null ],
    [ "SAADC", "hardware_driver_saadc.html", [
      [ "Driver configuration", "hardware_driver_saadc.html#saadc_configuration", null ],
      [ "Using the SAADC driver", "hardware_driver_saadc.html#saadc_using", [
        [ "Blocking mode", "hardware_driver_saadc.html#saadc_blocking", null ],
        [ "Non-blocking mode", "hardware_driver_saadc.html#saadc_nonblocking", null ],
        [ "Limits", "hardware_driver_saadc.html#saadc_limits", null ]
      ] ]
    ] ],
    [ "SPI master", "hardware_driver_spi_master.html", [
      [ "Driver configuration", "hardware_driver_spi_master.html#spi_master_configuration", null ],
      [ "Basic usage", "hardware_driver_spi_master.html#hardware_driver_spi_basic", null ],
      [ "Advanced usage", "hardware_driver_spi_master.html#hardware_driver_spi_advanced", [
        [ "Starting a transfer from PPI", "hardware_driver_spi_master.html#hardware_driver_spi_ppi", null ],
        [ "Repeated transfers", "hardware_driver_spi_master.html#hardware_driver_spi_repeated", null ]
      ] ],
      [ "Events", "hardware_driver_spi_master.html#Events", null ]
    ] ],
    [ "SPI slave", "hardware_driver_spi_slave.html", [
      [ "Driver configuration", "hardware_driver_spi_slave.html#spi_slave_configuration", null ],
      [ "Using the SPI slave driver", "hardware_driver_spi_slave.html#spi_slave_using", null ]
    ] ],
    [ "SWI", "hardware_driver_swi.html", [
      [ "Usage", "hardware_driver_swi.html#swi_usage", [
        [ "Initialization", "hardware_driver_swi.html#swi_usage_init", null ],
        [ "Allocation", "hardware_driver_swi.html#swi_usage_alloc", null ],
        [ "Event handler and flags", "hardware_driver_swi.html#swi_usage_event", null ],
        [ "Special options", "hardware_driver_swi.html#swi_special_options", null ]
      ] ],
      [ "Example", "hardware_driver_swi.html#swi_usage_example", null ],
      [ "Usage with a SoftDevice", "hardware_driver_swi.html#swi_with_sd", null ]
    ] ],
    [ "Timer", "hardware_driver_timer.html", null ],
    [ "TWI master", "hardware_driver_twi.html", [
      [ "Initialization", "hardware_driver_twi.html#hardware_driver_twi_init", null ],
      [ "Basic usage", "hardware_driver_twi.html#hardware_driver_twi_basic", null ],
      [ "Advanced usage", "hardware_driver_twi.html#hardware_driver_twi_advanced", [
        [ "Starting a transfer from PPI", "hardware_driver_twi.html#hardware_driver_twi_ppi", null ],
        [ "Repeated transfers", "hardware_driver_twi.html#hardware_driver_twi_repeated", null ]
      ] ],
      [ "Events", "hardware_driver_twi.html#Events", null ]
    ] ],
    [ "TWI slave", "hardware_driver_twis_slave.html", [
      [ "Driver configuration", "hardware_driver_twis_slave.html#twi_slave_configuration", null ],
      [ "Modes of operation", "hardware_driver_twis_slave.html#twi_slave_modes", [
        [ "Synchronous mode", "hardware_driver_twis_slave.html#twi_slave_synchronous_mode", null ],
        [ "Asynchronous mode", "hardware_driver_twis_slave.html#twi_slave_asynchronous_mode", null ]
      ] ]
    ] ],
    [ "UART", "hardware_driver_uart.html", [
      [ "Driver configuration", "hardware_driver_uart.html#uart_configuration", null ],
      [ "Using the UART driver", "hardware_driver_uart.html#uart_using", [
        [ "Blocking mode", "hardware_driver_uart.html#uart_blocking", null ],
        [ "Non-blocking mode", "hardware_driver_uart.html#uart_nonblocking", null ],
        [ "Enabling RX without providing data buffer", "hardware_driver_uart.html#uart_rx_enabling", null ]
      ] ]
    ] ],
    [ "USBD driver", "hardware_driver_usbd.html", [
      [ "Using the USBD driver", "hardware_driver_usbd.html#hardware_driver_usbd_usage", null ]
    ] ],
    [ "WDT", "hardware_driver_wdt.html", null ]
];