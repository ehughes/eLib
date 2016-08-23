#include <stdint.h>
#include "MonkeyBootConfig.h"

#ifndef IHEX_H
#define IHEX_H


#define IHEX_LINE_BUFFER_SIZE			256


#define IHEX_RECORD_TYPE_00				0x3030//This is the actual number value for the string 00
#define IHEX_RECORD_TYPE_01				0x3130//This is the actual number value for the string 01
#define IHEX_RECORD_TYPE_02				0x3230//This is the actual number value for the string 02
#define IHEX_RECORD_TYPE_03				0x3330//This is the actual number value for the string 03
#define IHEX_RECORD_TYPE_04				0x3430//This is the actual number value for the string 04
#define IHEX_RECORD_TYPE_05				0x3530//This is the actual number value for the string 05


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
	uint32_t DataStartAddress;
	uint32_t DataEndAddress;
	uint32_t ExtendedLinearAddress;

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

uint32_t  VerifyIHEX_32(char * Filename, iHEX_Info *IHI);
uint8_t ReadIHEX_32_Line(char *NextLine,iHEX_LineInfo *LI);
void StripRightWhiteSpace(char * S);
uint8_t CheckIfDataIsInValidRange(iHEX_LineInfo *LI);
uint32_t GetRangeIndex(iHEX_LineInfo *LI,const MemoryRange *M,uint32_t NumRanges);

#endif

