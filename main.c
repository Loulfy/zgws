#include "mongoose.h"
#include "mt_uart.h"
#include "znp/mt_sys.h"
#include "znp/mt_zdo.h"
#include "znp/mt_af.h"
#include "zcl/zcl_id.h"

#include "app/app.h"
#include "app/app_ws.h"

#include <json-c/json.h>

static const char *s_listen_on = "http://localhost:8000";
static const char *s_web_directory = WWW;

static const char *s_url = "mqtt://server:1883";
static const char *s_topic = "vac/relay/r1";

static mt_frame head, tail;
static struct mg_connection* device;

void toggleSwitch(uint16_t addr, uint8_t cmd)
{
    struct mtDataRequestFormat_t req;
    req.DstAddr = addr;
    req.DstEndpoint = 1;
    req.SrcEndpoint = 1;
    req.ClusterID = 6;
    req.TransID = 5;
    req.Options = 0;
    req.Radius = 0xEE;
    req.Data[2] = cmd;
    req.Data[1] = 0x0a;
    req.Data[0] = 0x11;
    req.Len = 3;

    mt_frame msg = znp_af_data(&req);
    mg_mt_send(device, msg);
    mt_destroy_frame(msg);
}

void deviceName(uint16_t addr)
{
    struct mtDataRequestFormat_t req;
    req.DstAddr = addr;
    req.DstEndpoint = 1;
    req.SrcEndpoint = 1;
    req.ClusterID = 0;
    req.TransID = 5;
    req.Options = 0;
    req.Radius = 0xEE;
    req.Data[4] = 0x00;
    req.Data[3] = 0x04;
    req.Data[2] = 0x00;
    req.Data[1] = 0x00;
    req.Data[0] = 0x00;
    req.Len = 5;

    mt_frame msg = znp_af_data(&req);
    mg_mt_send(device, msg);
    mt_destroy_frame(msg);
}

void readReporting(uint16_t addr)
{
    struct mtDataRequestFormat_t req;
    req.DstAddr = addr;
    req.DstEndpoint = 1;
    req.SrcEndpoint = 1;
    req.ClusterID = 6;
    req.TransID = 5;
    req.Options = 0;
    req.Radius = 0xEE;
    req.Data[5] = 0x00;
    req.Data[4] = 0x00;
    req.Data[3] = 0x00;
    req.Data[2] = 0x08;
    req.Data[1] = 0x00;
    req.Data[0] = 0x00;
    req.Len = 6;

    mt_frame msg = znp_af_data(&req);
    mg_mt_send(device, msg);
    mt_destroy_frame(msg);
}

void writeReporting(uint16_t addr)
{
    struct mtDataRequestFormat_t req;
    req.DstAddr = addr;
    req.DstEndpoint = 1;
    req.SrcEndpoint = 1;
    req.ClusterID = 6;
    req.TransID = 5;
    req.Options = 0;
    req.Radius = 0xEE; // 10 00 00 10 0E
    req.Data[10]= 0x0E;
    req.Data[9] = 0x10;
    req.Data[8] = 0x00;
    req.Data[7] = 0x00;
    req.Data[6] = 0x10;
    
    req.Data[5] = 0x00;
    req.Data[4] = 0x00;
    req.Data[3] = 0x00;
    req.Data[2] = 0x06;
    req.Data[1] = 0x00;
    req.Data[0] = 0x00;
    req.Len = 11;

    mt_frame msg = znp_af_data(&req);
    mg_mt_send(device, msg);
    mt_destroy_frame(msg);
}

void cb_desc(mt_frame rsp, struct mg_connection* src)
{
    json_object* main = json_object_new_object();

    //printf("DESC ");
    //mt_display_frame(cmd);
    uint16_t devId = rsp->data[9];
    devId |= rsp->data[10] << 8;
    //printf("Device type : %s\n", zcl_device_str(devId));

    json_object_object_add(main, "deviceId", json_object_new_string(zcl_device_str(devId)));

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
        //printf("ClusterIn[%d]:%s\n", i, zcl_cluster_str(devId));
        json_object_array_add(in, json_object_new_string(zcl_cluster_str(devId)));
    }
    json_object_object_add(main, "clusterIn", in);

    json_object* out = json_object_new_array();
    for(i = 0; i < outN*2; i+=2)
    {
        devId = rsp->data[outX + 1 + i];
        devId |= rsp->data[outX + 2 + i] << 8;
        //printf("ClusterOut[%d]:%s\n", i, zcl_cluster_str(devId));
        json_object_array_add(out, json_object_new_string(zcl_cluster_str(devId)));
    }
    json_object_object_add(main, "clusterOut", out);
    mg_http_reply(src, 200, "%s\n", json_object_to_json_string(main));
    json_object_put(main);
}

