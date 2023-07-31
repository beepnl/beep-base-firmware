2021-08-10 Release of version 1.5.11
- Hardware ID can now be written in the UICR if the ATECC is not bestucked.
- Programming script is now modified to also write the 9 byte hardware ID in the UICR when programming the firmware.

2021-04-22 Release of version 1.5.10
- Firmware release of the circular log implementation. With previous firmware version, the BEEPBASE log data was written to the MX flash until it was full. Then the log had to be cleared to store more data. That has been changed with this update, so that when the log reaches the maximum size, it will start overwriting the old data back at position 0. The logOffset value can be read or written via BLE commands. When the log is read, the logOffset value can be used as an extra check to ensure that the transition between new and old data runs smoothly when importing the data.

- In addition, additional BLE commands have been implemented to fill the flash log with simulation values ​​for testing the functionality. A command has also been added to read the reset reason from the nRF52840 for extra feedback during testing.
New BLE commands:
- 40d/0x28 - READ_LOG_OFFSET
- 168d/0xA8 - WRITE_LOG_OFFSET
- 41d/0x29 - START_FILL LOG
- 42d/0x2A - STOP_FILL LOG
- 43d/0x2B - RESET_REASON

- BLE commands are documented in the BEEPBASE documentation on Google Drive. Also added a description of the circular log and the fill log functionality.


2021-04-09 Release of version 1.5.9
- The RTC time is now sent in the startup message

2021-04-09 Release of version 1.5.8
- Fixed RTC timestamp issue. Each time the time in seconds since the last RTC interrupt is calculated, the rounding modulus of the 32kHz crystal was added. if
if the calculate function was called often enough this leads to a high time deviation as the modulus is added continuously instead of just every minute.


2021-03-31 Release of version 1.5.7
- Beep Control Point queue now uses the correct BLE event to send the messages in the queue after the previous message has been sent.

- Fixed duplicate messages on the following commands:
1) WRITE_TIME
2) WRITE_ON_STATE
3) WRITE_REED_STATE
4) WRITE_SQ_MIN_STATE



2021-03-30 Release of version 1.5.6
- Read flash command 0x22 now gives status 0xE0F/3599d (End Of File) as response when the flash has been read completely. Previously this was NRF_SUCCESS 0x0 I changed this
during testing, because there was a problem with repeating response codes and the NRF_SUCCESS could be generated in several places in the code.

- BLE MTU size is now used by the BEEP characteristic to limit the size of a BLE message so that reading the flash does not default to the maximum MTU size
used. It should now work fine under iOS as well. I couldn't test further than the MTU size was correct, since the nRF Connect app on IOS is completely different
log generated.

- The Beep control point now uses a Queue. Temporarily buffers messages to be sent to the smartphone. The queue can contain a maximum of
4 messages buffer. This fixes the problem that a previous response code is repeated under some situations.

- Flash clear command 0x2100 now works correctly. The flash will be reinitialized after erasing. The flash then responds correctly to commands 0x22(get flash size)
and 0x20(read flash).


2021-03-15 Release of version 1.5.5.
- Reduced optimization of the flash application state machine so that the flash readout also works correctly under release builds. On previous firmware releases, the
statemachine by clearing a boolean after a specific number of messages. No clear cause found for this than that with a lower optimization level
then optimize for size the problem no longer occurs.

2021-03-12 Release of version 1.5.4.
- A power reset is no longer necessary to read the log.

- TX characteristic no longer needs to be turned off->on notification when connecting to a tied device. The notification state will then be removed from the
BLE stack achieved.

- If a flash command is not executed, an error code is now returned:
NRF_ERROR_INVALID_STATE If the TX notifications are not enabled for reading the MX flash.
NRF_ERROR_BUSY When the flash application statemachine is busy with a previous command.
NRF_ERROR_INVALID_ADDR When an invalid flash command is issued.
This should also prevent that if the flash is not read and no state or error code is returned, the BLE will enter a state where no new
messages are handled. On the smartphone side, this resulted in the nRF connect App in GATT ERROR 128: no resources

- I can read this correctly with a small log file of 16879 bytes and the number of bytes corresponds to the flash size that is read with command 0x22


2021_02_02 Release of version 1.5.3
Hotfix for BLE command to read the flash, which wouldn't start due to compiler optimization. It worked fine with logging on.

2020_10_12 Release of version 1.5.2
Restored old BLE commands from 1.4.2 for the tilt sensor configuration and changed the BLE on/off and reedswitch commands to other values.
Tilt and reed switch commands to enable and disable the application are now split into separate commands.

Back:
19d/0x13 - READ_SQ_MIN_STATE: bit[0]:1=off, 0=on
147d/0x93 - WRITE_SQ_MIN_STATE

