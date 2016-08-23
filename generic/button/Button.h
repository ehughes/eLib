#include "stdint.h"

#ifndef BUTTON_H_
#define BUTTON_H_

#define _USE_LPC_V2_CHIP_LIB_IO_ACCESS

#define BUTTON_STATE_WAIT_FOR_PRESS						0
#define BUTTON_STATE_WAIT_FOR_WAIT_FOR_PRESS_STABLE		1
#define BUTTON_STATE_WAIT_FOR_RELEASE					2
#define BUTTON_STATE_WAIT_FOR_STABLE					3

#define BUTTON_POLARITY_LOW_ACTIVE						0
#define BUTTON_POLARITY_HIGH_ACTIVE						1


typedef struct
{
    volatile uint8_t Down;	//Flag to indicate button was pressed down
    volatile uint8_t Up;	//Flag to indicate button was release

    uint8_t Port;	//GPIO port of the button
    uint8_t Bit;	//GPIO bit of the button

    volatile uint16_t   HoldTime;	//Number of ticks the butt was pressed for
    int16_t   DebounceTicks; //Number of ticks for the debounce time

    volatile uint16_t   DebounceTicker; //used internally for the debouncer alogirhtm

    volatile uint8_t State;		//State of the debounce algorithm

    uint8_t   Polarity;	//is the button active high (>0) or active low (0)

    volatile uint8_t    Locked;   //Flag to indicate the struct is beign processed bu the Processbutton() function

    char * ButtonID;

} Button;


void InitButton(Button *B,
                uint8_t Port,
                uint8_t Bit,
                uint8_t Polarity,
                uint8_t DebounceTicks,
                char * ButtonID);

// Computes the Debounce algorithm  Must be done periodically
void ProcessButton(Button *B);

//Indicates that the button is being help down
uint8_t ButtonIsActive(Button *B);

//Reads the button down flag.  Flag will be cleared after read
uint8_t ButtonDown(Button *B);

//Reads the button Up flag.  Flag will be cleared after read
//Returns the number of ticks the button was held for
uint16_t ButtonUp(Button *B);

//returns the current hold time from the debounce algorithm
uint16_t ButtonCurrentHoldTime(Button *B);



#endif /* BUTTON_H_ */
