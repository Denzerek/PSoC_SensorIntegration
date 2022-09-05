/*
 * smif.h
 *
 *  Created on: 05-Sep-2022
 *      Author: AustinA
 */

#ifndef SMIF_H_
#define SMIF_H_


/*******************************************************************************
* Macros
********************************************************************************/
#define PACKET_SIZE             (64)     /* Memory Read/Write size */

/* Used when an array of data is printed on the console */
#define NUM_BYTES_PER_LINE      (16u)
#define MEM_SLOT_NUM            (0u)      /* Slot number of the memory to use */
#define FLASH_DATA_AFTER_ERASE  (0xFFu)   /* Flash data after erase */
#define TIMEOUT_1_S                 (1000U)
#define MEMORY_ADDRESS_SIZE 	(3u)
#define SMIF_MAX_RX_COUNT	65536ul


cy_en_smif_status_t smif_read(uint32_t ext_address, uint32_t readSize,uint8_t* rxBuffer);
cy_en_smif_status_t smif_write(uint32_t ext_address, uint32_t sendSize, uint8_t* txBuffer);
void smif_init();
cy_rslt_t smif_erase(uint32_t addr, uint32_t length);
size_t smif_get_erase_size(uint32_t addr);
size_t smif_flash_get_size(void);



#endif /* SMIF_H_ */
