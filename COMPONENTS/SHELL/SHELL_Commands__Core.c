#include "System.h"
#include "SHELL.h"
#include "SHELL_Commands__Core.h"
#include "nrf52840.h"
#include "core_cm4.h"
#include "system_nrf52840.h"
#include "stdbool.h"

cmd_function_t i(p_shell_context_t Shell,int32_t argc, char **argv)
{
        Shell->echo = true;
        Shell->prompt = ">";

        return 0;
}

cmd_function_t q(p_shell_context_t Shell,int32_t argc, char **argv)
{
        Shell->echo = false;
        Shell->prompt = "";

        return 0;
}


cmd_function_t cow(p_shell_context_t Shell,int32_t argc, char **argv)
{

        SHELL_printf(&MySerialShell,VT100_YELLOW);

        SHELL_printf(Shell,"\r\n");
        SHELL_printf(Shell,"     /)  (\\     \r\n");
        SHELL_printf(Shell,".-._((,~~.))_.-, \r\n");
        SHELL_printf(Shell," `-.   @@   ,-'  \r\n");
        SHELL_printf(Shell,"   / ,o--o. \\   \r\n");
        SHELL_printf(Shell,"  ( ( .__. ) )   \r\n");
        SHELL_printf(Shell,"   ) `----' (     \r\n");
        SHELL_printf(Shell,"  /          \\   \r\n");
        SHELL_printf(Shell," /            \\  \r\n");
        SHELL_printf(Shell,"/              \\  \r\n\r\n");
        SHELL_printf(Shell,"     MOO!\r\n");

    SHELL_printf(&MySerialShell,VT100_DEFAULT);

        return 0;
}


cmd_function_t shell_test(p_shell_context_t Shell,int32_t argc, char **argv)
{
        SHELL_printf(Shell,"\r\n\r\n%i arguments parsed:\r\n",argc);
        SHELL_printf(Shell,"--------------------------------\r\n\r\n",argc);

        for(int i=0;i<argc;i++)
        {
                SHELL_printf(Shell,"%s --> string length : %i\r\n",argv[i],strlen(argv[i]));
        }

        SHELL_printf(Shell,"\r\n\r\n");

        return 0;
}

cmd_function_t reset(p_shell_context_t Shell,int32_t argc, char **argv)
{

    #define BQ Shell->ShellOutQueue 
        
    Q_JSON_Start(BQ);

    Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"Result","OK"); Q_JSON_NextLine(BQ);
    Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"Message","Rebooting in 5 Seconds");

    Q_JSON_Stop(BQ);

    #undef BQ

     System__FlagReboot();
     return 0;
}


#define SU_PWD "MagicMonkeys"

cmd_function_t su(p_shell_context_t Shell, int32_t argc, char **argv)
{

        if(argc == 1)
        {
                SHELL_printf(Shell,"Current Privilege Level is %d\r\n\r\n",Shell->CurrentPrivilegeLevel);
        }
        else
        {
                if(strncmp(SU_PWD,argv[1],sizeof(SU_PWD)) == 0)
                {
                        Shell->CurrentPrivilegeLevel = 1;

                        SHELL_printf(Shell,"Current Privilege Level is %d\r\n\r\n",Shell->CurrentPrivilegeLevel);
                }
                else
                {
                        SHELL_printf(Shell,"You got monkey poop thrown at you.\r\n\r\n");
                        Shell->CurrentPrivilegeLevel = 0;
                }

        }
    return 0;
}

extern uint32_t DebugChannelFlag;

extern const char *DebugChannelNames[32];
extern const char *DebugChannelColors[32];


cmd_function_t debug(p_shell_context_t Shell, int32_t argc, char **argv)
{

        if(argc == 1)
        {
                SHELL_printf(Shell,"\r\nCurrent Debug flag is 0x%08x\r\n\r\n",DebugChannelFlag);
                    
                for(int i=0;i<32;i++)
                {
                    if(DebugChannelNames[i][0] != 0x00)
                    {
                        SHELL_printf(Shell,"Debug Channel %i : %s --> ",i,DebugChannelNames[i]);

                        if(DebugChannelFlag&(1<<i))
                        {
                            SHELL_printf(Shell,VT100_GREEN"ON\r\n"VT100_DEFAULT);
                        }
                        else
                        {
                            SHELL_printf(Shell,VT100_RED"OFF\r\n"VT100_DEFAULT);
                        }

                    }

                }

                 SHELL_printf(Shell,VT100_RED"\r\n"VT100_DEFAULT);

        }
        else
        {
               int Arg = 0;

               if(sscanf(argv[1],"%i",&Arg) == 1)
               {
                    
                    if(Arg>=0 && Arg<32)
                    {
                        DebugChannelFlag ^= 1<<Arg;
  
                        SHELL_printf(Shell,"Debug Channel %i : %s --> ",Arg,DebugChannelNames[Arg]);

                        if(DebugChannelFlag&(1<<Arg))
                        {
                            SHELL_printf(Shell,VT100_GREEN"ON\r\n"VT100_DEFAULT);
                        }
                        else
                        {
                            SHELL_printf(Shell,VT100_RED"OFF\r\n"VT100_DEFAULT);
                        }
                    }
                    else
                    {
                         SHELL_printf(Shell,"I need a debug channel between 0 and 31 or 'all' or 'none'");
                    }
               }
               else if (strcmp(argv[1],"all") == 0)
               {
                   DebugChannelFlag = 0xFFFFFFFF;
                   SHELL_printf(Shell,"Enabling all debug channels.\r\n");
               }
               else if (strcmp(argv[1],"none") == 0)
               {
                    DebugChannelFlag = 0;
                    SHELL_printf(Shell,"\r\nDisabling all debug channels.\r\n\r\n");
               }
       }
    return 0;
}

