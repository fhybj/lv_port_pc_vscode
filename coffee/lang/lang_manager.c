// lang/lang_manager.c
#include "lang_manager.h"
#include <stdlib.h> // For malloc, free
#include <string.h> // For memset
#include "lvgl/lvgl.h" // For LV_ASSERT_NULL

extern const char * const lang_en[STR_COUNT];
extern const char * const lang_zh_cn[STR_COUNT];

static const char * const *current_lang_data[LANG_COUNT] = {
    lang_en,
    lang_zh_cn
};

static lang_code_t current_lang_code = LANG_EN;

// 用于存储注册的文本对象和其对应的字符串ID
typedef struct {
    lv_obj_t *obj;
    string_id_t id;
    lang_update_cb_t update_cb;
} registered_label_t;

#define MAX_REGISTERED_LABELS 100 // 假设最多注册100个文本对象
static registered_label_t registered_labels[MAX_REGISTERED_LABELS];
static int registered_labels_count = 0;

void lang_manager_init(void) {
    memset(registered_labels, 0, sizeof(registered_labels));
    registered_labels_count = 0;
    LV_LOG_USER("Language Manager Initialized.");
}

const char* lang_get_string(string_id_t id) {
    if (id >= 0 && id < STR_COUNT && current_lang_code >= 0 && current_lang_code < LANG_COUNT) {
        return current_lang_data[current_lang_code][id];
    }
    return "INVALID_STR_ID";
}

void lang_set_current(lang_code_t code) {
    if (code >= 0 && code < LANG_COUNT) {
        current_lang_code = code;
        lang_update_all_registered_labels(); // 切换语言时更新所有注册的文本
        LV_LOG_USER("Language switched to %d", code);
    }
}

void lang_register_label_update_cb(lv_obj_t *obj, string_id_t id, lang_update_cb_t cb) {
    LV_ASSERT_NULL(obj);
    LV_ASSERT_NULL(cb);
    if (registered_labels_count < MAX_REGISTERED_LABELS) {
        registered_labels[registered_labels_count].obj = obj;
        registered_labels[registered_labels_count].id = id;
        registered_labels[registered_labels_count].update_cb = cb;
        registered_labels_count++;
        LV_LOG_USER("Registered label for update: %p, ID: %d", (void*)obj, id);
    } else {
        LV_LOG_WARN("Max registered labels reached!");
    }
}

void lang_unregister_label_update_cb(lv_obj_t *obj) {
    for (int i = 0; i < registered_labels_count; i++) {
        if (registered_labels[i].obj == obj) {
            // 移除并前移后面的元素
            for (int j = i; j < registered_labels_count - 1; j++) {
                registered_labels[j] = registered_labels[j+1];
            }
            registered_labels_count--;
            LV_LOG_USER("Unregistered label: %p", (void*)obj);
            return;
        }
    }
}

void lang_update_all_registered_labels(void) {
    for (int i = 0; i < registered_labels_count; i++) {
        if (registered_labels[i].obj && registered_labels[i].update_cb) {
            registered_labels[i].update_cb(registered_labels[i].obj, registered_labels[i].id);
        } else {
            // 如果对象已被删除，则取消注册
            LV_LOG_WARN("Detected null object or callback for registered label. Unregistering.");
            lang_unregister_label_update_cb(registered_labels[i].obj);
            i--; // 由于元素前移，需要回退索引
        }
    }
}

void lang_default_label_update_cb(lv_obj_t *obj, string_id_t id) {
    if (obj && lv_obj_check_type(obj, &lv_label_class)) {
        lv_label_set_text(obj, lang_get_string(id));
    }
}