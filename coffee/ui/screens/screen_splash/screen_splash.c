// ui/screens/screen_splash/screen_splash.c
#include "screen_splash.h"
#include "lvgl/lvgl.h"
#include "lang/lang_manager.h" // For translated label macro
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils/packed_anim_loader.h"

lv_obj_t *splash_screen;

#define PATH_PREFIX     "/home/geek/work/lvgl/lv_port_pc_vscode/coffee"
#define LOGO_IMG       "A:"PATH_PREFIX"/ui/assets/images/logo.bin"
#define LOGO_IMG_2       PATH_PREFIX"/ui/assets/images/output.bin"
#define BOOT_ANIM       PATH_PREFIX"/ui/assets/images/bootanim.bin"

// 计时器结构体
typedef struct {
    struct timespec start;
    struct timespec end;
} Timer;

// 开始计时
static inline void timer_start(Timer* t) {
    clock_gettime(CLOCK_MONOTONIC, &t->start);
}

// 结束计时
static inline void timer_end(Timer* t) {
    clock_gettime(CLOCK_MONOTONIC, &t->end);
}

// 计算耗时（纳秒）
static inline long timer_elapsed_ns(Timer* t) {
    return (t->end.tv_sec - t->start.tv_sec) * 1000000000L + 
           (t->end.tv_nsec - t->start.tv_nsec);
}

// 计算耗时（微秒）
static inline double timer_elapsed_us(Timer* t) {
    return timer_elapsed_ns(t) / 1000.0;
}

// 计算耗时（毫秒）
static inline double timer_elapsed_ms(Timer* t) {
    return timer_elapsed_ns(t) / 1000000.0;
}

// 计算耗时（秒）
static inline double timer_elapsed_s(Timer* t) {
    return timer_elapsed_ns(t) / 1000000000.0;
}

// 定义最大的打包动画文件大小（例如，你最大的一个动画文件是2.5MB）
#define MAX_ANIMATION_FILE_SIZE (1024 * 600 * 4 * 120)

// 用于存储当前加载的动画实例
static packed_animation_t g_current_animation;

extern lv_img_dsc_t* ui_anim_create(const char* path);

void screen_splash_init(void) {
    // Timer timer;
    
    // timer_start(&timer);
    // if (!packed_anim_loader_init(MAX_ANIMATION_FILE_SIZE)) {
    //     LV_LOG_USER("Failed to initialize packed animation loader. Exiting.");
    //     return; // 处理错误，可能无法继续
    // }
    // timer_end(&timer);
    // LV_LOG_USER("anim init: %.3f us, %.3f ms\n", timer_elapsed_us(&timer), timer_elapsed_ms(&timer));

    // timer_start(&timer);
    // // packed_anim_load(BOOT_ANIM, &g_current_animation);
    // if (packed_anim_load(BOOT_ANIM, &g_current_animation)) {
    //     // 设置LVGL动画源和持续时间
    //     LV_LOG_USER("Animation started successfully. Frame count: %u.",
    //                 g_current_animation.frame_count);
    // } else {
    //     LV_LOG_USER("Failed to load animation from %s.", BOOT_ANIM);
    //     // 如果加载失败，可以显示一个默认图片或错误提示
    // }
    // timer_end(&timer);
    // LV_LOG_USER("load anim file: %.3f us, %.3f ms\n", timer_elapsed_us(&timer), timer_elapsed_ms(&timer));

    // // lv_img_dsc_t *bg_dsc = g_current_animation.frames_ptrs[g_current_animation.frame_count-1];
    // LV_LOG_USER("data_size: %u, high: %u, width: %u, cf: %u, allow_zero: %u", bg_dsc->data_size, bg_dsc->header.h, bg_dsc->header.w, bg_dsc->header.cf, bg_dsc->header.always_zero);

    splash_screen = lv_obj_create(NULL);
    lv_obj_clear_flag(splash_screen, LV_OBJ_FLAG_SCROLLABLE);

    lv_img_dsc_t *bg_dsc = ui_anim_create(BOOT_ANIM);

    // lv_obj_t * bg = lv_animimg_create(splash_screen);
    lv_obj_t * bg = lv_img_create(splash_screen);
    // lv_img_set_src(bg, g_current_animation.frames_ptrs[g_current_animation.frame_count-1]);
    lv_img_set_src(bg, bg_dsc);
    lv_obj_set_width(bg, LV_PCT(100));
    lv_obj_set_height(bg, LV_PCT(100));
    lv_obj_center(bg);

    // lv_animimg_set_src(bg, (const void **) g_current_animation.frames_ptrs, g_current_animation.frame_count);
    // lv_animimg_set_duration(bg, g_current_animation.frame_count * (1000 / 30));
    // lv_animimg_set_repeat_count(bg, LV_ANIM_REPEAT_INFINITE);
    // lv_animimg_start(bg);

    lv_obj_t *label = CREATE_TRANSLATED_LABEL(splash_screen, STR_WELCOME);
    lv_obj_center(label);

    lv_obj_t *version_label = lv_label_create(splash_screen);
    lv_label_set_text(version_label, "LVGL v8.4 Coffee Machine Demo");
    lv_obj_align(version_label, LV_ALIGN_BOTTOM_MID, 0, -20);
}