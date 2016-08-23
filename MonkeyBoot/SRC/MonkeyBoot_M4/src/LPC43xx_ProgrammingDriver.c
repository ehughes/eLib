#include "System.h"




uint32_t LPC43xx_FLASH_CacheBase = 0xFFFFFFFF;
uint8_t LPC43xx_FLASH_Cache[LPC43xx_FLASH_CACHE_SIZE];

void LPC43xx_InitFlashDriver()
{
	uint32_t i;

	LPC43xx_FLASH_CacheBase = 0xFFFFFFFF;
	for(i=0;i<LPC43xx_FLASH_CACHE_SIZE;i++)
	{
		LPC43xx_FLASH_Cache[i] = 0xFF;
	}
}

uint32_t LPC43xx_GetSectorFromAddress(uint32_t Address,uint32_t *FlashBank)
{
	uint32_t R;

	if(Address>= 0x1A000000 &&
	   Address<=0x1A07FFFF)
	{
		*FlashBank = LPC43xx_FLASH_BANK_A;

		if(Address>=0x1A000000	&&	Address<=0x1A001FFF){R =  0	;}
		if(Address>=0x1A002000	&&	Address<=0x1A003FFF){R =  1	;}
		if(Address>=0x1A004000	&&	Address<=0x1A005FFF){R =  2	;}
		if(Address>=0x1A006000	&&	Address<=0x1A007FFF){R =  3	;}
		if(Address>=0x1A008000	&&	Address<=0x1A009FFF){R =  4	;}
		if(Address>=0x1A00A000	&&	Address<=0x1A00BFFF){R =  5	;}
		if(Address>=0x1A00C000	&&	Address<=0x1A00DFFF){R =  6	;}
		if(Address>=0x1A00E000	&&	Address<=0x1A00FFFF){R =  7	;}
		if(Address>=0x1A010000	&&	Address<=0x1A01FFFF){R =  8	;}
		if(Address>=0x1A020000	&&	Address<=0x1A02FFFF){R =  9	;}
		if(Address>=0x1A030000	&&	Address<=0x1A03FFFF){R =  10;}
		if(Address>=0x1A040000	&&	Address<=0x1A04FFFF){R =  11;}
		if(Address>=0x1A050000	&&	Address<=0x1A05FFFF){R =  12;}
		if(Address>=0x1A060000	&&	Address<=0x1A06FFFF){R =  13;}
		if(Address>=0x1A070000	&&	Address<=0x1A07FFFF){R =  14;}
	}
	else if (Address>= 0x1B000000 &&
			   Address<=0x1B07FFFF)
	{
		*FlashBank = LPC43xx_FLASH_BANK_B;

		if(Address>=0x1B000000	&&	Address<=0x1B001FFF){R =  0	;}
		if(Address>=0x1B002000	&&	Address<=0x1B003FFF){R =  1	;}
		if(Address>=0x1B004000	&&	Address<=0x1B005FFF){R =  2	;}
		if(Address>=0x1B006000	&&	Address<=0x1B007FFF){R =  3	;}
		if(Address>=0x1B008000	&&	Address<=0x1B009FFF){R =  4	;}
		if(Address>=0x1B00A000	&&	Address<=0x1B00BFFF){R =  5	;}
		if(Address>=0x1B00C000	&&	Address<=0x1B00DFFF){R =  6	;}
		if(Address>=0x1B00E000	&&	Address<=0x1B00FFFF){R =  7	;}
		if(Address>=0x1B010000	&&	Address<=0x1B01FFFF){R =  8	;}
		if(Address>=0x1B020000	&&	Address<=0x1B02FFFF){R =  9	;}
		if(Address>=0x1B030000	&&	Address<=0x1B03FFFF){R =  10;}
		if(Address>=0x1B040000	&&	Address<=0x1B04FFFF){R =  11;}
		if(Address>=0x1B050000	&&	Address<=0x1B05FFFF){R =  12;}
		if(Address>=0x1B060000	&&	Address<=0x1B06FFFF){R =  13;}
		if(Address>=0x1B070000	&&	Address<=0x1B07FFFF){R =  14;}

	}
	else
	{
		*FlashBank = 0xFF;
		R = LPC43xx_ADDRESS_NOT_IN_FLASH_BANK;
	}

	return R;
}




