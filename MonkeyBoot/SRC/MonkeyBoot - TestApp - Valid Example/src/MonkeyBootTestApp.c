/*
===============================================================================
 Name        : MonkeyBootTestApp.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

// TODO: insert other include files here

// TODO: insert other definitions and declarations here

extern const __TEXT(Flash2) uint8_t FlashB[524288];
extern const __TEXT(Flash) uint8_t FlashA[385024];


int main(void)
{

	uint32_t j = 0;
	uint32_t i = 0 ;


#if defined (__USE_LPCOPEN)
#if !defined(NO_BOARD_LIB)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    // TODO: insert code here

    j = FlashB[1];
    j += FlashA[1];

    // Enter an infinite loop, just incrementing a counter
    while(1) {
     for(i=0;i<2000000;i++)
     {

     }

     Board_LED_Set(0, true);

     for(i=0;i<2000000;i++)
         {

         }

     Board_LED_Set(0, false);
    }
    return 0 ;
}
