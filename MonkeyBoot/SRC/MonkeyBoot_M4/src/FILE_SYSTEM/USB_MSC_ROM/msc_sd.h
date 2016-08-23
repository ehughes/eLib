#ifndef _MSC_SD_H
#define _MSC_SD_H


int InitUSB_ROM_MSC(void);

extern uint8_t 				MSC_Cache[MSC_CACHE_SIZE];
extern int32_t			    MSC_CacheWritePtr;
extern uint32_t				MSC_CacheBaseAddress;
extern volatile uint32_t	MSC_CacheFlushTicker;

#define MARK_MSC_CACHE_AS_EMPTY			MSC_CacheWritePtr = -1
#define MSC_CACHE_IS_EMPTY				(MSC_CacheWritePtr == -1)
#define MSC_CACHE_IS_NOT_EMPTY			(MSC_CacheWritePtr != -1)

#endif
