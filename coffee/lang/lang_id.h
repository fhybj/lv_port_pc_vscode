// lang/lang_id.h
#ifndef LANG_ID_H
#define LANG_ID_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LANG_EN,
    LANG_ZH_CN,
    LANG_COUNT // 语言数量
} lang_code_t;

typedef enum {
    STR_EMPTY = 0,
    STR_WELCOME,
    STR_ESPRESSO,
    STR_LATTE,
    STR_AMERICANO,
    STR_PREHEATING_IN_PROGRESS,
    STR_PREHEATING_TITLE,
    STR_PREHEATING_MESSAGE,
    STR_FILL_WATER_TITLE,
    STR_FILL_WATER_MESSAGE,
    STR_GRINDER_JAMMED_TITLE,
    STR_GRINDER_JAMMED_MESSAGE,
    STR_WATER_TANK_EMPTY_TITLE,
    STR_WATER_TANK_EMPTY_MESSAGE,
    STR_CANCEL,
    STR_OK,
    STR_PREPARING,
    STR_COUNT // 字符串总数
} string_id_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // LANG_ID_H