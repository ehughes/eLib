#include "System.h"


void TerminalCmd_sd_check(char *arg);

//*****************************************************************
//Terminal Configuration
//*****************************************************************

#define MAX_TERMINAL_LINE_CHARS 64
#define MAX_TERMINAL_CMD_CHARS  32


typedef void (*TerminalCallback)(char *);


typedef struct 
{
	const char *CommandString;
	TerminalCallback Callback;
	const char *HelpString;
		
} TerminalCallbackRecord;

//Callback function prototypes
void TerminalCmd_Help(char *arg);

/*
void TerminalCmd_vs_spi_check(char *arg);
void TerminalCmd_start_sine(char *arg);
void TerminalCmd_stop_sine(char *arg);
void TerminalCmd_vs_info(char *arg);
void TerminalCmd_ls(char *arg);
void TerminalCmd_play(char *arg);
void TerminalCmd_stop(char *arg);
void TerminalCmd_speed(char *arg);
*/

//Populate this array with the callback functions and their terminal command string
const TerminalCallbackRecord MyTerminalCallbackRecords[] ={{"help",TerminalCmd_Help,"Lists available commands"} ,

		/*
                                                        {"sd_check",TerminalCmd_sd_check,"Checks the status of the sd card"},
                                                        {"vs_spi_check",TerminalCmd_vs_spi_check,"Tests the SPI read/write functions"},
                                                        {"start_sine",TerminalCmd_start_sine,"play a test sine wave to the vs1063"},
                                                        {"stop_sine",TerminalCmd_stop_sine,"stop the test sine wave to the vs1063"},
                                                        {"vs_info",TerminalCmd_vs_info,"reads some useful stuff from the VS1063 parameter struct"},
                                                        {"ls",TerminalCmd_ls,"lists files in the current directory"},
                                                        {"play",TerminalCmd_play,"plays a file in the current directory"},
                                                        {"stop",TerminalCmd_stop,"stops current playback"},
                                                        {"speed",TerminalCmd_speed,"file read write test"}*/
                                                         };


//#define enable_speed

#ifdef enable_speed

#define BLOCKS_TO_WRITE	  1024
#define WRITE_BLOCK_SIZE  (8192)

#define BLOCKS_TO_READ	1024
#define READ_BLOCK_SIZE	 WRITE_BLOCK_SIZE

uint8_t WriteBlock[WRITE_BLOCK_SIZE];
uint8_t ReadBlock[READ_BLOCK_SIZE];

#endif


void TerminalCmd_speed(char *arg)
{
#ifdef enable_speed

     	FRESULT rc;
		int i;
		UINT bw, br;


		FIL fileObj;


		float Speed;



		TERMINAL_PRINTF("\r\nCreate a new file (hello.txt).\r\n");
		rc = f_open(&fileObj, "HELLO.TXT", FA_WRITE | FA_CREATE_ALWAYS);
		if (rc) {
	//		die(rc);
			return;
		}
		else
		{


			TERMINAL_PRINTF("\r\nStarting Write Test.....\r\n");

			for(i=0;i<WRITE_BLOCK_SIZE;i++)
			{
				WriteBlock[i] = i;
			}

			SpeedTestTicker = 0;
			for(i=0;i<BLOCKS_TO_WRITE;i++)
			{
				rc = f_write(&fileObj, WriteBlock, WRITE_BLOCK_SIZE, &bw);
				if (rc) {
					TERMINAL_PRINTF("Error Opening file %d",rc);
					return;
				}
			}

			Speed = ((BLOCKS_TO_WRITE * WRITE_BLOCK_SIZE)/((float)(SpeedTestTicker)/1000.0))/1024;

			TERMINAL_PRINTF("Write Speed :%.2f kB per sec\r\n",Speed);

			rc = f_close(&fileObj);
			if (rc) {
				TERMINAL_PRINTF("Error c file %d",rc);
			}
		}


		rc = f_open(&fileObj, "HELLO.TXT", FA_READ);
		if (rc) {
				TERMINAL_PRINTF("Error opening file for read %d",rc);
			  return;
			}
			else {
				TERMINAL_PRINTF("\r\nStarting Read Test.....\r\n");


				SpeedTestTicker = 0;

					for(i=0;i<BLOCKS_TO_READ;i++)
							{
								rc = f_read(&fileObj, ReadBlock, READ_BLOCK_SIZE, &br);
							}

					Speed = ((BLOCKS_TO_READ * READ_BLOCK_SIZE)/((float)(SpeedTestTicker)/1000.0))/1024;

					TERMINAL_PRINTF("Read Speed :%.2f kB per sec\r\n",Speed);



				rc = f_close(&fileObj);
				if (rc) {
					TERMINAL_PRINTF("Error closing file %d",rc);
								  return;
				}
			}

		TERMINAL_PRINTF("\r\nTest completed.\r\n");
#endif

}

