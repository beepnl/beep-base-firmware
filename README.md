# BEEP base v3 - Firmware
Firmware files for the BEEP base PCB BLE/LoRaWAN API of the [Beep measurement system v3](https://github.com/beepnl/measurement-system-v3).

The BEEP base v3 firmware is part of the complete [BEEP system](https://github.com/beepnl/BEEP)

# Firmware
The firmware in this repo is to be installed on the BEEP base v3 PCB
![BEEP base - PCB v3](https://github.com/beepnl/measurement-system-v3/blob/master/hardware/pcb/beep-pcb-v3.jpg)

## Versions
The different 'YYYY_MM_DD_BeepBase_vX.X.X' folders contain the firmware versions. Each folder contains a Readme.md with the (Dutch) changes compared to the previous versions and the /Firmware folder. Inside it's /Firmware folder, the HEX bootloader and the standard BEEP application are available for upload to the PCB.

## Uploading a firmware version to the PCB
You can upload the firmware from the /Firmware folder inside the firmware version folder to the PCB:

- via Bluetooth from the [BEEP base iOS](https://apps.apple.com/us/app/beep-base/id1495605010) app. It already includes all stable app versions from this repo.
- via Bluetooth with the [Nordic nRF connect app](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-mobile). NB: The .zip files (*Beepbase_sd_boot_app.zip* and *Beepbase_app.zip (without bootloader)*) contain the uploadable firmware.
- via Serial using a [Nordic nRF52 Development Kit](https://www.nordicsemi.com/Products/Development-hardware/nrf52-dk) using the BeepBase.hex file.


After uploading, you will have the full BLE and LoRa API to your possession to configre the PCB as you like. 

# Firmware BLE/LoRaWAN API
To use the BEEP base PCB API (resulting from the firmware installation) over Bluetooth Low Energy (BLE) or LoRaWAN, read the manual:

- [Dutch manual](https://github.com/beepnl/beep-base-firmware/blob/master/BEEP%20base%20-%20ID190222-02%20-%20Firmware%20-%20Dutch.pdf)
- [English manual](https://github.com/beepnl/beep-base-firmware/blob/master/BEEP%20base%20-%20ID190222-02%20-%20Firmware%20-%20English.pdf)

Is shows all firmware and BLE API features. You will be amazed :-) 

## Building new firmware from Source

In the `Source/Code` folder, there is the Nordic nRF code to build your own version of the firmware.

In the `Source/Datasheets` folder, there is the list of used sensor specifications that are present on the PCB.

Before coding:

- Please make sure you install the Nordic SDK 15.3 and place it in the folder `firmware/Source/Code/nRF/nRF5_SDK_15.3`
- Let `nrfutil` create the `firmware/Source/Code/nRF/Key` folder for you with files: `private.key` and `public_key.c`
- Read from page 60 onward in the [English firmware manual](https://github.com/beepnl/beep-base-firmware/blob/master/BEEP%20base%20-%20ID190222-02%20-%20Firmware%20-%20English.pdf)

Happy coding!

# Contribution
Please do a pull request for every sensor you add :-)
