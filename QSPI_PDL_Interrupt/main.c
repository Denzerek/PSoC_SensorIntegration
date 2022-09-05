/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the Empty PSoC6 Application
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* (c) 2019-2021, Cypress Semiconductor Corporation. All rights reserved.
*******************************************************************************
* This software, including source code, documentation and related materials
* ("Software"), is owned by Cypress Semiconductor Corporation or one of its
* subsidiaries ("Cypress") and is protected by and subject to worldwide patent
* protection (United States and foreign), United States copyright laws and
* international treaty provisions. Therefore, you may use this Software only
* as provided in the license agreement accompanying the software package from
* which you obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software source
* code solely for use in connection with Cypress's integrated circuit products.
* Any reproduction, modification, translation, compilation, or representation
* of this Software except as specified above is prohibited without the express
* written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer of such
* system or application assumes all risk of such use and in doing so agrees to
* indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"
#include <inttypes.h>
#include "smif.h"


/*******************************************************************************
* Function Name: print_array
****************************************************************************//**
* Summary:
*  Prints the content of the buffer to the UART console.
*
* Parameters:
*  message - message to print before array output
*  buf - buffer to print on the console.
*  size - size of the buffer.
*
*******************************************************************************/
void print_array(char *message, uint8_t *buf, uint32_t size)
{
    printf("\r\n%s (%"PRIu32" bytes):\r\n", message, size);
    printf("-------------------------\r\n");

    for (uint32_t index = 0; index < size; index++)
    {
        printf("0x%02X ", buf[index]);

        if (0u == ((index + 1) % NUM_BYTES_PER_LINE))
        {
            printf("\r\n");
        }
    }
}


/*******************************************************************************
* Function Name: check_status
****************************************************************************//**
* Summary:
*  Prints the message, indicates the non-zero status by turning the LED on, and
*  asserts the non-zero status.
*
* Parameters:
*  message - message to print if status is non-zero.
*  status - status for evaluation.
*
*******************************************************************************/
void check_status(char *message, uint32_t status)
{
    if (0u != status)
    {
        printf("\r\n================================================================================\r\n");
        printf("\nFAIL: %s\r\n", message);
        printf("Error Code: 0x%08"PRIX32"\n", status);
        printf("\r\n================================================================================\r\n");

        /* On failure, turn the LED ON */
        cyhal_gpio_write(CYBSP_USER_LED, CYBSP_LED_STATE_ON);
        while(true); /* Wait forever here when error occurs. */
    }
}


int main(void)
{
    cy_rslt_t result;
    uint8_t tx_buf[PACKET_SIZE];
    uint8_t rx_buf[PACKET_SIZE];
    uint32_t ext_mem_address = 0x00;
    size_t sectorSize;

    /* Initialize the device and board peripherals */
    result = cybsp_init() ;
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

	cy_retarget_io_init(CYBSP_DEBUG_UART_TX,CYBSP_DEBUG_UART_RX,115200);

    __enable_irq();

    /* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
    printf("\x1b[2J\x1b[;H");
    printf("*************** PSoC 6 MCU PDL QSPI Flash Read and Write ***************\r\n\n");

	smif_init();
    uint8_t writeBuff[PACKET_SIZE];
    uint8_t readBuff[PACKET_SIZE];

    for(int i =0; i< PACKET_SIZE; i++)
    {
    	writeBuff[i] =  PACKET_SIZE + i;
    }

    sectorSize = smif_get_erase_size(ext_mem_address);
	printf("\r\nTotal Flash Size: %u bytes\r\n", smif_flash_get_size());

	/* Erase before write */
	printf("\r\n\n1. Erasing %u bytes of memory\r\n", sectorSize);
	result = smif_erase(ext_mem_address, sectorSize);
	check_status("1. Erasing memory failed", result);

    /* Read after Erase to confirm that all data is 0xFF */
    printf("\r\n\n2. Reading after Erase and verifying that each byte is 0xFF\r\n");
    result = smif_read(ext_mem_address, PACKET_SIZE, rx_buf);
    check_status("Reading memory failed", result);
    print_array("Received Data", rx_buf, PACKET_SIZE);
    memset(tx_buf, FLASH_DATA_AFTER_ERASE, PACKET_SIZE);
    check_status("Flash contains data other than 0xFF after erase",
            memcmp(tx_buf, rx_buf, PACKET_SIZE));

    /* Prepare the TX buffer */
    for (uint32_t index = PACKET_SIZE; index >0; index--)
    {
        tx_buf[index] = (uint8_t)index + PACKET_SIZE;
    }

    /* Write the content of the TX buffer to the memory */
    printf("\r\n\n3. Writing data to memory\r\n");
    result = smif_write(ext_mem_address, PACKET_SIZE, tx_buf);
    check_status("Writing to memory failed", result);
    print_array("Written Data", tx_buf, PACKET_SIZE);

    /* Read back after Write for verification */
    printf("\r\n\n4. Reading back for verification\r\n");
    result = smif_read(ext_mem_address, PACKET_SIZE, rx_buf);
    check_status("Reading memory failed", result);
    print_array("Received Data", rx_buf, PACKET_SIZE);

    /* Check if the transmitted and received arrays are equal */
    check_status("Read data does not match with written data. Read/Write operation failed.",
            memcmp(tx_buf, rx_buf, PACKET_SIZE));

    printf("\r\n================================================================================\r\n");
    printf("\r\nSUCCESS: Read data matches with written data!\r\n");
    printf("\r\n================================================================================\r\n");



    for (;;)
    {
        cyhal_gpio_toggle(CYBSP_USER_LED);
        cyhal_system_delay_ms(500);
    }
}

/* [] END OF FILE */
