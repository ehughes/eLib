#include "stdint.h"

#ifndef _DATA_BUFFER_H
#define _DATA_BUFFER_H


typedef struct
{
    uint16_t CurrentLength;
    uint16_t MaxLength;
    uint8_t *Data;
} DataBuffer;


typedef struct
{
    uint32_t ReadPtr;
    uint32_t WritePtr;
    

    DataBuffer ** QueueStorage;
    uint32_t QueueSize; //With this Queue,  the size is always one bigger than what we can actually store to distinuigh between Queue Full and Queue Empty

} DataBufferQueue;


typedef enum 
{
        DATA_BUFFER_QUEUE_OK = 0,
        DATA_BUFFER_QUEUE_FULL_REMOVING_OLDEST,
        DATA_BUFFER_QUEUE_BAD_ARG,

} DataBufferQueueResult;


void DataBufferQueue_Init(DataBufferQueue *DBQ, DataBuffer **QueueStorage, uint32_t QueueSize);

DataBufferQueueResult DataBuffer_Enqueue(DataBufferQueue *DBQ, DataBuffer *DB);

DataBuffer *DataBuffer_Dequeue(DataBufferQueue *DBQ);

uint32_t DataBuffer_ElementsInQueue(DataBufferQueue *DBQ);

void DataBuffer_MarkFree(DataBuffer *DB);

void DataBuffer_InitPools();

DataBuffer * DataBuffer_GetFrom128Pool();


uint32_t DataBuffer_QueueIsFull(DataBufferQueue *DBQ);

DataBuffer *DataBuffer_Dequeue(DataBufferQueue *DBQ);

uint32_t DataBuffer_QueueIsNotEmpty(DataBufferQueue *DBQ);

void DataBuffer_MarkFree(DataBuffer *DB);

#define NUM_128_BUFFERS 128


#endif