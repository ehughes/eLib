#ifndef _JSON_H
#define _JSON_H

#include "JSMN/JSMN.h"
#include "DATA_BUFFER/DataBuffer.h"
#include "QUEUE/eQueue.h"
#include "stdint.h"
#include "stdbool.h"



typedef enum 
{
    JSON_BUILD__OK = 0,

    JSON_BUILD__NO_MEM = -1

} JSON_BuildResult;


JSON_BuildResult DB_JSON_MakeEmpty(DataBuffer *DB);

//Removes ASCII characters below 0x21 (white space and framing)
JSON_BuildResult DB_JSON_Strip(DataBuffer *DB);


void Q_JSON_OutputStringVariable(ByteQueue *BQ,char * Name, char * Value);
void Q_JSON_Stop(ByteQueue *BQ);
void Q_JSON_Start(ByteQueue *BQ);
void Q_JSON_NextLine(ByteQueue *BQ);
void Q_JSON_Tab(ByteQueue *BQ);
void Q_JSON_OutputTab(ByteQueue *BQ);
void Q_JSON_OutputTabs(ByteQueue *BQ,uint32_t NumTabs);
void Q_JSON_OutputIntegerVariable(ByteQueue *BQ,char * Name, int32_t Value);
void Q_JSON_OutputUnsignedIntegerVariable(ByteQueue *BQ,char * Name, uint32_t Value);
void Q_JSON_OutputBooleanVariable(ByteQueue *BQ,char * Name, bool Value);
void Q_JSON_Output_floatVariable(ByteQueue *BQ,char * Name, float Value,uint32_t DecimalPlaces);


int jsoneq(const char *json, jsmntok_t *tok, const char *s);

#endif
