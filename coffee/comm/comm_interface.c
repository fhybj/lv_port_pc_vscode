// comm/comm_interface.c
#include <stdlib.h>
#include "comm_interface.h"
#include "lvgl/lvgl.h"
#include "ui/components/status_bar/status_bar.h"
#include "ui/ui.h" // 用于触发UI更新
#include "app/app_core.h" // 用于解析模拟命令
#include "app/coffee_logic.h" // 用于触发咖啡制作

// 模拟接收缓冲区
static char remote_cmd_buffer[64] = {0};
static char serial_rx_buffer[64] = {0};

// 模拟远程控制定时器回调
static void remote_control_sim_timer_cb(lv_timer_t *timer) {
    static int cmd_count = 0;
    if (cmd_count % 3 == 0) {
        comm_simulate_remote_command("MAKE_ESPRESSO");
    } else if (cmd_count % 3 == 1) {
        comm_simulate_remote_command("SHOW_ERROR"); // 模拟远程触发一个错误
    } else {
        comm_simulate_remote_command("QUERY_STATUS");
    }
    cmd_count++;
}

// 模拟串口数据接收定时器回调
static void serial_receive_sim_timer_cb(lv_timer_t *timer) {
    static int sensor_value = 100;
    char data_buf[32];
    lv_snprintf(data_buf, sizeof(data_buf), "SENSOR:%d", sensor_value++);
    comm_simulate_serial_receive(data_buf);
    if (sensor_value > 150) sensor_value = 100; // 循环模拟
}

void comm_init(void) {
    // 实际应用中会初始化Wi-Fi, 蓝牙, 串口等
    // 这里启动模拟定时器
    lv_timer_create(remote_control_sim_timer_cb, 5000, NULL); // 每5秒模拟一个远程命令
    lv_timer_create(serial_receive_sim_timer_cb, 3000, NULL); // 每3秒模拟一个串口数据
    LV_LOG_USER("Communication Module Initialized (Simulated).");
}

void comm_send_remote_status(const char *status) {
    LV_LOG_USER("Remote Send: %s", status);
    // 实际发送数据到远程服务器
}

void comm_send_serial_data(const char *data) {
    LV_LOG_USER("Serial Send: %s", data);
    // 实际发送数据到串口硬件
}

void comm_simulate_remote_command(const char *cmd) {
    LV_LOG_USER("Simulating Remote Command: %s", cmd);
    if (cmd == NULL) return;

    // 模拟命令解析和处理
    if (strcmp(cmd, "MAKE_ESPRESSO") == 0) {
        coffee_logic_start_making(COFFEE_ESPRESSO);
        comm_send_remote_status("Espresso_Making_Started");
    } else if (strcmp(cmd, "MAKE_LATTE") == 0) {
        coffee_logic_start_making(COFFEE_LATTE);
        comm_send_remote_status("Latte_Making_Started");
    } else if (strcmp(cmd, "CANCEL_MAKING") == 0) {
        coffee_logic_cancel_making();
        comm_send_remote_status("Making_Cancelled");
    } else if (strcmp(cmd, "SHOW_ERROR") == 0) {
        ui_load_info_screen(ERROR_GRINDER_JAMMED, SEVERITY_ERROR); // 远程触发错误界面
        comm_send_remote_status("Error_Displayed_Grinder_Jammed");
    } else if (strcmp(cmd, "QUERY_STATUS") == 0) {
        comm_send_remote_status("Status: Idle");
    }
    strncpy(remote_cmd_buffer, cmd, sizeof(remote_cmd_buffer) - 1); // 存储最近的命令
    remote_cmd_buffer[sizeof(remote_cmd_buffer) - 1] = '\0';
}

void comm_simulate_serial_receive(const char *data) {
    LV_LOG_USER("Simulating Serial Data: %s", data);
    if (data == NULL) return;

    // 模拟数据解析和处理
    if (strncmp(data, "SENSOR:", 7) == 0) {
        int value = atoi(data + 7);
        // 可以在这里更新一个内部传感器值，或直接通过UI反映
        // ui_update_sensor_value(value); // 假设UI有直接更新传感器值的接口
        // 暂时只打印到日志
        char log_msg[64];
        lv_snprintf(log_msg, sizeof(log_msg), "Received sensor: %d via serial", value);
        // UI界面没有直接的日志框，这里可以考虑通过status_bar_update_status来模拟显示
        // status_bar_update_status(log_msg);
    }
    strncpy(serial_rx_buffer, data, sizeof(serial_rx_buffer) - 1);
    serial_rx_buffer[sizeof(serial_rx_buffer) - 1] = '\0';
}