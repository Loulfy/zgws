//
// Created by loulfy on 31/01/2021.
//

#ifndef MONGOOSE_UART_MT_AF_H
#define MONGOOSE_UART_MT_AF_H

#include "../mt_uart.h"
#include "../mt_frame.h"

/* SREQ & SRSP */
#define AF_REGISTER                0x00
#define AF_DATA_REQUEST            0x01  /* AREQ optional, but no AREQ response. */
#define AF_DATA_REQUEST_EXT        0x02  /* AREQ optional, but no AREQ response. */
#define AF_DATA_REQUEST_SRC_RTG    0x03

#define AF_INTER_PAN_CTL           0x10
#define AF_DATA_STORE              0x11
#define AF_DATA_RETRIEVE           0x12
#define AF_APSF_CONFIG_SET         0x13

/* AREQ to host */
#define AF_DATA_CONFIRM            0x80
#define AF_INCOMING_MSG            0x81
#define AF_INCOMING_MSG_EXT        0x82
#define AF_REFLECT_ERROR           0x83

struct mtDataRequestFormat_t
{
    uint16_t DstAddr;
    uint8_t DstEndpoint;
    uint8_t SrcEndpoint;
    uint16_t ClusterID;
    uint8_t TransID;
    uint8_t Options;
    uint8_t Radius;
    uint8_t Len;
    uint8_t Data[128];
};

mt_frame znp_af_register();
mt_frame znp_af_data(struct mtDataRequestFormat_t* payload);

#endif //MONGOOSE_UART_MT_AF_H