Adjusted:
READ_ONOFF_CONTROL is now READ_REED_STATE 38d/0x26 bit[0]:1=off, 0=on
WRITE_ONOFF_CONTROL is now READ_ONOFF_CONTROL 166d/0xA6 bit[0]:1=off, 0=on
READ_ON_STATE 39/0x27 bit[0]:0=off, 1=on
WRITE_ON_STATE 167/0xA7 bit[0]:0=off, 1=on

Found another bug in the firmware version to uint32_t conversion that used only the major revision instead of also the minor and sub revision numbers.
Because of this, updating with minor or sub revisions may not always have gone correctly before, but with major revisions it did. This error has been in for a long time
the code.

2020_10_08 Release of version 1.5.1
Tilt switch functionality brought back and boot logic adapted to the combination of reed switch and tilt switch.
Added new BLE commands READ_ONOFF_CONTROL (0x26) and WRITE_ONOFF_CONTROL (0xA6), which allow setting the reed switch and til switch. Default
After programming with 1.5.1 or via DFU of an older version, the reed switch is always on. With the ONOFF_CONTROL commands, the reed switch or the tilt switch
be set.

Both at the same time is also possible, but I don't recommend it, since the on-off states no longer match or conflict. The tilt sensor is dominant in determining
of the on/off state. For example, switching on the BEEPBASE with the reed switch when it is vertical is fine until an orientation change takes place
measured by the tilt switch. If the BEEPBASE is turned off when horizontal, the BEEPBASE will become active again when it wakes up
measured by a small vibration with the tilt sensor.

The reed switch can still be energized to start BLE advertising or reset the PIN code when the reed switch is turned off to enable or disable the application.
to turn off.

Switching on with the reed switch now only takes 3 seconds instead of 10.


2020_09_16 Release of Version 1.5.0
SQ-MIN-645B Tilt switch functionality removed, as it is no longer on the hardware for the next production batches.
Enabling and disabling the BEEP application can now be performed with the reed switch. By activating the reed switch for 10 seconds, the BEEPBASE can be turned on or off
with audio feedback for the user. A beep is made every second with the buzzer rising or falling at that frequency until it beeps three times. If the magnet then
is removed, the BEEPBASE changes state with the already known beeps.
Via BLE or LoRaWAN, the BEEPBASE can also be disabled with commands READ_ON_STATE 0x13 and WRITE_ON_STATE 0x93, previously used to enable or disable the SQ-MIN-645B functionality.
links.
The bootloader activation during a reed switch reset is now disabled, as the bootloader continuously activated every attempt to start the application from the off state.
Epoch time added, it only works when the BEEPBASE is on. In the off state, all timers and clocks are disabled and the time cannot be kept.
The time can be read and written via BLE or LoRaWAN and is also added as a parameter to all logs. READ_TIME 0x25, WRITE_TIME 0xA5.
When the time is changed an additional message is logged in the flash with the old and new time settings.
By default, the Epoch time is set to 0.
Every 48 hours, the time is also stored in the flash of the nRF if no flash storage occurs before.
When the BEEPBASE is programmed, it is always off. During a firmware update, the BEEPBASE is always on.
Startup logic has been modified, as the tilt switch can no longer wake the nRF due to vibration and orientation changes.
Documentation has been updated to reflect the above changes.


2020_06_05 Release of Version 1.4.2
LoRaWAN offtime for KPN network improved. The time calculation was incorrect when the app_timer's RTC1 overflowed.

2020_06_02 Release of Version 1.4.1
LoRaWAN send function used wrong message length to check if MAC buffer should be sent. This caused problems for the KPN LoRaWAN network, because the MAC buffer is not
was sent, mThere were also no LoRaWAN messages sent.


2020_05_19 Release of Version 1.4.0
Command implemented to enable or disable the SQ-200-MIN orientation determination. With commands READ_SQ_MIN_STATE (19d/0x13) and WRITE_SQ_MIN_STATE (147d/0x93) the SQ orientation can be turned off by setting bit 0 to 1.
to make. All other bits are stored, but have no further function.

2020_02_26 Release of Version 1.3.5
The DS18B20 communication error counter is now reset when a new state or conversion starts. Previously, this only happened after a successful reading of the temperature. As a result, if a DS18B20
had more than 5 communication errors, the sensor stopped measuring with older firmware.

When the conversion starts, the last measured temperature is now set to -100.0, so if the sensor cannot be read, that value will be sent with LoRaWAN instead of the last measured value.

2020_01_28 Release of Version 1.3.4
READ_APPLICATION_CONFIG data added to the startup message.

One minute after successfully connecting to the LoRaWAN network, the startup message is sent. If this does not work due to the duty cycle, this will be
tried again a minute later.

2020_01_28 Release of Version 1.3.3
Default message interval is now 15 minutes instead of 1 minute.
This does mean that already programmed sensors that are not erased and are reprogrammed with the hex file, for example with DFU still have the
have default 1 minute interval after updating to 1.3.3.

Startup message with FPORT value 2 (BEEP_SENSOR_ON) is now sent again after network startup and join.



2020_01_06 Release of Version 1.3.2
Fix for the BME280 that did not work correctly and therefore continuously returned the same result.

