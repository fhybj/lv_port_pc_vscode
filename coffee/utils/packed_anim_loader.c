#include "packed_anim_loader.h"
#include <stdio.h>
#include <stdlib.h> // For malloc, free
#include <string.h> // For memcpy

// 内部缓冲区，由 packed_anim_loader_init 分配和管理
static uint8_t *s_packed_anim_global_buffer = NULL;
static size_t s_packed_anim_global_buffer_size = 0;

// LVGL颜色格式到字符串的映射（用于日志）
static const char* get_lv_color_format_str(lv_img_cf_t cf) {
    switch (cf) {
        case LV_IMG_CF_RGB565: return "RGB565";
        case LV_IMG_CF_RGB888: return "RGB888";
        case LV_IMG_CF_RGBA8888: return "ARGB8888";
        case LV_IMG_CF_RGBX8888: return "XRGB8888";
        // ... 其他格式根据需要添加
        default: return "UNKNOWN";
    }
}

static lv_img_header_t unpack_lv_img_header(uint32_t packed_value) {
    lv_img_header_t header;

    uint8_t byte0 = (uint8_t)(packed_value & 0xFF);
    uint8_t byte1 = (uint8_t)((packed_value >> 8) & 0xFF);
    uint8_t byte2 = (uint8_t)((packed_value >> 16) & 0xFF);
    uint8_t byte3 = (uint8_t)((packed_value >> 24) & 0xFF);

    header.cf = byte0 & 0x1F;
    header.always_zero = (byte0 >> 5) & 0x07;
    
    // 假设 'reserved' 在 byte1 的最低 2 位
    header.reserved = byte1 & 0x03;
    
    // 重构宽度w (11位): byte1 的高 6 位 + byte2 的低 5 位
    uint16_t w_msbs = (byte1 >> 2) & 0x3F;  // byte1 的位 2-7
    uint16_t w_lsbs = byte2 & 0x1F;         // byte2 的位 0-4
    header.w = (w_msbs << 5) | w_lsbs;

    // 重构高度h (11位): byte2 的高 3 位 + byte3 的所有 8 位
    uint16_t h_msbs = (byte2 >> 5) & 0x07;  // byte2 的位 5-7
    uint16_t h_lsbs = byte3;                // byte3 的所有 8 位
    header.h = (h_msbs << 8) | h_lsbs;

    return header;
}

bool packed_anim_loader_init(size_t max_packed_file_size) {
    if (s_packed_anim_global_buffer) {
        LV_LOG_WARN("Packed animation loader already initialized.");
        return true;
    }

    s_packed_anim_global_buffer = (uint8_t *)malloc(max_packed_file_size);
    if (!s_packed_anim_global_buffer) {
        LV_LOG_ERROR("Failed to allocate global buffer for packed animations, size: %zu bytes", max_packed_file_size);
        s_packed_anim_global_buffer_size = 0;
        return false;
    }
    s_packed_anim_global_buffer_size = max_packed_file_size;
    LV_LOG_INFO("Packed animation loader initialized with %zu bytes buffer.", s_packed_anim_global_buffer_size);
    return true;
}

void packed_anim_loader_deinit(void) {
    if (s_packed_anim_global_buffer) {
        free(s_packed_anim_global_buffer);
        s_packed_anim_global_buffer = NULL;
        s_packed_anim_global_buffer_size = 0;
        LV_LOG_INFO("Packed animation loader deinitialized.");
    }
}

