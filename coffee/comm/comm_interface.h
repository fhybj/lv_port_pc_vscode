// comm/comm_interface.h
#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void comm_init(void);
void comm_send_remote_status(const char *status);
void comm_send_serial_data(const char *data);

// 模拟接收
void comm_simulate_remote_command(const char *cmd);
void comm_simulate_serial_receive(const char *data);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // COMM_INTERFACE_H