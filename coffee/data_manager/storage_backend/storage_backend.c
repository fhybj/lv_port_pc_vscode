// data_manager/storage_backend/storage_backend.c
#include "storage_backend.h"
#include <string.h> // For strcmp, strcpy, strncpy
#include "lvgl/lvgl.h" // For LV_LOG_USER

// 简单模拟存储：用一个二维数组存储键值对
#define MAX_STORAGE_ITEMS 5
#define MAX_KEY_LEN 32
#define MAX_VALUE_LEN 64

static struct {
    char key[MAX_KEY_LEN];
    char value[MAX_VALUE_LEN];
    bool used;
} storage_data[MAX_STORAGE_ITEMS];

void storage_backend_init(void) {
    for (int i = 0; i < MAX_STORAGE_ITEMS; i++) {
        storage_data[i].used = false;
        storage_data[i].key[0] = '\0';
        storage_data[i].value[0] = '\0';
    }
    LV_LOG_USER("Storage Backend Initialized (Simulated).");
}

bool storage_backend_write(const char *key, const char *value) {
    if (key == NULL || value == NULL) return false;

    // 查找现有键
    for (int i = 0; i < MAX_STORAGE_ITEMS; i++) {
        if (storage_data[i].used && strcmp(storage_data[i].key, key) == 0) {
            strncpy(storage_data[i].value, value, MAX_VALUE_LEN - 1);
            storage_data[i].value[MAX_VALUE_LEN - 1] = '\0';
            LV_LOG_USER("Storage Write (Update): %s = %s", key, value);
            return true;
        }
    }

    // 查找空位
    for (int i = 0; i < MAX_STORAGE_ITEMS; i++) {
        if (!storage_data[i].used) {
            strncpy(storage_data[i].key, key, MAX_KEY_LEN - 1);
            storage_data[i].key[MAX_KEY_LEN - 1] = '\0';
            strncpy(storage_data[i].value, value, MAX_VALUE_LEN - 1);
            storage_data[i].value[MAX_VALUE_LEN - 1] = '\0';
            storage_data[i].used = true;
            LV_LOG_USER("Storage Write (New): %s = %s", key, value);
            return true;
        }
    }
    LV_LOG_WARN("Storage Write Failed: No space for %s", key);
    return false; // 没有空间
}

bool storage_backend_read(const char *key, char *buffer, size_t buffer_size) {
    if (key == NULL || buffer == NULL || buffer_size == 0) return false;

    for (int i = 0; i < MAX_STORAGE_ITEMS; i++) {
        if (storage_data[i].used && strcmp(storage_data[i].key, key) == 0) {
            strncpy(buffer, storage_data[i].value, buffer_size - 1);
            buffer[buffer_size - 1] = '\0';
            LV_LOG_USER("Storage Read: %s = %s", key, buffer);
            return true;
        }
    }
    LV_LOG_INFO("Storage Read: Key '%s' not found.", key);
    buffer[0] = '\0'; // 未找到时清空缓冲区
    return false;
}