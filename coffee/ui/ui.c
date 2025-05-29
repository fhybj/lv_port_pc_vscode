// ui/ui.c
#include "ui.h"
#include "lvgl/lvgl.h"
#include "ui/components/status_bar/status_bar.h"
#include "ui/screens/screen_making/screen_making.h"
#include "ui/screens/screen_info/screen_info.h"
#include "ui/screens/screen_splash/screen_splash.h"
#include "app/coffee_logic.h" // 引入咖啡逻辑以触发制作
#include "lang/lang_manager.h"

// 声明所有屏幕对象 (这些会在各自的screen_init中创建)
// lv_obj_t *splash_screen;
lv_obj_t *main_screen; // 模拟主屏幕
// lv_obj_t *making_screen;
// lv_obj_t *info_screen;

// 模拟主屏幕事件回调
static void main_screen_btn_event_cb(lv_event_t *e) {
    lv_obj_t *btn = lv_event_get_target(e);
    if (btn == lv_obj_get_child(main_screen, 1)) { // 第一个按钮
        LV_LOG_USER("Main Screen: Making Espresso");
        coffee_logic_start_making(COFFEE_ESPRESSO);
    } else if (btn == lv_obj_get_child(main_screen, 2)) { // 第二个按钮
        LV_LOG_USER("Main Screen: Show Info (Water Tank Empty)");
        ui_load_info_screen(ERROR_WATER_TANK_EMPTY, SEVERITY_ERROR);
    } else if (btn == lv_obj_get_child(main_screen, 3)) { // 第三个按钮
        LV_LOG_USER("Main Screen: Show Info (Preheating)");
        ui_load_info_screen(INFO_PREHEATING, SEVERITY_INFO);
    } else if (btn == lv_obj_get_child(main_screen, 4)) { // 第四个按钮
        LV_LOG_USER("Main Screen: Making Latte");
        coffee_logic_start_making(COFFEE_LATTE);
    }
}


void ui_init(void) {
    // 初始化各个屏幕
    screen_splash_init();
    screen_making_init();
    screen_info_init();

    // 模拟主屏幕 (简化版)
    main_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(main_screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label_main = lv_label_create(main_screen);
    lv_label_set_text(label_main, "Main Screen - Choose Coffee");
    lv_obj_align(label_main, LV_ALIGN_TOP_MID, 0, 30);

    //  static lv_style_t label_main_style;
    // lv_style_init(&label_main_style);
    // lv_style_set_text_font(&label_main_style, lv_default_large_font);
    // lv_obj_add_style(label_main, &label_main_style, 0);

    lv_obj_t *btn1 = lv_btn_create(main_screen);
    lv_obj_set_size(btn1, 150, 50);
    lv_obj_align_to(btn1, label_main, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_obj_add_event_cb(btn1, main_screen_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_btn1 = CREATE_TRANSLATED_LABEL(btn1, STR_ESPRESSO);//lv_label_create(btn1);
    // lv_label_set_text(label_btn1, "Espresso");
    lv_obj_center(label_btn1);

    lv_obj_t *btn2 = lv_btn_create(main_screen);
    lv_obj_set_size(btn2, 150, 50);
    lv_obj_align_to(btn2, btn1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_obj_add_event_cb(btn2, main_screen_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_btn2 = lv_label_create(btn2);
    lv_label_set_text(label_btn2, "Show Error");
    lv_obj_center(label_btn2);

    lv_obj_t *btn3 = lv_btn_create(main_screen);
    lv_obj_set_size(btn3, 150, 50);
    lv_obj_align_to(btn3, btn1, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 20);
    lv_obj_add_event_cb(btn3, main_screen_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_btn3 = lv_label_create(btn3);
    lv_label_set_text(label_btn3, "Show Info");
    lv_obj_center(label_btn3);

    lv_obj_t *btn4 = lv_btn_create(main_screen);
    lv_obj_set_size(btn4, 150, 50);
    lv_obj_align_to(btn4, btn3, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_obj_add_event_cb(btn4, main_screen_btn_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label_btn4 = CREATE_TRANSLATED_LABEL(btn4, STR_LATTE);// lv_label_create(btn4);
    // lv_label_set_text(label_btn4, "Latte");
    lv_obj_center(label_btn4);

    lv_obj_t *label_bottom = lv_label_create(main_screen);
    lv_label_set_text(label_bottom, "Bottom Sexy girls");
    lv_obj_align(label_bottom, LV_ALIGN_BOTTOM_MID, 0, 0);

    //  static lv_style_t label_bottom_style;
    // lv_style_init(&label_bottom_style);
    // lv_style_set_text_font(&label_bottom_style, lv_default_small_font);
    // lv_obj_add_style(label_bottom, &label_bottom_style, 0);
}

void ui_load_splash_screen(void) {
    lv_disp_load_scr(splash_screen);
    status_bar_hide(); // 启动屏不显示状态栏

    // 模拟启动屏显示一段时间后自动切换到主屏幕
    lv_timer_t *timer = lv_timer_create((lv_timer_cb_t)ui_load_main_screen, 10000, NULL); // 2秒后切换
    lv_timer_set_repeat_count(timer, 1);
}

void ui_load_main_screen(void) {
    lv_disp_load_scr(main_screen);
    status_bar_show();
    LV_LOG_USER("Switched to Main Screen.");
}

void ui_load_making_screen(app_coffee_type_t coffee_type) {
    status_bar_show();
    screen_making_update_content(coffee_type);
    lv_disp_load_scr(making_screen);
    LV_LOG_USER("Switched to Making Screen for type: %d", coffee_type);
}

void ui_load_info_screen(app_info_code_t info_code, app_info_severity_t severity) {
    status_bar_show();
    screen_info_update_content(info_code, severity);
    lv_disp_load_scr(info_screen);
    LV_LOG_USER("Switched to Info Screen for code: %d, severity: %d", info_code, severity);
}