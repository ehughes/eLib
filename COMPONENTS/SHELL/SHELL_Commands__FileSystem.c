#include "System.h"
#include "Shell.h"
#include "SHELL_Commands__FileSystem.h"

/*
cmd_function_t flash_info(p_shell_context_t Shell,int32_t argc, char **argv)
{
    uint32_t ID = 0;

    ID = SPIFI_GetJEDEC_ID();

    StartJSON(Shell);
    OutputTab(Shell); OutputObjID(Shell,"FlashInfo"); OutputNewJSON_Line(Shell);
    OutputTab(Shell); SHELL_printf(Shell,"\"Manufacturer ID\" : \"0x%02x\"",ID&0XFF);    OutputNewJSON_Line(Shell);
    OutputTab(Shell); SHELL_printf(Shell,"\"Memory Type\" : \"0x%02x\"",(ID>>8)&0XFF);     OutputNewJSON_Line(Shell);
    OutputTab(Shell); SHELL_printf(Shell,"\"Capacity Code\" : \"0x%02x\"",(ID>>16)&0XFF);  OutputNewJSON_Line(Shell);
    OutputTab(Shell); SHELL_printf(Shell,"\"Status Register\" : \"0x%02x\"",SPIFI_RDSR()&0XFF);  OutputNewLine(Shell);

    StopJSON(Shell);

    return 0;
}
*/

cmd_function_t touch(p_shell_context_t Shell,int32_t argc, char **argv)
{
    if(argc < 2)
    {
        SHELL_printf(Shell,"\r\nPlease provide an argument for touch.... the file name!\r\n");
        return (cmd_function_t)-1;
    }
    else
    {
        FIL MyFile;
        FRESULT FR;
         /* Open a text file */
          FR = f_open(&MyFile, argv[1], FA_CREATE_ALWAYS | FA_WRITE);

          if(FR!=FR_OK)
          {
              SHELL_printf(Shell,"Could not open / create %s : %s\r\n",argv[1],FRESULT_String[FR]);
              return (cmd_function_t)FR;
          }
          else
          {

              for(int i=2;i<argc;i++)
              {
                  f_printf(&MyFile,"%s\r\n",argv[i]);
              }

              FR = f_close(&MyFile);
              if(FR!=FR_OK)
              {
                  SHELL_printf(Shell,"Could not close %s : %",argv[1],FRESULT_String[FR]);
                  return (cmd_function_t)FR;
              }
           }

          return (cmd_function_t)FR_OK;
    }

}

cmd_function_t more(p_shell_context_t Shell,int32_t argc, char **argv)
{
    char ReadBuffer[128];

    if(argc < 2)
    {
        SHELL_printf(Shell,"\r\nPlease provide an argument for more.... the file name!\r\n");
        return (cmd_function_t)-1;
    }
    else
    {
        FIL MyFile;
        FRESULT FR;
         /* Open a text file */
          FR = f_open(&MyFile, argv[1], FA_READ);

          if(FR!=FR_OK)
          {
              SHELL_printf(Shell,"Could not open  %s : %s\r\n",argv[1],FRESULT_String[FR]);
              return (cmd_function_t)FR;
          }
          else
          {
              UINT BW;
              while(!f_eof(&MyFile))
              {
                    
                  f_read(&MyFile,&ReadBuffer[0],sizeof(ReadBuffer),&BW);
                  SHELL_SendBuffer(Shell,(uint8_t *)ReadBuffer,BW);
                  Delay_mS(10);
              }

              FR = f_close(&MyFile);
           }
          SHELL_printf(Shell,"\r\n");

          return (cmd_function_t)FR_OK;
    }

}

cmd_function_t rm(p_shell_context_t Shell,int32_t argc, char **argv)
{
    if(argc < 2)
    {
        SHELL_printf(Shell,"\r\nPlease provide an argument for rm.... the file name!\r\n");
        return (cmd_function_t)-1;
    }
    else
    {

         FRESULT FR;
         /* Open a text file */
          FR = f_unlink(argv[1]);

          if(FR!=FR_OK)
          {
              SHELL_printf(Shell,"Could not delete  %s : %s\r\n",argv[1],FRESULT_String[FR]);
              return (cmd_function_t)FR;
          }

          return (cmd_function_t)FR_OK;
    }

}

