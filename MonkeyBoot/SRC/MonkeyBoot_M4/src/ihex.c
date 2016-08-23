#include "ihex.h"
#include "stdlib.h"
#include "System.h"
#include "FileSystem.h"
#include "string.h"
#include "MonkeyBootConfig.h"

//Note,  FAT_FS _USE_STRFUNC must be set to 2 to strip \r from the lines for f_gets


//This function will verify the entire file is valid an i32 hex file:

//1.)Each line must start with a :
//2.)Each line must use an 00,01, 04 and 05 record
//3.)Each line must have a valid checksum
//4.)The last line must be a 01 record
//5.)The Byte Count Jives with what is in the the current line


uint8_t ASCII_NUM_TO_VAL(uint8_t x)
{
	if(x>=0x30 && x<=0x39)
		return x-0x30;
	else if(x>=0x61 && x<=0x66)
		return x- 0x57;
	else if(x>=0x41 && x<=0x46)
		return x - 0x37;
	else
		return 0;
}

uint8_t ASCII_BYTE_STRING_TO_VAL(char *S)
{
	uint8_t t;
	t =  (ASCII_NUM_TO_VAL(S[0]) * 16) + ASCII_NUM_TO_VAL(S[1]);
	return t;
}

uint32_t ASCII_UINT16_TO_VAL(char *S)
{
	uint32_t t;
	t =  ((uint32_t)ASCII_NUM_TO_VAL(S[0]) << 12) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[1]) << 8) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[2]) << 4) +
		  (uint32_t)ASCII_NUM_TO_VAL(S[3]);

	return t;
}

uint32_t ASCII_UINT32_TO_VAL(char *S)
{
	uint32_t t;
	t =  ((uint32_t)ASCII_NUM_TO_VAL(S[0]) << 28) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[1]) << 24) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[2]) << 20) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[3]) << 16) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[4]) << 12) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[5]) << 8) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[6]) << 4) +
		 ((uint32_t)ASCII_NUM_TO_VAL(S[7])     );

	return t;
}

void StripRightWhiteSpace(char * S)
{
	uint32_t i = 0;

	while(S[i] > 0x20 && S[i]<0x80 && (i<(IHEX_LINE_BUFFER_SIZE-1)))
	{
		i++;
	}

	S[i] = 0;
}


uint32_t ExtendedLinearAddress = 0x00000000; // Default to zero.  This will be modified by the 04 record types.


//Get the The Address in the line
uint32_t IHEX_GetAddress(char * IHEX_Line)
{
	uint32_t Address = ASCII_UINT16_TO_VAL(&IHEX_Line[3]);

	return Address;
}

uint32_t IHEX_GetByteCount(char * IHEX_Line)
{
	uint32_t BC = ASCII_BYTE_STRING_TO_VAL(&IHEX_Line[1]);
	return BC;
}

uint32_t IHEX_GetRecordType(char * IHEX_Line)
{
	uint32_t RT;

	memcpy(&RT,&IHEX_Line[7],2);

	return RT;
}



uint8_t ReadIHEX_32_Line(char *NextLine,iHEX_LineInfo *LI)
{
	uint8_t CheckSum = 0;
	uint8_t CheckSumInFile = 0;
	uint32_t i;

		//Make sure the line starts with a ":"
		if(NextLine[0] != ':')
		{
			EmitMsg("Line %d did not start with a :\r\n",LI->LineNumber);

			return IHEX_ERROR_BAD_LINE_START;
		}

		//now, evaluate the record type
		//We will store the record type based upon the 16-bit number formed by the ASCII codes,  not the actual numerical value

		LI->RecordType = IHEX_GetRecordType(&NextLine[0]);

		if(
				LI->RecordType!= IHEX_RECORD_TYPE_00 &&
				LI->RecordType!= IHEX_RECORD_TYPE_01 &&
				LI->RecordType!= IHEX_RECORD_TYPE_04 &&
				LI->RecordType!= IHEX_RECORD_TYPE_05
		  )
		{
			EmitMsg("Line %d had a record that is not valid ihex32\r\n",LI->LineNumber);
			LI->ByteCount = 0;
			return IHEX_ERROR_INVALID_RECORD;
		}

		//check that the byte count field matches the correct value from the number of bytes in the line
		LI->ByteCount = IHEX_GetByteCount(&NextLine[0]) ;
			if( LI->ByteCount !=
		 	((strlen(NextLine) - 11)/2) //we need to subtract out all the bytes not part of the data field
		  )
		{
				EmitMsg("Line %d has data field that doesn't match the byte count. \r\n",LI->LineNumber);

				return IHEX_ERROR_BAD_BYTE_COUNT;
		}


		//Check the the checksum
		CheckSum = 0;

		for(i=0;i<(LI->ByteCount + 4);i++)
		{
			CheckSum += ASCII_BYTE_STRING_TO_VAL(&NextLine[1 + (i*2)]);
		}

		CheckSumInFile = ASCII_BYTE_STRING_TO_VAL(&NextLine[ 9 + (LI->ByteCount *2)]);

		CheckSum &= 0xff;

		CheckSum = ~CheckSum;

		CheckSum +=1;

		if((CheckSum&0xFF) != CheckSumInFile)
		{
			EmitMsg("Line %d has a bad checksum \r\n",LI->LineNumber);
			return IHEX_ERROR_BAD_CHECKSUM;
		}

		if(LI->RecordType ==  IHEX_RECORD_TYPE_01)
		{

		}
		//The line passes all checks that it is valid.   Let's see if it is address or Data and if it falls in a valid range
		if(LI->RecordType == IHEX_RECORD_TYPE_04)
		{
				//If we have a 04 record,  then update our extended linear Address.  This is the upper sixteen bits of the
				//32-bit base address
				LI->ExtendedLinearAddress = ASCII_UINT16_TO_VAL(&NextLine[9])<<16;
		}
		if(LI->RecordType == IHEX_RECORD_TYPE_05)
		{
			LI->ExtendedLinearAddress = ASCII_UINT32_TO_VAL(&NextLine[9]);
		}

		if(LI->RecordType == IHEX_RECORD_TYPE_00)
		{
			LI->DataStartAddress = LI->ExtendedLinearAddress + IHEX_GetAddress(&NextLine[0]);
			LI->DataEndAddress = LI->DataStartAddress + IHEX_GetByteCount(&NextLine[0]);

			for(i=0;i<LI->ByteCount;i++)
			{
				LI->LineData[i] = ASCII_BYTE_STRING_TO_VAL(&NextLine[9 + (i*2)]);
			}
		}

		return IHEX_LINE_GOOD;
}

