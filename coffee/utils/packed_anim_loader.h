#ifndef PACKED_ANIM_LOADER_H
#define PACKED_ANIM_LOADER_H

#include "lvgl/lvgl.h"
#include <stdint.h>
#include <stdbool.h>

// 正确的 PackedAnimFileHeader 结构，与 Python 脚本输出精确匹配
typedef struct __attribute__((packed)) { // 确保没有填充
    uint32_t magic_word;                    // 4 bytes: 0x4D494E41 (Little-Endian 'ANIM')
    uint32_t lvgl_img_header_packed_value;  // 4 bytes: Python 脚本打包的 lv_img_header_t 的 32 位值
    uint16_t frame_count_in_file;           // 2 bytes: 文件中包含的帧数量
    uint32_t pixel_data_size_per_frame;     // 4 bytes: 每帧图片的原始像素数据大小 (例如: 宽度 * 高度 * 4)
} PackedAnimFileHeader; // 修正后总大小: 4 + 4 + 2 + 4 = 14 bytes

typedef struct __attribute__((packed)) { // 确保没有填充
    uint32_t offset;        // 帧数据在整个 BIN 文件中的相对偏移量
    uint32_t length;        // 帧数据的大小
} PackedAnimFrameIndexEntry;

// 表示一个已加载的动画实例
typedef struct {
    uint8_t *buffer;              // 指向整个打包动画文件在RAM中的缓冲区
    size_t buffer_size;           // 缓冲区实际大小
    lv_img_dsc_t *img_dscs;     // 指向动态分配的 lv_image_dsc_t 数组
    const lv_img_dsc_t **frames_ptrs; // 指向 const lv_image_dsc_t* 数组 (供 lv_animimg_set_src 使用)
    uint16_t frame_count;         // 该动画的帧数
    uint16_t width;               // 动画帧宽度
    uint16_t height;              // 动画帧高度
    lv_img_cf_t color_format; // 动画帧颜色格式
} packed_animation_t;

/**
 * @brief 初始化动画加载器。分配内部缓冲区。
 * 必须在LVGL初始化之后调用。
 * @param max_packed_file_size 预分配的最大缓冲区大小，应能容纳最大的打包动画文件。
 * @return true 成功，false 失败。
 */
bool packed_anim_loader_init(size_t max_packed_file_size);

/**
 * @brief 清理动画加载器。释放内部缓冲区。
 * 应在程序结束时调用。
 */
void packed_anim_loader_deinit(void);

/**
 * @brief 从eMMC加载一个打包动画文件。
 * 加载后，会自动填充一个 packed_animation_t 结构体。
 * @param path 打包动画文件的路径。
 * @param anim_out 指向 packed_animation_t 结构体的指针，用于返回加载的动画信息。
 * @return true 成功加载，false 失败。
 */
bool packed_anim_load(const char *path, packed_animation_t *anim_out);

/**
 * @brief 释放一个已加载的动画实例的资源。
 * 主要释放 lv_image_dsc_t 数组和帧指针数组。
 * 不释放底层的 packed_anim_loader_init 分配的缓冲区。
 * @param anim 指向要释放的 packed_animation_t 结构体的指针。
 */
void packed_anim_free(packed_animation_t *anim);

#endif // PACKED_ANIM_LOADER_H