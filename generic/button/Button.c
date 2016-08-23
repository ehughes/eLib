#include "Button.h"

#ifdef _USE_LPC_V2_CHIP_LIB_IO_ACCESS

#include "chip.h"

#endif



void InitButton(Button *B,
                uint8_t Port,
                uint8_t Bit,
                uint8_t Polarity,
                uint8_t DebounceTicks,
                char * ButtonID)
{
    if(B!=0)
    {
        B->Port = Port;
        B->Bit = Bit;
        B->Polarity = Polarity;
        B->DebounceTicks = DebounceTicks;
        B->Down = 0;
        B->Up = 0;
        B->HoldTime = 0;
        B->Locked = 0;
        B->State = BUTTON_STATE_WAIT_FOR_PRESS;
        B->DebounceTicker = 0;

        if(ButtonID == (char *)NULL)
        	strcpy(ButtonID,"Unknown");
        else
        	B->ButtonID = ButtonID;
    }
}


#define BUTTON_HAL_PRESSED		TRUE
#define BUTTON_HAL_NOT_PRESSED	FALSE


uint8_t ButtonHAL(Button *B)
{
    uint8_t P = 0;

    if(B->Polarity == BUTTON_POLARITY_LOW_ACTIVE)
    {
#ifdef	_USE_LPC_V2_CHIP_LIB_IO_ACCESS

        if(Chip_GPIO_ReadPortBit(LPC_GPIO_PORT,B->Port,B->Bit) == 0)
        {
            P = BUTTON_HAL_PRESSED;
        }

        else
        {
            P = BUTTON_HAL_NOT_PRESSED;
        }

#endif
    }

    else
    {
#ifdef	_USE_LPC_V2_CHIP_LIB_IO_ACCESS

        if(Chip_GPIO_ReadPortBit(LPC_GPIO_PORT,B->Port,B->Bit) == 1)
        {
            P = BUTTON_HAL_PRESSED;
        }

        else
        {
            P = BUTTON_HAL_NOT_PRESSED;
        }

#endif
    }

    return P;
}

// Computes the Debounce algorithm. Must be called periodically
void ProcessButton(Button *B)
{
    B->Locked = TRUE;

    switch(B->State)
    {
        default:
        case BUTTON_STATE_WAIT_FOR_PRESS:
            if(ButtonHAL(B) == BUTTON_HAL_PRESSED)
            {
                B->State = BUTTON_STATE_WAIT_FOR_WAIT_FOR_PRESS_STABLE;
                B->DebounceTicker = 0;
            }

            break;

        case BUTTON_STATE_WAIT_FOR_WAIT_FOR_PRESS_STABLE:
            if(ButtonHAL(B) == BUTTON_HAL_PRESSED)
            {
                B->DebounceTicker++;

                if(B->DebounceTicker > B->DebounceTicks)
                {
                    B->State = BUTTON_STATE_WAIT_FOR_RELEASE;
                    B->HoldTime = B->DebounceTicker;
                    B->Down = TRUE;
                }
            }

            else
            {
                B->State = BUTTON_STATE_WAIT_FOR_PRESS;
            }

            break;

        case BUTTON_STATE_WAIT_FOR_RELEASE:
            if(ButtonHAL(B) == BUTTON_HAL_PRESSED)
            {
                if(B->HoldTime<0xFFFF)
                	B->HoldTime++;
            }

            else
            {
                B->State = BUTTON_STATE_WAIT_FOR_STABLE;
                B->DebounceTicker = 0;
            }

            break;

        case BUTTON_STATE_WAIT_FOR_STABLE:
            if(ButtonHAL(B) == BUTTON_HAL_NOT_PRESSED)
            {

            	B->DebounceTicker++;

                if(B->DebounceTicker > B->DebounceTicks)
                {
                    B->State = BUTTON_STATE_WAIT_FOR_PRESS;
                    B->Up = TRUE;
                }
            }

            else
            {
                B->DebounceTicker = 0;
            }

            break;
    }

    B->Locked = FALSE;
}

//Indicates that the button is being help down
uint8_t ButtonIsActive(Button *B)
{
    if(B->State == BUTTON_STATE_WAIT_FOR_RELEASE)
        return TRUE;

    else
        return FALSE;
}

//Reads the button down flag.  Flag will be cleared after read
uint8_t ButtonDown(Button *B)
{
    uint8_t P = FALSE;

    if(B->Locked == FALSE)
    {
        if(B->Down == TRUE)
        {
            P = TRUE;
            B->Down = FALSE;
        }
    }
    else
    {
        P = FALSE;
    }

    return P;
}

//Reads the button Up flag.  Flag will be cleared after read
//Returns the number of ticks the button was held for
uint16_t ButtonUp(Button *B)
{
    uint16_t P = FALSE;

    if(B->Locked == FALSE)
    {
        if(B->Up == TRUE)
        {
            P = B->HoldTime;
            B->Up = FALSE;
        }
    }
    else
    {
        P = 0;
    }

    return P;
}

//returns the current hold time from the debounce algorithm
uint16_t ButtonCurrentHoldTime(Button *B)
{
    uint16_t P = FALSE;

    if(B->Locked == FALSE)
    {
        if(B->State == BUTTON_STATE_WAIT_FOR_RELEASE)
        {
            P = B->HoldTime;
        }
    }

    else
    {
        P = 0;
    }

    return P;
}


