v1.5.13 adds support for an external real-time clock (RTC). 

When using an external RTC, the logged measurement data from your BEEP base will have correct timestamps even when the base is offline and disconnected from the LoraWAN network. 
This will ensure that the timestamps match when importing and merging the logged data afterwards.

It is recommended to use the Maxim DS3231 real-time clock with a coin-cell battery as back-up power, connected to the I2C screw terminal on the 
BEEP base PCB.
 
