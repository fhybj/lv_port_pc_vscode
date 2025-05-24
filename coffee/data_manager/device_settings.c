// data_manager/device_settings.c
#include "device_settings.h"
#include "storage_backend/storage_backend.h" // 模拟后端
#include "lvgl/lvgl.h" // For LV_LOG_USER

static struct {
    lang_code_t language;
    bool wifi_enabled;
    // ... 其他设置
} current_settings;

void device_settings_init(void) {
    // 在实际系统中，这里可能读取默认值或初始化存储后端
    current_settings.language = LANG_EN; // 默认英文
    current_settings.wifi_enabled = true;
    LV_LOG_USER("Device Settings Initialized.");
}

void device_settings_load(void) {
    // 模拟从存储后端加载
    char lang_str[10];
    if (storage_backend_read("language", lang_str, sizeof(lang_str))) {
        if (strcmp(lang_str, "en") == 0) {
            current_settings.language = LANG_EN;
        } else if (strcmp(lang_str, "zh_cn") == 0) {
            current_settings.language = LANG_ZH_CN;
        }
    } else {
        current_settings.language = LANG_EN; // 默认值
    }

    char wifi_str[10];
    if (storage_backend_read("wifi_enabled", wifi_str, sizeof(wifi_str))) {
        current_settings.wifi_enabled = (strcmp(wifi_str, "true") == 0);
    } else {
        current_settings.wifi_enabled = true; // 默认值
    }
    LV_LOG_USER("Device Settings Loaded. Lang: %d, WiFi: %d", current_settings.language, current_settings.wifi_enabled);
}

void device_settings_save(void) {
    // 模拟保存到存储后端
    storage_backend_write("language", (current_settings.language == LANG_EN) ? "en" : "zh_cn");
    storage_backend_write("wifi_enabled", current_settings.wifi_enabled ? "true" : "false");
    LV_LOG_USER("Device Settings Saved.");
}

lang_code_t device_settings_get_language(void) {
    return current_settings.language;
}

void device_settings_set_language(lang_code_t lang) {
    if (current_settings.language != lang) {
        current_settings.language = lang;
        device_settings_save(); // 改变后保存
    }
}

bool device_settings_get_wifi_enabled(void) {
    return current_settings.wifi_enabled;
}

void device_settings_set_wifi_enabled(bool enabled) {
    if (current_settings.wifi_enabled != enabled) {
        current_settings.wifi_enabled = enabled;
        device_settings_save(); // 改变后保存
    }
}