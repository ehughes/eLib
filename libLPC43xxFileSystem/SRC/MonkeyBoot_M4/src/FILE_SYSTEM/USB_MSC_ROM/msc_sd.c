/*
 * @brief File contains callback to MSC driver backed by a memory disk.
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2013
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

#include <string.h>
#include "board.h"
#include "..\FileSystemMasterConfig.h"
#include "app_usbd_cfg.h"
#include "msc_disk.h"
#include "ff.h"
#include "diskio.h"
#include <stdlib.h>
#include "msc_sd.h"

#define COMPUTE_CACHE_BASE(x)			(x & ~(MSC_CACHE_SIZE - 1))
#define COMPUTE_CACHE_OFFSET(x)			(x &  (MSC_CACHE_SIZE - 1))
#define IS_ADDRESS_IN_CACHE(x,y)		(COMPUTE_CACHE_BASE(x) == y)


__BSS(MSC_CACHE_REGION)	uint8_t 	MSC_Cache[MSC_CACHE_SIZE];
int32_t							    MSC_CacheWritePtr = -1;
uint32_t							MSC_CacheBaseAddress = 0;
volatile uint32_t					MSC_CacheFlushTicker;


/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

static const uint8_t g_InquiryStr[] = {'N', 'X', 'P', ' ', ' ', ' ', ' ', ' ',	   \
                                       'L', 'P', 'C', ' ', 'M', 'e', 'm', ' ',	   \
                                       'D', 'i', 's', 'k', ' ', ' ', ' ', ' ',	   \
                                       '1', '.', '0', ' ',
                                      };
/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* USB device mass storage class read callback routine */
static void translate_rd(uint32_t offset, uint8_t **buff_adr, uint32_t length, uint32_t hi_offset)
{
    uint8_t TmpSector[512];

	if(length == 31 || length == 0)
        return;



#ifdef ENABLE_MSC_CACHE


    if(IS_ADDRESS_IN_CACHE(offset,MSC_CacheBaseAddress) && MSC_CACHE_IS_NOT_EMPTY)
     {
     	memcpy(*buff_adr,&MSC_Cache[COMPUTE_CACHE_OFFSET(offset)],length);
     }
     else
     {
    	 if(length == 512)
    	    {
    	        //Translate to reads through the FatFs DiskIO Layer
    	        disk_read(MSC_DRIVE,
    	                  *buff_adr,
    	                  // (((uint64_t) offset) | (((uint64_t) hi_offset) << 32))>>9,  //hi_offset is junk
    	                  offset>>9,  //shift down by 9 (divide by 512) to get block address
    	                  length>>9);
    	    }
    	    else if (length == 64)
    	    {
    	        disk_read(MSC_DRIVE,
    	                      &TmpSector,
    	                      // (((uint64_t) offset) | (((uint64_t) hi_offset) << 32))>>9,  //hi_offset is junk
    	                      offset>>9,  //shift down by 9 (divide by 512) to get block address
    	                      1);

    	    	memcpy(*buff_adr,&TmpSector[offset & 0x1C0],64);
    	    }

     }
#else

    if(length == 512)
    {
        //Translate to reads through the FatFs DiskIO Layer
        disk_read(MSC_DRIVE,
                  *buff_adr,
                  // (((uint64_t) offset) | (((uint64_t) hi_offset) << 32))>>9,  //hi_offset is junk
                  offset>>9,  //shift down by 9 (divide by 512) to get block address
                  length>>9);
    }
    else if (length == 64)
    {
        disk_read(MSC_DRIVE,
                      &TmpSector,
                      // (((uint64_t) offset) | (((uint64_t) hi_offset) << 32))>>9,  //hi_offset is junk
                      offset>>9,  //shift down by 9 (divide by 512) to get block address
                      1);

    	memcpy(*buff_adr,&TmpSector[offset & 0x1C0],64);
    }



#endif

}

