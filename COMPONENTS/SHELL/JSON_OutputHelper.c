#include "System.h"

void StartJSON(p_shell_context_t Shell)
{
	uint8_t Val = ASCII_STX;
	
        ByteEnqueue(Shell->ShellOutQueue,Val);
       // xQueueSend(Shell->ShellOutQueue,&Val,portTICK_PERIOD_MS * 2000);
	SHELL_printf(Shell,"{\r\n");

}

void StopJSON(p_shell_context_t Shell)
{
	uint8_t Val = ASCII_ETX;
	SHELL_printf(Shell,"}\r\n");
	//xQueueSend(Shell->ShellOutQueue,&Val,portTICK_PERIOD_MS * 2000);
        ByteEnqueue(Shell->ShellOutQueue,Val);
       
}


void OutputTab(p_shell_context_t Shell)
{
	SHELL_printf(Shell,"    ");
}

void OutputTabs(p_shell_context_t Shell,uint32_t NumTabs)
{
	for(int i=0;i<NumTabs;i++)
	{
		OutputTab(Shell);
	}
}

void OutputObjID(p_shell_context_t Shell, char * ID)
{
	SHELL_printf(Shell,"\"ObjID\": \"%s\"",ID);
}

void OutputNewLine(p_shell_context_t Shell)
{
	SHELL_printf(Shell,"\r\n");
}

void OutputNewJSON_Line(p_shell_context_t Shell)
{
	SHELL_printf(Shell,",\r\n");
}

void OutputJSON_StringVariable(p_shell_context_t Shell,char * Name, char * Value)
{
	SHELL_printf(Shell,"\"%s\": \"%s\"", Name, Value);
}

void OutputJSON_IntegerVariable(p_shell_context_t Shell,char * Name, int32_t Value)
{
	SHELL_printf(Shell,"\"%s\": %i", Name, Value);
}

void OutputJSON_UnsignedIntegerVariable(p_shell_context_t Shell,char * Name, uint32_t Value)
{
	SHELL_printf(Shell,"\"%s\": %u", Name, Value);
}
void OutputJSON_FloatVariable(p_shell_context_t Shell,char * Name, float Value,uint32_t DecimalPlaces)
{
	switch(DecimalPlaces)
	{
		default:
		case 1: SHELL_printf(Shell,"\"%s\": %.1f", Name, Value);break;
		case 2: SHELL_printf(Shell,"\"%s\": %.2f", Name, Value);break;
		case 3: SHELL_printf(Shell,"\"%s\": %.3f", Name, Value);break;
		case 4: SHELL_printf(Shell,"\"%s\": %.4f", Name, Value);break;
		case 5: SHELL_printf(Shell,"\"%s\": %.5f", Name, Value);break;
		case 6: SHELL_printf(Shell,"\"%s\": %.6f", Name, Value);break;
		case 7: SHELL_printf(Shell,"\"%s\": %.7f", Name, Value);break;
		case 8: SHELL_printf(Shell,"\"%s\": %.8f", Name, Value);break;
	}

}

void OutputJSON_BooleanVariable(p_shell_context_t Shell,char * Name, bool Value)
{
	if(Value)
	{
		SHELL_printf(Shell,"\"%s\": true",Name);
	}
	else
	{
		SHELL_printf(Shell,"\"%s\": false",Name);
	}
}

void OutputUID(p_shell_context_t Shell)
{
	SHELL_printf(Shell,"\"UID\": \"%08X%08X%08X%08X\"",
					   System_SerialNumber[0],
					   System_SerialNumber[1],
					   System_SerialNumber[2],
					   System_SerialNumber[3]);

}