bool packed_anim_load(const char *path, packed_animation_t *anim_out) {
    if (!s_packed_anim_global_buffer || s_packed_anim_global_buffer_size == 0) {
        LV_LOG_ERROR("Packed animation loader not initialized. Call packed_anim_loader_init first.");
        return false;
    }
    if (!path || !anim_out) {
        LV_LOG_ERROR("Invalid arguments for packed_anim_load (path or anim_out is NULL).");
        return false;
    }

    memset(anim_out, 0, sizeof(packed_animation_t)); // 清空输出结构体

    FILE *fp = fopen(path, "rb");
    if (!fp) {
        LV_LOG_ERROR("Failed to open packed animation file: %s", path);
        return false;
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size == -1 || (size_t)file_size > s_packed_anim_global_buffer_size) {
        LV_LOG_ERROR("File size (%ld) exceeds global buffer size (%zu) or invalid file size for %s.",
                     file_size, s_packed_anim_global_buffer_size, path);
        fclose(fp);
        return false;
    }

    size_t bytes_read = fread(s_packed_anim_global_buffer, 1, file_size, fp);
    fclose(fp);

    if (bytes_read != (size_t)file_size) {
        LV_LOG_ERROR("Failed to read all bytes from file '%s'. Read %zu, expected %ld.", path, bytes_read, file_size);
        return false;
    }
    LV_LOG_INFO("Successfully read %zu bytes from '%s'.", bytes_read, path);

    // 解析文件头
    PackedAnimFileHeader *header = (PackedAnimFileHeader *)s_packed_anim_global_buffer;

    // 检查魔术字
    if (header->magic_word != 0x4D494E41) { // 'ANIM' in Little-Endian
        LV_LOG_ERROR("Invalid magic word in packed animation file: 0x%08X", header->magic_word);
        return false;
    }

    // 解包 LVGL 图片头信息
    lv_img_header_t lv_img_header = unpack_lv_img_header(header->lvgl_img_header_packed_value);

    // 填充 anim_out 结构体
    anim_out->width = lv_img_header.w;
    anim_out->height = lv_img_header.h;
    anim_out->frame_count = header->frame_count_in_file; // 使用修正后的字段名
    anim_out->color_format = (lv_img_cf_t)lv_img_header.cf;
    anim_out->buffer = s_packed_anim_global_buffer;
    anim_out->buffer_size = bytes_read;

    // 校验文件头中记录的每帧数据大小与计算值是否一致
    uint32_t expected_pixel_data_size = anim_out->width * anim_out->height * 4; // ARGB8888 4字节/像素
    if (header->pixel_data_size_per_frame != expected_pixel_data_size) {
        LV_LOG_ERROR("Header pixel data size mismatch: Expected %u (from %ux%u), got %u. File might be corrupted or dimensions/format are incorrect.",
                     expected_pixel_data_size, anim_out->width, anim_out->height, header->pixel_data_size_per_frame);
        return false;
    }

    // 动态分配 lv_image_dsc_t 数组和帧指针数组
    anim_out->img_dscs = (lv_img_dsc_t *)lv_mem_alloc(sizeof(lv_img_dsc_t) * anim_out->frame_count);
    anim_out->frames_ptrs = (const lv_img_dsc_t **)lv_mem_alloc(sizeof(const lv_img_dsc_t *) * (anim_out->frame_count + 1));

    if (!anim_out->img_dscs || !anim_out->frames_ptrs) {
        LV_LOG_ERROR("Failed to allocate memory for image descriptors or frame pointers.");
        if (anim_out->img_dscs) lv_mem_free(anim_out->img_dscs);
        if (anim_out->frames_ptrs) lv_mem_free(anim_out->frames_ptrs);
        memset(anim_out, 0, sizeof(packed_animation_t));
        return false;
    }

    // 定位到索引表起始位置：在文件头之后
    uint8_t *index_table_read_ptr = s_packed_anim_global_buffer + sizeof(PackedAnimFileHeader);

    // 填充 lv_image_dsc_t 结构体和帧指针数组
    for (uint16_t i = 0; i < anim_out->frame_count; ++i) {
        uint32_t current_frame_offset;
        uint32_t current_frame_length;

        // 使用 memcpy 手动读取 offset 和 length，避免结构体对齐问题
        memcpy(&current_frame_offset, index_table_read_ptr, sizeof(uint32_t));
        index_table_read_ptr += sizeof(uint32_t);

        memcpy(&current_frame_length, index_table_read_ptr, sizeof(uint32_t));
        index_table_read_ptr += sizeof(uint32_t);

        // 校验单帧数据大小
        if (current_frame_length != header->pixel_data_size_per_frame) {
            LV_LOG_WARN("Frame %u data length mismatch: %u (expected %u). Offset: %u",
                        i, current_frame_length, header->pixel_data_size_per_frame, current_frame_offset);
            // 虽然是警告，但动画可能显示异常。如果出现大量这种警告，说明文件可能损坏。
        }

        anim_out->img_dscs[i].header = lv_img_header; // 使用统一的 LVGL 图片头信息
        anim_out->img_dscs[i].data_size = current_frame_length; // 使用从索引中读取的长度
        anim_out->img_dscs[i].data = s_packed_anim_global_buffer + current_frame_offset; // 使用从索引中读取的偏移量
        
        anim_out->frames_ptrs[i] = &anim_out->img_dscs[i];
    }
    anim_out->frames_ptrs[anim_out->frame_count] = NULL; // NULL 终止符

    LV_LOG_INFO("Loaded packed animation '%s': %u frames, %ux%u, format %s. Total size: %zu bytes.",
                path, anim_out->frame_count, anim_out->width, anim_out->height,
                get_lv_color_format_str(anim_out->color_format), anim_out->buffer_size);
    return true;
}

void packed_anim_free(packed_animation_t *anim) {
    if (!anim) {
        LV_LOG_WARN("Attempted to free NULL packed_animation_t.");
        return;
    }
    
    // 释放 lv_image_dsc_t 数组
    if (anim->img_dscs) {
        lv_mem_free(anim->img_dscs);
        anim->img_dscs = NULL;
    }
    
    // 释放帧指针数组
    if (anim->frames_ptrs) {
        lv_mem_free(anim->frames_ptrs);
        anim->frames_ptrs = NULL;
    }
    
    // 清空结构体
    memset(anim, 0, sizeof(packed_animation_t));
    LV_LOG_INFO("Packed animation resources freed.");
}