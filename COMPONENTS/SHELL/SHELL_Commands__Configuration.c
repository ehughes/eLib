#include "System.h"
#include "Shell.h"
#include "SHELL_Commands__Configuration.h"

void OutputParameterError(p_shell_context_t Shell, char * ErrorString)
{
        #define BQ Shell->ShellOutQueue 

        Q_JSON_Start(BQ);
        Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","Parameter");  Q_JSON_NextLine(BQ);
        Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result",ErrorString);  

        Q_JSON_Stop(BQ);

        #undef BQ
}

cmd_function_t load_config(p_shell_context_t Shell,int32_t argc, char **argv)
{
        if(Configuration_Load(&MyRuntimeConfiguration,CONFIG_FILE_NAME) != CONFIG_LOAD_OK)
        {
                OutputParameterError(Shell, "Error loading config file.  Using default values");
        }
        else
        {
                get(Shell,1,NULL);
        }

        return 0;
}

cmd_function_t save_config(p_shell_context_t Shell,int32_t argc, char **argv)
{

        if(Configuration_Export(&MyRuntimeConfiguration,CONFIG_FILE_NAME) != CONFIG_SAVE_OK)
        {
                OutputParameterError(Shell, "Error opening configuration file");
        }
        else
        {
                get(Shell,1,NULL);
        }

        return 0;
}


/*
    mini save doesn't call get after saving
*/
cmd_function_t msave(p_shell_context_t Shell,int32_t argc, char **argv)
{
   #define BQ Shell->ShellOutQueue 

        if(Configuration_Export(&MyRuntimeConfiguration,CONFIG_FILE_NAME) != CONFIG_SAVE_OK)
        {
                OutputParameterError(Shell, "Error opening configuration file");
        }
        else
        {
                Q_JSON_Start(BQ);
                Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","Save");  Q_JSON_NextLine(BQ);
                Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result","OK");  
                Q_JSON_Stop(BQ);
        }

        return 0;

        #undef BQ
}


cmd_function_t set(p_shell_context_t Shell,int32_t argc, char **argv)
{
        uint32_t Index;

        if(argc == 3)
        {
                Index = Configuration_GetIndex(&MyRuntimeConfiguration,argv[1]);

                if(Index == CONFIGURATION_ITEM_NOT_FOUND)
                {
                        OutputParameterError(Shell,"Parameter Not Found");
                }
                else
                {
                        Configuration_SetByName(&MyRuntimeConfiguration,argv[1], argv[2]);
                        get(Shell,2,argv);
                }
        }
        else
        {

                        OutputParameterError(Shell,"Bad Arguments");
        }

        return 0;
}


/*
   mini set calls mini get after complete  
*/

cmd_function_t mset(p_shell_context_t Shell,int32_t argc, char **argv)
{
        uint32_t Index;

        if(argc == 3)
        {
                Index = Configuration_GetIndex(&MyRuntimeConfiguration,argv[1]);

                if(Index == CONFIGURATION_ITEM_NOT_FOUND)
                {
                        OutputParameterError(Shell,"Parameter Not Found");
                }
                else
                {
                        Configuration_SetByName(&MyRuntimeConfiguration,argv[1], argv[2]);
                        mget(Shell,2,argv);
                }
        }
        else
        {

                        OutputParameterError(Shell,"Bad Arguments");
        }

        return 0;
}



/*
   mini get only outputs 1 parameter at a time  
*/

cmd_function_t mget(p_shell_context_t Shell,int32_t argc, char **argv)
{

        #define BQ Shell->ShellOutQueue 

        char *Value;
        uint32_t Index;

        if(argc == 1)
        {
            OutputParameterError(Shell, "Need Argument");
        }
        else
        {
                Value = Configuration_GetValueString(&MyRuntimeConfiguration,argv[1]);
                Index = Configuration_GetIndex(&MyRuntimeConfiguration,argv[1]);
 
                 Q_JSON_Start(BQ);
                 Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","SingleParameter");  Q_JSON_NextLine(BQ);
 
                 if(Value == NULL)
                 {
                     Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result","Not Found");  Q_JSON_NextLine(BQ);
                     Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Name","");   Q_JSON_NextLine(BQ);
                     Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Value","");   
                 }
                 else
                 {
                     Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result","OK");  Q_JSON_NextLine(BQ);
                     Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Name",argv[1]);  Q_JSON_NextLine(BQ);
                    Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Value",Value);   
                 }
                      
                 Q_JSON_Stop(BQ);
        }

        return 0;

        #undef BQ
}


