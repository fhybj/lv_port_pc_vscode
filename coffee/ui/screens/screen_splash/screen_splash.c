// ui/screens/screen_splash/screen_splash.c
#include "screen_splash.h"
#include "lvgl/lvgl.h"
#include "lang/lang_manager.h" // For translated label macro

lv_obj_t *splash_screen;

void screen_splash_init(void) {
    splash_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(splash_screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_t *label = CREATE_TRANSLATED_LABEL(splash_screen, STR_WELCOME);
    lv_obj_center(label);

    lv_obj_t *version_label = lv_label_create(splash_screen);
    lv_label_set_text(version_label, "LVGL v8.4 Coffee Machine Demo");
    lv_obj_align(version_label, LV_ALIGN_BOTTOM_MID, 0, -20);
}