#ifndef _CAN_QUEUE_H
#define _CAN_QUEUE_H

typedef struct {
			
				uint32_t	SID;
				uint8_t	Data[8];	
			
				} CAN_Msg;
			
typedef struct {
	    
	    CANMsg *Message;
		volatile  uint16_t ReadPtr;
		volatile  uint16_t  WritePtr;
		volatile  uint16_t  QueueSize;
		 
} CAN_MessageQueue;
		