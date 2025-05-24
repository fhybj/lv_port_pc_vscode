// app/coffee_logic.c
#include "coffee_logic.h"
#include "lvgl/lvgl.h"
#include "ui/ui.h"
#include "lang/lang_id.h" // For status text IDs

#ifndef LV_TIMER_REPEAT_INFINITE
#define LV_TIMER_REPEAT_INFINITE (-1)
#endif

static lv_timer_t *making_progress_timer = NULL;
extern void screen_making_update_progress(int progress, uint16_t status_str_id); // 添加函数声明
static int current_progress = 0;
static app_coffee_type_t current_coffee_type;

static void making_progress_timer_cb(lv_timer_t *timer) {
    current_progress += 10;
    if (current_progress > 100) {
        current_progress = 100;
        lv_timer_del(making_progress_timer);
        making_progress_timer = NULL;
        LV_LOG_USER("Coffee making finished!");
        ui_load_main_screen(); // 完成后返回主屏幕
        return;
    }
    LV_LOG_USER("Making progress: %d%%", current_progress);
    ui_load_making_screen(current_coffee_type); // 重新加载以更新进度条和状态
    screen_making_update_progress(current_progress, STR_PREPARING); // 更新进度和状态文本
}

void coffee_logic_start_making(app_coffee_type_t type) {
    LV_LOG_USER("Starting to make coffee type: %d", type);
    current_coffee_type = type;
    current_progress = 0;
    ui_load_making_screen(type); // 切换到制作屏幕
    if (making_progress_timer) {
        lv_timer_del(making_progress_timer);
    }
    making_progress_timer = lv_timer_create(making_progress_timer_cb, 500, NULL); // 每500ms更新一次
    lv_timer_set_repeat_count(making_progress_timer, LV_TIMER_REPEAT_INFINITE);
}

void coffee_logic_cancel_making(void) {
    if (making_progress_timer) {
        lv_timer_del(making_progress_timer);
        making_progress_timer = NULL;
    }
    LV_LOG_USER("Coffee making cancelled.");
    ui_load_main_screen(); // 返回主屏幕
}

void coffee_logic_simulate_progress_update(void) {
    // This function is for direct simulation if needed,
    // but the timer callback already handles it.
    // It could be used by an external module to force a progress update.
    if (making_progress_timer) {
        making_progress_timer_cb(making_progress_timer);
    }
}