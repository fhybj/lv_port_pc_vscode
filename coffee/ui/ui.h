// ui/ui.h
#ifndef UI_H
#define UI_H

#include "lvgl/lvgl.h"
#include "app/app_core.h" // 引入参数类型

#ifdef __cplusplus
extern "C" {
#endif

void ui_init(void);
void ui_load_splash_screen(void);
void ui_load_main_screen(void);
void ui_load_making_screen(app_coffee_type_t coffee_type);
void ui_load_info_screen(app_info_code_t info_code, app_info_severity_t severity);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // UI_H