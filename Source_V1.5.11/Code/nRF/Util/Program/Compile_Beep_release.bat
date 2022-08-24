echo off
echo Removing all previous intermediate and output files to prevent confusion when compilation fails without the user noticing.
if exist BL_settings.hex      		del /F BL_settings.hex
if exist BT_SET.hex           		del /F BT_SET.hex
if exist BT_SET.hex           		del /F BTLDR_settings.hex
if exist APP_SD.hex 		  		del /F APP_SD.hex
if exist APP_SD_BL.hex 		  		del /F APP_SD_BL.hex
if exist Beepbase.hex 				del /F "Beepbase.hex"
if exist Beepbase_app.zip 			del /F "Beepbase_app.zip"
if exist Beepbase_sd_boot_app.zip 	del /F "Beepbase_sd_boot_app.zip"




start /B /wait C:/"Program Files"/SEGGER/"SEGGER Embedded Studio for ARM 3.40"/bin/emBuild.exe -config "Release" ../../PRJ/ses/Beepbase.emProject
if %ERRORLEVEL% NEQ 0 (
	echo Application compilation failed with error: %ERRORLEVEL%
	pause
	exit
)
echo Application compiled

start /B /wait C:/"Program Files"/SEGGER/"SEGGER Embedded Studio for ARM 3.40"/bin/emBuild.exe -config "Release" ../../BTLDR/ses/beep_bootloader.emProject
if %ERRORLEVEL% NEQ 0 (
	echo Bootloader compilation failed with error: %ERRORLEVEL%
	pause
	exit
)
echo Bootloader compiled

start /B /wait ../nrfutil.exe settings generate --family NRF52840 --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --application-version 1 --bootloader-version 1 --bl-settings-version 1 BL_settings.hex
if %ERRORLEVEL% NEQ 0 (
	echo Merging failed with error: %ERRORLEVEL%
	pause
	exit
)
echo application settings generated.

start /B /wait ../mergehex.exe --merge BL_settings.hex ../../BTLDR/ses/Output/Release/Exe/beep_bootloader.hex --output BTLDR_settings.hex
if %ERRORLEVEL% NEQ 0 (
	echo Merging Bootloader + settings with error: %ERRORLEVEL%
	pause
	exit
)

start /B /wait ../mergehex.exe --merge ../../PRJ/ses/Output/Release/Exe/BeepBase.hex ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --output APP_SD.hex
if %ERRORLEVEL% NEQ 0 (
	echo Merging failed with error: %ERRORLEVEL%
	pause
	exit
)

start /B /wait ../mergehex.exe --merge ../../BTLDR/ses/Output/Release/Exe/beep_bootloader.hex APP_SD.hex --output APP_SD_BL.hex
if %ERRORLEVEL% NEQ 0 (
	echo Merging failed with error: %ERRORLEVEL%
	pause
	exit
)

start /B /wait ../mergehex.exe --merge BL_settings.hex App_SD_BL.hex --output Beepbase.hex
if %ERRORLEVEL% NEQ 0 (
	echo Merging failed with error: %ERRORLEVEL%
	pause
	exit
)

start /B /wait ../nrfutil.exe pkg generate --hw-version 52 --application-version 1 --application ../../PRJ/ses/Output/Release/Exe/BeepBase.hex --sd-req 0xB6 --key-file ../../Key/private.key Beepbase_app.zip 
if %ERRORLEVEL% NEQ 0 (
	echo Merging failed with error: %ERRORLEVEL%
	pause
	exit
)

echo --bootloader ../../BTLDR/ses/Output/Release/Exe/beep_bootloader.hex --bootloader-version 1
start /B /wait ../nrfutil.exe pkg generate --application ../../PRJ/ses/Output/Release/Exe/Beepbase.hex --application-version 1  --bootloader ../../BTLDR/ses/Output/Release/Exe/beep_bootloader.hex --bootloader-version 1 --softdevice ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex --hw-version 52 --sd-req 0xB6 --sd-id 0xB6 --key-file ../../Key/private.key Beepbase_sd_boot_app.zip
if %ERRORLEVEL% NEQ 0 (
	echo Merging SD+BOOT+APP failed with error: %ERRORLEVEL%
	pause
	exit
)

pause