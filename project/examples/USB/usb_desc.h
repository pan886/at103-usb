/**
 * @file usb_desc.h
 * @brief 
 * 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-11-21
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"
#define USB_DEVICE_DESCRIPTOR_TYPE        0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE 0x02
#define USB_STRING_DESCRIPTOR_TYPE        0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE     0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE      0x05

#define HID_DESCRIPTOR_TYPE   0x21
#define JOYSTICK_SIZ_HID_DESC 0x09
#define JOYSTICK_OFF_HID_DESC 0x12

#define SIZ_DEVICE_DESC    18
#define SIZ_CONFIG_DESC    34
#define SIZ_REPORT_DESC    46
#define SIZ_STRING_LANGID  4
#define SIZ_STRING_VENDOR  38
#define SIZ_STRING_PRODUCT 33
#define SIZ_STRING_SERIAL  21

#define STANDARD_ENDPOINT_DESC_SIZE 0x09
extern const uint8_t   ConfigDescriptor[SIZ_CONFIG_DESC];
extern uint8_t         StringSerial[SIZ_STRING_SERIAL];
extern const uint8_t   StringLangID[SIZ_STRING_LANGID];
extern const uint8_t   StringProduct[SIZ_STRING_PRODUCT];
extern const uint8_t   ReportDescriptor[SIZ_REPORT_DESC];
extern const uint8_t   DeviceDescriptor[SIZ_DEVICE_DESC];
extern USB_DeviceMess *pInformation;
extern void            Usb_init(void);
typedef struct _DEVICE_PROP {
    void (*Init)(void); /* Initialize the device */
    uint8_t MaxPacketSize;

} DEVICE_PROP;

DEVICE_PROP         Device_Property;
extern DEVICE_PROP *pProperty;