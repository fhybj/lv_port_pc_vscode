// status_bar.c
#include "status_bar.h"
#include "lvgl/lvgl.h" // LVGL 核心头文件
#include "device_settings.h"
#include "lang_manager.h"
#include "app_theme.h"

// --- 移除 coffee_strings.h 的依赖 ---

// 解决 LVGL 符号在 V8.4 中可能不存在的问题
#ifndef LV_SYMBOL_WIFI
#define LV_SYMBOL_WIFI "W" // 简单占位符
#endif
#ifndef LV_SYMBOL_BATTERY_FULL
#define LV_SYMBOL_BATTERY_FULL "B" // 简单占位符
#endif
#ifndef LV_SYMBOL_CLOSE
#define LV_SYMBOL_CLOSE "X" // 简单占位符
#endif


static lv_obj_t *status_bar_obj;
static lv_obj_t *wifi_icon;
static lv_obj_t *battery_label;
static lv_obj_t *lang_btn;
static lv_obj_t *language_label;

static void lang_button_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);
    const char* lang = lv_label_get_text(label);
    if (strcmp(lang, "EN") == 0) {
        lang_set_current(LANG_EN);
         lv_label_set_text(language_label, "中文"); // 初始语言文本
    } else {
        lang_set_current(LANG_ZH_CN);
         lv_label_set_text(language_label, "EN"); // 初始语言文本
    }

}
void status_bar_init(void) {
    status_bar_obj = lv_obj_create(lv_layer_top());
    lv_obj_set_size(status_bar_obj, 512, 30); // 状态栏高度
    lv_obj_align(status_bar_obj, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_clear_flag(status_bar_obj, LV_OBJ_FLAG_SCROLLABLE);

    // 深色背景
    lv_obj_set_style_bg_color(status_bar_obj, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(status_bar_obj, LV_OPA_0, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(status_bar_obj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(status_bar_obj, 5, LV_STATE_DEFAULT); // 内边距

    // WiFi 图标
    wifi_icon = lv_label_create(status_bar_obj);
    lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
    // 使用 lv_font_default，这是 LVGL 默认字体，保证存在
    // lv_obj_set_style_text_font(wifi_icon, &lv_font_default, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(wifi_icon, lv_color_make(0x00, 0x80, 0x00), LV_STATE_DEFAULT); // 绿色
    lv_obj_align(wifi_icon, LV_ALIGN_LEFT_MID, 0, 0);

    // 电池标签
    battery_label = lv_label_create(status_bar_obj);
    lv_label_set_text(battery_label, LV_SYMBOL_BATTERY_FULL " 100%"); // 初始文本
    // 使用 lv_font_default，这是 LVGL 默认字体，保证存在
    // lv_obj_set_style_text_font(battery_label, &lv_font_default, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(battery_label, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT); // 白色
    lv_obj_align_to(battery_label, wifi_icon, LV_ALIGN_OUT_RIGHT_MID, 10, 0); // 相对WiFi图标位置

    // 语言切换按钮 (示例)
    lang_btn = lv_btn_create(status_bar_obj);
    lv_obj_set_size(lang_btn, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(lang_btn, LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_set_style_bg_color(lang_btn, lv_color_hex(0x555555), LV_STATE_DEFAULT); // 灰色背景
    lv_obj_set_style_bg_opa(lang_btn, LV_OPA_COVER, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(lang_btn, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(lang_btn, 5, LV_STATE_DEFAULT);
    lv_obj_add_event_cb(lang_btn, lang_button_event_cb, LV_EVENT_CLICKED, NULL);

    static lv_style_t lang_label_style;
    lv_style_init(&lang_label_style);
    lv_style_set_text_font(&lang_label_style, lv_default_small_font);

    language_label = lv_label_create(lang_btn);
    lv_obj_add_style(language_label, &lang_label_style, 0);
    if (device_settings_get_language() == LANG_EN) {
        lv_label_set_text(language_label, "中文"); // 初始语言文本
    } else {
        lv_label_set_text(language_label, "EN"); // 初始语言文本
    }
    // 使用 lv_font_default，这是 LVGL 默认字体，保证存在
    // lv_obj_set_style_text_font(language_label, &lv_font_default, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(language_label, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT); // 白色
    lv_obj_center(language_label); // 居中显示
}

lv_obj_t* status_bar_get_obj(void) {
    return status_bar_obj;
}

void status_bar_update_wifi_status(bool connected) {
    if (wifi_icon) {
        if (connected) {
            lv_label_set_text(wifi_icon, LV_SYMBOL_WIFI);
            lv_obj_set_style_text_color(wifi_icon, lv_color_make(0x00, 0x80, 0x00), LV_STATE_DEFAULT); // 绿色
        } else {
            lv_label_set_text(wifi_icon, LV_SYMBOL_CLOSE); // 使用 LV_SYMBOL_CLOSE 作为断开连接图标
            lv_obj_set_style_text_color(wifi_icon, lv_color_make(0xFF, 0x00, 0x00), LV_STATE_DEFAULT); // 红色
        }
    }
}

void status_bar_update_battery_level(int percentage) {
    if (battery_label) {
        char buf[16];
        // lv_snprintf 在 LVGL v8.4 中通常是可用的，但如果报错，可能需要包含 <stdio.h> 并使用 snprintf
        lv_snprintf(buf, sizeof(buf), "%s %d%%", LV_SYMBOL_BATTERY_FULL, percentage);
        lv_label_set_text(battery_label, buf);

        if (percentage < 20) {
            lv_obj_set_style_text_color(battery_label, lv_color_make(0xFF, 0x00, 0x00), LV_STATE_DEFAULT); // 红色
        } else {
            lv_obj_set_style_text_color(battery_label, lv_color_hex(0xFFFFFF), LV_STATE_DEFAULT); // 白色
        }
    }
}

void status_bar_set_language_text(const char* text) {
    if (language_label && text) {
        lv_label_set_text(language_label, text);
    }
}

// 新增的函数实现
void status_bar_hide(void) {
    if (status_bar_obj) {
        lv_obj_add_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN); // 隐藏对象
    }
}

void status_bar_show(void) {
    if (status_bar_obj) {
        lv_obj_clear_flag(status_bar_obj, LV_OBJ_FLAG_HIDDEN); // 显示对象
    }
}