/*
 * smif.c
 *
 *  Created on: 05-Sep-2022
 *      Author: AustinA
 */



#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "smif.h"
#include "cy_serial_flash_qspi.h"


/*******************************************************************************
* Device Configurator Macros ( To be adusted as per name given in configurator)
********************************************************************************/
#define SMIF_BLOCK_BASE		SMIF_HW
#define SMIF_CONFIG_S		SMIF_config
// From cycfg_qspi_memslot.h file (edit accordingly)
#define SMIF_BLOCK_CONFIG	smifBlockConfig
/*********************************************************************************/

//#define USING_SMIF_API

// Do not edit
cy_stc_smif_context_t smifContext;


/* SMIF interrupt initialization */
cy_stc_sysint_t smifIntConfig =
{
	/* .intrSrc */ SMIF_IRQ, /* SMIF interrupt number (non M0 core)*/
    /* .intrPriority */ 7
};

void SMIF_Interrupt_User(void)
{
    uint32_t interruptStatus = Cy_SMIF_GetInterruptStatusMasked(SMIF_BLOCK_BASE);
    /* Check which interrupt occurred */
    if (0U != (interruptStatus & SMIF_INTR_TR_TX_REQ_Msk))
    {
    	printf("[ SMIF IRQ ] :  Send data Complete ");
    }
    else if (0U != (interruptStatus & SMIF_INTR_TR_RX_REQ_Msk))
    {
    	printf("[ SMIF IRQ ] :  Receive data Complete ");
    }
    else if (0U != (interruptStatus & SMIF_INTR_XIP_ALIGNMENT_ERROR_Msk))
    {
        printf("[ SMIF IRQ ] : An XIP alignment error ");
    }

    else if (0U != (interruptStatus & SMIF_INTR_TX_CMD_FIFO_OVERFLOW_Msk))
    {
    	printf("[ SMIF IRQ ] : TX CMD FIFO overflow ");
    }

    else if (0U != (interruptStatus & SMIF_INTR_TX_DATA_FIFO_OVERFLOW_Msk))
    {
    	printf("[ SMIF IRQ ] : A TX DATA FIFO overflow ");
    }

    else if (0U != (interruptStatus & SMIF_INTR_RX_DATA_FIFO_UNDERFLOW_Msk))
    {
    	printf("[ SMIF IRQ ] : RX DATA FIFO underflow ");
    }
    else
    {
        /* Processing of errors */
    	printf("[ SMIF IRQ ] : Undefined Error \r\n");
    }
    printf("\r\n");

    // Do not remove, necessary for clearing the interrupts
    Cy_SMIF_Interrupt(SMIF_BLOCK_BASE, &smifContext);
}



void smif_init()
{

	cy_en_sysint_status_t res;
    bool isQuadEnabled = false;

    if(Cy_SysInt_Init(&smifIntConfig, SMIF_Interrupt_User) != CY_SYSINT_SUCCESS)
    {
    	printf("SMIF Interrupt Init Failed\r\n");
    	while(1);
    }

    /* Enable SMIF interrupt */
        NVIC_EnableIRQ(smifIntConfig.intrSrc);

	/* SMIF initialization */
	if(Cy_SMIF_Init(SMIF_BLOCK_BASE, &SMIF_CONFIG_S, TIMEOUT_1_S, &smifContext) != CY_SMIF_SUCCESS)
	{
		printf("SMIF Initialization Failed\r\n");
		while(1);
	}

	Cy_SMIF_Enable(SMIF_BLOCK_BASE, &smifContext);

	Cy_SMIF_SetDataSelect(SMIF_BLOCK_BASE,CY_SMIF_SLAVE_SELECT_0,CY_SMIF_DATA_SEL0);

    /* Memslot level initialization */
    if(Cy_SMIF_MemInit(SMIF_BLOCK_BASE, &SMIF_BLOCK_CONFIG, &smifContext) != CY_SMIF_SUCCESS)
	{
		printf("SMIF Memory Initialization Failed\r\n");
		while(1);
	}


    if(Cy_SMIF_MemIsQuadEnabled(SMIF_BLOCK_BASE,
    		SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM],
                                                 &isQuadEnabled, &smifContext) != CY_SMIF_SUCCESS)
    printf("QUAD Mode Init Failed\r\n");
    else
		Cy_SMIF_MemEnableQuadMode(SMIF_BLOCK_BASE,
		SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM],
				  5000ul,
				  &smifContext);
	if (!isQuadEnabled)
	{
		printf("Initializaing QUAD Mode\r\n");
		Cy_SMIF_MemEnableQuadMode(SMIF_BLOCK_BASE,
		SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM],
				  5000ul,
				  &smifContext);
	}
	else
	{
		printf("QUAD Mode Enabled\r\n");
	}

}


