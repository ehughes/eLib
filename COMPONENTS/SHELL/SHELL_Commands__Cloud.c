#include "System.h"
#include "Shell.h"
#include "SHELL_Commands__Configuration.h"

#define CLOUD_DEBUG(...)                  DEBUG_CHANNEL(DEBUG_CHANNEL_CLOUD,__VA_ARGS__)                                               
#define CLOUD_DEBUG_NO_MARKER(...)        DEBUG_NO_MARKER(DEBUG_CHANNEL_CLOUD,__VA_ARGS__)

cmd_function_t CloudBeep(p_shell_context_t Shell,int32_t argc, char **argv)
{

        CLOUD_DEBUG("Recieved Cloud Command : Beep\r\n");


       #define BQ Shell->ShellOutQueue 


        Mario11();


                Q_JSON_Start(BQ);
                Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","Mario11");  Q_JSON_NextLine(BQ);
                Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result","OK");  
                Q_JSON_Stop(BQ);

        #undef BQ

        return 0;
}

