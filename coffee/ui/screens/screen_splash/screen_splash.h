// ui/screens/screen_splash/screen_splash.h
#ifndef SCREEN_SPLASH_H
#define SCREEN_SPLASH_H

#include "lvgl/lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

extern lv_obj_t *splash_screen; // 声明屏幕对象

void screen_splash_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // SCREEN_SPLASH_H