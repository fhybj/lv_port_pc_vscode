// ui/screens/screen_info/screen_info.h
#ifndef SCREEN_INFO_H
#define SCREEN_INFO_H

#include "lvgl/lvgl.h"
#include "app/app_core.h"

#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t *info_screen; // 声明屏幕对象

void screen_info_init(void);
void screen_info_update_content(app_info_code_t info_code, app_info_severity_t severity);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // SCREEN_INFO_H