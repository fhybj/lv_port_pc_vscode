// data_manager/device_settings.h
#ifndef DEVICE_SETTINGS_H
#define DEVICE_SETTINGS_H

#include <stdbool.h>
#include "lang/lang_id.h" // For lang_code_t

#ifdef __cplusplus
extern "C" {
#endif

void device_settings_init(void);
void device_settings_load(void);
void device_settings_save(void);

lang_code_t device_settings_get_language(void);
void device_settings_set_language(lang_code_t lang);

// 模拟其他设置
bool device_settings_get_wifi_enabled(void);
void device_settings_set_wifi_enabled(bool enabled);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // DEVICE_SETTINGS_H