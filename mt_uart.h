//
// Created by loulfy on 19/01/2021.
//

#ifndef MONGOOSE_UART_H
#define MONGOOSE_UART_H

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

#include "mongoose.h"
#include "mt_frame.h"

struct mg_connection* mg_mt_listen(struct mg_mgr* mgr, mg_event_handler_t fn, char* port, void* fn_data);
uint8_t mg_mt_send(struct mg_connection* c, mt_frame req);
int mg_mt_parse(uint8_t* buf, size_t len, mt_frame msg);

#endif //MONGOOSE_UART_H