/*
EmitMsg("Hex file: 0x%08X  to  0x%08X intersects bootloader memory 0x%08X to 0x%08x \r\n",
						LI->DataStartAddress,
						LI->DataEndAddress,
						MyBootloaderMemoryRanges[i].Start,
						(MyBootloaderMemoryRanges[i].Start + MyBootloaderMemoryRanges[i].Length)
						);
*/

//This function checks to see if we are in a particular memory range
//returns the memory range index if the line falls in a memory range.
//returns 0xFFFFFFFF if not in any range

uint32_t GetRangeIndex(iHEX_LineInfo *LI,const MemoryRange *M,uint32_t NumRanges)
{
	uint32_t i;

	for(i=0;i<NumRanges;i++)
		{
			if(	    (
					   (LI->DataStartAddress >= M[i].Start)
					&& (LI->DataStartAddress < (M[i].Start + M[i].Length))
					)

					||

					(
					   (LI->DataEndAddress >= M[i].Start)
					&& (LI->DataEndAddress < (M[i].Start + M[i].Length))
					)
				)
			{

				return i;
			}
		}

	return IHEX_ERROR_DATA_NOT_IN_VALID_RANGE;
}

/*
uint8_t CheckIfDataIsInValidRange(iHEX_LineInfo *LI)
{
	uint32_t i;
	uint8_t LineInValidRange = 0;
	 //See if this data intersects the memory where the bootloader is located.
	for(i=0;i<NUM_BOOTLOADER_MEMORY_RANGES;i++)
	{
		if(	    (
				   (LI->DataStartAddress >= MyBootloaderMemoryRanges[i].Start)
				&& (LI->DataStartAddress < (MyBootloaderMemoryRanges[i].Start + MyBootloaderMemoryRanges[i].Length))
				)

				||

				(
				   (LI->DataEndAddress >= MyBootloaderMemoryRanges[i].Start)
				&& (LI->DataEndAddress < (MyBootloaderMemoryRanges[i].Start + MyBootloaderMemoryRanges[i].Length))
				)
			)
		{
			EmitMsg("Hex file: 0x%08X  to  0x%08X intersects bootloader memory 0x%08X to 0x%08x \r\n",
					LI->DataStartAddress,
					LI->DataEndAddress,
					MyBootloaderMemoryRanges[i].Start,
					(MyBootloaderMemoryRanges[i].Start + MyBootloaderMemoryRanges[i].Length)
					);

			return IHEX_ERROR_DATA_IN_BOOTLOADER_SPACE;
		}
	}

	LineInValidRange = 0;
	//See if data is a valid range
	for(i=0;i<NUM_PROGRAMMABLE_MEMORY_RANGES;i++)
	{
			if(	    (
					(LI->DataStartAddress >= MyProgrammableMemoryRanges[i].Start)
					&& (LI->DataStartAddress <= (MyBootloaderMemoryRanges[i].Start + MyProgrammableMemoryRanges[i].Length))
					)

					||

					(
					(LI->DataEndAddress >= MyProgrammableMemoryRanges[i].Start)
					&& (LI->DataEndAddress <= (MyProgrammableMemoryRanges[i].Start + MyProgrammableMemoryRanges[i].Length))
					)
				)
				{
						LineInValidRange = 1;
						break;
				}
	}


	if(LineInValidRange == 0)
	{

		EmitMsg("Hex file: 0x%08X  to  0x%08X is not in any of the valid ranges. \r\n",
										LI->DataStartAddress,
										LI->DataEndAddress
										);

		for(i=0;i<NUM_PROGRAMMABLE_MEMORY_RANGES;i++)
		{
			EmitMsg("Range %d: 0x%08X  to  0x%08X\r\n",
					i,
					MyProgrammableMemoryRanges[i].Start,
					(MyProgrammableMemoryRanges[i].Start + MyProgrammableMemoryRanges[i].Length)
					);
		}

		return IHEX_ERROR_DATA_NOT_IN_VALID_RANGE;
	}

	return IHEX_LINE_IS_IN_VALID_RANGE;
}

*/