> Pim reported a problem with the tilt sensor, which did not work when the BEEPBASE was idle for a long time. I was not able to reproduce this myself with the print from China.
Not even over periods of a few days.



2019_12_23 Release of Version 1.3.1
Message interval now uses the normal multiplier of 60 instead of the debug value of 20.

BME280 state machine further extended with additional checks in case no sensor is connected.

LoRaWAN duty cycle limitation now works and can be turned on and off.

Disabling LoRaWAN did not reset the LoRaWAN stack. Changing the LoRaWAN configuration with WRITE_LORAWAN_STATE now always resets the LoRaWAN stack.

The message ratio did not work properly because the state machine hung in the same state and still sent a message after a few iterations.
The first start-up message and downlink-response messages now ignore the message ratio and are always sent at the next message interval.

The tilt sensor on the new PCBs should now work correctly. The application now remembers the last boot state. If this was vertical and the current orientation is also
vertical then the BEEPBASE is woken up by a vibration and the BEEPBASE goes directly to SYSTEM OFF without buzzer sound pattern.



2019_12_20 Release of Version 1.3.0
TLV Audio ADC added with FFT functionality.
All three channels work, both Left and Right.
Measurement result has been added to the flash storage and the LoRaWAN message.

BME280 added, measures temperature, humidity and air pressure
Measurement result has been added to the flash storage and the LoRaWAN message.

Flash log payload is now binary instead of hexadecimal. Now the payload is half the size.

ERASE_MX_FLASH now has an additional parameter that allows two options to be selected; Erase fatfs log or Erase MX chip

BLE command added:
- BME280_CONVERSION_READ
- BME280_CONVERSION_START
- READ_AUDIO_ADC_CONFIG
- WRITE_AUDIO_ADC_CONFIG
- READ_AUDIO_ADC_CONVERSION
- START_AUDIO_ADC_CONVERSION
- ALARM_CONFIG_READ
- ALARM_CONFIG_WRITE
- ALARM_STATUS_READ

Problems with random resets in combination with the Nordic Power Profiler are caused by the input voltage dropping too much when a LORaWAN message is sent.
For testing with the Nordic PKK it is recommended to remove the diode at the input or to bridge it with a cable.

Firmware manual 1.3 is now on Google drive

2019_12_03 Release of Version 1.2.2
Fix for the issue with newer smartphones not getting a pin code popup because the encryption fails.

2019_12_02 Release of Version 1.2.1
Recompile of release 1.2.0, but without Pin code. Turns out this doesn't work correctly on devices other than the one I tested on.

2019_11_29 Release of Version 1.2.0
Added flash logging for measurement data based on Image Transfer example: https://github.com/NordicPlayground/nrf52-ble-image-transfer-demo
For the app development you can also look at this link: https://github.com/NordicPlayground/Android-Image-Transfer-Demo
The speed isn't exactly high with nRF Connect on an S5, but this is largely due to the low interval connection and the large MTU. On my S5 the OS doesn't want any other
parameters other than the settings when connecting. The developer(https://devzone.nordicsemi.com/f/nordic-q-a/47503/bluetooth-5-0-file-transfer) of the OTS service says
to have achieved speeds of 1200kbps, so maybe there is more possible from the App than I can achieve from the peripheral in terms of connection interval.
I did set the MTU default to 240 btes instead of 23, because negotiating with the client (S5 with nRF Connect) yielded nothing.

Added BLE commands for the Bootcount, FLASH_READ, FLASH_ERASE, FLASH_SIZE

TX characteristic added to the BEEP service.

Reading the flash log via the Beep control point and TX data characteristic

PIN protection turned on. PIN can be reset by pressing the reed switch for 30 seconds. The BEEPBASE will beep when reset.
This also means that bonding is now mandatory. If one of the two devices has forgotten a previous bond, it may take a few tries before the pin code pops up. if
Encryption fails on the BEEPBASE the connection is broken. This means that the connection has to be repeated a number of times before the existing bond is deleted first
must be.
Energizing the reed switch clears all unions on the BEEPBASE.


2019_11_11 Release of Version 1.1.1
Release 1.1.1 made with buzzer enabled in firmware and an extra default tune on index 2. Beep protocol command: 0x9102
Modify the script so that it no longer expects the hex file to be in a folder called "release."
Firmware release 1.1.0 added for convenience as it is almost identical.
The problem of the missing DS18B20 temperature in the LoRaWAN messages was very simple: The decoder expected more bytes than were left in the message and stopped decoding.
So the temperature was in the LoRaWAN message, but in my haste I didn't see the DS18B20 READ command in the hexadecimal message.

2019_11_04 Release of version 1.1.0
First release of firmware.
With further releases, changes will be recorded in this document.
Beep Base Manual updated due to changes to HX711 read and Write conversion command, added HX711 state R/W command, added Buzzer custom and default tune commands.


2019_11_01
Release Beep Base manual 1V0