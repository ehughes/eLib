#include "System.h"
#include "System.h"

volatile int32_t sdio_wait_exit = 0;
mci_card_struct sdcardinfo;

/* SDMMC card info structure */
mci_card_struct sdcardinfo;
volatile uint32_t SD_Errors = 0;



/* Delay callback for timed SDIF/SDMMC functions */
static void sdmmc_waitms(uint32_t time)
{
    Delay_mS(time);
}

/**
 * @brief	Sets up the SD event driven wakeup
 * @param	bits : Status bits to poll for command completion
 * @return	Nothing
 */
static void sdmmc_setup_wakeup(void *bits)
{
    uint32_t bit_mask = *((uint32_t *)bits);
    /* Wait for IRQ - for an RTOS, you would pend on an event here with a IRQ based wakeup. */
    NVIC_ClearPendingIRQ(SDIO_IRQn);
    sdio_wait_exit = 0;
    Chip_SDIF_SetIntMask(LPC_SDMMC, bit_mask);
    NVIC_EnableIRQ(SDIO_IRQn);
}

/**
 * @brief	A better wait callback for SDMMC driven by the IRQ flag
 * @return	0 on success, or failure condition (-1)
 */
static uint32_t sdmmc_irq_driven_wait(void)
{
    uint32_t status = 0;
    uint32_t SD_TimeoutTicker = 0;

    while (sdio_wait_exit == 0)
    {
    	//ideally we would want a precise time out but this will do for now.
    	if(++SD_TimeoutTicker > 5000000) //if we are past a half second,  break out!
    	{

    		SD_Errors++;
    		status = Chip_SDIF_GetIntStatus(LPC_SDMMC);
    	    Chip_SDIF_ClrIntStatus(LPC_SDMMC, status);
    	    Chip_SDIF_SetIntMask(LPC_SDMMC, 0);
    	    return  SD_INT_ERROR;
    	}
    }


    /* Get status and clear interrupts */
    status = Chip_SDIF_GetIntStatus(LPC_SDMMC);
    Chip_SDIF_ClrIntStatus(LPC_SDMMC, status);
    Chip_SDIF_SetIntMask(LPC_SDMMC, 0);

    return status;
}

/* Initialize SD/MMC */
void InitSD()
{
    memset(&sdcardinfo, 0, sizeof(sdcardinfo));
    sdcardinfo.card_info.evsetup_cb = sdmmc_setup_wakeup;
    sdcardinfo.card_info.waitfunc_cb = sdmmc_irq_driven_wait;
    sdcardinfo.card_info.msdelay_func = sdmmc_waitms;

    /*  SD/MMC initialization */

    Board_SDMMC_Init();

    /* The SDIO driver needs to know the SDIO clock rate */
    Chip_SDIF_Init(LPC_SDMMC);

    NVIC_SetPriority(SDIO_IRQn,(1 << __NVIC_PRIO_BITS) -3);
}





/**
 * @brief	SDIO controller interrupt handler
 * @return	Nothing
 */
void SDIO_IRQHandler(void)
{
    /* All SD based register handling is done in the callback
       function. The SDIO interrupt is not enabled as part of this
       driver and needs to be enabled/disabled in the callbacks or
       application as needed. This is to allow flexibility with IRQ
       handling for applicaitons and RTOSes. */
    /* Set wait exit flag to tell wait function we are ready. In an RTOS,
       this would trigger wakeup of a thread waiting for the IRQ. */
    NVIC_DisableIRQ(SDIO_IRQn);
    sdio_wait_exit = 1;
}


