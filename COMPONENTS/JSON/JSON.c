#include "JSON.h"
#include "Components/DATA_STRUCT/DATA_BUFFER/DataBuffer.h"
#include "Components/DATA_STRUCT/QUEUE/eQueue.h"

#include "stdio.h"

#define ASCII_STX       0x02
#define ASCII_ETX       0x03

JSON_BuildResult DB_CheckBuffer(DataBuffer *DB)
{
    if(DB == NULL)
        return JSON_BUILD__NO_MEM;

    if(DB->CurrentLength>=DB->MaxLength)
        return JSON_BUILD__NO_MEM;

    return JSON_BUILD__OK;
}


JSON_BuildResult DB_JSON_Init(DataBuffer *DB)
{
    JSON_BuildResult BR = DB_CheckBuffer(DB);
    if(BR !=JSON_BUILD__OK)
        return BR;

    DB->CurrentLength = 0;
    DB->Data[0] = 0;

}


JSON_BuildResult DB_JSON_Strip(DataBuffer *DB) 
{ 

    JSON_BuildResult BR = DB_CheckBuffer(DB);
    
    if(BR !=JSON_BUILD__OK)
        return BR;

    int Count = 0; 

    for (int i = 0; (DB->Data[i] && (i<DB->CurrentLength)); i++) 
    {
        if (DB->Data[i]!= ASCII_ETX &&
            DB->Data[i]!= ASCII_STX
            )
        {
         DB->Data[Count++] = DB->Data[i];  
         }
    }              
                  
    DB->Data[Count] = '\0'; 
    DB->CurrentLength = Count;

    return JSON_BUILD__OK;

}
 


JSON_BuildResult DB_JSON_MakeEmpty(DataBuffer *DB)
{
    JSON_BuildResult BR = DB_JSON_Init(DB);

    if(BR!=JSON_BUILD__OK)
        return BR;

    if(DB->MaxLength <5)
        return JSON_BUILD__NO_MEM;
 
    DB->Data[0] = ASCII_STX;
    DB->Data[1] = '{';
    DB->Data[2] = '}';
    DB->Data[3] = ASCII_ETX;
    DB->Data[4] = 0;

    DB->CurrentLength = 5;
}



void Q_JSON_Start(ByteQueue *BQ)
{
        uint8_t Val = ASCII_STX;
        ByteEnqueue(BQ,Val);
        StringEnqueue(BQ,"{\r\n");
}

void Q_JSON_Stop(ByteQueue *BQ)
{
        uint8_t Val = ASCII_ETX;
        StringEnqueue(BQ,"\r\n}");
        ByteEnqueue(BQ,Val);
}

void Q_JSON_OutputStringVariable(ByteQueue *BQ,char * Name, char * Value)
{
        Qprintf(BQ,"\"%s\": \"%s\"", Name, Value);
}


void Q_JSON_OutputIntegerVariable(ByteQueue *BQ,char * Name, int32_t Value)
{
        Qprintf(BQ,"\"%s\": %i", Name, Value);
}

void Q_JSON_OutputUnsignedIntegerVariable(ByteQueue *BQ,char * Name, uint32_t Value)
{
        Qprintf(BQ,"\"%s\": %u", Name, Value);
}


void Q_JSON_OutputBooleanVariable(ByteQueue *BQ,char * Name, bool Value)
{
        if(Value)
        {
                Qprintf(BQ,"\"%s\": true",Name);
        }
        else
        {
                Qprintf(BQ,"\"%s\": false",Name);
        }
}

void Q_JSON_Output_floatVariable(ByteQueue *BQ,char * Name, float Value,uint32_t DecimalPlaces)
{
        switch(DecimalPlaces)
        {
                default:
                case 1: Qprintf(BQ,"\"%s\": %.1f", Name, Value);break;
                case 2: Qprintf(BQ,"\"%s\": %.2f", Name, Value);break;
                case 3: Qprintf(BQ,"\"%s\": %.3f", Name, Value);break;
                case 4: Qprintf(BQ,"\"%s\": %.4f", Name, Value);break;
                case 5: Qprintf(BQ,"\"%s\": %.5f", Name, Value);break;
                case 6: Qprintf(BQ,"\"%s\": %.6f", Name, Value);break;
                case 7: Qprintf(BQ,"\"%s\": %.7f", Name, Value);break;
                case 8: Qprintf(BQ,"\"%s\": %.8f", Name, Value);break;
        }

}

void Q_JSON_Tab(ByteQueue *BQ)
{
        StringEnqueue(BQ,"    ");
}

void Q_JSON_NextLine(ByteQueue *BQ)
{
        StringEnqueue(BQ,",\r\n");
}


void Q_JSON_OutputTab(ByteQueue *BQ)
{
        StringEnqueue(BQ,"    ");
}

void Q_JSON_OutputTabs(ByteQueue *BQ,uint32_t NumTabs)
{
        for(int i=0;i<NumTabs;i++)
        {
                Q_JSON_OutputTab(BQ);
        }
}


JSON_BuildResult Q_JSON_MakeEmpty(ByteQueue *BQ)
{
  //  JSON_BuildResult BR = DB_JSON_Init(DB);

   // if(BR!=JSON_BUILD__OK)
    //    return BR;

  //  if(DB->MaxLength <3)
   //     return JSON_BUILD__NO_MEM;
 
   //- DB->CurrentLength = sprintf(DB->Data,"{}") + 1;
}


int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
        if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
                        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
                return 0;
        }
        return -1;
}




