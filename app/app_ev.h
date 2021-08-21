#ifndef MONGOOSE_UART_APP_EV_H
#define MONGOOSE_UART_APP_EV_H

#include "app.h"

void ev_cb_desc_ws(mt_frame rsp, struct mg_connection* src);
void ev_cb_who_ws(mt_frame rsp, struct mg_connection* src);

#endif //MONGOOSE_UART_APP_EV_H
