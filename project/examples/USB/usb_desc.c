/**
 * @file usb_desc.c
 * @brief 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-11-20
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "usb_desc.h"

DEVICE_PROP *   pProperty;
USB_DeviceMess *pInformation;

DEVICE_PROP Device_Property =
    {
        Usb_init,
        0x40 /*MAX PACKET SIZE*/
};
/* USB Standard Device Descriptor */
const uint8_t DeviceDescriptor[SIZ_DEVICE_DESC] =
    {
        0x12,                       /*bLength */
        USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
        0x00,                       /*bcdUSB */
        0x02,
        0x00, /*bDeviceClass*/
        0x00, /*bDeviceSubClass*/
        0x00, /*bDeviceProtocol*/
        0x40, /*bMaxPacketSize 64*/
        0xFF, /*idVendor (0xFFFF), need user to modify*/
        0xFF,
        0x10, /*idProduct = 0x5710*/
        0x57,
        0x00, /*bcdDevice rel. 2.00*/
        0x02,
        1,   /*Index of string descriptor describing manufacturer */
        2,   /*Index of string descriptor describing product*/
        3,   /*Index of string descriptor describing the device serial number */
        0x01 /*bNumConfigurations*/
};           /* Joystick_DeviceDescriptor */

/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t ConfigDescriptor[SIZ_CONFIG_DESC] =
    {
        0x09,                              /* bLength: Configuration Descriptor size */
        USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
        SIZ_CONFIG_DESC,                   /* wTotalLength: Bytes returned */
        0x00,
        0x01, /*bNumInterfaces: 1 interface*/
        0x01, /*bConfigurationValue: Configuration value*/
        0x00, /*iConfiguration: Index of string descriptor describing
                                     the configuration*/
        0xE0, /*bmAttributes: Self powered */
        0x32, /*MaxPower 100 mA: this current is used for detecting Vbus*/

        /************** Descriptor of Joystick Mouse interface ****************/
        /* 09 */
        0x09,                          /*bLength: Interface Descriptor size*/
        USB_INTERFACE_DESCRIPTOR_TYPE, /*bDescriptorType: Interface descriptor type*/
        0x00,                          /*bInterfaceNumber: Number of Interface*/
        0x00,                          /*bAlternateSetting: Alternate setting*/
        0x01,                          /*bNumEndpoints*/
        0x03,                          /*bInterfaceClass: HID*/
        0x01,                          /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
        0x02,                          /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
        0,                             /*iInterface: Index of string descriptor*/
        /******************** Descriptor of Joystick Mouse HID ********************/
        /* 18 */
        0x09,                /*bLength: HID Descriptor size*/
        HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
        0x00,                /*bcdHID: HID Class Spec release number*/
        0x01,
        0x00,            /*bCountryCode: Hardware target country*/
        0x01,            /*bNumDescriptors: Number of HID class descriptors to follow*/
        0x22,            /*bDescriptorType*/
        SIZ_REPORT_DESC, /*wItemLength: Total length of Report descriptor*/
        0x00,
        /******************** Descriptor of Joystick Mouse endpoint ********************/
        /* 27 */
        0x07,                         /*bLength: Endpoint Descriptor size*/
        USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

        0x81, /*bEndpointAddress: Endpoint Address (IN)*/
        0x03, /*bmAttributes: Interrupt endpoint*/
        0x04, /*wMaxPacketSize: 4 Byte max */
        0x00,
        0x20, /*bInterval: Polling Interval (32 ms)*/
              /* 34 */
};            /* MOUSE_ConfigDescriptor */

const uint8_t ReportDescriptor[SIZ_REPORT_DESC] =
    {

        0x5, /*Usage Page(Generic Desktop)*/
        0x1,
        0x9, /*Usage(Mouse)*/
        0x2,
        0xA1, /*Collection(Logical)*/
        0x1,
        0x9, /*Usage(Pointer)*/
        0x1,
        0xA1, /*Collection(Linked)*/
        0x0,
        0x5, /*Usage Page(Buttons)*/
        0x9,
        0x19, /*Usage Minimum(1)*/
        0x1,
        0x29, /*Usage Maximum(3)*/
        0x3,
        0x15, /*Logical Minimum(0)*/
        0x0,
        0x25, /*Logical Maximum(1)*/
        0x1,
        0x95, /*Report Count(3)*/
        0x8,
        0x75, /*Report Size(1)*/
        0x1,
        0x81, /*Input(Variable)*/
        0x2,
        0x5,
        0x1,
        0x9,
        0x30,
        0x9,
        0x31,
        0x9,
        0x38,
        0x15,
        0x81,
        0x25,
        0x7f,
        0x75,
        0x8,
        0x95,
        0x3,
        0x81,
        0x6,
        0xC0,
        0xC0};

/* USB String Descriptors (optional) */
const uint8_t StringLangID[SIZ_STRING_LANGID] =
    {
        SIZ_STRING_LANGID,
        USB_STRING_DESCRIPTOR_TYPE,
        0x09,
        0x04}; /* LangID = 0x0409: U.S. English */

uint8_t StringSerial[SIZ_STRING_SERIAL] =
    {
        SIZ_STRING_SERIAL,          /* bLength */
        USB_STRING_DESCRIPTOR_TYPE, /* bDescriptorType */
        'A',
        0,
        'I',
        0,
        'P',
        0,
        '3',
        0,
        '2',
        0,
        'R',
        0,
        'V',
        0,
        '1',
        0,
        '5',
        0,
};

const uint8_t StringProduct[SIZ_STRING_PRODUCT] =
    {
        SIZ_STRING_PRODUCT,         /* bLength */
        USB_STRING_DESCRIPTOR_TYPE, /* bDescriptorType */
        'A', 0, 'I', 0, 'P', 0, '3', 0, '2', 0, 'R', 0, 'V', 0, '1', 0, '5', 0, ' ', 0,
        'm', 0, 'o', 0, 'u', 0, 's', 0, 'e', 0};

USB_OneDescriptor Device_Descriptor = {(uint8_t *)DeviceDescriptor, SIZ_DEVICE_DESC};

USB_OneDescriptor Config_Descriptor = {(uint8_t *)ConfigDescriptor, SIZ_CONFIG_DESC};

USB_OneDescriptor Joystick_Report_Descriptor = {(uint8_t *)ReportDescriptor, SIZ_REPORT_DESC};

USB_OneDescriptor Mouse_Hid_Descriptor = {(uint8_t *)ConfigDescriptor + JOYSTICK_OFF_HID_DESC, JOYSTICK_SIZ_HID_DESC};

USB_OneDescriptor String_Descriptor[3] = {{(uint8_t *)StringLangID, SIZ_STRING_LANGID},

                                          {(uint8_t *)StringProduct, SIZ_STRING_PRODUCT},
                                          {(uint8_t *)StringSerial, SIZ_STRING_SERIAL}};

// /**
//  * @brief
//  * @param[in] Length
//  * @return uint8_t*
//  */
// uint8_t *GetConfigDescriptor(uint16_t Length)
// {
//     return Standard_GetDescriptorData(Length, &Config_Descriptor);
// }
