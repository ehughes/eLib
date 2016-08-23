#include "board.h"
#include <cr_section_macros.h>
#include "System.h"
#include <stdarg.h>
#include "MonkeyBootConfig.h"


//Define the range the bootloader is at
const MemoryRange MyBootloaderMemoryRanges[NUM_BOOTLOADER_MEMORY_RANGES] = {
																				{0x1a000000,0x20000,MEM_RANGE_LPC43xx_INT_FLASH},
																			};


//Define the range where valid memory is at
const MemoryRange MyProgrammableMemoryRanges[NUM_PROGRAMMABLE_MEMORY_RANGES] = {
																					{0x1a020000,0x60000,MEM_RANGE_LPC43xx_INT_FLASH},
																					{0x1b000000,0x80000,MEM_RANGE_LPC43xx_INT_FLASH}
																				};

#define VECTOR_TABLE_ADDRESS	 0x1a020000
#define MAGIC_KEY				 0xDEADBEEF
#define MAGIC_KEY_ADDRESS		 (VECTOR_TABLE_ADDRESS + 0x28)

iHEX_Info My_iHEX_Info;

#define BOOT_STATE_ENTRY								1
#define BOOT_STATE_INIT_BOOT_HARDWARE					2
#define BOOT_STATE_CHECK_FILE_SYSTEM_SANITY				3
#define BOOT_STATE_CHECK_BOOT_IMAGE						4
#define BOOT_STATE_ERASE_TARGET_MEMORY					5
#define BOOT_STATE_PROGRAM_TARGET_MEMORY				6
#define BOOT_STATE_RENAME_BOOT_IMAGE					7
#define BOOT_CHECK_FOR_VALID_APPLICATION				8
#define BOOT_JUMP_TO_APPLICATION						9
#define BOOT_START_MSC									10
#define BOOT_WAIT_FOR_FILE_COPY							11
#define BOOT_DISPLAY_FILE_SYSTEM_ERROR					12
#define BOOT_DISPLAY_CRITICAL_PROGRAM_ERASE_ERROR		13

uint8_t BootState = BOOT_STATE_ENTRY;


void ChangeBootState(uint8_t NextState)
{
	switch(NextState)
	{
		default:
			BootState = NextState;
		break;

		case BOOT_STATE_CHECK_FILE_SYSTEM_SANITY:

			BootState = NextState;
			DRIVE_CHECK_TIMER = 0;

		break;

		case BOOT_STATE_CHECK_BOOT_IMAGE:

			BootState = NextState;
			DRIVE_CHECK_TIMER = 0;

			break;

		case BOOT_CHECK_FOR_VALID_APPLICATION:

			BootState = NextState;
			EmitMsg("Checking for valid application in flash\r\n");

		break;

		case BOOT_WAIT_FOR_FILE_COPY:

			BootState = BOOT_WAIT_FOR_FILE_COPY;
			EmitMsg("Waiting for a file to be copied.\r\n");

		break;
		case BOOT_START_MSC:

			BootState = NextState;
			EmitMsg("Starting Mass Storage Class Interface\r\n");
			MSC_Enable();
			break;

		case BOOT_DISPLAY_FILE_SYSTEM_ERROR:

			break;

	}
}

uint32_t ErrorCounter = 0;


char * IAP_GetErrorString(uint8_t r)
{
	char * s = "";

	switch(r)
	{
		case IAP_CMD_SUCCESS:
			s = "IAP_SUCCESS";
		break;

		case IAP_BUSY:
			s = "IAP_BUSY";
		break;

		case IAP_SECTOR_NOT_PREPARED:
			s = "IAP_SECTOR_NOT_PREPARED";
		break;

		case IAP_INVALID_SECTOR:
			s = "IAP_INVALID_SECTOR";
		break;

		case IAP_ADDR_NOT_MAPPED:
			s = "IAP_ADDR_NOT_MAPPED";
		break;

		default :
			s =" IAP_UNKNOWN_ERROR";
			break;
	}

	return s;
}


uint32_t ProgramLine(iHEX_LineInfo *LI);

