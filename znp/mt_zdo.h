//
// Created by loulfy on 31/01/2021.
//

#ifndef MONGOOSE_UART_MT_ZDO_H
#define MONGOOSE_UART_MT_ZDO_H

#include "../mt_uart.h"
#include "../mt_frame.h"

/* SREQ & SRSP */
#define ZDO_NWK_ADDR_REQ            0x00
#define ZDO_IEEE_ADDR_REQ           0x01
#define ZDO_NODE_DESC_REQ           0x02
#define ZDO_POWER_DESC_REQ          0x03
#define ZDO_SIMPLE_DESC_REQ         0x04
#define ZDO_ACTIVE_EP_REQ           0x05
#define ZDO_MATCH_DESC_REQ          0x06
#define ZDO_COMPLEX_DESC_REQ        0x07
#define ZDO_USER_DESC_REQ           0x08
#define ZDO_DEVICE_ANNCE            0x0A
#define ZDO_USER_DESC_SET           0x0B
#define ZDO_SERVER_DISC_REQ         0x0C
#define ZDO_END_DEVICE_BIND_REQ     0x20
#define ZDO_BIND_REQ                0x21
#define ZDO_UNBIND_REQ              0x22

#define ZDO_SET_LINK_KEY            0x23
#define ZDO_REMOVE_LINK_KEY         0x24
#define ZDO_GET_LINK_KEY            0x25
#define ZDO_NWK_DISCOVERY_REQ       0x26
#define ZDO_JOIN_REQ                0x27

#define ZDO_MGMT_NWK_DISC_REQ       0x30
#define ZDO_MGMT_LQI_REQ            0x31
#define ZDO_MGMT_RTG_REQ            0x32
#define ZDO_MGMT_BIND_REQ           0x33
#define ZDO_MGMT_LEAVE_REQ          0x34
#define ZDO_MGMT_DIRECT_JOIN_REQ    0x35
#define ZDO_MGMT_PERMIT_JOIN_REQ    0x36
#define ZDO_MGMT_NWK_UPDATE_REQ     0x37

/* AREQ optional, but no AREQ response. */
#define ZDO_MSG_CB_REGISTER         0x3E
#define ZDO_MSG_CB_REMOVE           0x3F
#define ZDO_STARTUP_FROM_APP        0x40

/* AREQ from host */
#define ZDO_AUTO_FIND_DESTINATION   0x41

mt_frame znp_zdo_startup(uint16_t startDelay);
mt_frame znp_zdo_simple_desc(uint16_t srcAddr, uint16_t dstAddr, uint8_t ep);
mt_frame znp_zdo_mgmt_permit(uint16_t dstAddr, uint8_t duration, uint8_t tc);
mt_frame znp_zdo_mgmt_leave(uint16_t dstAddr, uint8_t ieee[8]);
mt_frame znp_zdo_mgmt_lqi(uint16_t dstAddr, uint8_t startIndex);
mt_frame znp_zdo_mgmt_bind(uint16_t dstAddr, uint8_t startIndex);

#endif //MONGOOSE_UART_MT_ZDO_H
