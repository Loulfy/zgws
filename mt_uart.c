//
// Created by loulfy on 19/01/2021.
//

#include "mt_uart.h"

static void mt_cb(struct mg_connection *c, int ev, void *ev_data, void *fn_data)
{
    if (ev == MG_EV_CLOSE)
    {
        printf("Closed : hard reset !\n");
    }
    else if (ev == MG_EV_READ)
    {
        //printf("Res : [ ");
        //for(size_t i = 0; i < c->recv.len; i++) printf("%02X ", c->recv.buf[i]);
        //printf("]\n");

        struct mt_frame_t msg;
        int rc = mg_mt_parse(c->recv.buf, c->recv.len, &msg);
        if(rc > 0)
        {
            switch (msg.cmd0 & MT_CMD_TYPE_MASK)
            {
                default:
                case MT_SRSP:
                    mt_display_frame(&msg);
                    mg_call(c, MG_EV_MT_SRSP, &msg);
                    break;
                case MT_AREQ:
                    mg_call(c, MG_EV_MT_AREQ, &msg);
                    break;
            }
            mg_iobuf_delete(&c->recv, rc);
        }
    }
}

struct mg_connection* mg_mt_listen(struct mg_mgr* mgr, mg_event_handler_t fn, char* port, void* fn_data)
{
    struct mg_connection* c = NULL;
    struct termios tio;

    /* open the device */
    int fd = open(port, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror(port);
        LOG(LL_ERROR, ("open failed %s", port));
    }
    else if ((c = calloc(1, sizeof(struct mg_connection))) == NULL)
    {
        LOG(LL_ERROR, ("OOM"));
    }
    else
    {
        c->fd = (void *) (long) fd;
        c->fn = fn;

        c->is_tty = 1;
        c->pfn = mt_cb;
        c->pfn_data = mgr;
        c->fn_data = fn_data;

        tio.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
        tio.c_cflag |= CRTSCTS;
        tio.c_iflag = IGNPAR & ~ICRNL;
        tio.c_oflag = 0;
        tio.c_lflag = 0;
        //Make it non blocking
        tio.c_cc[VMIN] = 0;
        tio.c_cc[VTIME] = 0;

        tcflush(fd, TCIFLUSH);
        tcsetattr(fd, TCSANOW, &tio);

        LIST_ADD_HEAD(struct mg_connection, &mgr->conns, c);
        return c;
    }
    return NULL;
}

static uint8_t fcs(uint8_t* msg, uint8_t size)
{
    uint8_t result = 0;
    // calculate FCS by XORing all bytes
    while (size--) result ^= *msg++;
    return result;
}

uint8_t mg_mt_send(struct mg_connection* c, mt_frame req)
{
    uint8_t buf[256];
    int32_t status = MT_SUCCESS;

    // fill in header bytes
    buf[0] = MT_SOF;
    buf[1] = req->size;
    buf[2] = req->cmd0;
    buf[3] = req->cmd1;

    if (req->size > 0)
    {
        // copy payload to buffer
        memcpy(buf + MT_HDR_LEN, req->data, req->size);
    }

    // calculate FCS field
    buf[MT_HDR_LEN + req->size] = fcs(&buf[1], MT_HDR_LEN + req->size - 1);

    // Send
    mg_send(c, buf, MT_HDR_LEN + req->size + MT_FCS_LEN);
    mt_display_frame(req);
    //printf("Req : [ ");
    //for(size_t i = 0; i < MT_HDR_LEN + req->size + MT_FCS_LEN; i++) printf("%02X ", buf[i]);
    //printf("]\n");

    return status;
}

int mg_mt_parse(uint8_t* buf, size_t len, mt_frame msg)
{
    if(len < 6) return -1;
    if(buf[0] == MT_SOF)
    {
        if(MT_HDR_LEN + buf[1] + MT_FCS_LEN <= len)
        {
            uint8_t idx_fcs = MT_HDR_LEN + buf[1];
            if(fcs(&buf[1], idx_fcs - 1) == buf[idx_fcs])
            {
                //printf("FCS OK !\n");
                uint8_t type = buf[2] & MT_CMD_TYPE_MASK;
                if (type == MT_SRSP || type == MT_AREQ)
                {
                    msg->size = buf[1];
                    msg->cmd0 = buf[2];
                    msg->cmd1 = buf[3];
                    memcpy(msg->data, &buf[4], msg->size);
                    return MT_HDR_LEN + buf[1] + MT_FCS_LEN;
                }
            }
        }
    }
    return -1;
}