
#include "System.h"
#include "SHELL_Commands__Application.h"

cmd_function_t probe(p_shell_context_t Shell,int32_t argc, char **argv)
{
    #define BQ Shell->ShellOutQueue 
        
    Q_JSON_Start(BQ);

 
    Qprintf(BQ,"\"probe\":[\r\n");

        


    for(int i=0;i<NUM_SENSOR_CHANNELS;i++)
    {
        if(BirthRecordAcquired[i] == false)
        {
            Qprintf(BQ,"{}");
        }
        else
        {
             Qprintf(BQ,"%s",BirthRecords[i]);
        }

        if(i<NUM_SENSOR_CHANNELS-1)
        {
            Q_JSON_NextLine(BQ);
        }

    }

     Qprintf(BQ,"]");


     Q_JSON_Stop(BQ);

     return 0;

     #undef BQ

}


cmd_function_t pp(p_shell_context_t Shell,int32_t argc, char **argv)
{

       #define BQ Shell->ShellOutQueue 
   
       int Arg = 0;

       char *Result = "OK";
       char *Message = "";
       char MessageBuf[64];

        MessageBuf[0] = 0;

       if(argc == 1)
       {
              FourChannel__RequestPowerCycleAll();

              Result = "OK";
              Message = "Scheduling power cycle on all channels";

       }
       else  if (argc == 2)
       {
            if(sscanf(argv[1],"%i",&Arg) == 1)
            {
                if(Arg>0 && Arg<=5)
                {
                     Result = "OK";
                     sprintf(MessageBuf,"Scheduling power cycle on channel %i",Arg);
                     FourChannel__RequestPowerCycle(Arg-1);
                }
                else
                {
                     Result = "ERROR";
                     sprintf(MessageBuf,"Invalid channel of %i.  Need 1,2,3 or 4",Arg);
                }
            }


            Message = &MessageBuf[0];
       }

        
    Q_JSON_Start(BQ);

    Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"Result",Result);  Q_JSON_NextLine(BQ);
  
    Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"Message",Message);  
  
    Q_JSON_Stop(BQ);

     return 0;

     #undef BQ

}


cmd_function_t rt(p_shell_context_t Shell,int32_t argc, char **argv)
{

       #define BQ Shell->ShellOutQueue 
   
       int Arg = 0;

       char *Result = "OK";
       char *Message = "";
       char MessageBuf[64];

        MessageBuf[0] = 0;


        Result = "OK";
        Message = "Resyncing time";
        
        Q_JSON_Start(BQ);

        Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"Result",Result);  Q_JSON_NextLine(BQ);
  
        Q_JSON_Tab(BQ); Q_JSON_OutputStringVariable(BQ,"Message",Message);  
  
        Q_JSON_Stop(BQ);

        NTP_ForceResync();

         return 0;

         #undef BQ

}