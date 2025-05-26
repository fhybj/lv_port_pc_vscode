// ui/screens/screen_making/screen_making.c
#include "screen_making.h"
#include "lvgl/lvgl.h"
#include "lang/lang_manager.h"
#include "ui/ui.h" // 返回主屏幕
#include "app/coffee_logic.h" // 取消制作

// 模拟图片资源（实际使用LVGL自带图标或颜色）
// 为了简化，这里用LVGL图标代替
static const void *coffee_type_images[COFFEE_TYPE_COUNT] = {
    LV_SYMBOL_CHARGE, // Espresso
    LV_SYMBOL_GPS,   // Latte (用滴水代替)
    LV_SYMBOL_ENVELOPE // Americano (用信封代替)
};

static string_id_t coffee_type_names[COFFEE_TYPE_COUNT] = {
    STR_ESPRESSO,
    STR_LATTE,
    STR_AMERICANO
};

lv_obj_t *making_screen;
static lv_obj_t *coffee_image, *coffee_name_label, *progress_bar, *status_label, *cancel_button;

static void cancel_button_event_cb(lv_event_t *e) {
    LV_UNUSED(e);
    coffee_logic_cancel_making(); // 调用应用逻辑取消制作
}

void screen_making_init(void) {
    making_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(making_screen, LV_OBJ_FLAG_SCROLLABLE);

    coffee_image = lv_img_create(making_screen);
    lv_obj_align(coffee_image, LV_ALIGN_TOP_MID, 0, 50);

    coffee_name_label = CREATE_TRANSLATED_LABEL(making_screen, STR_EMPTY); // 初始为空，由update_content更新
    lv_obj_align_to(coffee_name_label, coffee_image, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    progress_bar = lv_bar_create(making_screen);
    lv_obj_set_size(progress_bar, 200, 20);
    lv_obj_align_to(progress_bar, coffee_name_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
    lv_bar_set_range(progress_bar, 0, 100);
    lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF);

    status_label = CREATE_TRANSLATED_LABEL(making_screen, STR_EMPTY);
    lv_obj_align_to(status_label, progress_bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    cancel_button = lv_btn_create(making_screen);
    lv_obj_set_size(cancel_button, 120, 50);
    lv_obj_align(cancel_button, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_obj_add_event_cb(cancel_button, cancel_button_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *cancel_label = CREATE_TRANSLATED_LABEL(cancel_button, STR_CANCEL);
    lv_obj_center(cancel_label);
}

void screen_making_update_content(app_coffee_type_t coffee_type) {
    if (coffee_image && coffee_name_label && coffee_type >= 0 && coffee_type < COFFEE_TYPE_COUNT) {
        lv_img_set_src(coffee_image, coffee_type_images[coffee_type]);
        lang_update_label(coffee_name_label, coffee_type_names[coffee_type]); // 更新翻译文本
        lv_bar_set_value(progress_bar, 0, LV_ANIM_OFF); // 重置进度
        lang_update_label(status_label, STR_PREPARING); // 初始状态
    }
}

void screen_making_update_progress(int progress, string_id_t status_text_id) {
    if (progress_bar && status_label) {
        lv_bar_set_value(progress_bar, progress, LV_ANIM_ON);
        lang_update_label(status_label, status_text_id);
    }
}