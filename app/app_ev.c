//
// Created by loulfy on 07/03/2021.
//

#include "app_ev.h"

void ev_cb_desc_ws(mt_frame rsp, struct mg_connection* src)
{
    json_object* main = json_object_new_object();

    uint16_t devId = rsp->data[9];
    devId |= rsp->data[10] << 8;

    json_object_object_add(main, "deviceId", json_object_new_int(devId));

    uint8_t i, inX, outX, inN, outN;
    inX = 12;
    inN = rsp->data[inX];
    outX = inX + 2*inN + 1;
    outN = rsp->data[outX];

    json_object* in = json_object_new_array();
    for(i = 0; i < inN*2; i+=2)
    {
        devId = rsp->data[inX + 1 + i];
        devId |= rsp->data[inX + 2 + i] << 8;
        json_object_array_add(in, json_object_new_int(devId));
    }
    json_object_object_add(main, "clusterIn", in);

    json_object* out = json_object_new_array();
    for(i = 0; i < outN*2; i+=2)
    {
        devId = rsp->data[outX + 1 + i];
        devId |= rsp->data[outX + 2 + i] << 8;
        json_object_array_add(out, json_object_new_int(devId));
    }
    json_object_object_add(main, "clusterOut", out);
    //mg_http_reply(src, 200, "%s\n", json_object_to_json_string(main));
    const char* msg = json_object_to_json_string(main);
    mg_ws_send(src, msg, strlen(msg), WEBSOCKET_OP_TEXT);
    json_object_put(main);
}

void ev_cb_who_ws(mt_frame rsp, struct mg_connection* src)
{
    json_object* main = json_object_new_object();
    char addr[24];
    sprintf(addr,"%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", rsp->data[0], rsp->data[1], rsp->data[2], rsp->data[3], rsp->data[4], rsp->data[5], rsp->data[6], rsp->data[7]);
    json_object_object_add(main, "addr", json_object_new_string(addr));
    const char* msg = json_object_to_json_string(main);
    mg_ws_send(src, msg, strlen(msg), WEBSOCKET_OP_TEXT);
    json_object_put(main);
}
