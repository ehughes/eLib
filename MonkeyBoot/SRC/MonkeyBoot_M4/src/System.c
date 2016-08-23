#include "System.h"

/***
 *      _____ ___ __  __ _____ ____  ____
 *     |_   _|_ _|  \/  | ____|  _ \/ ___|
 *       | |  | || |\/| |  _| | |_) \___ \
 *       | |  | || |  | | |___|  _ < ___) |
 *       |_| |___|_|  |_|_____|_| \_\____/
 *
 */

volatile static uint32_t mS_DelayTicker = 0;
volatile uint32_t GP_Timer[NUM_GP_TIMERS];
volatile uint32_t BeepTicker = 0;


void SysTick_Handler()
{
	uint32_t i;

    mS_DelayTicker++;

    if(DiskManagerTicker<0xFFFFFFFF)
    	DiskManagerTicker++;

    for(i=0;i<NUM_GP_TIMERS;i++)
    {
    	if(GP_Timer[i]<0xFFFFFFFF)
    		GP_Timer[i]++;
    }
}

void InitSystemTimer()
{

    NVIC_SetPriority(SysTick_IRQn,0);
    SysTick_Config(SystemCoreClock/1000);
}

void Delay_mS(uint32_t mS_TicksToDelay)
{
    mS_DelayTicker = 0;

    while(mS_DelayTicker < mS_TicksToDelay)
    {
    }
}

//Init all of the SystemIO
void InitIO()
{


}

static char StringBuffer[512];

void EmitMsg(const char *FormatString,...)
{
    va_list argptr;
    va_start(argptr,FormatString);
    vsprintf((char *)StringBuffer,FormatString,argptr);
    va_end(argptr);

    DEBUGOUT(StringBuffer);

}

