//
// Created by loulfy on 31/01/2021.
//

#include "mt_sys.h"
#include "nvram.h"

mt_frame znp_sys_nv_write(uint16_t id, uint8_t value)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_SYS;
    req->cmd1 = SYS_OSAL_NV_WRITE;
    req->data[0] = LO_UINT16(id);
    req->data[1] = HI_UINT16(id);
    req->data[2] = 0;
    req->data[3] = 1;
    req->data[4] = value;
    req->size = 5;
    return req;
}

mt_frame znp_sys_reset(uint8_t type)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_AREQ | MT_SYS;
    req->cmd1 = SYS_RESET_REQ;
    req->data[0] = type;
    req->size = 1;
    req->cb = 0x4180;
    return req;
}

mt_frame znp_sys_get_extaddr()
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_SYS;
    req->cmd1 = SYS_GET_EXTADDR;
    req->size = 0;
    req->cb = 0x6104;
    return req;
}

mt_frame znp_nv_startup(uint8_t option)
{
    return znp_sys_nv_write(ZCD_NV_STARTUP_OPTION, option);
}

mt_frame znp_nv_callback(uint8_t enable)
{
    return znp_sys_nv_write(ZCD_NV_ZDO_DIRECT_CB, enable);
}
