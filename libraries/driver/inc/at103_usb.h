/**
 * @file at103_usb.h
 * @brief 
 * 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-11-20
 * @copyright Copyright (c) 2022 Icore, Inc
 */

#include "at103.h"

#ifdef __cplusplus
extern "C" {
#endif
#define REG8(x)  (*((volatile uint8_t *)(x)))
#define REG16(x) (*((volatile uint16_t *)(x)))
#define REG32(x) (*((volatile uint32_t *)(x)))

typedef union {
    uint16_t w;
    struct BW {
        uint8_t bb0;
        uint8_t bb1;
    } bw;
} uint16_t_uint8_t;

typedef struct _DEVICE_INFO {
    uint8_t          bmRequestType; /* bmRequestType */
    uint8_t          bRequest;      /* bRequest */
    uint16_t_uint8_t wValues;       /* wValue */
    uint16_t_uint8_t wIndexs;       /* wIndex */
    uint16_t_uint8_t wLengths;      /* wLength */

} USB_DeviceMess;

typedef struct OneDescriptor {
    uint8_t *Descriptor;
    uint16_t Descriptor_Size;
} USB_OneDescriptor, *PONE_DESCRIPTOR;

typedef enum _STANDARD_REQUESTS {
    GET_STATUS = 0,
    CLR_FEATURE,
    RESERVED1,
    SET_FEATURE,
    RESERVED2,
    SET_ADDRESS,
    GET_DESCRIPTOR,
    SET_DESCRIPTOR,
    GET_CONFIGURATION,
    SET_CONFIGURATION,
    GET_INTERFACE,
    SET_INTERFACE,
    TOTAL_SREQUEST, /* Total number of Standard request */
    SYNCH_FRAME = 12
} STANDARD_REQUESTS;

/* Definition of "USBwValue" */
typedef enum _DESCRIPTOR_TYPE {
    DEVICE_DESCRIPTOR = 1,
    CONFIG_DESCRIPTOR,
    STRING_DESCRIPTOR,
    INTERFACE_DESCRIPTOR,
    ENDPOINT_DESCRIPTOR,
    QUALIFIER_DESCRIPTOR,
    REPORT_DESCRIPTOR = 0x22,

} DESCRIPTOR_TYPE;

enum {
    STRING0 = 0x0,
    STRING2 = 0x2,
    STRING3 = 0x3,
} STRING_TYPE;

enum {
    EP0,
    EP1,
    EP2,
    EP3,
    EP4,
    EP5,
    EP6,
    EP7,
    EP8,
    EP9,
    EP10,
    EP11,
    EP12,
    EP13,
    EP14,
    EP15
} EP_NUM;

#define USBwValue   wValues.w
#define USBwValue0  wValues.bw.bb0
#define USBwValue1  wValues.bw.bb1
#define USBwIndex   wIndexs.w
#define USBwIndex0  wIndexs.bw.bb0
#define USBwIndex1  wIndexs.bw.bb1
#define USBwLength  wLengths.w
#define USBwLength0 wLengths.bw.bb0
#define USBwLength1 wLengths.bw.bb1

/* TXCSR in Peripheral mode */
#define MUSB_TXCSR_P_AUTOSET    0x8000
#define MUSB_TXCSR_P_ISO        0x4000
#define MUSB_TXCSR_P_MODE       0x2000
#define MUSB_TXCSR_P_FrcDataTog 0x800
#define MUSB_TXCSR_P_INCOMPTX   0x0080
#define MUSB_TXCSR_P_SENTSTALL  0x0020
#define MUSB_TXCSR_P_SENDSTALL  0x0010
#define MUSB_TXCSR_P_UNDERRUN   0x0004
#define MUSB_TXCSR_P_FLUSHFIFO  0x100

#define CONFIG_EP(X) (USB_BASE + 0x100 + 0x10 * X)