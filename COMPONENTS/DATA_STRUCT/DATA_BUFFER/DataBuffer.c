#include "MasterConfiguration.h"
#include "stdint.h"
#include "stddef.h"
#include "DataBuffer.h"


DataBuffer Buffer_128[NUM_128_BUFFERS];

uint8_t BufferPool128_Region[128*NUM_128_BUFFERS];

void DataBuffer_InitPools()
{
    for(int i=0;i<NUM_128_BUFFERS;i++)
    {
         Buffer_128[i].CurrentLength = 0;
         Buffer_128[i].MaxLength = 128;
         Buffer_128[i].Data = &BufferPool128_Region[i*128];
    }
}

DataBuffer * DataBuffer_GetFrom128Pool()
{
    for(int i=0;i<NUM_128_BUFFERS;i++)
    {
         if(Buffer_128[i].CurrentLength == 0)
         {
                return &Buffer_128[i];
         }
    }
    
    return NULL;
}

void DataBufferQueue_Init(DataBufferQueue *DBQ, DataBuffer **QueueStorage, uint32_t QueueSize)
{
    if(DBQ == NULL)
    {
        return;
    }

    DBQ->QueueStorage = QueueStorage;
    DBQ->QueueSize = QueueSize;
    DBQ->ReadPtr = 0;
    DBQ->WritePtr = 0;
}

DataBufferQueueResult DataBuffer_Enqueue(DataBufferQueue *DBQ, DataBuffer *DB)
{
     DataBufferQueueResult Result = DATA_BUFFER_QUEUE_OK;

    if(DBQ == NULL || DB == NULL)
    {
        return DATA_BUFFER_QUEUE_BAD_ARG;
    }


     if(DataBuffer_QueueIsFull(DBQ))
     {
        DataBuffer * DB_Temp = DataBuffer_Dequeue(DBQ);

        DataBuffer_MarkFree(DB_Temp);

        Result = DATA_BUFFER_QUEUE_FULL_REMOVING_OLDEST;
     }
      
    DBQ->QueueStorage[DBQ->WritePtr] = DB;

    DBQ->WritePtr++;
    if(DBQ->WritePtr >= DBQ->QueueSize)
    {
      DBQ->WritePtr = 0;
    }

   return Result;

}

DataBuffer *DataBuffer_Peek(DataBufferQueue *DBQ)
{
    DataBuffer * Result = NULL;

    if(DBQ == NULL)
    {
        return Result;
    }

    if(DataBuffer_QueueIsNotEmpty(DBQ))
    {
        Result = DBQ->QueueStorage[DBQ->ReadPtr];
    }
    
    return Result;
}

DataBuffer *DataBuffer_Dequeue(DataBufferQueue *DBQ)
{
    DataBuffer * Result = NULL;

    if(DBQ == NULL)
    {
        return Result;
    }
  
    if(DataBuffer_QueueIsNotEmpty(DBQ))
    {
        Result = DBQ->QueueStorage[DBQ->ReadPtr];

        DBQ->ReadPtr++;

        if(DBQ->ReadPtr >= DBQ->QueueSize)
        {
          DBQ->ReadPtr = 0;
        }

    }
    
    return Result;
}

uint32_t DataBuffer_QueueIsFull(DataBufferQueue *DBQ)
{
    if(DBQ == NULL)
    {
        return 0;
    }


    if(DataBuffer_ElementsInQueue(DBQ) == ((DBQ->QueueSize)-1))
    {
      return 1;
    }
    else
    {
      return 0;
    }
}

uint32_t DataBuffer_ElementsInQueue(DataBufferQueue *DBQ)
{

    if(DBQ == NULL)
    {
        return 0;
    }

    if (DBQ->ReadPtr > DBQ->WritePtr) 
    {
        return (DBQ->QueueSize - DBQ->ReadPtr + DBQ->WritePtr);
    } 
    else if (DBQ->WritePtr > DBQ->ReadPtr)
    {
        return     (DBQ->WritePtr - DBQ->ReadPtr);
    } 
    else 
    {
        return 0;
    }
}

uint32_t DataBuffer_QueueIsNotEmpty(DataBufferQueue *DBQ)
{
    if(DBQ == NULL)
    {
        return 0;
    }

    if(DBQ->WritePtr == DBQ->ReadPtr)
    {
      return 0;
    }
    else
    {
      return 1;
    }
}

void DataBuffer_MarkFree(DataBuffer *DB)
{
    if(DB!=NULL)
    {
        DB->CurrentLength = 0;
    }
   
}

