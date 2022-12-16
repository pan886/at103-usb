/**
 * @file at103_usb_core.c
 * @brief 
 * 
 * @author pantianwen (pantianwen@163.com)
 * @version 1.0
 * @date 2022-11-15
 * @copyright Copyright (c) 2022 Icore, Inc
 */
#include "at103.h"
#include "usb_desc.h"
const uint8_t Joystick_ConfigDescriptor[SIZ_CONFIG_DESC];
const uint8_t Joystick_StringSerial[SIZ_STRING_SERIAL];
const uint8_t Joystick_StringProduct[SIZ_STRING_PRODUCT];
const uint8_t Joystick_StringLangID[SIZ_STRING_LANGID];
const uint8_t Joystick_ConfigDescriptor[SIZ_CONFIG_DESC];
const uint8_t Joystick_ReportDescriptor[SIZ_REPORT_DESC];
const uint8_t DeviceDescriptor[SIZ_DEVICE_DESC];
void          musb_disable_interrupts()
{

    /* disable interrupts */
    REG8(USB_BASE + REG_INTRUSBE) = 0;
    REG8(USB_BASE + REG_INTRTXE)  = 0;
    REG8(USB_BASE + REG_INTRRXE)  = 0;
    REG8(USB_BASE + REG_INTRUSB);
    REG8(USB_BASE + REG_INTRTX);
    REG8(USB_BASE + REG_INTRRX);
}

void musb_enable_interrupts()
{
    REG8(USB_BASE + REG_INTRUSBE) = 0xf7;
    REG8(USB_BASE + REG_INTRTXE)  = 0xffff;
    REG8(USB_BASE + REG_INTRRXE)  = 0xfffe;
}

void musb_stop()
{
    musb_disable_interrupts();
    REG8(USB_BASE + REG_DEVCTL) &= ~MUSB_DEVCTL_SESSION;
}

const uint8_t musb_test_packet[53] = {

    /* JKJKJKJK x9 */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    /* JJKKJJKK x8 */
    0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa,
    /* JJJJKKKK x8 */
    0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
    /* JJJJJJJKKKKKKK x8 */
    0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    /* JJJJJJJK x8 */
    0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd,
    /* JKKKKKKK x10, JK */
    0xfc, 0x7e, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0x7e

};

void musb_write_fifo(uint8_t EP_Index, const uint8_t Buffer[], uint8_t Length)
{
    uint8_t i = 0;
    for (i = 0; i < Length; i++) {
        REG8(USB_BASE + REG_EP0_FIFO + EP_Index * 4) = Buffer[i];
    }
}

uint8_t *musb_read_fifo(uint8_t index, uint8_t Buffer[], uint8_t Length)
{
    uint8_t i = 0;
    for (i = 0; i < Length; i++) {
        Buffer[i] = REG8(USB_BASE + REG_EP0_FIFO);
    }
    return Buffer;
}

void musb_load_testpacket()
{

    musb_write_fifo(EP0, musb_test_packet, 53);
    REG8(USB_BASE + REG_TESTMODE) = MUSB_TEST_PACKET | MUSB_TEST_FIFO_ACCESS | MUSB_TEST_FORCE_FS;
    REG8(USB_BASE + REG_CSR0L)    = MUSB_CSR0_TXPKTRDY;
}


 void musb_set_host()
 {
    uint8_t devctl;
    devctl = REG8(USB_BASE + REG_DEVCTL);
    if (!(devctl & MUSB_DEVCTL_BDEVICE)) {
       debug("already in host mode\n");
    }
    devctl |= MUSB_DEVCTL_SESSION;
    REG8(USB_BASE + REG_DEVCTL) = devctl;
 }

 void musb_h_tx_start(uint8_t EP)
{
    uint16_t txcsr;

    /* NOTE: no locks here; caller should lock and select EP */
    if (EP) {
        txcsr = REG16(CONFIG_EP(EP) + MUSB_TXCSR);
        txcsr |= MUSB_TXCSR_TXPKTRDY | MUSB_TXCSR_H_WZC_BITS;
        REG16(CONFIG_EP(EP) + MUSB_TXCSR) = txcsr;
    } else {
        txcsr                            = MUSB_CSR0_H_SETUPPKT | MUSB_CSR0_TXPKTRDY;
        REG16(CONFIG_EP(EP) + MUSB_CSR0) = txcsr;
    }
}