/* USB device mass storage class write callback routine */
static void translate_wr(uint32_t offset, uint8_t **buff_adr, uint32_t length, uint32_t hi_offset)
{

    uint8_t TmpSector[512];

    if(length == 31 || length == 0)
        return;

#ifdef ENABLE_MSC_CACHE

    //step one,

    //see if the cache is empty
    if(MSC_CACHE_IS_EMPTY)
    {
      //in this case the cache is empty.   Fill the cache
      MSC_CacheBaseAddress = COMPUTE_CACHE_BASE(offset);
      MSC_CacheWritePtr = COMPUTE_CACHE_OFFSET(offset);

        //Translate to reads through the FatFs DiskIO Layer
        disk_read(MSC_DRIVE,
        		  &MSC_Cache[0],
    	              // (((uint64_t) offset) | (((uint64_t) hi_offset) << 32))>>9,  //hi_offset is junk
                  MSC_CacheBaseAddress>>9,  //shift down by 9 (divide by 512) to get block address
                 (MSC_CACHE_SIZE)>>9); // disk read needs the length in blocks
    }

    //See if the write is in our cache location
    if(IS_ADDRESS_IN_CACHE(offset,MSC_CacheBaseAddress))
    {
    	MSC_CacheWritePtr = COMPUTE_CACHE_OFFSET(offset);

    	memcpy(&MSC_Cache[MSC_CacheWritePtr],*buff_adr,length);
    }
    else
    {
    	//if we are out of the cache,  then, flush the old and re fill
    	disk_write(MSC_DRIVE,
    				&MSC_Cache[0],
    	               // ((uint64_t) offset | (((uint64_t) hi_offset) << 32)) >> 9,
    				 MSC_CacheBaseAddress>>9,  //shift down by 9 (divide by 512) to get block address
    				 MSC_CACHE_SIZE/MSC_CACHE_WRITE_BLOCK_SIZE);

    	//refill the cache
    	MSC_CacheBaseAddress = COMPUTE_CACHE_BASE(offset);
    	MSC_CacheWritePtr = COMPUTE_CACHE_OFFSET(offset);

		disk_read(MSC_DRIVE,
		   		  &MSC_Cache[0],
		              // (((uint64_t) offset) | (((uint64_t) hi_offset) << 32))>>9,  //hi_offset is junk
		          MSC_CacheBaseAddress>>9,  //shift down by 9 (divide by 512) to get block address
		          (MSC_CACHE_SIZE)>>9); // disk read needs the length in blocks

    	//Write to the new location
	 	memcpy(&MSC_Cache[MSC_CacheWritePtr],*buff_adr,length);
    }

    //reset the timeout on flushes
    MSC_CacheFlushTicker = 0;

#else


	if(length ==512)
	{
		 disk_write(MSC_DRIVE,
               *buff_adr,
               // ((uint64_t) offset | (((uint64_t) hi_offset) << 32)) >> 9,
               offset>>9,  //shift down by 9 (divide by 512) to get block address
               1);
	}
	else if(length == 64)
	{
        disk_read(MSC_DRIVE,
                      &TmpSector,
                      // (((uint64_t) offset) | (((uint64_t) hi_offset) << 32))>>9,  //hi_offset is junk
                      offset>>9,  //shift down by 9 (divide by 512) to get block address
                      1);

    	memcpy(&TmpSector[offset & 0x1C0],*buff_adr,64);

    	disk_write(MSC_DRIVE,
    			 &TmpSector,
                  // ((uint64_t) offset | (((uint64_t) hi_offset) << 32)) >> 9,
                  offset>>9,  //shift down by 9 (divide by 512) to get block address
                  1);

	}
#endif

}


/* USB device mass storage class verify callback routine */
static ErrorCode_t translate_verify(uint32_t offset, uint8_t *src, uint32_t length, uint32_t hi_offset)
{
    return LPC_OK;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/* Memory storage based MSC_Disk init routine */
ErrorCode_t mscDisk_init(USBD_HANDLE_T hUsb, USB_CORE_DESCS_T *pDesc, USBD_API_INIT_PARAM_T *pUsbParam)
{
    USBD_MSC_INIT_PARAM_T msc_param;
    ErrorCode_t ret = LPC_OK;
    DWORD NumSectors = 0;

    memset((void *) &msc_param, 0, sizeof(USBD_MSC_INIT_PARAM_T));

    msc_param.mem_base = pUsbParam->mem_base;
    msc_param.mem_size = pUsbParam->mem_size;
    /* mass storage paramas */
    msc_param.InquiryStr = (uint8_t *) g_InquiryStr;


    disk_ioctl(MSC_DRIVE, GET_SECTOR_COUNT, (void *)&NumSectors);


    if(NumSectors >= (1<<(32-9)))
    NumSectors = (1<<(32-9)) - 1;

    msc_param.BlockCount = NumSectors;
    msc_param.BlockSize = 512;
    msc_param.MemorySize = (uint64_t)NumSectors   * (uint64_t)512;



    /* Install memory storage callback routines */
    msc_param.MSC_Write = translate_wr;
    msc_param.MSC_Read = translate_rd;
    msc_param.MSC_Verify = translate_verify;
    msc_param.MSC_GetWriteBuf = 0;
    //msc_param.MSC_GetWriteBuf = 0;
    msc_param.intf_desc = (uint8_t *) find_IntfDesc(pDesc->high_speed_desc, USB_DEVICE_CLASS_STORAGE);

    ret = USBD_API->msc->init(hUsb, &msc_param);
    /* update memory variables */
    pUsbParam->mem_base = msc_param.mem_base;
    pUsbParam->mem_size = msc_param.mem_size;


    MSC_CacheWritePtr = -1;

    return ret;
}
