// coffee_machine_ctrl/ui/theme/app_theme.h

#ifndef APP_THEME_H
#define APP_THEME_H

#include "lvgl/lvgl.h"

extern lv_font_t *lv_default_small_font;
extern lv_font_t *lv_default_normal_font;
extern lv_font_t *lv_default_large_font;

// 声明一些独立的 LV_STYLE 样式，用于无法通过 apply_cb 统一设置的特殊情况
extern lv_style_t style_status_bar_bg; // 状态栏背景样式
extern lv_style_t style_default_font;

/**
 * @brief 初始化应用程序的主题。
 * 这个函数将创建自定义主题，并设置其应用回调。
 * 应在 LVGL 初始化后，但在任何UI对象创建之前调用。
 */
void app_theme_init(void);

#endif // APP_THEME_H