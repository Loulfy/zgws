//
// Created by loulfy on 31/01/2021.
//

#include "mt_af.h"

mt_frame znp_af_register()
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_AF;
    req->cmd1 = AF_REGISTER;
    req->data[0] = 1;
    req->data[1] = 0x04;
    req->data[2] = 0x01;
    req->data[3] = 0x50;
    req->data[4] = 0x00;
    req->data[5] = 1;
    req->data[6] = 0;
    req->data[7] = 1;
    req->data[8] = 0x06;
    req->data[9] = 0x00;
    req->data[10] = 0;
    req->size = 11;
    return req;
}

mt_frame znp_af_data(struct mtDataRequestFormat_t* payload)
{
    mt_frame req = mt_create_frame();
    req->cmd0 = MT_SREQ | MT_AF;
    req->cmd1 = AF_DATA_REQUEST;
    req->data[0] = LO_UINT16(payload->DstAddr);
    req->data[1] = HI_UINT16(payload->DstAddr);
    req->data[2] = payload->DstEndpoint;
    req->data[3] = payload->SrcEndpoint;
    req->data[4] = LO_UINT16(payload->ClusterID);
    req->data[5] = HI_UINT16(payload->ClusterID);
    req->data[6] = payload->TransID;
    req->data[7] = payload->Options;
    req->data[8] = payload->Radius;
    req->data[9] = payload->Len;
    memcpy(req->data + 10, payload->Data, payload->Len);
    req->size = 10 + payload->Len;
    return req;
}
