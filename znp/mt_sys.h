//
// Created by loulfy on 31/01/2021.
//

#ifndef MONGOOSE_UART_MT_SYS_H
#define MONGOOSE_UART_MT_SYS_H

#include "../mt_uart.h"
#include "../mt_frame.h"

/* AREQ from host */
#define SYS_RESET_REQ           0x00

/* SREQ & SRSP */
#define SYS_PING                0x01
#define SYS_VERSION             0x02
#define SYS_SET_EXTADDR         0x03
#define SYS_GET_EXTADDR         0x04
#define SYS_RAM_READ            0x05
#define SYS_RAM_WRITE           0x06
#define SYS_OSAL_NV_ITEM_INIT   0x07
#define SYS_OSAL_NV_READ        0x08
#define SYS_OSAL_NV_WRITE       0x09
#define SYS_OSAL_START_TIMER    0x0A
#define SYS_OSAL_STOP_TIMER     0x0B
#define SYS_RANDOM              0x0C
#define SYS_ADC_READ            0x0D
#define SYS_GPIO                0x0E
#define SYS_STACK_TUNE          0x0F
#define SYS_SET_TIME            0x10
#define SYS_GET_TIME            0x11
#define SYS_OSAL_NV_DELETE      0x12
#define SYS_OSAL_NV_LENGTH      0x13
#define SYS_SET_TX_POWER        0x14

/* AREQ to host */
#define SYS_RESET_IND           0x80
#define SYS_OSAL_TIMER_EXPIRED  0x81

mt_frame znp_sys_nv_write(uint16_t id, uint8_t value); // Fixed offset & length
mt_frame znp_sys_reset(uint8_t type);
mt_frame znp_sys_get_extaddr();

mt_frame znp_nv_startup(uint8_t option);
mt_frame znp_nv_callback(uint8_t enable);

#endif //MONGOOSE_UART_MT_SYS_H
