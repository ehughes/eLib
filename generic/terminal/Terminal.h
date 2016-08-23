#include "System.h"

#ifndef TERMINAL_H_
#define TERMINAL_H_

void InitTerminal();
void ProcessTerminal();



#define TERMINAL_OUT_QUEUE		    UART3_Tx
#define TERMINAL_IN_QUEUE		    UART3_Rx
#define TERMINAL_PRINTF(...)   		Q_printf(&TERMINAL_OUT_QUEUE,__VA_ARGS__)
#define TERMINAL_PUTC(c)        	ByteEnqueue(&TERMINAL_OUT_QUEUE,c)
#define TERMINAL_READABLE       	BytesInQueue(&TERMINAL_IN_QUEUE)
#define TERMINAL_GETC           	ForcedByteDequeue(&TERMINAL_IN_QUEUE)

#define TERMINAL_READ_LIGHT	0
#define TERMINAL_READ_HEAVY	1


#define TERMINAL_READ_AGRESSIVENESS	TERMINAL_READ_HEAVY	//Define  TERMINAL_READ_LIGHT for 1 character read in the ProcessTerminal Loop
														//Define TERMINAL_READ_HEAVY for read queue until empty in the ProcessTerminal() loop


#endif /* TERMINAL_H_ */
