#include "ihex.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
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


//Makes ans ASCII character from the lower 4bits of the Data variable
void HEX_NIBBLE_TO_ASCII(char *S, uint8_t Data)
{
	Data &= 0xF;

	if (Data < 0xA)
	{
		*S = 0x30 + Data;
	}
	else
	{
		*S= 'A' + (Data - 0xA);
	}
}



void BYTE_TO_ASCII(char * S, uint8_t Data)
{
	 HEX_NIBBLE_TO_ASCII(&S[0],Data >> 4);
	 HEX_NIBBLE_TO_ASCII(&S[1], Data);
}

void UINT16_TO_ASCII(char * S, uint16_t Data)
{
	HEX_NIBBLE_TO_ASCII(&S[0], (uint8_t)(Data >> 12));
	HEX_NIBBLE_TO_ASCII(&S[1], (uint8_t)(Data >> 8));
	HEX_NIBBLE_TO_ASCII(&S[2], (uint8_t)(Data >> 4));
	HEX_NIBBLE_TO_ASCII(&S[3], (uint8_t)(Data));
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

uint8_t ComputeCheckSumFromLineData(char *NextLine, iHEX_LineInfo *LI)
{
	//Check the the checksum
	uint8_t CheckSum = 0;
	uint32_t i;

	for (i = 0; i<(LI->ByteCount + 4); i++)
	{
		CheckSum += ASCII_BYTE_STRING_TO_VAL(&NextLine[1 + (i * 2)]);
	}

	CheckSum &= 0xff;

	CheckSum = ~CheckSum;

	CheckSum += 1;

	return CheckSum;
}

void MakeIHEX_32_Line(char *Line, iHEX_LineInfo *LI)
{
	uint32_t i = 0;
	uint8_t CheckSum = 0;

	Line[IHEX_START_CODE_LOC] = ':';

	//Write the Byte count field

	BYTE_TO_ASCII(&Line[IHEX_BYTE_COUNT_LOC], LI->ByteCount);
	
	//Write the address field

	UINT16_TO_ASCII(&Line[IHEX_ADDRESS_LOC], LI->iHEX_AddressField);
	
	//write the Record type
	Line[IHEX_RECORD_TYPE_LOC] = (uint8_t)(LI->RecordType);
	Line[IHEX_RECORD_TYPE_LOC + 1] = (uint8_t)(LI->RecordType >> 8);

	for (i = 0; i < LI->ByteCount; i++)
	{
		BYTE_TO_ASCII(&Line[IHEX_DATA_LOC + (2 * i)], LI->LineData[i]);
	}

	CheckSum = ComputeCheckSumFromLineData(&Line[0], LI);

	BYTE_TO_ASCII(&Line[IHEX_DATA_LOC + (2 * LI->ByteCount)], CheckSum);

	//Null Terminate the line
	Line[IHEX_DATA_LOC + (2 * LI->ByteCount) + 2] = 0;

}



uint8_t ReadIHEX_32_Line(char *NextLine,iHEX_LineInfo *LI)
{
	uint8_t CheckSum = 0;
	uint8_t CheckSumInFile = 0;
	uint32_t i;

		//Make sure the line starts with a ":"
		if(NextLine[0] != ':')
		{
			printf("Line %d did not start with a :\r\n",LI->LineNumber);

			return IHEX_ERROR_BAD_LINE_START;
		}

		//now, evaluate the record type
		//We will store the record type based upon the 16-bit number formed by the ASCII codes,  not the actual numerical value

		LI->RecordType = IHEX_GetRecordType(&NextLine[0]);

		if(
				LI->RecordType!= IHEX_RECORD_TYPE_00 &&
				LI->RecordType!= IHEX_RECORD_TYPE_01 &&
				LI->RecordType!= IHEX_RECORD_TYPE_04 &&
				LI->RecordType!= IHEX_RECORD_TYPE_05 &&
				LI->RecordType!= IHEX_RECORD_TYPE_06
		  )
		{
			printf("Line %d had a record that is not valid ihex32\r\n",LI->LineNumber);
			LI->ByteCount = 0;
			return IHEX_ERROR_INVALID_RECORD;
		}

		//check that the byte count field matches the correct value from the number of bytes in the line
		LI->ByteCount = IHEX_GetByteCount(&NextLine[0]) ;
			if( LI->ByteCount !=
		 	((strlen(NextLine) - 11)/2) //we need to subtract out all the bytes not part of the data field
		  )
		{
				printf("Line %d has data field that doesn't match the byte count. \r\n", LI->LineNumber);

				return IHEX_ERROR_BAD_BYTE_COUNT;
		}


		//Check the the checksum
		CheckSum = ComputeCheckSumFromLineData(&NextLine[0], LI);

		CheckSumInFile = ASCII_BYTE_STRING_TO_VAL(&NextLine[ 9 + (LI->ByteCount *2)]);

		if((CheckSum&0xFF) != CheckSumInFile)
		{
			printf("Line %d has a bad checksum \r\n", LI->LineNumber);
			return IHEX_ERROR_BAD_CHECKSUM;
		}

		//Store the address  in the line so we have it

		LI->iHEX_AddressField = IHEX_GetAddress(&NextLine[0]);

	
		if(LI->RecordType ==  IHEX_RECORD_TYPE_01)
		{
			LI->ByteCount = 0;
		}
		//The line passes all checks that it is valid.   Let's see if it is address or Data and if it falls in a valid range
		if(LI->RecordType == IHEX_RECORD_TYPE_04)
		{
				//If we have a 04 record,  then update our extended linear Address.  This is the upper sixteen bits of the
				//32-bit base address
				LI->ExtendedLinearAddress = ASCII_UINT16_TO_VAL(&NextLine[9])<<16;
				//Copy over the line data!
				for (i = 0; i<LI->ByteCount; i++)
				{
					LI->LineData[i] = ASCII_BYTE_STRING_TO_VAL(&NextLine[9 + (i * 2)]);
				}
		}
		if(LI->RecordType == IHEX_RECORD_TYPE_05)
		{
			LI->ExtendedLinearAddress = ASCII_UINT32_TO_VAL(&NextLine[9]);
			//The extended linear adddress is also put in the line data.... Let's copy it over!
			for (i = 0; i<LI->ByteCount; i++)
			{
				LI->LineData[i] = ASCII_BYTE_STRING_TO_VAL(&NextLine[9 + (i * 2)]);
			}
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

		//A 06 record is a special non standard record used for encoding a version string.
		//Parse it like a data record.   The actual data will be a null terminated string
		//Address data is not used
		if(LI->RecordType == IHEX_RECORD_TYPE_06)
		{
			LI->DataStartAddress = LI->ExtendedLinearAddress + IHEX_GetAddress(&NextLine[0]);
			LI->DataEndAddress = LI->DataStartAddress + IHEX_GetByteCount(&NextLine[0]);

			for(i=0;i<LI->ByteCount;i++)
			{
				LI->LineData[i] = ASCII_BYTE_STRING_TO_VAL(&NextLine[9 + (i*2)]);
			}

			if(i<IHEX_LINE_BUFFER_SIZE-1)
				i++;

			LI->LineData[i] = 0x00;  //Make sure to null terminate
		}

		return IHEX_LINE_GOOD;
}
