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
    

 #define BQ Shell->ShellOutQueue 
        
    Q_JSON_Start(BQ);

    Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","Cell");Q_JSON_NextLine(BQ);
    
    Q_JSON_Tab(BQ); SHELL_printf(Shell,"\"State\":\"[0x%02x] %s\"",Cell_GetConnectionState(),Cell_GetConnectionStateString(Cell_GetConnectionState()));Q_JSON_NextLine(BQ);
    Q_JSON_Tab(BQ); SHELL_printf(Shell,"\"RSSI\":\"-%ddbm\"",Cell_GetRSSI());Q_JSON_NextLine(BQ);
    Q_JSON_Tab(BQ); SHELL_printf(Shell,"\"IMEI\":\"%s\"",Cell_GetIMEI());Q_JSON_NextLine(BQ);
    Q_JSON_Tab(BQ); SHELL_printf(Shell,"\"ICCID\":\"%s\"",Cell_GetICCID());Q_JSON_NextLine(BQ);
    Q_JSON_Tab(BQ); SHELL_printf(Shell,"\"Operator\":\"%s\"",Cell_GetOperator());Q_JSON_NextLine(BQ);
    Q_JSON_Tab(BQ); SHELL_printf(Shell,"\"Firmware\":\"%x\"",CellFirmwareVersion);

    Q_JSON_Stop(BQ);

    return 0;

#undef BQ
     
}