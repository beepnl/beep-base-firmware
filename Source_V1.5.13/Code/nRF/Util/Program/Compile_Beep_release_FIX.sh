####################################
# 01-01-24 edits om DFU zip te fixen
####################################

$DIR/Code/nRF/Key/public_key.c.new # !!! 

# nrfutil keys display --key pk --format code
    __ALIGN(4) const uint8_t pk[64] =
    {
        0xdc, 0x1c, 0xc7, 0x3f, 0xb2, 0xec, 0xe9, 0x60, 0x94, 0x19, 0x11, 0x1c, 0x01, 0xc4, 0xff, 0xc9, 0xa8, 0x5e, 0xaa, 0x7c, 0xc4, 0x23, 0xb7, 0x83, 0x4d, 0x3a, 0xab, 0xa1, 0x48, 0xe0, 0xb6, 0x8e,
        0x04, 0x00, 0x8c, 0xcc, 0x3d, 0xf0, 0xb3, 0x5a, 0x34, 0x5d, 0xf1, 0x59, 0x3f, 0x49, 0x40, 0xf8, 0x48, 0x38, 0x8d, 0x2d, 0x72, 0x1a, 0xfc, 0x4a, 0x43, 0x8b, 0xc8, 0x7c, 0x1e, 0x6c, 0xf2, 0xca
    };

# nrfutil keys display --key pk --format pem
    -----BEGIN PUBLIC KEY-----
    MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEjrbgSKGrOk2DtyPEfKpeqMn/xAEc
    ERmUYOnssj/HHNzK8mwefMiLQ0r8GnItjThI+EBJP1nxXTRas/A9zIwABA==
    -----END PUBLIC KEY-----


##########################################
fixed Compile_Beep_release.sh nrfutil cmd:
##########################################


        nrfutil pkg generate
        --application  /home/orlo/beep-base-firmware/Source_V1.5.13/Code/nRF/BTLDR/ses/Output/Release_skip_CRC/Exe/APP_SD.hex 
        # Firmware version: If the image contains a bootloader, the image is accepted if the new firmware version is greater than (>) the existing version of the bootloader. 
        # If the image contains an application, the image is accepted if the new firmware version is greater than or equal to (>=) the existing version of the application. 
        # If the image contains a SoftDevice and no SoftDevice is already present, the fw_version is checked against the existing application's version to determine whether the update can overwrite it.
        --application-version 1  

        # met flash_placement.xml aangepast !
        --bootloader /home/orlo/beep-base-firmware/Source_V1.5.13/Code/nRF/BTLDR/ses/Output/Release_skip_CRC/Exe/beep_bootloader.hex  
        --bootloader-version 1
        --hw-version 52 

        # skip softdevice if poss
        --softdevice ../../nRF5_SDK_15.3/components/softdevice/s140/hex/s140_nrf52_6.1.1_softdevice.hex
        --sd-req 0xB6 
        # A firmware ID of 0x00 in the sd_req list means "The update does not depend on the SoftDevice". 
        # See section Updates without a SoftDevice for the implications of this.
        --sd-id 0xB6 

        # has to match $DIR/Code/nRF/Key/public_key.c.new
        --key-file ../../Key/private.key 

        # output zip
        Beepbase_sd_boot_app.zip
##########################################