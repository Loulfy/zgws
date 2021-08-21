//
// Created by loulfy on 31/01/2021.
//

#include "mt_frame.h"

mt_frame mt_create_frame()
{
    mt_frame frame = malloc(sizeof(struct mt_frame_t));
    frame->next = NULL;
    frame->cb = 0;
    return frame;
}

void mt_destroy_frame(mt_frame frame)
{
    free(frame);
}

void mt_display_frame(mt_frame frame)
{
    switch (frame->cmd0 & MT_CMD_TYPE_MASK)
    {
        case MT_SREQ:
            printf("SREQ : ");
            break;
        case MT_SRSP:
            printf("SRSP : ");
            break;
        case MT_AREQ:
            printf("AREQ : ");
            break;
        case MT_POLL:
            printf("POLL : ");
            break;
    }

    //printf("%02x %02x\n", frame->cmd0, frame->cmd1);
    printf("[ ");
    for(size_t i = 0; i < frame->size; i++) printf("%02X ", frame->data[i]);
    printf("]\n");
}

void mt_list_add_tail(mt_frame head, mt_frame next)
{
    mt_frame cursor = head;
    while(cursor->next != NULL) cursor = cursor->next;
    cursor->next = next;
}

void mt_list_pop_head(mt_frame head, mt_frame cursor)
{
    if(head != NULL && head->next != NULL)
    {
        cursor = head;
        head = head->next;
    }
}