cmd_function_t check_fs(p_shell_context_t Shell,int32_t argc, char **argv)
{
    FATFS *fs;
    DWORD fre_clust, fre_sect, tot_sect;

    //FR = f_mount(0,"",0);
    FRESULT FR =  f_getfree("0:", &fre_clust, &fs);

    if(FR != FR_OK)
    {
        SHELL_printf(Shell,"Error checking file system : %s\r\n",FRESULT_String[FR]);
    }
    else
    {
          tot_sect = (fs->n_fatent - 2) * fs->csize;

          fre_sect = fre_clust * fs->csize;

            /* Print the free space (assuming 4096 bytes/sector) */
          SHELL_printf(Shell,"%u KiB total drive space.\r\n%u KiB available.\r\n",
                   tot_sect*8 / 2, fre_sect*8 / 2);

    }

    return 0;

}


cmd_function_t format(p_shell_context_t Shell,int32_t argc, char **argv)
{
    FRESULT FR;
    uint8_t Data[FF_MAX_SS];

    //FR = f_mount(0,"",0);
    FR = f_mkfs("", FM_FAT, 4096,Data,sizeof(Data));

    if(FR != FR_OK)
    {
        SHELL_printf(Shell,"Error formatting file system : %s\r\n",FRESULT_String[FR]);
    }
    else
    {
        SHELL_printf(Shell,"File system is formatted.\r\n");

        FR = f_mount(&FlashFileSystem,"0:",1);

        if(FR != FR_OK)
        {
            SHELL_printf(Shell,"Error Mounting File System : %s\r\n",FRESULT_String[FR]);
        }
        else
        {
            SHELL_printf(Shell,"File System successfully mounted\r\n");
        }

    }

    return 0;

}


cmd_function_t ls(p_shell_context_t Shell,int32_t argc, char **argv)
{
        FRESULT res;
        FILINFO fno;
        DIR dir;

        char *fn;   /* This function assumes non-Unicode configuration */
    #if _USE_LFN
        static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
        fno.lfname = lfn;
        fno.lfsize = sizeof lfn;
    #endif
        char path[128];

        SHELL_printf(Shell,"\r\nDirectory listing\r\n");
        SHELL_printf(Shell,"--------------------------------\r\n\r\n");

        f_getcwd (
                   path, /* [OUT] Buffer to return path name */
                  sizeof(path)     /* [IN] The length of the buffer */
                  );

        res = f_opendir(&dir, "/");                       /* Open the directory */
        if (res == FR_OK)
        {

            for (;;) {
                res = f_readdir(&dir, &fno);                   /* Read a directory item */
                if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
                if (fno.fname[0] == '.') continue;             /* Ignore dot entry */
    #if _USE_LFN
                fn = *fno.lfname ? fno.lfname : fno.fname;
    #else
                fn = fno.fname;
    #endif
                if (fno.fattrib & AM_DIR)
                {
                    SHELL_printf(Shell,VT100_MAGENTA);
                    /* It is a directory. */
                    SHELL_printf(Shell,"%s/%s\r\n",path,fn);
                }
                else
                {

                    /* It is a file. */
                   SHELL_printf(Shell,VT100_YELLOW);
                   SHELL_printf(Shell,"%s",fn);
                   SHELL_printf(Shell,VT100_DEFAULT);
                   SHELL_printf(Shell,":",fn,fno.fsize);
                   SHELL_printf(Shell,VT100_CYAN);
                   SHELL_printf(Shell,"%d bytes\r\n",fno.fsize);
                }
            }
            f_closedir(&dir);
        }

        SHELL_printf(Shell,VT100_DEFAULT);

        SHELL_printf(Shell,"\r\n",path,fn);

        check_fs(Shell,argc,argv);

    return 0;
}


cmd_function_t flash_sector_read(p_shell_context_t Shell,int32_t argc, char **argv)
{
    uint8_t Data[FF_MAX_SS];
    uint32_t SectorNumber = 0;

    if(argc < 2)
    {
        SHELL_printf(Shell,"I need a sector number\r\n");
        return 0;
    }

        SectorNumber = atoi(argv[1]);

        SHELL_printf(Shell,"\r\n\r\n");

        disk_read(DEV_SPIFI,Data,SectorNumber,1);

        for(int i=0;i<FF_MAX_SS/16;i++)
        {

            uint32_t BaseAddress = (SectorNumber * FF_MAX_SS) + (i * 16);

            SHELL_printf(Shell,VT100_YELLOW);

            SHELL_printf(Shell,"0x%06x : ",BaseAddress);

            SHELL_printf(Shell,VT100_WHITE);

            for(int j=0;j<16;j++)
            {
                SHELL_printf(Shell,"0x%02x ",Data[i * 16 + j]);
            }

            SHELL_printf(Shell,"    ");

            SHELL_printf(Shell,VT100_CYAN);

            for(int j=0;j<16;j++)
            {
                uint8_t tmp = Data[i * 16 + j];

                if(tmp<0x20)
                    tmp = 0x20;


                SHELL_printf(Shell,"%c",tmp);
            }
            SHELL_printf(Shell,"\r\n");

            if(Shell_CheckBreakCharacter(Shell))
            {
                break;
            }
        }

        SHELL_printf(Shell,"\r\n");

    SHELL_printf(Shell,VT100_DEFAULT);
    return 0;
}




