#include "System.h"
#include "Shell.h"
#include "SHELL_Commands__Configuration.h"

void OutputParameterError(p_shell_context_t Shell, char * ErrorString)
{
        StartJSON(Shell);
        OutputTab(Shell); OutputObjID(Shell,"Parameter");  OutputNewJSON_Line(Shell);
        OutputTab(Shell); OutputUID(Shell);                                       OutputNewJSON_Line(Shell);
        OutputTab(Shell); OutputJSON_StringVariable(Shell, "Result",ErrorString);  OutputNewJSON_Line(Shell);

        OutputTab(Shell); SHELL_printf(Shell,"\"Parameters\":");  OutputNewLine(Shell);
        OutputTab(Shell); SHELL_printf(Shell,"[");  OutputNewLine(Shell);
        OutputTab(Shell); SHELL_printf(Shell,"]");  OutputNewLine(Shell);
        StopJSON(Shell);
}

cmd_function_t load_config(p_shell_context_t Shell,int32_t argc, char **argv)
{
        if(Configuration_Load(CONFIG_FILE_NAME) != CONFIG_LOAD_OK)
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

        if(Configuration_Export(CONFIG_FILE_NAME) != CONFIG_SAVE_OK)
        {
                OutputParameterError(Shell, "Error opening configuration file");
        }
        else
        {
                get(Shell,1,NULL);
        }

        return 0;
}

cmd_function_t set(p_shell_context_t Shell,int32_t argc, char **argv)
{
        uint32_t Index;

        if(argc == 3)
        {
                Index = Configuration_GetIndex(argv[1]);

                if(Index == CONFIGURATION_ITEM_NOT_FOUND)
                {
                        OutputParameterError(Shell,"Parameter Not Found");
                }
                else
                {
                        Configuration_SetByName(argv[1], argv[2]);
                        get(Shell,2,argv);
                }
        }
        else
        {

                        OutputParameterError(Shell,"Bad Arguments");
        }

        return 0;
}





cmd_function_t get(p_shell_context_t Shell,int32_t argc, char **argv)
{
        char *Value;
        uint32_t Index;

        if(argc == 1)
        {

                StartJSON(Shell);

                OutputTab(Shell); OutputObjID(Shell,"Parameter");  OutputNewJSON_Line(Shell);
            OutputTab(Shell); OutputUID(Shell);                                   OutputNewJSON_Line(Shell);
            OutputTab(Shell); OutputJSON_StringVariable(Shell, "Result","OK");  OutputNewJSON_Line(Shell);
                OutputTab(Shell); SHELL_printf(Shell,"\"Parameters\":");  OutputNewLine(Shell);
                OutputTab(Shell); SHELL_printf(Shell,"[");  OutputNewLine(Shell);

            for(int i=0;i<Configuration_GetNumItems();i++)
                {

        
                OutputTabs(Shell,2);SHELL_printf(Shell,"{"); OutputNewLine(Shell);

                OutputTabs(Shell,3);OutputTab(Shell);OutputJSON_StringVariable(Shell,"Name",MyConfigurationItems[i].Name); SHELL_printf(Shell," , "); OutputNewLine(Shell);

                OutputTabs(Shell,3);OutputTab(Shell);OutputJSON_StringVariable(Shell,"Description",MyConfigurationItems[i].DescriptionString); SHELL_printf(Shell," , "); OutputNewLine(Shell);

                Value = Configuration_GetValueString(MyConfigurationItems[i].Name);
                        OutputTabs(Shell,3);OutputTab(Shell);OutputJSON_StringVariable(Shell,"Value",Value);  SHELL_printf(Shell," , "); OutputNewLine(Shell);

                        OutputTabs(Shell,3);OutputTab(Shell);OutputJSON_StringVariable(Shell,"Min",MyConfigurationItems[i].Minimum);    SHELL_printf(Shell," , "); OutputNewLine(Shell);
                        OutputTabs(Shell,3);OutputTab(Shell);OutputJSON_StringVariable(Shell,"Max",MyConfigurationItems[i].Maximum);    SHELL_printf(Shell," , "); OutputNewLine(Shell);
                        OutputTabs(Shell,3);OutputTab(Shell);OutputJSON_StringVariable(Shell,"Default",MyConfigurationItems[i].Default); OutputNewLine(Shell);

                if(i<Configuration_GetNumItems()-1)
                {
                        OutputTabs(Shell,2);SHELL_printf(Shell,"},"); OutputNewLine(Shell);
                }
                else
                {
                        OutputTabs(Shell,2);SHELL_printf(Shell,"}"); OutputNewLine(Shell);
                }

                }

                OutputTab(Shell); SHELL_printf(Shell,"]");  OutputNewLine(Shell);
                StopJSON(Shell);

                Delay_mS(20);
        }
        else
        {



                Value = Configuration_GetValueString(argv[1]);
                Index = Configuration_GetIndex(argv[1]);
                if(Value == NULL)
                {
                        OutputParameterError(Shell, "Parameter Not Found");
                }
                else
                {

                        StartJSON(Shell);
                        OutputTab(Shell); OutputObjID(Shell,"Parameter");  OutputNewJSON_Line(Shell);
                        OutputTab(Shell); OutputUID(Shell);                                       OutputNewJSON_Line(Shell);

                        OutputTab(Shell); OutputJSON_StringVariable(Shell, "Result","OK");  OutputNewJSON_Line(Shell);

                        OutputTab(Shell); SHELL_printf(Shell,"\"Parameters\":");  OutputNewLine(Shell);
                        OutputTab(Shell); SHELL_printf(Shell,"[");  OutputNewLine(Shell);

                        OutputTab(Shell);

                        OutputTabs(Shell,2);SHELL_printf(Shell,"{");OutputNewLine(Shell);

                        OutputTabs(Shell,3);OutputJSON_StringVariable(Shell,"Name",argv[1]); SHELL_printf(Shell," , ");  OutputNewLine(Shell);
                        OutputTabs(Shell,3);OutputJSON_StringVariable(Shell,"Description",MyConfigurationItems[Index].DescriptionString); SHELL_printf(Shell," , ");  OutputNewLine(Shell);
                        OutputTabs(Shell,3);OutputJSON_StringVariable(Shell,"Value",Value);  SHELL_printf(Shell," , ");  OutputNewLine(Shell);
                        OutputTabs(Shell,3);OutputJSON_StringVariable(Shell,"Min",MyConfigurationItems[Index].Minimum);    SHELL_printf(Shell," , ");  OutputNewLine(Shell);
                        OutputTabs(Shell,3);OutputJSON_StringVariable(Shell,"Max",MyConfigurationItems[Index].Maximum);    SHELL_printf(Shell," , ");  OutputNewLine(Shell);
                        OutputTabs(Shell,3);OutputJSON_StringVariable(Shell,"Default",MyConfigurationItems[Index].Default);   OutputNewLine(Shell);
                        OutputTabs(Shell,2);SHELL_printf(Shell,"}"); OutputNewLine(Shell);

                        OutputTab(Shell); SHELL_printf(Shell,"]");  OutputNewLine(Shell);

                        StopJSON(Shell);
                }

        }

        return 0;
}

