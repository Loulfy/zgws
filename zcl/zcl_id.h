
#ifndef MONGOOSE_UART_ZCL_ID_H
#define MONGOOSE_UART_ZCL_ID_H

#include <stdint.h>

#define ZCL_DEVICE_SIZE 50
#define ZCL_CLUSTER_SIZE 28

struct dict_t
{
	uint16_t id;
	char* name;
};

char* zcl_device_str(uint16_t devId);
char* zcl_cluster_str(uint16_t id);


#endif //MONGOOSE_UART_ZCL_ID_H
