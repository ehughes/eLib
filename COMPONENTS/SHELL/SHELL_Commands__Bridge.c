#include "System.h"

int wolfcrypt_test(void* args);

cmd_function_t wolf_test(p_shell_context_t Shell,int32_t argc, char **argv)
{

    #if ENABLE_WATCHDOG  == 1
    SHELL_printf(Shell,"Watchdog is enabled.   Disable this compile time option before running the SSL tests.  You will get watchdog resets as it takes awhile!");
    #endif
   

    if(System_GetEpoch() == 0)
    {
      System_SetEpoch(1588102113);
      SHELL_printf(Shell,"epoch not set,  using %i to pass ASN test\r\n",System_GetEpoch());
    }
   
    SHELL_printf(Shell,"Testing wolfSSL integration. This may take several minutes\r\n");
   
    wolfcrypt_test(NULL);

    return 0;
     
}


cmd_function_t epoch(p_shell_context_t Shell,int32_t argc, char **argv)
{

    #define BQ Shell->ShellOutQueue 

    uint32_t EpochTime = System_GetEpoch();

    Q_JSON_Start(BQ);
   
    Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","Epoch");  Q_JSON_NextLine(BQ);
    Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result","OK");  Q_JSON_NextLine(BQ);
    Q_JSON_OutputTab(BQ); Q_JSON_OutputUnsignedIntegerVariable(BQ, "Epoch",EpochTime);  
  
    Q_JSON_Stop(BQ);

    return 0;
   


   #undef BQ

}
