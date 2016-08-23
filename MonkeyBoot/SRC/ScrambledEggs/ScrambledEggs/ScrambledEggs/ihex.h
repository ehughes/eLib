#include <stdint.h>

#ifndef IHEX_H
#define IHEX_H


#define IHEX_LINE_BUFFER_SIZE			256


#define IHEX_RECORD_TYPE_00				0x3030//This is the actual number value for the string 00
#define IHEX_RECORD_TYPE_01				0x3130//This is the actual number value for the string 01
#define IHEX_RECORD_TYPE_02				0x3230//This is the actual number value for the string 02
#define IHEX_RECORD_TYPE_03				0x3330//This is the actual number value for the string 03
#define IHEX_RECORD_TYPE_04				0x3430//This is the actual number value for the string 04
#define IHEX_RECORD_TYPE_05				0x3530//This is the actual number value for the string 05
#define IHEX_RECORD_TYPE_06				0x3630//This is the actual number value for the string 06
											  //and 06 record is a custom record that stores a string

#define IHEX_START_CODE_LOC	0
#define IHEX_BYTE_COUNT_LOC		(IHEX_START_CODE_LOC+1)
#define IHEX_ADDRESS_LOC		(IHEX_BYTE_COUNT_LOC+2)
#define IHEX_RECORD_TYPE_LOC	(IHEX_ADDRESS_LOC+4)
#define IHEX_DATA_LOC			(IHEX_RECORD_TYPE_LOC+2)

typedef struct
{
	uint32_t BinaryFileSize;
	uint32_t LineCount;
	uint8_t FileIsVerified;
}	iHEX_Info;


typedef struct
{
	char LineData[IHEX_LINE_BUFFER_SIZE];
	uint32_t ByteCount;
	uint16_t RecordType;
	uint32_t LineNumber;
	uint32_t DataStartAddress;		// This is the compute address of the data based upon the last 04/05 record and the current data record
	uint32_t DataEndAddress;		// This is the compute address of the data based upon the last 04/05 record and the current data record
	uint32_t ExtendedLinearAddress;  //this is the Address loaded from a 05 record.
	uint16_t iHEX_AddressField;      //This is the actual address in the address field of the line

} iHEX_LineInfo;



#define IHEX_LINE_GOOD									0
#define IHEX_HEX_FILE_OK								0
#define IHEX_ERROR_COULD_NOT_OPEN_FILE					1
#define IHEX_ERROR_BAD_LINE_START						2
#define IHEX_ERROR_INVALID_RECORD						3
#define IHEX_ERROR_BAD_BYTE_COUNT						4
#define IHEX_ERROR_BAD_CHECKSUM							5
#define IHEX_ERROR_BAD_END								6
#define IHEX_ERROR_DATA_IN_BOOTLOADER_SPACE				7
#define IHEX_ERROR_DATA_NOT_IN_VALID_RANGE				0xFFFFFFFF
#define IHEX_LINE_IS_IN_VALID_RANGE						0

uint8_t ReadIHEX_32_Line(char *NextLine,iHEX_LineInfo *LI);
void StripRightWhiteSpace(char * S);
void MakeIHEX_32_Line(char *Line, iHEX_LineInfo *LI);

#endif

