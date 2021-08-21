//
// Created by loulfy on 31/01/2021.
//

#include "mt_zdo.h"

mt_frame znp_zdo_startup(uint16_t startDelay)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_ZDO;
    req->cmd1 = ZDO_STARTUP_FROM_APP;
    req->data[0] = LO_UINT16(startDelay);
    req->data[1] = HI_UINT16(startDelay);
    req->size = 2;
    return req;
}

mt_frame znp_zdo_simple_desc(uint16_t srcAddr, uint16_t dstAddr, uint8_t ep)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_ZDO;
    req->cmd1 = ZDO_SIMPLE_DESC_REQ;
    req->data[0] = LO_UINT16(srcAddr);
    req->data[1] = HI_UINT16(srcAddr);
    req->data[2] = LO_UINT16(dstAddr);
    req->data[3] = HI_UINT16(dstAddr);
    req->data[4] = ep;
    req->size = 5;
    req->cb = 0x4584;
    return req;
}

mt_frame znp_zdo_mgmt_permit(uint16_t dstAddr, uint8_t duration, uint8_t tc)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_ZDO;
    req->cmd1 = ZDO_MGMT_PERMIT_JOIN_REQ;
    req->data[0] = LO_UINT16(dstAddr);
    req->data[1] = HI_UINT16(dstAddr);
    req->data[2] = duration;
    req->data[3] = tc;
    req->size = 4;
    return req;
}

mt_frame znp_zdo_mgmt_leave(uint16_t dstAddr, uint8_t ieee[8])
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_ZDO;
    req->cmd1 = ZDO_MGMT_LEAVE_REQ;
    req->data[0] = LO_UINT16(dstAddr);
    req->data[1] = HI_UINT16(dstAddr);
    memcpy(req->data + 2, ieee, 8);
    req->data[10] = 0;
    req->size = 11;
    return req;
}

mt_frame znp_zdo_mgmt_lqi(uint16_t dstAddr, uint8_t startIndex)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_ZDO;
    req->cmd1 = ZDO_MGMT_LQI_REQ;
    req->data[0] = LO_UINT16(dstAddr);
    req->data[1] = HI_UINT16(dstAddr);
    req->data[2] = startIndex;
    req->size = 3;
    return req;
}

mt_frame znp_zdo_mgmt_bind(uint16_t dstAddr, uint8_t startIndex)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_ZDO;
    req->cmd1 = ZDO_MGMT_BIND_REQ;
    req->data[0] = LO_UINT16(dstAddr);
    req->data[1] = HI_UINT16(dstAddr);
    req->data[2] = startIndex;
    req->size = 3;
    return req;
}
