// ui/screens/screen_making/screen_making.h
#ifndef SCREEN_MAKING_H
#define SCREEN_MAKING_H

#include "lvgl/lvgl.h"
#include "app/app_core.h"
#include "lang/lang_id.h" // For string_id_t

#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t *making_screen; // 声明屏幕对象

void screen_making_init(void);
void screen_making_update_content(app_coffee_type_t coffee_type);
void screen_making_update_progress(int progress, string_id_t status_text_id);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // SCREEN_MAKING_H