// coffee/main_coffee.c
#include "main_coffee.h"
#include "lvgl/lvgl.h"

// 引入各模块
#include "app/app_core.h"
#include "ui/ui.h"
#include "ui/theme/app_theme.h"
#include "lang/lang_manager.h"
#include "data_manager/device_settings.h"
#include "ui/components/status_bar/status_bar.h"
#include "comm/comm_interface.h" // 模拟通信

void coffee_app_init(void) {
    // 1. 初始化数据管理器 (模拟加载设置)
    device_settings_load();

    // 2. 初始化主题
    app_theme_init();

    // 3. 初始化国际化管理器
    lang_manager_init();
    lang_set_current(device_settings_get_language()); // 设置初始语言

    // 4. 初始化UI组件 (例如状态栏)
    status_bar_init();

    // 5. 初始化所有UI屏幕
    ui_init();

    // 6. 初始化通信模块 (启动模拟定时器)
    comm_init();

    // 7. 加载启动屏
    ui_load_splash_screen();

    LV_LOG_USER("Coffee Machine Application Initialized.");
}

void coffee_app_run(void) {
    // 应用程序主循环的逻辑，这里将由 LVGL 的定时器和事件驱动
    // 例如，检查是否有新的通信数据，更新UI等
    // comm_process_data(); // 实际应用中可能需要周期性处理通信数据
}