cmd_function_t erase_flash(p_shell_context_t Shell,int32_t argc, char **argv)
{
    SHELL_printf(Shell,"\r\nErasing Flash Array\r\n");

    //SPIFI_EraseEntireFlashArray();

    SHELL_printf(Shell,"Flash Array Erased\r\n");

    return 0;

}
cmd_function_t test_flash(p_shell_context_t Shell,int32_t argc, char **argv)
{
    uint8_t Data[FF_MAX_SS];

    SHELL_printf(Shell,"\r\nTesting flash through the disk_io layer\r\n\r\n");
    int CurrentSector;
    for(CurrentSector=0; CurrentSector<SPIFI_ATTACHED_FLASH_SIZE_IN_SECTORS; CurrentSector++)
    {

              Shell_MoveQueues();

        SHELL_printf(Shell,"Checking Sector %d/%d\r\n",CurrentSector+1,SPIFI_ATTACHED_FLASH_SIZE_IN_SECTORS);
        for(int z=0;z<sizeof(Data);z++){Data[z] = (CurrentSector)&0xFF;}

        disk_write(DEV_SPIFI,Data, CurrentSector,1);

        memset(Data,0,sizeof(Data));

        disk_read(DEV_SPIFI,Data, CurrentSector,1);

        for(int k=0;k<sizeof(Data);k++)
         {
              if(Data[k]!=(CurrentSector&0xFF))
                  {
                       SHELL_printf(Shell,"Sector %d has bad data.  0x%02x found at address 0x%08x. Expected 0x%02x\r\n"
                                                   ,CurrentSector,Data[k],(CurrentSector*FF_MAX_SS) + k,(CurrentSector)&0xFF);
                    return 0;
                  }
        }

        if(Shell_CheckBreakCharacter(Shell))
        {
            break;
        }
    }

    if(CurrentSector != SPIFI_ATTACHED_FLASH_SIZE_IN_SECTORS)
        SHELL_printf(Shell,"Flash test aborted.\r\n");
    else
        SHELL_printf(Shell,"Flash test passed.\r\n");



    return 0;
}

FRESULT TestFile(p_shell_context_t Shell,char * TF)
{
        FIL TestFile;
        FRESULT FR;

        SHELL_printf(Shell,"\r\nOpening %s\r\n",TF);

        FR = f_open(&TestFile,TF,FA_OPEN_ALWAYS | FA_WRITE);

        if(FR != FR_OK)
        {
            SHELL_printf(Shell,"Could not open %s : %s\r\n",TF,FRESULT_String[FR]);
            return FR;
        }

        /*We are going to write out ascii symbols 0x30 to 0x5A until we get to to our target length
         *Every 32 characters we will put in a \r\n
         * We will do this one byte at a time to punish the system.   The idea is to write a file that crosses a few sector boundarys
         * and ends up on an uneven address
         */

        char CurrentChar = 0x30;
        UINT Junk = 0;

        for(uint32_t i = 0 ; i< ((FF_MAX_SS *13 ) + (FF_MAX_SS/7));i++)
        {
            FR = f_write(&TestFile,&CurrentChar,1,&Junk);

            if(FR != FR_OK)
            {
                SHELL_printf(Shell,"Error writing to file at index %i : %s\r\n",i,FRESULT_String[FR]);
                f_close(&TestFile);
                return FR;
            }

            if((i%32)==31)
            {
                FR = f_write(&TestFile,"\r\n",2,&Junk);

                if(FR != FR_OK)
                    {
                        SHELL_printf(Shell,"Error writing to file at index %i : %s\r\n",i,FRESULT_String[FR]);
                        f_close(&TestFile);
                        return FR;
                    }
            }

            CurrentChar++;
            if(CurrentChar == 0x5B)
                CurrentChar = 0x30;
        }

         FR = f_close(&TestFile);

         if(FR != FR_OK)
         {
            SHELL_printf(Shell,"Error closing file : %s\r\n",FRESULT_String[FR]);
            return FR;
         }

         SHELL_printf(Shell,"%d bytes written to %s\r\n\r\n",((FF_MAX_SS *2 ) + (FF_MAX_SS/3)),TF);

         SHELL_printf(Shell,"Checking File...\r\n");

         FR = f_open(&TestFile,TF,FA_OPEN_EXISTING | FA_READ);

         if(FR != FR_OK)
         {
            SHELL_printf(Shell,"Could not open %s: %s\r\n",TF,FRESULT_String[FR]);
            return FR;
         }


         char CheckChar[2];
         char ExpectChar = 0x30;


            for(uint32_t i = 0 ; i< ((FF_MAX_SS *2 ) + (FF_MAX_SS/3));i++)
            {

                    FR = f_read(&TestFile,&CheckChar[0],1,&Junk);

                    if(FR != FR_OK)
                    {
                        SHELL_printf(Shell,"Error reading  file at index %i : %s\r\n",i,FRESULT_String[FR]);

                        f_close(&TestFile);

                        return FR;
                    }

                    if(CheckChar[0] != ExpectChar)
                    {
                        SHELL_printf(Shell,
                                    "Error at index %d expecting 0x%x read 0x%x \r\n",i,
                                    CheckChar[0],ExpectChar);

                        f_close(&TestFile);

                        return  FR;
                    }


                    if((i%32)==31)
                    {
                        FR = f_read(&TestFile,&CheckChar[0],2,&Junk);

                        if(FR != FR_OK)
                            {
                                SHELL_printf(Shell,"Error reading  file at index %i : %s\r\n",i,FRESULT_String[FR]);
                                f_close(&TestFile);
                                return FR;
                            }

                            if ((CheckChar[0] != '\r')  || (CheckChar[1] != '\n'))
                                {
                                    SHELL_printf(Shell,
                                        "Error at index %d. expecting carriage return and line feed but received 0x%x and 0x%x\r\n",
                                        CheckChar[0],CheckChar[1]);
                                 }
                    }

                    ExpectChar++;

                    if(ExpectChar == 0x5B)
                        ExpectChar = 0x30;

            }

             FR = f_close(&TestFile);

             if(FR != FR_OK)
             {
                SHELL_printf(Shell,"Error closing file : %s\r\n",FRESULT_String[FR]);
                return  FR;
             }

            SHELL_printf(Shell,"File Test successful!\r\n");

         return 0;
}

