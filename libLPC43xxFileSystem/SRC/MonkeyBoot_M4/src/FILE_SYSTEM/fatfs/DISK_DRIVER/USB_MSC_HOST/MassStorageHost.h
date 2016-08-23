/*
 * MassStorageHost.h
 *
 *  Created on: May 23, 2014
 *      Author: ehughes
 */
#include "stdint.h"
#include ".\lpcusblib\Drivers\USB\USB.h"

#ifndef MASSSTORAGEHOST_H_
#define MASSSTORAGEHOST_H_

extern volatile uint8_t USB_HostEnumState;

#define USB_HOST_STATE_DEVICE_INIT							0
#define USB_HOST_STATE_WAIT_FOR_DEVICE_ATTACH				1
#define USB_HOST_STATE_DEVICE_ATTACHED_ENUMERATING			2
#define USB_HOST_STATE_DEVICE_ATTACHED_GET_DISK_CAPACITY	3
#define USB_HOST_STATE_DEVICE_ATTACHED_READY				4
#define USB_HOST_STATE_DEVICE_ENUMERATION_ERROR				5
#define USB_HOST_STATE_HOST_ERROR							6

extern const char *USB_HostStateStringTable[7];

/** Configures the board hardware and chip peripherals for the demo's functionality. */
void InitUSB_MSC_Host();


#endif /* MASSSTORAGEHOST_H_ */
