// app/app_core.h
#ifndef APP_CORE_H
#define APP_CORE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    COFFEE_ESPRESSO,
    COFFEE_LATTE,
    COFFEE_AMERICANO,
    COFFEE_TYPE_COUNT
} app_coffee_type_t;

typedef enum {
    INFO_NONE,
    INFO_PREHEATING,
    INFO_FILL_WATER,
    ERROR_GRINDER_JAMMED,
    ERROR_WATER_TANK_EMPTY,
    INFO_CODE_COUNT
} app_info_code_t;

typedef enum {
    SEVERITY_INFO,
    SEVERITY_WARNING,
    SEVERITY_ERROR
} app_info_severity_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // APP_CORE_H