cy_en_smif_status_t smif_read(uint32_t ext_address, uint32_t readSize,uint8_t* rxBuffer)
{
	cy_en_smif_status_t status = CY_SMIF_BAD_PARAM;
    uint32_t chunk = 0UL;
    uint8_t addrArray[CY_SMIF_FOUR_BYTES_ADDR] = {0U};
    cy_stc_smif_mem_cmd_t *cmdRead;
    uint32_t length = readSize;
    uint32_t address = ext_address;

    CY_ASSERT_L1(NULL != SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]);
    CY_ASSERT_L1(NULL != rxBuffer);

    cmdRead = SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->readCmd;

    if((address + length) <= SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->memSize)  /* Check if the address exceeds the memory size */
    {
        uint32_t interruptState = 0;


        /* SMIF can read only up to 65536 bytes in one go. Split the larger read into multiple chunks */
        while (length > 0UL)
        {
            /* Get the number of bytes which can be read during one operation */
            chunk = (length > SMIF_MAX_RX_COUNT) ? (SMIF_MAX_RX_COUNT) : length;
            ValueToByteArray(address, &addrArray[0], 0UL,
                             SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->numOfAddrBytes);
			status = Cy_SMIF_TransmitCommand(SMIF_BLOCK_BASE,
												(uint8_t)cmdRead->command,
												cmdRead->cmdWidth,
												(const uint8_t *)addrArray,
												SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->numOfAddrBytes,
												cmdRead->addrWidth,
												SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->slaveSelect,
												CY_SMIF_TX_NOT_LAST_BYTE,
												&smifContext);

			if((CY_SMIF_SUCCESS == status) && (CY_SMIF_NO_COMMAND_OR_MODE != cmdRead->mode))
			{
				status = Cy_SMIF_TransmitCommand(SMIF_BLOCK_BASE,
													(uint8_t)cmdRead->mode,
													cmdRead->modeWidth,
													NULL,
													CY_SMIF_CMD_WITHOUT_PARAM,
													CY_SMIF_WIDTH_NA,
													SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->slaveSelect,
													CY_SMIF_TX_NOT_LAST_BYTE,
													&smifContext);
			}

			if((CY_SMIF_SUCCESS == status) && (0U < cmdRead->dummyCycles))
			{
				status = Cy_SMIF_SendDummyCycles(SMIF_BLOCK_BASE, cmdRead->dummyCycles);
			}

			if(CY_SMIF_SUCCESS == status)
			{

				status = Cy_SMIF_ReceiveData(SMIF_BLOCK_BASE,
										rxBuffer,
										chunk,
										cmdRead->dataWidth,
										NULL,	 /* Callback function is not used */
										&smifContext);

				/* Check SMIF fishes reading operation */
				while(Cy_SMIF_GetTransferStatus(SMIF_BLOCK_BASE, &smifContext) == CY_SMIF_RX_BUSY);
			}

			if(CY_SMIF_SUCCESS != status)
			{
				break;
			}

			/* Recalculate the next rxBuffer offset */
			length -= chunk;
			address += chunk;
			rxBuffer = (uint8_t *)rxBuffer + chunk;
		}
	}

	return status;
}

