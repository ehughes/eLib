#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#ifndef QUEUE_H_
#define QUEUE_H_


#define MAX_QUEUE_PRINTF_LENGTH 512

typedef struct {
    
        uint16_t ReadPtr;
    uint16_t WritePtr;
    uint16_t QueueSize;
    uint8_t *QueueStorage;
    
} ByteQueue;

#define QUEUE_FULL       -1
#define QUEUE_EMPTY      -2
#define QUEUE_OK          0


void InitByteQueue(ByteQueue *BQ,uint16_t Size,uint8_t * Storage); 
uint16_t BytesInQueue(ByteQueue *BQ);
int16_t ByteEnqueue(ByteQueue *BQ,uint8_t Val);
int16_t ByteArrayEnqueue(ByteQueue *BQ,uint8_t *Buf,uint16_t);
int16_t ByteDequeue(ByteQueue *BQ,uint8_t *Val);
uint8_t ForcedByteDequeue(ByteQueue *BQ);
void ByteArrayDequeue(ByteQueue *BQ,uint8_t *Val,uint32_t Len);

int16_t Qprintf(ByteQueue *BQ, const char *FormatString,...);



#endif /* QUEUE_H_ */