/*
 * @brief SDMMC Chan FATFS simple abstraction layer
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "board.h"
#include "chip.h"

#include "..\FileSystem.h"
#include "..\FileSystemMasterConfig.h"
#include ".\USB_MSC_HOST\fsusb_cfg.h"
#include ".\SD\fsmci_cfg.h"
#include ".\SD\SD.h"
#include "..\..\USB_MSC_ROM\USB_MSC_DEVICE.h"
#include "..\..\USB_MSC_ROM\usbd_rom_api.h"
#include "..\..\USB_MSC_ROM\msc_sd.h"

FATFS MySD_FatFs;
FATFS MyUSB_FatFs;

volatile uint32_t DiskManagerTicker;

uint8_t MSC_Attached = FALSE;
uint8_t MSC_EnableFlag = FALSE;

void MSC_Enable()
{
	 MSC_EnableFlag = TRUE;

}
void MSC_Disable()
{
	 MSC_EnableFlag = FALSE;

}
uint8_t MSC_IsEnabled()
{
	return MSC_EnableFlag;
}




/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Disk Status */
static volatile DSTATUS SD_Stat = STA_NOINIT;
static volatile DSTATUS USB_Stat = STA_NOINIT;

static CARD_HANDLE_T *hCard;
static DISK_HANDLE_T *hDisk;

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Initialize Disk Drive */
DSTATUS disk_initialize(BYTE drv)
{
    //the disk manager gets called in a different process to intialize drives.
    //this routine will just return the status

    switch(drv)
    {
        default:
        case DRIVE_SD:
            return SD_Stat;
            break;

            break;

        case DRIVE_USB:
            return USB_Stat;
            break;
    }


}

/* Disk Drive miscellaneous Functions */
DRESULT disk_ioctl(BYTE drv, BYTE ctrl, void *buff)
{
    DRESULT res;
    BYTE *ptr = buff;

    res = RES_ERROR;

    switch(drv)
    {
        default:
        case DRIVE_SD:

            if (SD_Stat & STA_NOINIT)
            {
                return RES_NOTRDY;
            }

            switch (ctrl)
            {
                case CTRL_SYNC:	/* Make sure that no pending write process */
                    if (FSMCI_CardReadyWait(hCard, 50))
                    {
                        res = RES_OK;
                    }

                    break;

                case GET_SECTOR_COUNT:	/* Get number of sectors on the disk (DWORD) */
                    *(DWORD *) buff = FSMCI_CardGetSectorCnt(hCard);
                    res = RES_OK;
                    break;

                case GET_SECTOR_SIZE:	/* Get R/W sector size (WORD) */
                    *(WORD *) buff = FSMCI_CardGetSectorSz(hCard);
                    res = RES_OK;
                    break;

                case GET_BLOCK_SIZE:/* Get erase block size in unit of sector (DWORD) */
                    *(DWORD *) buff = FSMCI_CardGetBlockSz(hCard);
                    res = RES_OK;
                    break;

                case MMC_GET_TYPE:		/* Get card type flags (1 byte) */
                    *ptr = FSMCI_CardGetType(hCard);
                    res = RES_OK;
                    break;

                case MMC_GET_CSD:		/* Receive CSD as a data block (16 bytes) */
                    *((uint32_t *) buff + 0) = FSMCI_CardGetCSD(hCard, 0);
                    *((uint32_t *) buff + 1) = FSMCI_CardGetCSD(hCard, 1);
                    *((uint32_t *) buff + 2) = FSMCI_CardGetCSD(hCard, 2);
                    *((uint32_t *) buff + 3) = FSMCI_CardGetCSD(hCard, 3);
                    res = RES_OK;
                    break;

                case MMC_GET_CID:		/* Receive CID as a data block (16 bytes) */
                    *((uint32_t *) buff + 0) = FSMCI_CardGetCID(hCard, 0);
                    *((uint32_t *) buff + 1) = FSMCI_CardGetCID(hCard, 1);
                    *((uint32_t *) buff + 2) = FSMCI_CardGetCID(hCard, 2);
                    *((uint32_t *) buff + 3) = FSMCI_CardGetCID(hCard, 3);
                    res = RES_OK;
                    break;

                case MMC_GET_SDSTAT:/* Receive SD status as a data block (64 bytes) */
                    if (FSMCI_CardGetState(hCard, (uint8_t *) buff))
                    {
                        res = RES_OK;
                    }

                    break;

                default:
                    res = RES_PARERR;
                    break;
            }

            break;

        case DRIVE_USB:
            switch (ctrl)
            {
                case CTRL_SYNC:	/* Make sure that no pending write process */
                    if (FSUSB_DiskReadyWait(hDisk, 50))
                    {
                        res = RES_OK;
                    }

                    break;

                case GET_SECTOR_COUNT:	/* Get number of sectors on the disk (DWORD) */
                    *(DWORD *) buff = FSUSB_DiskGetSectorCnt(hDisk);
                    res = RES_OK;
                    break;

                case GET_SECTOR_SIZE:	/* Get R/W sector size (WORD) */
                    *(WORD *) buff = FSUSB_DiskGetSectorSz(hDisk);
                    res = RES_OK;
                    break;

                case GET_BLOCK_SIZE:/* Get erase block size in unit of sector (DWORD) */
                    *(DWORD *) buff = FSUSB_DiskGetBlockSz(hDisk);
                    res = RES_OK;
                    break;

                default:
                    res = RES_PARERR;
                    break;
            }

            break;

    }


    return res;
}

