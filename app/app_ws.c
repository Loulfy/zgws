//
// Created by loulfy on 07/03/2021.
//

#include "app_ws.h"

void app_ws_process(zgw_app app, struct mg_ws_message* wm, struct mg_connection* c)
{
    if(wm->flags & WEBSOCKET_OP_TEXT)
    {
        json_tokener* tok = json_tokener_new();
        json_object* body = json_tokener_parse_ex(tok, wm->data.ptr, wm->data.len);
        printf("RCV WS %zu\n", wm->data.len);
        if(json_tokener_get_error(tok) == json_tokener_success)
        {
            mt_frame req;
            json_object *cmd, *addr;
            json_object_object_get_ex(body, "op", &cmd);
            switch(json_object_get_int(cmd))
            {
                default:
                case 0:
                    json_object_object_get_ex(body, "addr", &addr);
                    uint16_t nwkAddr = strtoul(json_object_get_string(addr), NULL, 16);
                    req = znp_zdo_simple_desc(nwkAddr, nwkAddr, 1);
                    app_exec_req(app, req, c, ev_cb_desc_ws, false);
                    break;
                case 3:
                    req = znp_sys_get_extaddr();
                    app_exec_req(app, req, c, ev_cb_who_ws, false);
                    break;
            }
        }
        json_object_put(body);
        json_tokener_free(tok);
    }
}