int main(void)
{
	int32_t i,r,Error;
	iHEX_LineInfo LI;
	FIL MyBootImage;
	char LineBuffer[IHEX_LINE_BUFFER_SIZE];
	char VerificationBuffer[IHEX_LINE_BUFFER_SIZE];
	char VerificationBufferLength;

	uint32_t BytesProgrammed = 0;
	uint32_t Result;

	fpuInit();

    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();

    InitSystemTimer();

    InitFileSystem();

    ChangeBootState(BOOT_STATE_ENTRY);

    while(1)
    {
    	ProcessFileSystem();

    	switch(BootState)
    	{
			default:
			case BOOT_STATE_ENTRY:

				EmitMsg("MonkeyBoot Version : %s\r\n",FIRMWARE_VERSION_STRING);

				ChangeBootState(BOOT_STATE_INIT_BOOT_HARDWARE);

				break;

			case BOOT_STATE_INIT_BOOT_HARDWARE:

				ChangeBootState(BOOT_STATE_CHECK_FILE_SYSTEM_SANITY);

				break;

			case BOOT_STATE_CHECK_FILE_SYSTEM_SANITY:

				//let's check the status for a bit before we error out
				if(DRIVE_CHECK_TIMER >= 5)
				{
					DRIVE_CHECK_TIMER = 0;
					if(disk_status(BOOT_DRIVE) != RES_OK)
					{
						ErrorCounter++;

						if(ErrorCounter>25)
						{
							EmitMsg("Disk not ready, checking for a valid internal application.\r\n");
							ChangeBootState(BOOT_CHECK_FOR_VALID_APPLICATION);
						}
					}
					else
					{
						ChangeBootState(BOOT_STATE_CHECK_BOOT_IMAGE);
					}
				}
				break;

			case BOOT_STATE_CHECK_BOOT_IMAGE:


				if(f_open(&MyBootImage,BOOT_IMAGE_NAME	,FA_READ) == FR_OK)
				{

					f_close(&MyBootImage);

					EmitMsg("Boot image found.   Checking Hex File.\r\n");

					if( VerifyIHEX_32(BOOT_IMAGE_NAME,&My_iHEX_Info) != IHEX_HEX_FILE_OK)
					{
						EmitMsg("Error in hex file verification,  checking for a valid resident application\r\n");
						ChangeBootState(BOOT_CHECK_FOR_VALID_APPLICATION);

					}
					else
					{
						EmitMsg("Valid image with %d bytes in %d lines found.\r\n",My_iHEX_Info.BinaryFileSize,My_iHEX_Info.LineCount);
						ChangeBootState(BOOT_STATE_ERASE_TARGET_MEMORY);
					}

				}
				else
				{
					EmitMsg("Boot Image Not Found.\r\n");
					ChangeBootState(BOOT_CHECK_FOR_VALID_APPLICATION);
				}

				break;


			case BOOT_STATE_ERASE_TARGET_MEMORY:

				Error = 0; //Reset our error flag

				__disable_irq();
				//Hard code for now...
				//Later we need to do this based upon the actual memory locations.
				for(i=9;i<15;i++)
				{
					EmitMsg("Erasing Sector %d in Flash Bank A\r\n",i);
					r = Chip_IAP_PreSectorForReadWrite(i,i,0);

					if(r!=IAP_CMD_SUCCESS)
					{
						EmitMsg("Error Preparing sector %d in Flash Bank A : %s\r\n",IAP_GetErrorString(r));
						Error = 1;
						break;
					}

					r = Chip_IAP_EraseSector(i,i,0);

					if(r!=IAP_CMD_SUCCESS)
					{
						EmitMsg("Error Erasing Sector %d in Flash Bank A : %s\r\n",IAP_GetErrorString(r));
						Error = 1;
						break;
					}
				}

				if(Error == 0)
				{
					for(i=0;i<15;i++)
					{
						EmitMsg("Erasing Sector %d in Flash Bank B\r\n",i);
						Chip_IAP_PreSectorForReadWrite(i,i,1);

						if(r!=IAP_CMD_SUCCESS)
						{
							EmitMsg("Error Preparing sector %d in Flash Bank B : %s\r\n",IAP_GetErrorString(r));
							Error = 1;
							break;
						}

						Chip_IAP_EraseSector(i,i,1);

						if(r!=IAP_CMD_SUCCESS)
						{
							EmitMsg("Error Erasing Sector %d in Flash Bank B : %s\r\n",IAP_GetErrorString(r));
							Error = 1;

						}
					}
				}


				if(Error == 0)
				{
					EmitMsg("Target Memory has been successfully erased.\r\n");
					ChangeBootState(BOOT_STATE_PROGRAM_TARGET_MEMORY);
					BytesProgrammed = 0;
				}
				else
				{
					ChangeBootState(BOOT_DISPLAY_CRITICAL_PROGRAM_ERASE_ERROR);
				}

				__enable_irq();

			break;


			case BOOT_DISPLAY_CRITICAL_PROGRAM_ERASE_ERROR:



			break;

			case BOOT_STATE_PROGRAM_TARGET_MEMORY:



				EmitMsg("Programming Application...\r\n");


				if((i = f_open(&MyBootImage,BOOT_IMAGE_NAME,FA_READ)) != FR_OK)
				{
					EmitMsg("Error opening hex file for programming.\r\n");
					ChangeBootState(BOOT_CHECK_FOR_VALID_APPLICATION);
					break;
				}

				LI.LineNumber = 1;
				LI.RecordType = 0xff;
				LI.ExtendedLinearAddress = 0;
				LI.DataStartAddress = 0;
				LI.DataEndAddress = 0;

				while(!f_eof(&MyBootImage))
				{
					f_gets (
							  &LineBuffer[0],
							  IHEX_LINE_BUFFER_SIZE,
							  &MyBootImage
							);

					StripRightWhiteSpace(&LineBuffer[0]);

					if(strlen((char *)&LineBuffer) == 0)
						continue;

					Result =  ReadIHEX_32_Line(&LineBuffer[0],&LI);

					if(Result != IHEX_LINE_GOOD)
					{
						EmitMsg("Error on line %d of hex image.\r\n",LI.LineNumber);
						f_close(&MyBootImage);
						ChangeBootState(BOOT_CHECK_FOR_VALID_APPLICATION);
						return Result;
					}
					else
					{
						if(LI.RecordType == IHEX_RECORD_TYPE_01)
						{

							break;
						}
							//if the line has data, let's make sure it is in the range of what we can program!
							if((LI.RecordType == IHEX_RECORD_TYPE_00))
							{
								if(ProgramLine(&LI)!=PROGRAM_OK)
								{
									f_close(&MyBootImage);
									ChangeBootState(BOOT_DISPLAY_CRITICAL_PROGRAM_ERASE_ERROR);
									goto ExitProgram;
								}
							}

						}

					LI.LineNumber++;
				}

				if( FinalizeProgramming() != PROGRAM_OK)
				{
					f_close(&MyBootImage);
					ChangeBootState(BOOT_DISPLAY_CRITICAL_PROGRAM_ERASE_ERROR);
					break;
				}

				EmitMsg("Programming Complete. Verifying...\r\n");
				f_lseek(&MyBootImage,0);
				LI.LineNumber = 1;

				while(!f_eof(&MyBootImage))
				{
					f_gets (
							  &LineBuffer[0],
							  IHEX_LINE_BUFFER_SIZE,
							  &MyBootImage
							);

					StripRightWhiteSpace(&LineBuffer[0]);

					if(strlen((char *)&LineBuffer) == 0)
						continue;

					Result =  ReadIHEX_32_Line(&LineBuffer[0],&LI);

					if(Result != IHEX_LINE_GOOD)
					{
						EmitMsg("Error on line %d of hex image.\r\n",LI.LineNumber);
						f_close(&MyBootImage);
						ChangeBootState(BOOT_CHECK_FOR_VALID_APPLICATION);
						return Result;
					}
					else
					{
						if(LI.RecordType == IHEX_RECORD_TYPE_01)
						{
							break;
						}
							//if the line has data, let's make sure it is in the range of what we can program!
							if((LI.RecordType == IHEX_RECORD_TYPE_00))
							{
								VerificationBufferLength = LI.ByteCount;

								memcpy(&VerificationBuffer[0],(uint32_t *)LI.DataStartAddress,VerificationBufferLength);

								if(memcmp(&VerificationBuffer[0],&LI.LineData[0],VerificationBufferLength) != 0)
								{
									EmitMsg("Verification error on Line %d Address 0x%08x\r\n",LI.LineNumber,LI.DataStartAddress);
									ChangeBootState(BOOT_DISPLAY_CRITICAL_PROGRAM_ERASE_ERROR);
									goto ExitProgram;
								}
								else
								{
									EmitMsg("Line %d Address 0x%08x verified\r\n",LI.LineNumber,LI.DataStartAddress);
								}
							}
						}

					LI.LineNumber++;
				}

				f_close(&MyBootImage);


				EmitMsg("Verification Complete\r\n");

				f_rename(BOOT_IMAGE_NAME,BOOT_IMAGE_NAME_PROGRAMMED);
				ChangeBootState(BOOT_CHECK_FOR_VALID_APPLICATION);

ExitProgram:

				break;

			case BOOT_CHECK_FOR_VALID_APPLICATION:

				i = *(uint32_t *)MAGIC_KEY_ADDRESS;
				if(i != MAGIC_KEY)
				{
					EmitMsg("Magic Key in hex file is not correct!\r\n");
					ChangeBootState(BOOT_DISPLAY_CRITICAL_PROGRAM_ERASE_ERROR);
				}
				else
				{



					EmitMsg("Magic Key found,  jumping to application. \r\n");

					//Deinit the systick
					 SysTick->CTRL = 0;

					__asm("MOV r0,#0x0000");
					__asm("MOVT r0,#0x1a02");

					//Move the Vector table for the new application
					SCB->VTOR = VECTOR_TABLE_ADDRESS;

					__asm("LDR SP, [R0]");  //Load new stack pointer address
					__asm("LDR PC, [R0, #4]");//Load new program counter address

				}

				ChangeBootState(BOOT_START_MSC);

				break;

			case 	BOOT_START_MSC:

				if(MSC_IsAttached())
					ChangeBootState(BOOT_WAIT_FOR_FILE_COPY);


				break;


			case BOOT_WAIT_FOR_FILE_COPY:

				break;

			case BOOT_DISPLAY_FILE_SYSTEM_ERROR:

				break;

    	}


    }
    return 0 ;
}