cmd_function_t test_fs(p_shell_context_t Shell,int32_t argc, char **argv)
{
    //we are going to generate 3 test files.   Each will be (_MAX_SS * 2) + (_MAX_SS /3) bytes in length
    //This should spread some data across the sectors

    TestFile(Shell,"test1.txt");
    TestFile(Shell,"test2.txt");
    TestFile(Shell,"test3.txt");
    TestFile(Shell,"test4.txt");

    return 0;
}


cmd_function_t mkdir(p_shell_context_t Shell,int32_t argc, char **argv)
{
    FRESULT FR;

    if(argc < 2)
    {
        SHELL_printf(Shell,"mkdir requires an argument for the directory to create\r\n");
        return (cmd_function_t)-1;
    }

    FR = f_mkdir(argv[1]);

    if(FR!= FR_OK)
    {
        SHELL_printf(Shell,"Could not create directory %s : %s\r\n",FRESULT_String[FR]);
        return (cmd_function_t)FR;
    }
    else
    {
        SHELL_printf(Shell,"%s created\r\n",argv[1]);
    }

    return FR_OK;
}

uint8_t HexDigitToNumber(char HexChar)
{

    if(HexChar>=0x30 && HexChar<=0x39)
    {
        return HexChar-0x30;
    }
    else if(HexChar>= 0x41 && HexChar<=0x46)
    {
        return HexChar - 0x41 + 0xa;
    }
    else if(HexChar>= 0x61 && HexChar<=0x66)
    {
        return HexChar - 0x61 + 0xa;
    }
    else
    {
        return 0;
    }
}

void HexStringToArray(char *HexString,uint8_t *Data,uint32_t DigitsToProcess)
{
    for(int i=0;i<DigitsToProcess/2;i++)
    {
        Data[i] = HexDigitToNumber(HexString[i*2])*16 +
                  HexDigitToNumber(HexString[i*2+1]);
    }
}


cmd_function_t file_rename(p_shell_context_t Shell,int32_t argc, char **argv)
{
    FRESULT FR;

    if(argc < 3 )
    {
        SHELL_printf(Shell," I need 2 arguments.  The file that needs renamed and its new name.\r\n");
        return 0;
    }

    FR= f_rename(argv[1],argv[2]);

    if(FR!=FR_OK)
    {
        SHELL_printf(Shell,"Error renaming %s to %s : %s\r\n",argv[1],argv[2],FRESULT_String[FR]);
    }
    else
    {
        SHELL_printf(Shell,"%s renamed to to %s\r\n",argv[1],argv[2]);
    }

    return 0;
}