#ifdef qwertyu
void TerminalCmd_play(char *arg)
{
    uint8_t i;

    i = PlayFile(arg);
    
     switch(i)
    {
        case PLAY_ERROR_NONE:
             TERMINAL_PRINTF("Playing %s...\r\n",arg);
            break;
        
        case PLAY_ERROR_COULD_NOT_OPEN_FILE:
             TERMINAL_PRINTF("Could not open %s...\r\n",arg);
            break;
            
        default:
            TERMINAL_PRINTF("Unknown error %d...\r\n",i);
            break;
    }

}

void TerminalCmd_stop(char *arg)
{
    TERMINAL_PRINTF("Terminating playback...\r\n");
    TerminateCurrentPlayback();
}


void TerminalCmd_ls(char *arg)
{

    FRESULT res;
    FILINFO fno;
    DIR dir;
    int i;
    char *fn;

    char* path = "";

#if _USE_LFN
    static char lfn[_MAX_LFN + 1];
    fno.lfname = lfn;
    fno.lfsize = sizeof lfn;
#endif


    res = f_opendir(&dir, path);
    if (res == FR_OK) {
        i = strlen(path);
        for (;;) {
            res = f_readdir(&dir, &fno);
            if (res != FR_OK || fno.fname[0] == 0) break;
            if (fno.fname[0] == '.') continue;
	#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
		#else
            fn = fno.fname;
	#endif
            if (fno.fattrib & AM_DIR) {
            	TERMINAL_PRINTF("[%s]", fn);
                //res = scan_files(path);
                //if (res != FR_OK) break;
                //path[i] = 0;
            } else {
            	TERMINAL_PRINTF("%s/%s\r\n", path, fn);
            }
        }
    }


}


void TerminalCmd_vs_info(char *arg)
{


	TERMINAL_PRINTF("Reading some useful parameter info from the VS1063....\r\n");
    TERMINAL_PRINTF("------------------------------------------------------\r\n\r\n");

    TERMINAL_PRINTF("VS1063 Chip ID: 0x%.4x\r\n",(uint32_t)ReadVS1063ReadChipID());
    TERMINAL_PRINTF("VS1063 Parameter Struct Version: 0x%.4x\r\n",(uint32_t)ReadVS1063ReadParameterStructVersion());
    TERMINAL_PRINTF("VS1063 end fill byte: 0x%.2x\r\n",(uint32_t)ReadVS1063EndFillByte());

}

void TerminalCmd_start_sine(char *arg)
{
   StartVS1063SineTest();
}

void TerminalCmd_stop_sine(char *arg)
{
    StopVS1063SineTest();
}

