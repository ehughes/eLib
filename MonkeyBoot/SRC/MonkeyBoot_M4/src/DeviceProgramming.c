#include "System.h"

#define PROGRAM_OK						0
#define PROGRAM_ERROR_NOT_IN_RANGE		1
#define PROGRAM_ERROR_UNKNOWN_ALGORITHM	2
#define PROGRAM_ERROR_COULD_NOT_PROGRAM	3

uint32_t ProgramLine(iHEX_LineInfo *LI)
{
	uint32_t RangeIndex;
	uint32_t ProgrammingAlgorithm;
	RangeIndex = GetRangeIndex(LI,MyProgrammableMemoryRanges,NUM_PROGRAMMABLE_MEMORY_RANGES);

	if(RangeIndex == IHEX_ERROR_DATA_NOT_IN_VALID_RANGE)
	{
		return PROGRAM_ERROR_NOT_IN_RANGE;
	}
	else
	{
		ProgrammingAlgorithm = MyProgrammableMemoryRanges[RangeIndex].Type;

		switch(ProgrammingAlgorithm)
		{
			default:
				EmitMsg("I cannot program a memory type of %d",ProgrammingAlgorithm);
				return PROGRAM_ERROR_UNKNOWN_ALGORITHM;
				break;

			case MEM_RANGE_LPC43xx_INT_FLASH:

				if(LPC43xx_FLASH_Program_iHexLine(LI) != LPC43xx_PROGRAMMING_OK)
				{
					return PROGRAM_ERROR_COULD_NOT_PROGRAM;
				}
								break;
		}

	}

	return PROGRAM_OK;
}

//We need this for drivers that cache line data.  it will ensure that the
//the last buffer gets written
uint32_t FinalizeProgramming()
{

	if(LPC43xx_FLASH_FlushCache() != LPC43xx_PROGRAMMING_OK)
	{
		return PROGRAM_ERROR_COULD_NOT_PROGRAM;
	}

	return PROGRAM_OK;
}

