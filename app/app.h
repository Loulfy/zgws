#ifndef MONGOOSE_UART_APP_H
#define MONGOOSE_UART_APP_H

#include <json-c/json.h>

#include "../mongoose.h"
#include "../mt_frame.h"

#include "../mt_uart.h"
#include "../znp/mt_sys.h"
#include "../znp/mt_zdo.h"
#include "../znp/mt_af.h"

typedef void (*ev_cb_t)(mt_frame rsp, struct mg_connection* src);

struct promise_t
{
	bool keep;
	struct mg_connection* c;
	ev_cb_t f;
	uint16_t cmd;
	struct promise_t* next;
};

struct app_t
{
    struct mg_mgr* mgr;
    struct promise_t* then;
    struct mg_connection* dev;
};

typedef struct app_t* zgw_app;

struct promise_t* app_new_promise(uint16_t cmd, bool keep);
inline void app_exec_req(zgw_app app, mt_frame req, struct mg_connection* c, ev_cb_t cb, bool keep);

void app_init(struct app_t* app);
void app_subscribe(struct app_t* app, struct promise_t* then);
void app_publish(struct app_t* app, mt_frame rsp);

#endif //MONGOOSE_UART_APP_H