/* Read Sector(s) */
DRESULT disk_read(BYTE drv, BYTE *buff, DWORD sector, BYTE count)
{
    switch(drv)
    {
        default:
        case DRIVE_SD:

            if (SD_Stat & STA_NOINIT)
            {
                return RES_NOTRDY;
            }

            if (FSMCI_CardReadSectors(hCard, buff, sector, count))
            {
                return RES_OK;
            }

            return RES_ERROR;
            break;

        case DRIVE_USB:
        	if (!count) {
        		return RES_PARERR;
        	}
        	if (USB_Stat & STA_NOINIT) {
        		return RES_NOTRDY;
        	}

        	if (FSUSB_DiskReadSectors(hDisk, buff, sector, count)) {
        		return RES_OK;
        	}
        	break;
    }

    return RES_ERROR;
}

/* Get Disk Status */
DSTATUS disk_status(BYTE drv)
{
    switch(drv)
    {
        default:
        case DRIVE_SD:
            return SD_Stat;
            break;

            break;

        case DRIVE_USB:
            return USB_Stat;
            break;
    }

}

/* Write Sector(s) */
DRESULT disk_write(BYTE drv, const BYTE *buff, DWORD sector, BYTE count)
{
    switch(drv)
    {
        default:
        case DRIVE_SD:

            if (SD_Stat & STA_NOINIT)
            {
                return RES_NOTRDY;
            }

            if (FSMCI_CardWriteSectors(hCard, (void *) buff, sector, count))
            {
                return RES_OK;
            }

            break;

        case DRIVE_USB:
        	if (!count) {
        		return RES_PARERR;
        	}
        	if (USB_Stat & STA_NOINIT) {
        		return RES_NOTRDY;
        	}

        	if (FSUSB_DiskWriteSectors(hDisk, (void *) buff, sector, count)) {
        		return RES_OK;
        	}

        	return RES_ERROR;

        	break;
    }

    return RES_NOTRDY;
}

void InitFileSystem()
{

    SD_Stat = STA_NOINIT | STA_NODISK;
    USB_Stat = STA_NOINIT | STA_NODISK;
    InitSD();
    InitUSB_MSC_Host();
    MSC_Disable();

    f_mount(0,&MySD_FatFs);
    f_mount(1,&MyUSB_FatFs);

}

