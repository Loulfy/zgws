//
// Created by loulfy on 31/01/2021.
//

#ifndef MONGOOSE_UART_MT_FRAME_H
#define MONGOOSE_UART_MT_FRAME_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// Utilities MACRO
#define HI_UINT16(a) (((a) >> 8) & 0xFF)
#define LO_UINT16(a) ((a) & 0xFF)
#define BREAK_UINT32(var, ByteNum) (uint8_t)((uint32_t)(((var)>>((ByteNum) * 8)) & 0x00FF))

// SOF (Start of Frame) indicator byte byte
#define MT_SOF           (0xFE)
#define MT_HDR_LEN       (4)
#define MT_FCS_LEN       (1)

// The 3 MSB's of the 1st command field byte (Cmd0) are for command type
#define MT_CMD_TYPE_MASK (0xE0)

// The 5 LSB's of the 1st command field byte (Cmd0) are for the subsystem
#define MT_SUB_TYPE_MASK (0x1F)

// Cmd0 Command Type
typedef enum {
    MT_POLL = 0x00,
    MT_SREQ = 0x20,
    MT_AREQ = 0x40,
    MT_SRSP = 0x60
} mtCmdType_t;

// Cmd0 Command Subsystem
typedef enum
{
    MT_RES0,   // RPC Error interface
    MT_SYS,    // SYS interface
    MT_MAC,
    MT_NWK,
    MT_AF,     // AF interface
    MT_ZDO,    // ZDO interface
    MT_SAPI,   // Simple API interface
    MT_UTIL,   // UTIL interface
    MT_DBG,
    MT_APP,    // APP interface
    MT_OTA,
    MT_ZNP,
} mtSubType_t;

// Error codes in Attribute byte of SRSP packet
typedef enum
{
    MT_SUCCESS = 0,         // success
    MT_ERR_SUBSYSTEM = 1,   // invalid subsystem
    MT_ERR_COMMAND_ID = 2,  // invalid command ID
    MT_ERR_PARAMETER = 3,   // invalid parameter
    MT_ERR_LENGTH = 4       // invalid length
} mtErrorCode_t;

struct mt_frame_t
{
    uint8_t cmd0;
    uint8_t cmd1;
    uint8_t data[250];
    uint8_t size;
    struct mt_frame_t* next;
    uint16_t cb;
};

typedef struct mt_frame_t* mt_frame;

mt_frame mt_create_frame();
void mt_destroy_frame(mt_frame frame);
void mt_display_frame(mt_frame frame);

void mt_list_add_tail(mt_frame head, mt_frame next);
void mt_list_pop_head(mt_frame head, mt_frame cursor);

#endif //MONGOOSE_UART_MT_FRAME_H
