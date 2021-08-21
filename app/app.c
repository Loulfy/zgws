//
// Created by loulfy on 14/02/2021.
//

#include "app.h"

struct promise_t* app_new_promise(uint16_t cmd, bool keep)
{
    struct promise_t* p = malloc(sizeof(struct promise_t));
    p->next = NULL;
    p->keep = keep;
    p->cmd = cmd;
    p->f = NULL;
    return p;
}

void app_exec_req(zgw_app app, mt_frame req, struct mg_connection* c, ev_cb_t cb, bool keep)
{
    mg_mt_send(app->dev, req);
    struct promise_t* p = app_new_promise(req->cb, keep);
    p->f = cb;
    p->c = c;
    app_subscribe(app, p);
    mt_destroy_frame(req);
}

void app_init(struct app_t* app)
{
    //for(int i = 0; i < 10; ++i) app->then[i] = NULL;
    app->then = NULL;
}

int app_get_bin(struct app_t* app, uint16_t cmd)
{
    switch (cmd)
    {
        default:
            return -1;
        case 0x4180:
            return 0;
        case 0x4584:
            return 1;
    }
}

int appn(struct app_t* app)
{
    int n = 0;
    //int i = app_get_bin(app, cmd);
    for (struct promise_t* p = app->then; p != NULL; p = p->next)
    {
        n+= 1;
    }
    return n;
}

void app_subscribe(struct app_t* app, struct promise_t* then)
{
    then->next = app->then;
    app->then = then;
}

void app_publish(struct app_t* app, mt_frame rsp)
{
    uint16_t cmd = (rsp->cmd0 << 8) | rsp->cmd1;
    struct promise_t *next, **h = &app->then;
    while(*h != NULL)
    {
        if((*h)->cmd == cmd && (*h)->f != NULL)
        {
            (*h)->f(rsp, (*h)->c);
            if((*h)->keep == 0)
            {
                next = (*h)->next;
                free(*h);
                *h = next;
                continue;
            }
        }

        h = &(*h)->next;
    }
}