void TerminalCmd_vs_spi_check(char *arg)
{
  uint16_t Test = 0;

    WriteVS1063_SCI_Register(VS1063_REG_SCI_BASS,0x0);
                Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS);
                TERMINAL_PRINTF("\r\n");
                TERMINAL_PRINTF("WriteVS1063_SCI_Register(VS1063_REG_SCI_BASS,0x0)\r\n");
                TERMINAL_PRINTF("Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS)  --> 0x%.4x\r\n\r\n",Test);

                SetBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0xAAAA);
                Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS);
                TERMINAL_PRINTF("SetBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0x5555)\r\n");
                TERMINAL_PRINTF(" Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS) --> 0x%.4x\r\n\r\n",Test);

                SetBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0x5555);
                Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS);

                TERMINAL_PRINTF("SetBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0xAAAA\r\n");
                TERMINAL_PRINTF("Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS) --> 0x%.4x\r\n\r\n",Test);

               ClearBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0x5555);
                Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS);

                TERMINAL_PRINTF("ClearBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0x5555);\r\n");
                TERMINAL_PRINTF("Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS) --> 0x%.4x\r\n\r\n",Test);

                ClearBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0xAAAA);
                Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS);

                TERMINAL_PRINTF(" ClearBitsVS1063_SCI_Register(VS1063_REG_SCI_BASS,0xAAAA);\r\n");
                TERMINAL_PRINTF("Test = ReadVS1063_SCI_Register(VS1063_REG_SCI_BASS); --> 0x%.4x\r\n\r\n",Test);;

}



void TerminalCmd_sd_check(char *arg)
{
/*
    FSFILE  * NewFile;

    if(SD_CD == 1)
    {
    TERMINAL_PRINTF("SD Card detect failed! \r\n");
    return;
    }

    TERMINAL_PRINTF("SD Card detected \r\n");

    if(SD_WE == 1)
    {
        TERMINAL_PRINTF("SD Write Protect switch enabled.  Please disable! \r\n");
        return;
    }

        TERMINAL_PRINTF("SD write protect switch is not enabled  \r\n");

    if(!FSInit())
    {
       TERMINAL_PRINTF("File System Initialization failed! \r\n");
       return;
    }

      TERMINAL_PRINTF("File System Initialization success! \r\n");


      NewFile = FSfopen("fileeli12394.txt",FS_APPENDPLUS);

      if(NewFile == NULL)
      {
          TERMINAL_PRINTF("Could not open test_file.txt in append_plus mode");
          return;
      }

      TERMINAL_PRINTF("\r\nWriting Test Data into test_file.txt\r\n");



     if(FSfprintf(NewFile,"This is a file write test!\r\n\r\n1234567890\r\n") == 0)
       {
           TERMINAL_PRINTF("\r\nError Writing Test Data into test_file.txt\r\n");

       }

      TERMINAL_PRINTF("\r\nClosing file_test.txt...\r\n");
      FSfclose(NewFile);
    */
}

#endif


//*****************************************************************
//Plumbing.....
//*****************************************************************

#define NUM_TERMINAL_COMMANDS  (sizeof(MyTerminalCallbackRecords)/sizeof(TerminalCallbackRecord))

char TerminalLineBuf[MAX_TERMINAL_LINE_CHARS];
uint8_t TerminalPos;
char TerminalCmdBuf[MAX_TERMINAL_CMD_CHARS+1];
char TerminalArgs[MAX_TERMINAL_LINE_CHARS-MAX_TERMINAL_CMD_CHARS];
uint8_t NextCharIn;
uint8_t CmdFound;


void TerminalCmd_Help(char *arg)
{
    uint8_t i;

    TERMINAL_PRINTF("\r\nCommand List:\r\n");
    TERMINAL_PRINTF("----------------------\r\n");

    for(i=0;i<NUM_TERMINAL_COMMANDS;i++)
    {
         TERMINAL_PRINTF("%s  ---->  %s\r\n",MyTerminalCallbackRecords[i].CommandString,MyTerminalCallbackRecords[i].HelpString);    
    }

}

void TerminalBootMsg()
{

TERMINAL_PRINTF("\r\n");
TERMINAL_PRINTF("***********************************\r\n");
TERMINAL_PRINTF("	TIME MACHINE	           \r\n");
TERMINAL_PRINTF("Pyrotechnic Management Inc.    \r\n");
TERMINAL_PRINTF("***********************************\r\n\r\n>");

}