cmd_function_t get(p_shell_context_t Shell,int32_t argc, char **argv)
{

       #define BQ Shell->ShellOutQueue 

        char *Value;
        uint32_t Index;

        if(argc == 1)
        {

            Q_JSON_Start(BQ);
            Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","Parameter");  Q_JSON_NextLine(BQ);
            Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result","OK");  Q_JSON_NextLine(BQ);
            Q_JSON_OutputTab(BQ); SHELL_printf(Shell,"\"Parameters\":");  SHELL_printf(Shell,"\r\n");  
            Q_JSON_OutputTab(BQ); SHELL_printf(Shell,"[\r\n");  

            uint32_t NumItems = Configuration_GetCount(&MyRuntimeConfiguration);

            for(int i=0;i<NumItems;i++)
                {

        
                Q_JSON_OutputTabs(BQ,2);SHELL_printf(Shell,"{\r\n"); 

                Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Name",MyRuntimeConfiguration[i].Name);  Q_JSON_NextLine(BQ);

                Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Description",MyRuntimeConfiguration[i].DescriptionString); Q_JSON_NextLine(BQ);

                Value = Configuration_GetValueString(&MyRuntimeConfiguration, MyRuntimeConfiguration[i].Name);
               
                Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Value",Value);   Q_JSON_NextLine(BQ);

                Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Min",MyRuntimeConfiguration[i].Minimum);     Q_JSON_NextLine(BQ);
                Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Max",MyRuntimeConfiguration[i].Maximum);     Q_JSON_NextLine(BQ);
                Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputTab(BQ);Q_JSON_OutputStringVariable(BQ,"Default",MyRuntimeConfiguration[i].Default); SHELL_printf(Shell,"\r\n");

                if(i<Configuration_GetCount(&MyRuntimeConfiguration)-1)
                {
                        Q_JSON_OutputTabs(BQ,2);SHELL_printf(Shell,"},\r\n"); 
                }
                else
                {
                        Q_JSON_OutputTabs(BQ,2);SHELL_printf(Shell,"}\r\n"); 
                }

                  System__Delay_mS(100);

                }

                Q_JSON_OutputTab(BQ); SHELL_printf(Shell,"]");  
                Q_JSON_Stop(BQ);

              
        }
        else
        {



                Value = Configuration_GetValueString(&MyRuntimeConfiguration, argv[1]);
                Index = Configuration_GetIndex(&MyRuntimeConfiguration, argv[1]);
                if(Value == NULL)
                {
                        OutputParameterError(Shell, "Parameter Not Found");
                }
                else
                {

                        Q_JSON_Start(BQ);
                        Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","Parameter");  Q_JSON_NextLine(BQ);
                     
                        Q_JSON_OutputTab(BQ); Q_JSON_OutputStringVariable(BQ, "Result","OK");  Q_JSON_NextLine(BQ);

                        Q_JSON_OutputTab(BQ); SHELL_printf(Shell,"\"Parameters\":\r\n");  
                        Q_JSON_OutputTab(BQ); SHELL_printf(Shell,"[\r\n"); 

               
                        Q_JSON_OutputTabs(BQ,2);SHELL_printf(Shell,"{\r\n");

                        Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputStringVariable(BQ,"Name",argv[1]);  Q_JSON_NextLine(BQ);
                        Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputStringVariable(BQ,"Description",MyRuntimeConfiguration[Index].DescriptionString);   Q_JSON_NextLine(BQ);
                        Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputStringVariable(BQ,"Value",Value);    Q_JSON_NextLine(BQ);
                        Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputStringVariable(BQ,"Min",MyRuntimeConfiguration[Index].Minimum);    Q_JSON_NextLine(BQ);
                        Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputStringVariable(BQ,"Max",MyRuntimeConfiguration[Index].Maximum);    Q_JSON_NextLine(BQ);
                        Q_JSON_OutputTabs(BQ,3);Q_JSON_OutputStringVariable(BQ,"Default",MyRuntimeConfiguration[Index].Default);   SHELL_printf(Shell,"\r\n"); 
                        Q_JSON_OutputTabs(BQ,2);SHELL_printf(Shell,"}\r\n"); 

                        Q_JSON_OutputTab(BQ); SHELL_printf(Shell,"]");  

                        Q_JSON_Stop(BQ);
                }

        }

        return 0;

        #undef BQ
}


cmd_function_t br(p_shell_context_t Shell,int32_t argc, char **argv)
{
        #define BQ Shell->ShellOutQueue 
        
        Q_JSON_Start(BQ);

        Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"ObjID","BirthRecord");  Q_JSON_NextLine(BQ);
        Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"FirmwareVersion",FIRMWARE_VERSION_STRING);  
        Q_JSON_Stop(BQ);

        return 0;

        #undef BQ

}