uint32_t  VerifyIHEX_32(char * Filename,iHEX_Info *IHI)
{
	char LineBuffer [256];
	FIL  MyFile;
	uint8_t EndOfFileRecordFound = 0;
    uint8_t Result = 0;
	iHEX_LineInfo LI;
	uint32_t i;

	IHI->BinaryFileSize = 0;
	IHI->FileIsVerified = 0;
	IHI->LineCount = 0;

	if(f_open(&MyFile,Filename,FA_READ) != FR_OK)
	{
		EmitMsg("Error opening hex file for verification.\r\n");
		return IHEX_ERROR_COULD_NOT_OPEN_FILE;
	}

	LI.LineNumber = 1;
	LI.RecordType = 0xff;
	LI.ExtendedLinearAddress = 0;
	LI.DataStartAddress = 0;
	LI.DataEndAddress = 0;



	while(!f_eof(&MyFile))
	{
		f_gets (
				  &LineBuffer[0],
				  IHEX_LINE_BUFFER_SIZE,
				  &MyFile
				);

		StripRightWhiteSpace(&LineBuffer[0]);

		if(strlen((char *)&LineBuffer) == 0)
			continue;

		if(EndOfFileRecordFound == 1)
		{
			EmitMsg("More data found after the end of file record!\r\n");
			f_close(&MyFile);
			return IHEX_ERROR_BAD_END;
		}

		Result =  ReadIHEX_32_Line(&LineBuffer[0],&LI);

		if(Result != IHEX_LINE_GOOD)
		{
			f_close(&MyFile);
			return Result;
		}
		else
		{
			if(LI.RecordType == IHEX_RECORD_TYPE_01)
				EndOfFileRecordFound = 1;

			//if the line has data, let's make sure it is in the range of what we can program!
			if(LI.RecordType == IHEX_RECORD_TYPE_00)
			{
				if( (i = GetRangeIndex(&LI,MyBootloaderMemoryRanges,NUM_BOOTLOADER_MEMORY_RANGES) != IHEX_ERROR_DATA_NOT_IN_VALID_RANGE))
				{
					EmitMsg("Hex file: 0x%08X  to  0x%08X intersects bootloader memory 0x%08X to 0x%08x \r\n",
										LI.DataStartAddress,
										LI.DataEndAddress,
										MyBootloaderMemoryRanges[i].Start,
										(MyBootloaderMemoryRanges[i].Start + MyBootloaderMemoryRanges[i].Length)
										);

					f_close(&MyFile);
					return IHEX_ERROR_DATA_IN_BOOTLOADER_SPACE;

				}
				else if (GetRangeIndex(&LI,MyProgrammableMemoryRanges,NUM_PROGRAMMABLE_MEMORY_RANGES) == IHEX_ERROR_DATA_NOT_IN_VALID_RANGE)
				{
					EmitMsg("Hex file: 0x%08X  to  0x%08X is not in any of the valid ranges. \r\n",
															LI.DataStartAddress,
															LI.DataEndAddress
															);

							for(i=0;i<NUM_PROGRAMMABLE_MEMORY_RANGES;i++)
							{
								EmitMsg("Range %d: 0x%08X  to  0x%08X\r\n",
										i,
										MyProgrammableMemoryRanges[i].Start,
										(MyProgrammableMemoryRanges[i].Start + MyProgrammableMemoryRanges[i].Length)
										);
							}

					f_close(&MyFile);
					return IHEX_ERROR_DATA_NOT_IN_VALID_RANGE;
				}
				else
				{
					IHI->BinaryFileSize += LI.ByteCount;
					IHI->LineCount++;
					LI.LineNumber++;
				}
			}

		}
	}

	if(EndOfFileRecordFound != 1)
	{
		EmitMsg("End of file record not found.\r\n");
		f_close(&MyFile);
		return IHEX_ERROR_BAD_END;
	}

	IHI->FileIsVerified = TRUE;

	EmitMsg("%d lines analyzed. Hex file looks good! \r\n", IHI->LineCount);
	EmitMsg("%d bytes in file \r\n", 	IHI->BinaryFileSize);

	f_close(&MyFile);
	return IHEX_HEX_FILE_OK;

}