/*
cmd_function_t ee(p_shell_context_t Shell,int32_t argc, char **argv)
{
        if(argc == 5)
        {
                if(strcmp(argv[1],"MagicMonkeys") == 0)
                {
                        snprintf(&MyEEPROM_Settings.DoB[0],sizeof(MyEEPROM_Settings.DoB),"%s",argv[2]);
                        snprintf(&MyEEPROM_Settings.SN[0],sizeof(MyEEPROM_Settings.DoB),"%s",argv[3]);
                        snprintf(&MyEEPROM_Settings.HW_ID[0],sizeof(MyEEPROM_Settings.DoB),"%s",argv[4]);


                        EEPROM_Save();
                        br(Shell,1,NULL);
                }
                else
                {
                        SHELL_printf(Shell,"You got monkey poop thrown at you.\r\n");
                }

        }
        else
        {
                SHELL_printf(Shell,"You got monkey poop thrown at you.\r\n");
        }


        return 0;
}
*/
/*
cmd_function_t br(p_shell_context_t Shell,int32_t argc, char **argv)
{

        StartJSON(Shell);
        OutputTab(Shell); OutputObjID(Shell,"BirthRecord");  OutputNewJSON_Line(Shell);
        OutputTab(Shell); OutputUID(Shell);                                       OutputNewJSON_Line(Shell);

        OutputTab(Shell); OutputJSON_StringVariable(Shell, "DoB",&MyEEPROM_Settings.DoB[0]);  OutputNewJSON_Line(Shell);
        OutputTab(Shell); OutputJSON_StringVariable(Shell, "SN",&MyEEPROM_Settings.SN[0]);  OutputNewJSON_Line(Shell);
        OutputTab(Shell); OutputJSON_StringVariable(Shell, "Hardware Build Version",&MyEEPROM_Settings.HW_ID[0]);  OutputNewJSON_Line(Shell);
        OutputTab(Shell); OutputJSON_StringVariable(Shell, "PCB Version",GetPCB_Version());  OutputNewJSON_Line(Shell);

        OutputTab(Shell); OutputJSON_StringVariable(Shell, "Firmware Version",FIRMWARE_VERSION_STRING);  OutputNewJSON_Line(Shell);

        if(SCB->VTOR == 0)
        {
                OutputTab(Shell); OutputJSON_StringVariable(Shell, "Bootloader Version","Not Present");  OutputNewLine(Shell);
        }
        else
        {
                OutputTab(Shell); OutputJSON_StringVariable(Shell, "Bootloader Version",BOOTLOADER_VERSION_STRING);  OutputNewLine(Shell);
        }




        StopJSON(Shell);

        return 0;

}
*/