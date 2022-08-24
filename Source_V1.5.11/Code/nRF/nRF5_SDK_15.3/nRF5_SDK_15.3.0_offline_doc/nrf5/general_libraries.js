var general_libraries =
[
    [ "ANT libraries", "lib_ant.html", "lib_ant" ],
    [ "Atomic FIFO", "lib_atfifo.html", [
      [ "Usage", "lib_atfifo.html#lib_atfifo_usage", null ],
      [ "Requirements", "lib_atfifo.html#lib_atfifo_reqs", null ],
      [ "Handling interruptions", "lib_atfifo.html#lib_atfifo_interrupt", null ],
      [ "Multithreaded preemptive operating system limitation", "lib_atfifo.html#lib_atfifo_multi", null ],
      [ "Implementation details", "lib_atfifo.html#lib_atfifo_implementation", null ]
    ] ],
    [ "BLE libraries", "lib_ble.html", "lib_ble" ],
    [ "BLE Services", "ble_services.html", "ble_services" ],
    [ "Block allocator library", "lib_balloc.html", [
      [ "Usage", "lib_balloc.html#lib_balloc_usage", null ]
    ] ],
    [ "Bootloader and DFU modules", "lib_bootloader_modules.html", "lib_bootloader_modules" ],
    [ "Board Support Package (BSP)", "lib_bsp.html", "lib_bsp" ],
    [ "Button handling library", "lib_button.html", null ],
    [ "Capacitive Sensor library", "lib_csense.html", [
      [ "Resource usage", "lib_csense.html#csense_lib_resource", null ],
      [ "Initialization and starting", "lib_csense.html#csense_lib_init", null ],
      [ "Reading values from sensors", "lib_csense.html#csense_lib_reading", null ],
      [ "Limitations", "lib_csense.html#csense_lib_limitations", null ],
      [ "Example", "lib_csense.html#csense_lib_example", null ]
    ] ],
    [ "Capacitive Sensor low-level library", "lib_csense_lowlevel.html", [
      [ "Implementation details", "lib_csense_lowlevel.html#csense_implementation", null ],
      [ "Resource usage", "lib_csense_lowlevel.html#csense_drv_resource", null ],
      [ "Hardware setup", "lib_csense_lowlevel.html#csense_drv_hardware", null ],
      [ "Initialization and starting", "lib_csense_lowlevel.html#csense_drv_init", null ],
      [ "Reading values from the pads", "lib_csense_lowlevel.html#csense_reading", null ],
      [ "Example", "lib_csense_lowlevel.html#csense_driver_example", null ]
    ] ],
    [ "Command Line Interface library", "lib_cli.html", [
      [ "Usage", "lib_cli.html#lib_cli_usage", null ],
      [ "Commands", "lib_cli.html#lib_cli_commands", [
        [ "Creating commands", "lib_cli.html#lib_cli_commands_sub_creation", null ],
        [ "Commands execution", "lib_cli.html#lib_cli_commands_sub_execution", null ],
        [ "Built-in commands", "lib_cli.html#lib_cli_commands_sub_built_in", null ],
        [ "Commands help", "lib_cli.html#lib_cli_commands_help", null ],
        [ "Print functions", "lib_cli.html#lib_cli_printing", null ],
        [ "Wildcards", "lib_cli.html#lib_cli_commands_wildcards", null ]
      ] ],
      [ "Meta keys", "lib_cli.html#lib_cli_meta_keys", null ],
      [ "Terminal settings", "lib_cli.html#lib_cli_terminal_settings", [
        [ "UART and USB settings", "lib_cli.html#lib_cli_terminal_settings_uart", null ],
        [ "RTT settings", "lib_cli.html#lib_cli_terminal_settings_RTT", null ]
      ] ],
      [ "Usage examples", "lib_cli.html#lib_cli_examples", null ]
    ] ],
    [ "Cryptography library - nrf_crypto", "lib_crypto.html", "lib_crypto" ],
    [ "Error code to string converter", "lib_strerror.html", null ],
    [ "Error module", "lib_error.html", null ],
    [ "FIFO library", "lib_fifo.html", [
      [ "Initializing FIFO", "lib_fifo.html#lib_app_fifo_init", null ],
      [ "FIFO instance", "lib_fifo.html#lib_app_fifo_struct", null ],
      [ "Adding an element", "lib_fifo.html#lib_app_fifo_put", null ],
      [ "Fetching an element", "lib_fifo.html#lib_app_fifo_get", null ],
      [ "Empty buffer", "lib_fifo.html#lib_app_fifo_empty", null ],
      [ "Full buffer", "lib_fifo.html#lib_app_fifo_full", null ],
      [ "Multi-byte operations", "lib_fifo.html#lib_app_fifo_multibyte", null ],
      [ "FIFO readable/writeable size operations", "lib_fifo.html#lib_app_fifo_size", null ]
    ] ],
    [ "Flash handling library", "lib_flash.html", null ],
    [ "fprintf library", "lib_fprintf.html", null ],
    [ "Gazell Pairing library", "lib_gzp.html", null ],
    [ "GFX library", "lib_gfx.html", [
      [ "LCD abstraction", "lib_gfx.html#gfx_lib_lcd", null ],
      [ "Initialization and starting", "lib_gfx.html#gfx_lib_init", null ],
      [ "Drawing objects", "lib_gfx.html#gfx_lib_drawing", null ],
      [ "LCD frame buffer", "lib_gfx.html#gfx_lib_frame_buffer", null ],
      [ "GFX fonts", "lib_gfx.html#gfx_lib_fonts", null ],
      [ "Usage example", "lib_gfx.html#gfx_lib_example", null ]
    ] ],
    [ "GPIOTE handling library", "lib_gpiote.html", [
      [ "Initializing GPIOTE module", "lib_gpiote.html#lib_app_gpiote_init", null ],
      [ "Registering with GPIOTE", "lib_gpiote.html#lib_app_gpiote_register", null ],
      [ "Enable/Disable GPIOTE", "lib_gpiote.html#lib_app_gpiote_enable_disable", null ],
      [ "Reading GPIOTE State", "lib_gpiote.html#lib_gpiote_get_states", null ]
    ] ],
    [ "HardFault handling library", "lib_hardfault.html", [
      [ "Using the library", "lib_hardfault.html#lib_hardfault_usage", null ],
      [ "Stack pointer", "lib_hardfault.html#lib_hardfault_sp", [
        [ "Process stack", "lib_hardfault.html#lib_hardfault_sp_process", null ],
        [ "Main stack", "lib_hardfault.html#lib_hardfault_sp_main", null ]
      ] ]
    ] ],
    [ "IEEE 802.15.4 MAC library", "lib_802_15_4.html", [
      [ "Physical layer", "lib_802_15_4.html#phy_15_4_library", null ],
      [ "MAC layer", "lib_802_15_4.html#mac_15_4_library", [
        [ "Initialization and starting", "lib_802_15_4.html#mac_15_4_init", null ]
      ] ],
      [ "Security abstraction library", "lib_802_15_4.html#sec_15_4_library", [
        [ "Implementation details", "lib_802_15_4.html#sec_15_4_implementation", null ],
        [ "Initialization and starting", "lib_802_15_4.html#sec_15_4_init", null ],
        [ "Encrypting and decrypting", "lib_802_15_4.html#sec_15_4_reading", null ]
      ] ],
      [ "System abstraction library", "lib_802_15_4.html#sys_15_4_library", [
        [ "Initialization and starting", "lib_802_15_4.html#sys_15_4_init", null ]
      ] ],
      [ "802.15.4 MAC library usage example", "lib_802_15_4.html#example_15_4_usage", null ]
    ] ],
    [ "Infineon I2C Protocol Stack Library", "lib_ifx_i2c.html", [
      [ "Stack Layers", "lib_ifx_i2c.html#Protocol", [
        [ "Transport Layer (TL)", "lib_ifx_i2c.html#ifx_i2c_transport", null ],
        [ "Data Link Layer (DL)", "lib_ifx_i2c.html#ifx_i2c_data_link", null ],
        [ "Physical Layer (PL)", "lib_ifx_i2c.html#ifx_i2c_physical", null ]
      ] ],
      [ "Platform Abstraction Layer (PAL)", "lib_ifx_i2c.html#ifx_i2c_hal", null ]
    ] ],
    [ "Infineon OPTIGA™ Trust X Software Framework", "lib_ifx_optiga.html", [
      [ "Trust X Software Framework", "lib_ifx_optiga.html#lib_ifx_optiga_architecture", null ],
      [ "External resources and documentation", "lib_ifx_optiga.html#lib_ifx_optiga_links", null ],
      [ "Platform-specific configuration", "lib_ifx_optiga.html#lib_ifx_optiga_readme", [
        [ "Required configuration", "lib_ifx_optiga.html#lib_ifx_optiga_readme_configuration", null ],
        [ "Hardware compatibility", "lib_ifx_optiga.html#lib_ifx_optiga_readme_compatiblity", null ]
      ] ],
      [ "Trust X Crypt and Util API (native API)", "lib_ifx_optiga.html#lib_fix_optiga_native", [
        [ "Initialization", "lib_ifx_optiga.html#lib_fix_optiga_native_init", null ],
        [ "Data objects and personalization", "lib_ifx_optiga.html#lib_fix_optiga_native_perso", null ]
      ] ],
      [ "Infineon I2C Protocol Stack Library", "lib_ifx_optiga.html#lib_ifx_optiga_i2c", [
        [ "Protocol stack layers", "lib_ifx_optiga.html#lib_ifx_optiga_i2c_layers", [
          [ "Transport Layer (TL)", "lib_ifx_optiga.html#lib_ifx_optiga_i2c_layers_tl", null ],
          [ "Data Link Layer (DL)", "lib_ifx_optiga.html#lib_ifx_optiga_i2c_layers_dl", null ],
          [ "Physical Layer (PL)", "lib_ifx_optiga.html#lib_ifx_optiga_i2c_layers_pl", null ]
        ] ],
        [ "Platform Abstraction Layer (PAL)", "lib_ifx_optiga.html#lib_ifx_optiga_i2c_layers_pal", null ]
      ] ]
    ] ],
    [ "IoT libraries", "lib_iot.html", "lib_iot" ],
    [ "LED softblink library", "lib_led_softblink.html", [
      [ "Starting LED softblink", "lib_led_softblink.html#lib_led_softblink_init", null ],
      [ "Example", "lib_led_softblink.html#usage_example", null ],
      [ "Usage with a SoftDevice", "lib_led_softblink.html#usage_with_sd", null ]
    ] ],
    [ "Logger module", "lib_nrf_log.html", [
      [ "Log processing", "lib_nrf_log.html#nrf_log_processing", [
        [ "In-place processing", "lib_nrf_log.html#lib_nrf_log_in_place", null ],
        [ "Deferred processing", "lib_nrf_log.html#lib_nrf_log_deferred", null ]
      ] ],
      [ "Logger configuration", "lib_nrf_log.html#nrf_log_config", null ],
      [ "Usage", "lib_nrf_log.html#nrf_log_usage", [
        [ "Controlling the logger", "lib_nrf_log.html#nrf_log_usage_control", null ],
        [ "Logging", "lib_nrf_log.html#nrf_log_usage_logging", [
          [ "Logging customization", "lib_nrf_log.html#nrf_log_usage_logging_customization", null ]
        ] ]
      ] ],
      [ "Filtering logs on instance level", "lib_nrf_log.html#nrf_log_instance_logging", [
        [ "Setting up instance level filtering", "lib_nrf_log.html#nrf_log_instance_setup", null ],
        [ "Instance logging", "lib_nrf_log.html#nrf_log_instance_usage", null ]
      ] ],
      [ "Performance", "lib_nrf_log.html#nrf_log_performance", null ],
      [ "Logger backend interface", "lib_nrf_log.html#nrf_log_custom", [
        [ "Flash logger backend", "lib_nrf_log.html#nrf_log_backend_flash", [
          [ "Flash operations", "lib_nrf_log.html#nrf_log_backend_flash_ops", null ],
          [ "Flashlog operations", "lib_nrf_log.html#nrf_log_backend_flash_flashlog", null ],
          [ "Crashlog operations", "lib_nrf_log.html#nrf_log_backend_flash_crashlog", null ],
          [ "Command Line Interface support", "lib_nrf_log.html#nrf_log_backend_flash_cli", null ]
        ] ]
      ] ],
      [ "Panic mode", "lib_nrf_log.html#nrf_log_panic", null ],
      [ "Command Line Interface", "lib_nrf_log.html#nrf_log_cli", null ]
    ] ],
    [ "Low-power PWM library", "lib_low_power_pwm.html", [
      [ "Generating a low-power PWM signal", "lib_low_power_pwm.html#lib_pwm_init", null ],
      [ "Limitations", "lib_low_power_pwm.html#limitations", null ],
      [ "Example", "lib_low_power_pwm.html#usage_example", null ],
      [ "Usage with a SoftDevice", "lib_low_power_pwm.html#usage_with_sd", null ]
    ] ],
    [ "Mapped flags", "lib_mapped_flags.html", null ],
    [ "Memory Manager", "lib_mem_manager.html", [
      [ "Memory allocation", "lib_mem_manager.html#MEM_MANAGER", null ],
      [ "Configuration parameters", "lib_mem_manager.html#MEM_MANAGER_CONF", [
        [ "MEM_MANAGER_ENABLE_LOGS", "lib_mem_manager.html#mem_manager_params_logs", null ],
        [ "MEM_MANAGER_ENABLE_DIAGNOSTICS", "lib_mem_manager.html#mem_manager_params_diagnostics", null ],
        [ "MEM_MANAGER_DIAGNOSTICS_LOGS_ONLY", "lib_mem_manager.html#mem_manager_params_diag_only", null ],
        [ "MEM_MANAGER_DISABLE_API_PARAM_CHECK", "lib_mem_manager.html#mem_manager_params_api_check", null ],
        [ "Block count", "lib_mem_manager.html#mem_manager_params_count", null ],
        [ "Block size", "lib_mem_manager.html#mem_manager_params_size", null ]
      ] ]
    ] ],
    [ "Memory object library", "lib_memobj.html", [
      [ "Implementation details", "lib_memobj.html#lib_memobj_implementation", [
        [ "Object", "lib_memobj.html#lib_memobj_object_structure", null ],
        [ "Multiple users", "lib_memobj.html#lib_memobj_multiple_users", null ]
      ] ],
      [ "Working with objects", "lib_memobj.html#lib_memobj_usage", null ]
    ] ],
    [ "MPU driver", "lib_mpu.html", [
      [ "Usage", "lib_mpu.html#lib_mpu_usage", null ],
      [ "CLI commands", "lib_mpu.html#lib_mpu_cli", null ]
    ] ],
    [ "NFC library and modules", "library_nfc.html", "library_nfc" ],
    [ "Power Management library", "lib_pwr_mgmt.html", [
      [ "CPU usage tracer", "lib_pwr_mgmt.html#cpu_usage_tracer", null ],
      [ "Standby timeout", "lib_pwr_mgmt.html#standby_timeout", null ],
      [ "Shutdown retry mechanism", "lib_pwr_mgmt.html#shutdown_retry_mechanism", null ],
      [ "Schedule handling library support", "lib_pwr_mgmt.html#app_scheduler_support", null ],
      [ "FPU support", "lib_pwr_mgmt.html#fpu_support", null ]
    ] ],
    [ "PWM library", "lib_pwm.html", [
      [ "Resource usage", "lib_pwm.html#lib_pwm_resources", null ],
      [ "Generating a low-power PWM signal", "lib_pwm.html#lib_pwm_init", null ],
      [ "Limitations", "lib_pwm.html#limitations", null ],
      [ "Example", "lib_pwm.html#usage_example", null ],
      [ "Usage with a SoftDevice", "lib_pwm.html#usage_with_sd", null ]
    ] ],
    [ "Queue library", "lib_queue.html", [
      [ "Creating a queue", "lib_queue.html#lib_queue_create", null ],
      [ "Using the queue", "lib_queue.html#mailbox_usage", null ]
    ] ],
    [ "Ring buffer library", "lib_ringbuf.html", [
      [ "Initialization", "lib_ringbuf.html#lib_ringbuf_init", null ],
      [ "Access through copying", "lib_ringbuf.html#lib_ringbuf_memcpy", null ],
      [ "Direct access to the memory", "lib_ringbuf.html#lib_ringbuf_nocpy", null ]
    ] ],
    [ "RTOS support", "lib_rtos.html", "lib_rtos" ],
    [ "Schedule handling library", "lib_scheduler.html", [
      [ "Requirements:", "lib_scheduler.html#app_scheduler_req", [
        [ "Logic in main context:", "lib_scheduler.html#main_context_logic", null ],
        [ "Logic in interrupt context:", "lib_scheduler.html#int_context_logic", null ]
      ] ],
      [ "Applications using the Scheduler", "lib_scheduler.html#seq_diagrams_sched", null ],
      [ "Applications not using the Scheduler", "lib_scheduler.html#seq_diagrams_no_sched", null ]
    ] ],
    [ "SD card library", "lib_sdcard.html", [
      [ "Configuration", "lib_sdcard.html#sdcard_config", null ],
      [ "Initialization and operation", "lib_sdcard.html#sdcard_init", null ],
      [ "Usage", "lib_sdcard.html#sdcard_usage", null ]
    ] ],
    [ "Sensor Data Simulator", "lib_sensorsim.html", null ],
    [ "Serial port library", "lib_serial.html", null ],
    [ "Serialization", "lib_serialization.html", "lib_serialization" ],
    [ "SHA-256 hash computation", "lib_sha256.html", null ],
    [ "Simple timer", "lib_simple_timer.html", null ],
    [ "SLIP library", "lib_slip.html", [
      [ "SLIP basics", "lib_slip.html#lib_slip_general", null ],
      [ "Encoding a SLIP packet", "lib_slip.html#lib_slip_encoding", null ],
      [ "Decoding a SLIP packet", "lib_slip.html#lib_slip_decoding", null ]
    ] ],
    [ "SoftDevice Handler library", "lib_softdevice_handler.html", [
      [ "Library components", "lib_softdevice_handler.html#lib_sdh_components", [
        [ "nrf_sdh component", "lib_softdevice_handler.html#lib_sdh_components_nrf_sdh", null ],
        [ "nrf_sdh_soc component", "lib_softdevice_handler.html#lib_sdh_components_nrf_sdh_soc", null ],
        [ "nrf_sdh_ant component", "lib_softdevice_handler.html#lib_sdh_components_nrf_sdh_ant", null ],
        [ "nrf_sdh_ble component", "lib_softdevice_handler.html#lib_sdh_components_nrf_sdh_ble", null ]
      ] ],
      [ "Functions of the library components", "lib_softdevice_handler.html#lib_sdh_components_functions", null ],
      [ "Registering with the nrf_sdh_* components", "lib_softdevice_handler.html#lib_sdh_register", [
        [ "Observers and their priorities", "lib_softdevice_handler.html#lib_sdh_register_observer", null ],
        [ "Assigning priorities", "lib_softdevice_handler.html#lib_sdh_priority", null ]
      ] ],
      [ "Example of enabling the SoftDevice and BLE stack", "lib_softdevice_handler.html#lib_sdh_stack_init", null ],
      [ "Example of registering a BLE observer", "lib_softdevice_handler.html#lib_sdh_ble_example", null ],
      [ "Example of registering a SoftDevice state observer", "lib_softdevice_handler.html#lib_sdh_state_example", null ],
      [ "Delaying a SoftDevice's state change", "lib_softdevice_handler.html#lib_sdh_example_delay", null ],
      [ "Example of registering a SoftDevice state request observer", "lib_softdevice_handler.html#lib_sdh_request_example", null ],
      [ "Message sequence chart for SoftDevice state requests", "lib_softdevice_handler.html#lib_sdh_example_delay_msc", null ],
      [ "Preprocessor defines", "lib_softdevice_handler.html#compiler_defines", null ]
    ] ],
    [ "Sorted list library", "lib_sortlist.html", [
      [ "Creating a list", "lib_sortlist.html#lib_sortlist_create", null ],
      [ "Using the list", "lib_sortlist.html#sortlist_usage", null ]
    ] ],
    [ "SPI transaction manager", "lib_nrf_spi_mngr.html", null ],
    [ "Stack guard", "lib_stack_guard.html", null ],
    [ "Supervisor call interface", "lib_svc.html", [
      [ "Supervisor calls", "lib_svc.html#lib_svc_details", [
        [ "Indirect Supervisor calls", "lib_svc.html#lib_svc_details_indirect", null ],
        [ "Limitations", "lib_svc.html#lib_svc_limitations", null ],
        [ "Redirecting Supervisor calls to an external application", "lib_svc.html#lib_svc_redirect_handler", null ],
        [ "Calling an external application by changing the vector table base address", "lib_svc.html#lib_svc_redirect_handler_code", null ]
      ] ],
      [ "Reserved SVC numbers", "lib_svc.html#lib_svc_reserved_svc_number", [
        [ "Reserved SVC numbers in the Secure DFU Bootloader", "lib_svc.html#lib_svc_reserved_svc_numbers_bootloader", null ]
      ] ],
      [ "Supervisor call interface", "lib_svc.html#lib_svc_caller", [
        [ "Declaring an SVC function", "lib_svc.html#lib_svc_caller_code", null ],
        [ "Declaring an indirect SVC function", "lib_svc.html#lib_svc_caller_indirect_code", null ]
      ] ],
      [ "Supervisor call handler interface", "lib_svc.html#lib_svc_handler", [
        [ "Registering an SVC function in the SVC handler", "lib_svc.html#lib_svc_handler_code", null ],
        [ "Example registration", "lib_svc.html#lib_svc_indirect_handler_code", null ]
      ] ],
      [ "Asynchronous Supervisor interface", "lib_svc.html#lib_svci_async_interface", [
        [ "Limitations", "lib_svc.html#lib_svci_async_interface_limitations", null ],
        [ "Shared data", "lib_svc.html#lib_svc_async_shared", null ],
        [ "Asynchronous Supervisor interface caller", "lib_svc.html#lib_svc_async_caller", null ],
        [ "Registration of a caller", "lib_svc.html#lib_svc_async_caller_code", null ],
        [ "Asynchronous Supervisor interface handler", "lib_svc.html#lib_svc_async_handler", null ],
        [ "Registration of a handler", "lib_svc.html#lib_svc_async_handler_code", null ],
        [ "Convenience functions", "lib_svc.html#lib_svci_async_interface_convenience", null ],
        [ "Convenience functions example", "lib_svc.html#lib_Svc_async_interface_convenience_code", null ]
      ] ]
    ] ],
    [ "Timer library", "lib_timer.html", [
      [ "Usage", "lib_timer.html#lib_timer_usage", [
        [ "Debugging", "lib_timer.html#lib_timer_debugging", null ]
      ] ],
      [ "Migration note", "lib_timer.html#lib_timer_migration", null ]
    ] ],
    [ "Transport services", "transport_libraries.html", "transport_libraries" ],
    [ "TWI transaction manager", "lib_nrf_twi_mngr.html", null ],
    [ "nrf_cc310 and ARM TrustZone CryptoCell", "lib_cryptocell.html", [
      [ "Using the CryptoCell library", "lib_cryptocell.html#lib_cryptocell_usage", [
        [ "Supported IDEs", "lib_cryptocell.html#lib_nrf_cc310_compiler", null ]
      ] ]
    ] ],
    [ "Experimental: Flash Data Storage", "lib_fds.html", "lib_fds" ],
    [ "Experimental: Flash Storage", "lib_fstorage.html", [
      [ "Defining an instance", "lib_fstorage.html#lib_fstorage_instance", null ],
      [ "Initialization", "lib_fstorage.html#lib_fstorage_init", null ],
      [ "Writing data", "lib_fstorage.html#lib_fstorage_write", null ],
      [ "Reading data", "lib_fstorage.html#lib_fstorage_read", null ],
      [ "Erasing flash pages", "lib_fstorage.html#lib_fstorage_erase", null ],
      [ "Backends", "lib_fstorage.html#lib_fstorage_backends", [
        [ "SoftDevice backend", "lib_fstorage.html#lib_fstorage_sd", null ],
        [ "NVMC backend", "lib_fstorage.html#lib_fstorage_nvmc", null ]
      ] ],
      [ "Example", "lib_fstorage.html#lib_fstorage_example", null ]
    ] ],
    [ "Experimental: Section variables", "lib_section_vars.html", "lib_section_vars" ],
    [ "Experimental: Task manager", "lib_task_manager.html", [
      [ "Usage", "lib_task_manager.html#task_manager_usage", null ],
      [ "Efficient power management", "lib_task_manager.html#task_manager_power_mngmt", null ]
    ] ],
    [ "USB device library", "lib_usbd.html", "lib_usbd" ]
];