#include "board.h"
#include <cr_section_macros.h>
#include "System.h"
#include <stdarg.h>

//__BSS(RAM2) uint8_t DataBuffer[8192];
//uint32_t BlockSizeMultiple;

#define WRITE_SIZE	(1<<20)

#define DRIVE_SPEED_TIMER	 GP_Timer[1]

float WriteSpeed;

int main(void)
{
	uint32_t i,j;

	fpuInit();

    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();

    InitSystemTimer();

    InitFileSystem();

	MSC_Enable();

	//for(i=0;i<sizeof(DataBuffer);i++)
	//	DataBuffer[i] = rand();

	Chip_SCU_PinMuxSet(0x2, 11, (SCU_MODE_20MA_DRIVESTR | SCU_PINIO_FAST | SCU_MODE_FUNC0));

	Chip_GPIO_SetDir(LPC_GPIO_PORT,1,11,1);


   while(1)
    {
    	ProcessFileSystem();

    	/*
        if(disk_status( DRIVE_SD) == 0)
        {
        	DEBUGOUT("Starting Speed Test....\r\n");

        	for(i=1;i<17;i++)
        	{
        		DRIVE_SPEED_TIMER = 0;
        		for(j=0;j<WRITE_SIZE/(i*512);j++)
        		{
        			disk_write(DRIVE_SD,DataBuffer,j,i);
        		}

        		WriteSpeed = WRITE_SIZE / ((float)DRIVE_SPEED_TIMER/1000);

        		DEBUGOUT("Wrote %d bytes with %d blocksize : %d kbytes/sec\r\n",WRITE_SIZE,i*512,(int32_t)(WriteSpeed/1024));
        	}
        }
        */
    }
    return 0 ;
}
