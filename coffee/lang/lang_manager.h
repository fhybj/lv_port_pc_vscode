// lang/lang_manager.h
#ifndef LANG_MANAGER_H
#define LANG_MANAGER_H

#include "lvgl/lvgl.h"
#include "lang_id.h"

#ifdef __cplusplus
extern "C" {
#endif

// 定义一个回调函数类型，用于更新翻译文本对象
typedef void (*lang_update_cb_t)(lv_obj_t *obj, string_id_t id);

void lang_manager_init(void);
const char* lang_get_string(string_id_t id);
void lang_set_current(lang_code_t code);

// 注册和更新机制
void lang_register_label_update_cb(lv_obj_t *obj, string_id_t id, lang_update_cb_t cb);
void lang_unregister_label_update_cb(lv_obj_t *obj);
void lang_update_all_registered_labels(void);

// 默认的LVGL label更新回调
void lang_default_label_update_cb(lv_obj_t *obj, string_id_t id);

// 宏: 创建LVGL label并注册其翻译更新回调
#define CREATE_TRANSLATED_LABEL(parent, id) \
    ({ lv_obj_t* _label_obj = lv_label_create(parent); \
       lv_label_set_text(_label_obj, lang_get_string(id)); \
       lang_register_label_update_cb(_label_obj, id, lang_default_label_update_cb); \
       _label_obj; })

// 更新已注册的label
#define lang_update_label(obj, id) lang_default_label_update_cb(obj, id)

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LANG_MANAGER_H