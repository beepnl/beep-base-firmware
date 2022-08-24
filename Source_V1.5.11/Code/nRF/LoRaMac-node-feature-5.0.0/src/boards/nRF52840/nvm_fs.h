#ifndef __NVM_FS_H__ 
#define __NVM_FS_H__

    #include <stdint.h>
    #include <string.h>
    #include <stdbool.h>


    /* File ID and Key used for the configuration record. */
    #define CONFIG_FILE     (0xF010)
    #define CONFIG_REC_KEY  (0x7010)
	#define LORAWAN_NVM_SIZE 1600


    /* A structure definition to save data in flash. */
    typedef struct
    {
		uint32_t    Major_version;
		uint32_t    Minor_version;
		uint32_t    boot_count;
		
		/* LoRaWAN Non-volatile storage array */
		uint8_t		LoRaWANnvm[LORAWAN_NVM_SIZE];	
    } flash_struct_s;


    
    void nvm_fds_changed		(void);
    void nvm_fds_check_pending	(void);
    void nvm_fds_init			(void);

    void nvm_fds_eeprom_wrapper_read( uint16_t addr, uint8_t *buffer, uint16_t size );
    void nvm_fds_eeprom_wrapper_write( uint16_t addr, uint8_t *buffer, uint16_t size );



#endif /* __NVM_FS_H__ */