void TerminalCmd_Reboot(char *arg)
{
      TerminalBootMsg();
}


void InitTerminal()
{
	TerminalPos = 0;
	CmdFound = 0;

    TerminalBootMsg();
}


void ProcessTerminal()
{
     uint8_t i,j;
     uint8_t ArgsFound;

#if (TERMINAL_READ_AGRESSIVENESS == TERMINAL_READ_LIGHT)
    if(TERMINAL_READABLE)
#else
    while(TERMINAL_READABLE)
#endif

    {
       NextCharIn = TERMINAL_GETC;
       
        switch(NextCharIn)
        {
            case '\r':
             
             TerminalLineBuf[TerminalPos++] = 0x0;
             TERMINAL_PUTC(NextCharIn);
           
             if(TerminalPos > 1)
             {
                 //find the command
                 i=0;
                 while(TerminalLineBuf[i]>0x20 &&  TerminalLineBuf[i]<0x7f)
                 {
                      TerminalCmdBuf[i] = TerminalLineBuf[i];
                      i++;
    
                    if(i==MAX_TERMINAL_CMD_CHARS)
                        {
                         break;
                        }
                 }
                    
                TerminalCmdBuf[i] = 0;
                TerminalCmdBuf[i+1] = 0;
                
                
                ArgsFound = TRUE;
                memset(TerminalArgs,0x00,sizeof(TerminalArgs));
                //scan for num terminator or next non whitespace
                while(TerminalLineBuf[i]<=0x20 && (i<MAX_TERMINAL_LINE_CHARS))
                {
                    if(TerminalLineBuf[i] == 0x00)
                    {
                    
                        //if we find a NULL terminator before a non whitespace character they flag for no arguments
                        ArgsFound = FALSE;
                        break;
                    }   
                    i++; 
                }
                
                if(ArgsFound == TRUE)
                {
                    strcpy(TerminalArgs,&TerminalLineBuf[i]);
                    
                    //trim trailing whitespace
                    i = sizeof(TerminalArgs)-1;
                    
                    while((TerminalArgs[i]<0x21) && (i>0))
                    {
                        TerminalArgs[i]= 0x00;
                        i--;
                    }       
                }
                
                CmdFound = FALSE;
                for(j=0;j<NUM_TERMINAL_COMMANDS;j++)
                {           
                    if(strcmp(TerminalCmdBuf,MyTerminalCallbackRecords[j].CommandString) == 0)
                    {
                        TERMINAL_PRINTF("\r\n");
                        if(MyTerminalCallbackRecords[j].Callback != NULL)
                            MyTerminalCallbackRecords[j].Callback(TerminalArgs);
                    
                        CmdFound = TRUE;
                        break;
                    }             
                }        
                if(CmdFound == FALSE)
                {
                  TERMINAL_PRINTF("\r\n%s command not recognized.\r\n",TerminalCmdBuf);
                  TerminalCmd_Help("no arg");
                }
              }    
             TERMINAL_PRINTF("\r\n>");
             TerminalPos = 0;
            
            break;
            
            case 127:
            case '\b':
                if(TerminalPos > 0)
                {
                    TerminalPos--;    
                    TERMINAL_PUTC(NextCharIn);
                }
            break;
            
            default:
                
                if(TerminalPos == 0 && NextCharIn == 0x020)
                {
                     //Do nothing if space bar is pressed at beginning of line    
                }
                   else if(NextCharIn >= 0x20 && NextCharIn<0x7F)
                {
                    
                    if(TerminalPos < MAX_TERMINAL_LINE_CHARS-1)
                    {
                         TerminalLineBuf[TerminalPos++] = NextCharIn;
                         TERMINAL_PUTC(NextCharIn);
                    }
                }
            
            break;
        
        }
    }
 
}