// This RESTful server implements the following endpoints:
//   /websocket - upgrade to Websocket, and implement websocket echo server
//   /api/rest - respond with JSON string {"result": 123}
//   any other URI serves static files from s_web_directory
static void http_fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        if (mg_http_match_uri(hm, "/gateway")) {
            // Upgrade to websocket. From now on, a connection is a full-duplex
            // Websocket connection, which will receive MG_EV_WS_MSG events.
            mg_ws_upgrade(c, hm);
        } else if (mg_http_match_uri(hm, "/toggle")) {
            // Serve REST response
            json_object *addr, *body = json_tokener_parse(hm->body.ptr);
            json_object_object_get_ex(body, "addr", &addr);
            uint16_t nwkAddr = strtoul(json_object_get_string(addr), NULL, 16);
            toggleSwitch(nwkAddr, 0x02);
            mg_http_reply(c, 200, "{\"result\": %d}\n", 0);
        } else if (mg_http_match_uri(hm, "/toggle/9d3d")) {
            toggleSwitch(0x9d3d, 0x02);
            mg_http_reply(c, 200, "{\"result\": %d}\n", 0);
        } else if (mg_http_match_uri(hm, "/list")) {
            json_object *addr, *body = json_tokener_parse(hm->body.ptr);
            json_object_object_get_ex(body, "addr", &addr);
            uint16_t nwkAddr = strtoul(json_object_get_string(addr), NULL, 16);
            mt_frame req = znp_zdo_mgmt_lqi(nwkAddr, 0);
            mg_mt_send(device, req);
            mt_destroy_frame(req);
            mg_http_reply(c, 200, "{\"result\": %d}\n", 0);
        } else if (mg_http_match_uri(hm, "/permit")) {
            mt_frame req = znp_zdo_mgmt_permit(0x0000, 0x1E, 1);
            mg_mt_send(device, req);
            mt_destroy_frame(req);
            mg_http_reply(c, 200, "{\"result\": %d}\n", 0);
        } else if (mg_http_match_uri(hm, "/desc")) {
            json_object *addr, *body = json_tokener_parse(hm->body.ptr);
            json_object_object_get_ex(body, "addr", &addr);
            uint16_t nwkAddr = strtoul(json_object_get_string(addr), NULL, 16);
            mt_frame req = znp_zdo_simple_desc(nwkAddr, nwkAddr, 1);
            mg_mt_send(device, req);
            json_object_put(body);

            struct app_t* app = (struct app_t*) fn_data;
            struct promise_t* p = app_new_promise(req->cb, false);
            p->f = cb_desc;
            p->c = c;
            app_subscribe(app, p);

            mt_destroy_frame(req);
            //req = znp_zdo_mgmt_bind(nwkAddr, 0);
            //mg_mt_send(device, req);
            //mt_destroy_frame(req);
            //deviceName(nwkAddr);
            //writeReporting(nwkAddr);
            //mg_http_reply(c, 200, "{\"result\": %d}\n", 0);
        } else if (mg_http_match_uri(hm, "/who")) {
            mt_frame req = znp_sys_get_extaddr();
            mg_mt_send(device, req);
            mt_destroy_frame(req);
        } else {
            // Serve static files
            mg_http_serve_dir(c, hm, s_web_directory);
        }
    } else if (ev == MG_EV_WS_MSG) {
        // Got websocket frame.
        struct mg_ws_message* wm = (struct mg_ws_message *) ev_data;
        struct app_t* app = (struct app_t*) fn_data;
        // Process it.
        app_ws_process(app, wm, c);
        // Clean.
        mg_iobuf_delete(&c->recv, c->recv.len);
    }
    (void) fn_data;
}

