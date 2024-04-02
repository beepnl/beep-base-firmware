###
### Requires NRF Command Line Tools: https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download
###
### Note: install "nrf5sdk-tools" module if using nrfutil version >7
###

echo "\nGenerating DFU files..."

nrfutil nrf5sdk-tools settings generate --family NRF52840 --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --application-version 1 --bootloader-version 2 --bl-settings-version 2 --sd-boot-validation=NO_VALIDATION --softdevice ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --app-boot-validation=NO_VALIDATION ../../BTLDR/ses/Output/Release_skip_CRC/Exe/BL_settings.hex

mergehex --merge ../../BTLDR/ses/Output/Release_skip_CRC/Exe/BL_settings.hex ../../BTLDR/ses/Output/Release_skip_CRC/Exe/beep_bootloader.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/../../BTLDR_settings.hex
mergehex --merge ../../PRJ/ses/Output/Release/Exe/BeepBase.hex ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD.hex
mergehex --merge ../../BTLDR/ses/Output/Release_skip_CRC/Exe/beep_bootloader.hex ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD_BL.hex
mergehex --merge ../../BTLDR/ses/Output/Release_skip_CRC/Exe/BL_settings.hex ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD_BL.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/Beepbase.hex

echo ""

# No CRC validation to allow DFU from any previous version!

# build DFU package with application only
nrfutil nrf5sdk-tools pkg generate --hw-version 52 --application-version 1  --debug-mode --app-boot-validation=NO_VALIDATION --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --sd-req 0xB6 --key-file ../../Key/private.key Beepbase_app.zip

# also build DFU package from app + softdevice + bootloader
nrfutil nrf5sdk-tools pkg generate --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --application-version 2 --bootloader ../../BTLDR/ses/Output/Release_skip_CRC/Exe/beep_bootloader.hex --sd-boot-validation=NO_VALIDATION --app-boot-validation=NO_VALIDATION --debug-mode --app-boot-validation=NO_VALIDATION --bootloader-version 2 --softdevice ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --hw-version 52 --sd-req 0xB6 --sd-id 0xB6 --key-file ../../Key/private.key  Beepbase_sd_boot_app.zip
###
### Requires NRF Command Line Tools: https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download
###

echo "\nGenerating DFU files..."
sleep 2

nrfutil nrf5sdk-tools settings generate --family NRF52840 --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --application-version 1 --bootloader-version 2 --bl-settings-version 2 --sd-boot-validation=NO_VALIDATION --softdevice ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --app-boot-validation=NO_VALIDATION ../../BTLDR/ses/Output/Release_skip_CRC/Exe/BL_settings.hex

mergehex --merge ../../BTLDR/ses/Output/Release_skip_CRC/Exe/BL_settings.hex ../../BTLDR/ses/Output/Release_skip_CRC/Exe/beep_bootloader.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/../../BTLDR_settings.hex

mergehex --merge ../../PRJ/ses/Output/Release/Exe/BeepBase.hex ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD.hex

mergehex --merge ../../BTLDR/ses/Output/Release_skip_CRC/Exe/beep_bootloader.hex ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD_BL.hex

mergehex --merge ../../BTLDR/ses/Output/Release_skip_CRC/Exe/BL_settings.hex ../../BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD_BL.hex --output ../../BTLDR/ses/Output/Release_skip_CRC/Exe/Beepbase.hex

echo ""
sleep 1

nrfutil nrf5sdk-tools pkg generate --hw-version 52 --application-version 1  --debug-mode --app-boot-validation=NO_VALIDATION --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --sd-req 0xB6 --key-file ../../Key/private.key Beepbase_app.zip

echo ""
sleep 1

nrfutil nrf5sdk-tools pkg generate --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --application-version 2 --bootloader ../../BTLDR/ses/Output/Release_skip_CRC/Exe/beep_bootloader.hex --sd-boot-validation=NO_VALIDATION --app-boot-validation=NO_VALIDATION --debug-mode --app-boot-validation=NO_VALIDATION --bootloader-version 2 --softdevice ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --hw-version 52 --sd-req 0xB6 --sd-id 0xB6 --key-file ../../Key/private.key  Beepbase_sd_boot_app.zip
