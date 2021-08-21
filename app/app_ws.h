//
// Created by loulfy on 07/03/2021.
//

#ifndef MONGOOSE_UART_APP_WS_H
#define MONGOOSE_UART_APP_WS_H

#include <json-c/json.h>

#include "app.h"
#include "app_ev.h"

void app_ws_process(zgw_app app, struct mg_ws_message* wm, struct mg_connection* c);

#endif //MONGOOSE_UART_APP_WS_H