static void znp_fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    if(ev == MG_EV_MT_SRSP) {
        if(head != NULL)
        {
            mt_frame del = head;
            if(head->next != NULL)
            {
                head = head->next;
                usleep(500);
                mg_mt_send(c, head);
            }
            //mt_destroy_frame(del);
        }
    } else if(ev == MG_EV_MT_AREQ) {
        mt_frame msg = (mt_frame) ev_data;
        zgw_app app = (zgw_app) fn_data;
        app_publish(app, msg);

        if(msg->cmd0 == 0x45 && msg->cmd1 == 0xB1)
        {
            printf("LQI ");
            mt_display_frame(msg);
        }
        else if(msg->cmd0 == 0x44 && msg->cmd1 == 0x81)
        {
            printf("ZCL ");
            mt_display_frame(msg);
        }
        else if(msg->cmd0 == 0x45 && msg->cmd1 == 0x84)
        {
            json_object* main = json_object_new_object();
            
            //printf("DESC ");
            //mt_display_frame(msg);
            uint16_t devId = msg->data[9];
            devId |= msg->data[10] << 8;
            //printf("Device type : %s\n", zcl_device_str(devId));
            
            json_object_object_add(main, "deviceId", json_object_new_string(zcl_device_str(devId)));
            
            uint8_t i, inX, outX, inN, outN;
            inX = 12;
            inN = msg->data[inX];
            outX = inX + 2*inN + 1;
            outN = msg->data[outX];
            
            json_object* in = json_object_new_array();
            for(i = 0; i < inN*2; i+=2)
            {
                devId = msg->data[inX + 1 + i];
                devId |= msg->data[inX + 2 + i] << 8;
                //printf("ClusterIn[%d]:%s\n", i, zcl_cluster_str(devId));
                json_object_array_add(in, json_object_new_string(zcl_cluster_str(devId)));
            }
            json_object_object_add(main, "clusterIn", in);
            
            json_object* out = json_object_new_array();
            for(i = 0; i < outN*2; i+=2)
            {
                devId = msg->data[outX + 1 + i];
                devId |= msg->data[outX + 2 + i] << 8;
                //printf("ClusterOut[%d]:%s\n", i, zcl_cluster_str(devId));
                json_object_array_add(out, json_object_new_string(zcl_cluster_str(devId)));
            }
            json_object_object_add(main, "clusterOut", out);
            
            printf("%s\n", json_object_to_json_string(main));
            json_object_put(main);
        }
        else if(msg->cmd0 == 0x45 && msg->cmd1 == 0xB3)
        {
            printf("BIND ");
            mt_display_frame(msg);
        }
        else if(msg->cmd0 == 0x45 && msg->cmd1 == 0xC0)
        {
            printf("Device State : %d\n", msg->data[0]);
        }
        else if(msg->cmd0 == 0x45 && msg->cmd1 == 0xC4)
        {
            printf("RTG ");
            mt_display_frame(msg);
        }
        else
        {
            printf("UNKNOW %02X %02X ", msg->cmd0, msg->cmd1);
            mt_display_frame(msg);
        }
    }
    (void) ev_data;
    (void) fn_data;
}

static void mqtt_fn(struct mg_connection *c, int ev, void *ev_data, void *fn_data) {
    if (ev == MG_EV_ERROR) {
        // On error, log error message
        LOG(LL_ERROR, ("%p %s", c->fd, (char *) ev_data));
    } else if (ev == MG_EV_CONNECT) {
        // If target URL is SSL/TLS, command client connection to use TLS
        if (mg_url_is_ssl(s_url)) {
            struct mg_tls_opts opts = {.ca = CA};
            mg_tls_init(c, &opts);
        }
    } else if (ev == MG_EV_MQTT_OPEN) {
        // MQTT connect is successful
        struct mg_str topic = mg_str(s_topic);
        LOG(LL_INFO, ("CONNECTED to %s", s_url));
        mg_mqtt_sub(c, &topic);
        LOG(LL_INFO, ("SUBSCRIBED to %.*s", (int) topic.len, topic.ptr));
    } else if (ev == MG_EV_MQTT_MSG) {
        // When we get echo response, print it
        struct mg_mqtt_message *mm = (struct mg_mqtt_message *) ev_data;
        LOG(LL_INFO, ("RECEIVED %.*s <- %.*s", (int) mm->data.len, mm->data.ptr,
                (int) mm->topic.len, mm->topic.ptr));
        if(strncmp("off", mm->data.ptr, mm->data.len) == 0)
        {
            printf("receive OFF\n");
        }
    } else if (ev == MG_EV_CLOSE) {
        printf("MQTT CLOSED\n");
    }
}

int main() {
    struct mg_mgr mgr; // Event manager
    mg_mgr_init(&mgr); // Initialise event manager

    struct app_t app;
    app_init(&app);
    app.mgr = &mgr;

    // Create HTTP listener
    mg_http_listen(&mgr, s_listen_on, http_fn, &app);

    // Create UART listener
    device = mg_mt_listen(&mgr, znp_fn, "/dev/ttyUSB0", &app);
    app.dev = device;

    // Queue commands
    head = znp_nv_startup(0);
    //mt_list_add_tail(head, znp_sys_reset(1));

    mt_list_add_tail(head, znp_af_register());
    mt_list_add_tail(head, znp_zdo_startup(500));
    mt_list_add_tail(head, znp_nv_callback(1));

    mg_mt_send(device, head);

    for (;;) mg_mgr_poll(&mgr, 1000); // Infinite event loop
    mg_mgr_free(&mgr);
    return 0;
}
