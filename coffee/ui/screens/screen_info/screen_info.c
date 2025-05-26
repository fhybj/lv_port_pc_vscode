// ui/screens/screen_info/screen_info.c
#include "screen_info.h"
#include "lvgl/lvgl.h"
#include "lang/lang_manager.h"
#include "ui/ui.h" // 返回主屏幕

// 模拟图标资源
static const void *info_icons[] = {
    LV_SYMBOL_BULLET,      // INFO
    LV_SYMBOL_WARNING   // WARNING
};

typedef struct {
    string_id_t title_id, message_id;
    const void *icon_src;
    lv_color_t bg_color;
} info_display_data_t;

static info_display_data_t info_data_map[INFO_CODE_COUNT];

lv_obj_t *info_screen;
static lv_obj_t *icon_image, *title_label, *message_label, *ok_button;

static void init_info_data_map(void) {

    info_data_map[INFO_NONE] = (info_display_data_t){STR_EMPTY, STR_EMPTY, NULL, lv_color_make(0xFF, 0xFF, 0xFF)}; // 如果没有直接的INFO符号，可以使用空字符串
    info_data_map[INFO_PREHEATING] = (info_display_data_t){STR_PREHEATING_TITLE, STR_PREHEATING_MESSAGE, LV_SYMBOL_PLAY, lv_color_make(0x80, 0x80, 0xFF)};
    info_data_map[INFO_FILL_WATER] = (info_display_data_t){STR_FILL_WATER_TITLE, STR_FILL_WATER_MESSAGE, LV_SYMBOL_TINT, lv_color_make(0x66, 0xCC, 0xEE)}; // 浅蓝色调 (替换 DROP)
    info_data_map[ERROR_GRINDER_JAMMED] = (info_display_data_t){STR_GRINDER_JAMMED_TITLE, STR_GRINDER_JAMMED_MESSAGE, LV_SYMBOL_WARNING, lv_color_make(0xFF, 0x00, 0x00)}; // 红色 (如果 LV_SYMBOL_WARNING 存在)
    info_data_map[ERROR_WATER_TANK_EMPTY] = (info_display_data_t){STR_WATER_TANK_EMPTY_TITLE, STR_WATER_TANK_EMPTY_MESSAGE, LV_SYMBOL_CLOSE, lv_color_make(0xFF, 0x00, 0x00)}; // 红色 (替换 REMOVE)
}

static void ok_button_event_cb(lv_event_t *e) {
    LV_UNUSED(e);
    ui_load_main_screen(); // 返回主屏幕
}

void screen_info_init(void) {
    info_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(info_screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(info_screen, lv_color_white(), 0); // 默认背景

    icon_image = lv_label_create(info_screen); // 用label显示symbol作为图标
    lv_obj_set_style_text_font(icon_image, &lv_font_montserrat_30, 0); // 大字体显示图标
    lv_obj_align(icon_image, LV_ALIGN_TOP_MID, 0, 30);

    title_label = CREATE_TRANSLATED_LABEL(info_screen, STR_EMPTY);
    lv_obj_set_style_text_font(title_label, &lv_font_montserrat_24, 0);
    lv_obj_align_to(title_label, icon_image, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    message_label = CREATE_TRANSLATED_LABEL(info_screen, STR_EMPTY);
    lv_obj_set_width(message_label, LV_PCT(80));
    lv_obj_set_style_text_align(message_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align_to(message_label, title_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

    ok_button = lv_btn_create(info_screen);
    lv_obj_set_size(ok_button, 120, 50);
    lv_obj_align(ok_button, LV_ALIGN_TOP_LEFT, 0, -20);
    lv_obj_add_event_cb(ok_button, ok_button_event_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *ok_label = CREATE_TRANSLATED_LABEL(ok_button, STR_OK);
    lv_obj_center(ok_label);

    init_info_data_map();
}

void screen_info_update_content(app_info_code_t info_code, app_info_severity_t severity) {
    const info_display_data_t *data = &info_data_map[info_code];

    lv_obj_set_style_bg_color(info_screen, data->bg_color, 0);

    // 使用LVGL Symbol作为图标
    if (data->icon_src) {
        lv_label_set_text(icon_image, (const char *)data->icon_src);
    } else {
        lv_label_set_text(icon_image, ""); // 清空
    }

    lang_update_label(title_label, data->title_id);
    lang_update_label(message_label, data->message_id);

    // if (severity == SEVERITY_ERROR) {
    //     lv_obj_add_flag(ok_button, LV_OBJ_FLAG_HIDDEN); // 错误不显示OK按钮
    // } else {
    //     lv_obj_clear_flag(ok_button, LV_OBJ_FLAG_HIDDEN);
    // }
}