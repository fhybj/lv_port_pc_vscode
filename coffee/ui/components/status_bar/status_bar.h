// ui/components/status_bar/status_bar.h
#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

void status_bar_init(void);
void status_bar_show(void);
void status_bar_hide(void);
void status_bar_update_wifi_status(bool connected);
void status_bar_update_battery_level(int percentage); // 模拟

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // STATUS_BAR_H