#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "packed_anim_loader.h"

static uint8_t* addr;
static uint8_t* index_table_ptr;

lv_img_dsc_t* ui_anim_create(const char* path)
{
    int fd;
    struct stat  sb;
    
    uint32_t current_frame_offset = 0;
    uint32_t current_frame_length = 0;


    fd = open(path, O_RDONLY);
    if (fd == -1) {
        LV_LOG_ERROR("Failed to open packed animation file: %s", path);
        return NULL;
    }

    if (fstat(fd, &sb) == -1)
        return NULL;

    addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED) {
        LV_LOG_USER("[%s] mmap animation file failed!\n", __func__);
        return NULL;
    }
    close(fd);

    index_table_ptr = addr + sizeof(PackedAnimFileHeader);

    for (uint16_t i = 0; i < 118; ++i) {

        // 使用 memcpy 手动读取 offset 和 length，避免结构体对齐问题
        memcpy(&current_frame_offset, index_table_ptr, sizeof(uint32_t));
        index_table_ptr += sizeof(uint32_t);

        memcpy(&current_frame_length, index_table_ptr, sizeof(uint32_t));
        index_table_ptr += sizeof(uint32_t);
    }

    lv_img_dsc_t * dsc = (lv_img_dsc_t *)lv_mem_alloc(sizeof(lv_img_dsc_t));
    dsc->header.always_zero = 0;
    dsc->header.w = 1024;
    dsc->header.h = 600;
    dsc->header.cf = LV_IMG_CF_TRUE_COLOR_ALPHA;
    dsc->data_size = current_frame_length;
    dsc->data = index_table_ptr + current_frame_offset;

    return dsc;
}