cy_en_smif_status_t smif_write(uint32_t ext_address, uint32_t sendSize, uint8_t* txBuffer)
{
	cy_en_smif_status_t status = CY_SMIF_BAD_PARAM;
    uint32_t offset = 0UL;
    uint32_t chunk = 0UL;
    uint32_t pageSize;
    uint8_t addrArray[CY_SMIF_FOUR_BYTES_ADDR] = {0U};
    cy_stc_smif_mem_cmd_t *cmdProgram;
    uint32_t length = sendSize;
    uint32_t address = ext_address;

    CY_ASSERT_L1(NULL != SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]);
    CY_ASSERT_L1(NULL != txBuffer);

    pageSize = SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->programSize; /* Get the page size */
    cmdProgram = SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->programCmd; /* Get the program command */

    if((address + length) <= SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->memSize)  /* Check if the address exceeds the memory size */
    {
        uint32_t interruptState = 0;

        /* SMIF can read only up to 65536 bytes in one go. Split the larger read into multiple chunks */
        while (length > 0UL)
        {
            /* Get the number of bytes which can be written during one operation */
            offset = address % pageSize;
            chunk = ((offset + length) < pageSize) ? length : (pageSize - offset);

            /* The Write Enable bit may be cleared by the memory after every successful
             * operation of write or erase operations. Therefore, must be set for
             * every loop.
             */
            status = Cy_SMIF_MemCmdWriteEnable(SMIF_BLOCK_BASE, SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM], &smifContext);

            if(CY_SMIF_SUCCESS == status)
            {
                ValueToByteArray(address, &addrArray[0], 0UL,
                                 SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->numOfAddrBytes);

                status = Cy_SMIF_TransmitCommand(SMIF_BLOCK_BASE,
                                                 (uint8_t)cmdProgram->command,
                                                 cmdProgram->cmdWidth,
                                                 (const uint8_t *)addrArray,
                                                 SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->numOfAddrBytes,
                                                 cmdProgram->addrWidth,
                                                 SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->slaveSelect,
                                                 CY_SMIF_TX_NOT_LAST_BYTE,
                                                 &smifContext);

                if((CY_SMIF_SUCCESS == status) && (CY_SMIF_NO_COMMAND_OR_MODE != cmdProgram->mode))
                {
                    status = Cy_SMIF_TransmitCommand(SMIF_BLOCK_BASE,
                                                     (uint8_t)cmdProgram->mode,
                                                     cmdProgram->modeWidth,
                                                     NULL,
                                                     CY_SMIF_CMD_WITHOUT_PARAM,
                                                     CY_SMIF_WIDTH_NA,
                                                     SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->slaveSelect,
                                                     CY_SMIF_TX_NOT_LAST_BYTE,
                                                     &smifContext);
                }

                if((CY_SMIF_SUCCESS == status) && (cmdProgram->dummyCycles > 0U))
                {
                    status = Cy_SMIF_SendDummyCycles(SMIF_BLOCK_BASE, cmdProgram->dummyCycles);
                }

                if(CY_SMIF_SUCCESS == status)
                {
                    status = Cy_SMIF_TransmitData(SMIF_BLOCK_BASE,
                                                         (uint8_t *)txBuffer,
                                                         chunk,
                                                         cmdProgram->dataWidth,
														 NULL,
                                                         &smifContext);
                }

                if(CY_SMIF_SUCCESS == status)
                {
                    /* Check if the memory has completed the write operation. ProgramTime is in microseconds */
                    status = Cy_SMIF_MemIsReady(SMIF_BLOCK_BASE, SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM], SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->programTime, &smifContext);
                }
            }

            if(CY_SMIF_SUCCESS != status)
            {
                break;
            }

            /* Recalculate the next rxBuffer offset */
            txBuffer = (uint8_t *)txBuffer + chunk;
            address += chunk;
            length -= chunk;
        }
    }
	return status;

}


size_t smif_get_erase_size(uint32_t addr)
{
    size_t 	erase_sector_size;
    cy_stc_smif_hybrid_region_info_t* hybrid_info = NULL;

    cy_en_smif_status_t smif_status =
        Cy_SMIF_MemLocateHybridRegion(SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM], &hybrid_info, addr);

    if (CY_SMIF_SUCCESS != smif_status)
    {
        erase_sector_size = (size_t)SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->eraseSize;
    }
    else
    {
        erase_sector_size = (size_t)hybrid_info->eraseSize;
    }

    return erase_sector_size;
}

cy_rslt_t smif_erase(uint32_t addr, uint32_t length)
{

	cy_rslt_t result;
	// If the erase is for the entire chip, use chip erase command
	if ((addr == 0u) && (length >= smif_flash_get_size()))
	{
		result = (cy_rslt_t)Cy_SMIF_MemEraseChip(SMIF_BLOCK_BASE,
					SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM],
					&smifContext);
		printf("Chip erase %X\r\n",result);
	}
	else
	{
		// Cy_SMIF_MemEraseSector() returns error if (addr + length) > total flash size or if
		// addr is not aligned to erase sector size or if (addr + length) is not aligned to
		// erase sector size.
		result = (cy_rslt_t)Cy_SMIF_MemEraseSector(SMIF_BLOCK_BASE,
					SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM],
											  addr, length, &smifContext);
		printf("Sector erase, %X\r\n",result);
	}
	while(Cy_SMIF_BusyCheck(SMIF_BLOCK_BASE));

    /* Give priority to the status of SMIF operation when both SMIF operation
     * and mutex release fail.
     */
    return result;
}


size_t smif_flash_get_size(void)
{
    return (size_t)SMIF_BLOCK_CONFIG.memConfig[MEM_SLOT_NUM]->deviceCfg->memSize;
}
