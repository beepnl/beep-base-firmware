###
### Requires NRF Command Line Tools: https://www.nordicsemi.com/Products/Development-tools/nrf-command-line-tools/download
###

echo "\nGenerating DFU files..."; sleep 2;
nrfutil settings generate --family NRF52840 --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --application-version 1 --bootloader-version 1 --bl-settings-version 1 ../../BTLDR/ses/Output/Release/Exe/BL_settings.hex;
mergehex --merge ../../BTLDR/ses/Output/Release/Exe/BL_settings.hex ../../BTLDR/ses/Output/Release/Exe/beep_bootloader.hex --output ../../BTLDR/ses/Output/Release/Exe/../../BTLDR_settings.hex;
mergehex --merge ../../PRJ/ses/Output/Release/Exe/BeepBase.hex ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --output ../../BTLDR/ses/Output/Release/Exe/APP_SD.hex;
mergehex --merge ../../BTLDR/ses/Output/Release/Exe/beep_bootloader.hex ../../BTLDR/ses/Output/Release/Exe/APP_SD.hex --output ../../BTLDR/ses/Output/Release/Exe/APP_SD_BL.hex;
mergehex --merge ../../BTLDR/ses/Output/Release/Exe/BL_settings.hex ../../BTLDR/ses/Output/Release/Exe/APP_SD_BL.hex --output ../../BTLDR/ses/Output/Release/Exe/Beepbase.hex;
echo ""; sleep 1;
nrfutil pkg generate --hw-version 52 --application-version 1 --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --sd-req 0xB6 --key-file ../../Key/private.key Beepbase_app.zip;
echo ""; sleep 1;
nrfutil pkg generate --application ../../BTLDR/ses/Output/Release/Exe/Beepbase.hex --application-version 1  --bootloader ../../BTLDR/ses/Output/Release/Exe/beep_bootloader.hex --bootloader-version 1 --softdevice ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --hw-version 52 --sd-req 0xB6 --sd-id 0xB6 --key-file ../../Key/private.key Beepbase_sd_boot_app.zip
