#include "System.h"
#include "cr_section_macros.h"

#ifndef FILESYSTEMMASTERCONFIG_H_
#define FILESYSTEMMASTERCONFIG_H_

/* Comment below and uncomment USE_USB1 to enable USB1 */
//#define USB_MSC_DEVICE_IS_USB0

#define USB_MSC_DEVICE_IS_USB0

//#define USB_MSC_DEVICE_IS_USB1

//#define USB_MSC_HOST_IS_USB0
#define USB_MSC_HOST_IS_USB1


#ifdef USB_MSC_DEVICE_IS_USB0
	#ifdef USB_MSC_HOST_IS_USB0
		#error Cannot enable MSC device and host on USB 0
	#endif
#endif

#ifdef USB_MSC_DEVICE_IS_USB1
	#ifdef USB_MSC_HOST_IS_USB1
		#error "Cannot enable MSC device and host on USB 0"
	#endif
#endif



#define MSC_DRIVE			DRIVE_SD

#define TERMINAL_PRINTF		DEBUGOUT

//THis Variable must be incremented by a system timer.   It is scaled to 1mS per bit
extern volatile uint32_t DiskManagerTicker;
extern volatile uint32_t MSC_CacheFlushTicker;

#define MSC_CACHE_REGION				RAM2
#define ENABLE_MSC_CACHE
#define	MSC_CACHE_WRITE_BLOCK_SIZE		(512)
#define MSC_CACHE_SIZE					(1<<14)


#endif /* FILESYSTEMMASTERCONFIG_H_ */
