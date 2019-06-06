#include "System.h"


cmd_function_t cell_at(p_shell_context_t Shell,int32_t argc, char **argv)
{
    char * Params;
    if(argc == 2)
    {
           Params = "";
    }
    else if(argc != 3)
    {
        SHELL_printf(Shell,"I need and AT command and 1 (optional) parameter.\r\n");
        return 0;
    }

    if(strnlen(argv[1],8)>2)
    {
        SHELL_printf(Shell,"AT command must be 2 characters.\r\n");
        return 0;
    }
    

    Params = argv[2];

    SHELL_printf(Shell,"\r\nSending : %s %s\r\n",argv[1],Params);
   
    Cell_SendAT_Cmd(argv[1],Params,0xFE);

    return 0;
     
}


cmd_function_t cell_txt(p_shell_context_t Shell,int32_t argc, char **argv)
{
  if(argc != 3)
    {
        SHELL_printf(Shell,"I need and a phone number and a message");
        return 0;
    }

    
    Cell_SendTxt(argv[1],argv[2]);

    return 0;
     
}


cmd_function_t cell(p_shell_context_t Shell,int32_t argc, char **argv)
{
    
     SHELL_printf(Shell,"State : [0x%02x] %s\r\n",Cell_GetConnectionState(),Cell_GetConnectionStateString(Cell_GetConnectionState()));
     SHELL_printf(Shell,"RSSI : -%ddbm\r\n",Cell_GetRSSI());
     SHELL_printf(Shell,"IMEI : %s\r\n",Cell_GetIMEI());
     SHELL_printf(Shell,"ICCID : %s\r\n",Cell_GetICCID());
     SHELL_printf(Shell,"Operator : %s\r\n",Cell_GetOperator());

    return 0;
     
}