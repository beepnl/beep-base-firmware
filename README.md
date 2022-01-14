# BEEP base v3 - Firmware
Firmware files for the [Beep measurement system v3](https://github.com/beepnl/measurement-system-v3).

## Firmware
The firmware in this repo is to be installed on the BEEP base v3 PCB
![BEEP base - PCB v3](https://github.com/beepnl/measurement-system-v3/blob/master/hardware/pcb/beep-pcb-v3.jpg)

In the different 'firmware' folders, the HEX bootloader and the standard BEEP application are available for upload to the PCB.

You can upload the firmware via Bluetooth with the Nordic NRF connect app. After uploading, you will have the full BLE and LoRa API to your possession to configre the PCB as you like. 

## Firmware BLE/LoRaWAN API
To use the BEEP base PCB firmware over Bluetooth Low Energy (BLE) or LoRaWAN, read the manuals:

- [Dutch manual](https://github.com/beepnl/beep-base-firmware/blob/master/BEEP%20base%20-%20ID190222-02%20-%20Firmware%20-%20Dutch.pdf)
- [English manual](https://github.com/beepnl/beep-base-firmware/blob/master/BEEP%20base%20-%20ID190222-02%20-%20Firmware%20-%20English.pdf)

Is shows all firmware and BLE API features. You will be amazed :-) 

### firmware/Source

In the `Source/Code` folder, there is the Nordic nRF code to build your own version of the firmware.

In the `Source/Datasheets` folder, there is the list of used sensor specifications that are present on the PCB.

Before coding:

- Please make sure you install the Nordic SDK 15.3 and place it in the folder `firmware/Source/Code/nRF/nRF5_SDK_15.3`
- Let `nrfutil` create the `firmware/Source/Code/nRF/Key` folder for you with files: `private.key` and `public_key.c`
- Read from page 60 onward in the [English firmware manual](https://github.com/beepnl/beep-base-firmware/blob/master/BEEP%20base%20-%20ID190222-02%20-%20Firmware%20-%20English.pdf)


Happy coding!

Please do a pull request for every sensor you add :-)
