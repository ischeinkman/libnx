/**
 * @file usb_comms.h
 * @brief USB comms.
 * @author yellows8
 * @author plutoo
 * @copyright libnx Authors
 */
#pragma once
#include "../../types.h"
#include "../../services/usbds.h"

typedef struct {
    u8 bInterfaceClass;
    u8 bInterfaceSubClass;
    u8 bInterfaceProtocol;
} UsbCommsInterfaceInfo;

/// Initializes usbComms with the default number of interfaces (1)
Result usbCommsInitialize(void);

/// Initializes usbComms with a specific number of interfaces.
Result usbCommsInitializeEx(u32 num_interfaces, const UsbCommsInterfaceInfo *infos);

/// Exits usbComms.
void usbCommsExit(void);

/// Sets whether to throw a fatal error in usbComms{Read/Write}* on failure, or just return the transferred size. By default (false) the latter is used.
void usbCommsSetErrorHandling(bool flag);

/// Read data with the default interface.
size_t usbCommsRead(void* buffer, size_t size);

/// Write data with the default interface.
size_t usbCommsWrite(const void* buffer, size_t size);

/// Same as usbCommsRead except with the specified interface.
size_t usbCommsReadEx(void* buffer, size_t size, u32 interface);

/// Same as usbCommsWrite except with the specified interface.
size_t usbCommsWriteEx(const void* buffer, size_t size, u32 interface);

/// Gets the raw usbDs data for the default usbComms interface.
Result usbCommsGetInfo(UsbDsInterface ** interface_data, UsbDsEndpoint ** endpoint_in, UsbDsEndpoint ** endpoint_out);

/// Same as usbCommsGetInfo except with the specified interface. 
Result usbCommsGetInfoEx(UsbDsInterface ** interface_data, UsbDsEndpoint ** endpoint_in, UsbDsEndpoint ** endpoint_out, u32 interface);