//This function must be called every 10mS
void ProcessFileSystem()
{
   // uint8_t ErrorCode;

    if(DiskManagerTicker > 10)
    {
        DiskManagerTicker = 0;



        /***
           *      __  __ ___  ___     _ _____ _____ _   ___ _  _   _____  ___ _____ _   ___ _  _
           *     |  \/  / __|/ __|   /_\_   _|_   _/_\ / __| || | / /   \| __|_   _/_\ / __| || |
           *     | |\/| \__ \ (__   / _ \| |   | |/ _ \ (__| __ |/ /| |) | _|  | |/ _ \ (__| __ |
           *     |_|  |_|___/\___| /_/ \_\_|   |_/_/ \_\___|_||_/_/ |___/|___| |_/_/ \_\___|_||_|
           *
           */

          if(MSC_EnableFlag == TRUE)
          {
      		if(MSC_Attached == FALSE)
      		{
      			//See if we have a valid US connection before trying to connect
      			#ifdef	USB_MSC_DEVICE_IS_USB0
      				if(LPC_USB0->OTGSC & (1<<11) && (disk_status(MSC_DRIVE) == 0x0))
      				{
      					InitUSB_ROM_MSC();
      					MSC_Attached = TRUE;
      					TERMINAL_PRINTF("MSC is attached.\r\n");
      				}
      			#endif
      		}
      		else
      		{
      			 //if the disk status is greater than zero, something is wrong and disconnect
      			if(disk_status(MSC_DRIVE))
      			{
      				//Kill the USB
      				TERMINAL_PRINTF("Disk Problem. MSC is detached.\r\n");
      				USBD_API->hw->Connect(g_hUsb, 0);
      				MSC_Attached = FALSE;
      			}
      			else if((LPC_USB0->OTGSC & (1<<11)) == 0)
      			{
      				#ifdef ENABLE_MSC_CACHE
      				MARK_MSC_CACHE_AS_EMPTY;
      				#endif
      				//Kill the USB
      				TERMINAL_PRINTF("MSC is detached. \r\n");
      				USBD_API->hw->Connect(g_hUsb, 0);
      				MSC_Attached = FALSE;
      			}
      		}
          }
          else
          {
          	if(MSC_Attached == TRUE)
          	{
          		TERMINAL_PRINTF("MSC has been programmatically detached. \r\n");
          		USBD_API->hw->Connect(g_hUsb, 0);
          		MSC_Attached = FALSE;

          	}
          }


        /***
         *      __  __ ___  ___    ___         _
         *     |  \/  / __|/ __|  / __|__ _ __| |_  ___
         *     | |\/| \__ \ (__  | (__/ _` / _| ' \/ -_)
         *     |_|  |_|___/\___|  \___\__,_\__|_||_\___|
         *
         */

		#ifdef ENABLE_MSC_CACHE
        if((MSC_CacheFlushTicker++ > 5) && (MSC_CACHE_IS_NOT_EMPTY))
        {
        	MSC_CacheFlushTicker = 0;
        	//We need turn off USB interrupts 1st!
			#ifdef USB_MSC_DEVICE_IS_USB0
        	NVIC_DisableIRQ(USB0_IRQn);
			#else
        	NVIC_DisableIRQ(USB1_IRQn);
			#endif


        	disk_write(MSC_DRIVE,
        				(const char *)&MSC_Cache[0],
        	 	               // ((uint64_t) offset | (((uint64_t) hi_offset) << 32)) >> 9,
        				 MSC_CacheBaseAddress>>9,  //shift down by 9 (divide by 512) to get block address
        				 MSC_CACHE_SIZE/MSC_CACHE_WRITE_BLOCK_SIZE);

        	//Mark the cache as empty
        	 MARK_MSC_CACHE_AS_EMPTY;

        	#ifdef USB_MSC_DEVICE_IS_USB0
        		NVIC_EnableIRQ(USB0_IRQn);
        	#else
        		NVIC_EnableIRQ(USB1_IRQn);
        	#endif
        }
		#endif

        /***
         *      ___ ___
         *     / __|   \
         *     \__ \ |) |
         *     |___/___/
         *
         */


        //1st process the SD card

        //1st, seeif a desk is inserted
        if(SD_Stat & STA_NODISK)
        {
        	if(Chip_SDIF_CardNDetect(LPC_SDMMC) == 0)
        	{
        		TERMINAL_PRINTF("SD Inserted\r\n");
        		SD_Stat &= ~STA_NODISK;
        		SD_Stat |= STA_NOINIT ;//Set this to ensure we will properly init
        	}
        }
        //if inserted, let's see if we have initialized
        else
        {
        	if(SD_Stat & STA_NOINIT)
        	{
        	  InitSD();
        	  hCard = FSMCI_CardInit(); // this just gets a pointer to SD register base address

        	   if (!FSMCI_CardAcquire(hCard))
        	    {
        	              TERMINAL_PRINTF("SD Initialization Failure\r\n");
        	    }
               else
               {
                   TERMINAL_PRINTF("SD Initialization Success\r\n");
                   SD_Stat &= ~STA_NOINIT;
               }

        	}
        	else
        	{
        		if(Chip_SDIF_CardNDetect(LPC_SDMMC) == 1)
        		{
        			//Flag card removal
        			SD_Stat = STA_NOINIT | STA_NODISK;

        			TERMINAL_PRINTF("SD Removed.\r\n");

        		}
        	}
        }

        //see if the card is present
        if(Chip_SDIF_CardNDetect(LPC_SDMMC) == 0) //Disk is present if set to zero
        {

            SD_Stat &= ~STA_NODISK;

            if(SD_Stat & STA_NOINIT)
            {
                InitSD();
                hCard = FSMCI_CardInit(); // this just gets a pointer to SD register base address

                if (!FSMCI_CardAcquire(hCard))
                {
                    TERMINAL_PRINTF("SD Initialization Failure.\r\n");
                }

                else
                {
                    TERMINAL_PRINTF("SD Initialization Success.\r\n");
                    SD_Stat &= ~STA_NOINIT;

                }
            }

        }
        else // No disk present in this case
        {
            if((SD_Stat & STA_NODISK) == 0)
            {
                TERMINAL_PRINTF("SD Removed.\r\n");
            }

            SD_Stat|=STA_NODISK;
            SD_Stat|=STA_NOINIT;
        }
    }



    /***
     *      _   _ ___ ___   _  _  ___  ___ _____
     *     | | | / __| _ ) | || |/ _ \/ __|_   _|
     *     | |_| \__ \ _ \ | __ | (_) \__ \ | |
     *      \___/|___/___/ |_||_|\___/|___/ |_|
     *
     */

    switch(USB_HostEnumState)
    {
        case USB_HOST_STATE_DEVICE_INIT:

            USB_Stat = STA_NODISK | STA_NOINIT;

            hDisk = FSUSB_DiskInit();

            USB_HostEnumState =  USB_HOST_STATE_WAIT_FOR_DEVICE_ATTACH;

            break;

        case 	 USB_HOST_STATE_WAIT_FOR_DEVICE_ATTACH:

            USB_Stat = STA_NOINIT;

            if (USB_HostState[hDisk->Config.PortNumber] != HOST_STATE_Configured)
            {
                MS_Host_USBTask(hDisk);
                USB_USBTask(hDisk->Config.PortNumber, USB_MODE_Host);
            }


            break;

        case USB_HOST_STATE_DEVICE_ATTACHED_ENUMERATING:

            USB_Stat = STA_NOINIT;



            break;


        case USB_HOST_STATE_DEVICE_ATTACHED_GET_DISK_CAPACITY:

            USB_Stat = STA_NOINIT;

            if (FSUSB_GetDeviceCapacity(hDisk) == 0)
            {
                DEBUGOUT("Error retrieving device capacity.\r\n");
                USB_Host_SetDeviceConfiguration(hDisk->Config.PortNumber, 0);
                USB_HostEnumState = USB_HOST_STATE_DEVICE_ENUMERATION_ERROR;
            }
            else
            {
                USB_HostEnumState = USB_HOST_STATE_DEVICE_ATTACHED_READY;
                DEBUGOUT(("%lu blocks of %lu bytes on USB drive\r\n"), FSUSB_DiskGetSectorCnt(hDisk), FSUSB_DiskGetSectorSz(hDisk));
            }

            break;

        case USB_HOST_STATE_DEVICE_ATTACHED_READY:

            USB_Stat = 0;

            break;

        case USB_HOST_STATE_DEVICE_ENUMERATION_ERROR:

            USB_Stat =  STA_NOINIT;

            break;

        case USB_HOST_STATE_HOST_ERROR:
            USB_Stat =  STA_NOINIT;
            break;
    }

}