uint32_t LPC43xx_FLASH_FlushCache()
{
	uint32_t FlashBank = 0xFFFFFFFF;
	uint32_t Sector;
	uint32_t i;

	Sector = LPC43xx_GetSectorFromAddress(LPC43xx_FLASH_CacheBase,&FlashBank);


	if(FlashBank != LPC43xx_FLASH_BANK_A &&
	   FlashBank != LPC43xx_FLASH_BANK_B)
	{
		EmitMsg("LPC43xx Cache base of 0x%08x is not in flash bank A or B", LPC43xx_FLASH_CacheBase);
		return LPC43xx_PROGRAMMING_ERROR_NOT_IN_VALID_RANGE;
	}
	else if (Sector > 14)
	{
		EmitMsg("LPC43xx Sector %d is not valid (must be less than 14)\r\n", Sector);
		return LPC43xx_PROGRAMMING_ERROR_NOT_IN_VALID_RANGE;
	}
	else
	{

		EmitMsg("Flushing %d bytes to address 0x%08x in Flash bank %d Sector %d\r\n",
				LPC43xx_FLASH_CACHE_SIZE,
				LPC43xx_FLASH_CacheBase,
				FlashBank,
				Sector);

		__disable_irq();
		i =  Chip_IAP_PreSectorForReadWrite(Sector, Sector, FlashBank);
		__enable_irq();

		if (i!= IAP_CMD_SUCCESS)
		{
			EmitMsg("LPC43xx Sector %d is not valid (must be less than 14)\r\n", Sector);
			return LPC43xx_PROGRAMMING_ERROR_PREPARE_SECTOR;
		}

		__disable_irq();
		i = Chip_IAP_CopyRamToFlash(LPC43xx_FLASH_CacheBase,
									(uint32_t *)&LPC43xx_FLASH_Cache[0],
									LPC43xx_FLASH_CACHE_SIZE);
		__enable_irq();

		if (i!= IAP_CMD_SUCCESS)
			{
				EmitMsg("LPC43xx Flash Programming Error at address 0x%08x.  Return code %i\r\n", LPC43xx_FLASH_CacheBase,i);
				return LPC43xx_PROGRAMMING_ERROR_PREPARE_SECTOR;
			}

	}

	return LPC43xx_PROGRAMMING_OK;
}




uint32_t LPC43xx_FLASH_Program_iHexLine(iHEX_LineInfo * LI)
{
	uint32_t CacheOffset;

	//Check to see if the Cache has it's init values

	if(LPC43xx_FLASH_CacheBase == 0xFFFFFFFF)
	{

		LPC43xx_FLASH_CacheBase = LPC43xx_COMPUTE_FLASH_CACHE_BASE(LI->DataStartAddress);
	}

	/*
	if( (LI->DataStartAddress + LI->ByteCount) > (LPC43xx_FLASH_CacheBase + LPC43xx_FLASH_CACHE_SIZE))
	{

		EmitMsg("iHEX Cache Misalignment for LPC43xx Flash driver\r\n");
		return LPC43xx_PROGRAMMING_ERROR_CACHE_MISALIGNMENT;
	}*/

	//if we are out of the current Cache range, we need to Flush the cache to the actual buffer
	if((LI->DataStartAddress >= (LPC43xx_FLASH_CacheBase + LPC43xx_FLASH_CACHE_SIZE))  ||
	   (LI->DataStartAddress < (LPC43xx_FLASH_CacheBase))
	   )
	{
		LPC43xx_FLASH_FlushCache();
		LPC43xx_FLASH_CacheBase = LPC43xx_COMPUTE_FLASH_CACHE_BASE(LI->DataStartAddress);
	}


	CacheOffset = LPC43xx_COMPUTE_FLASH_CACHE_OFFSET(LI->DataStartAddress);

	memcpy(&LPC43xx_FLASH_Cache[CacheOffset], &LI->LineData[0], LI->ByteCount);

	return LPC43xx_PROGRAMMING_OK;
}

