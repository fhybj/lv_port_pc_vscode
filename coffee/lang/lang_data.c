// lang/lang_data.c
#include "lang_id.h"

const char * const lang_en[STR_COUNT] = {
    [STR_EMPTY] = "",
    [STR_WELCOME] = "Welcome to Coffee Machine!",
    [STR_ESPRESSO] = "Espresso",
    [STR_LATTE] = "Latte",
    [STR_AMERICANO] = "Americano",
    [STR_PREHEATING_IN_PROGRESS] = "Preheating in progress...",
    [STR_PREHEATING_TITLE] = "Preheating",
    [STR_PREHEATING_MESSAGE] = "The machine is heating up. Please wait.",
    [STR_FILL_WATER_TITLE] = "Refill Water",
    [STR_FILL_WATER_MESSAGE] = "Please fill the water tank.",
    [STR_GRINDER_JAMMED_TITLE] = "Grinder Jammed",
    [STR_GRINDER_JAMMED_MESSAGE] = "Grinder is jammed. Please clean the bean container.",
    [STR_WATER_TANK_EMPTY_TITLE] = "Water Tank Empty",
    [STR_WATER_TANK_EMPTY_MESSAGE] = "Please refill the water tank to continue operation.",
    [STR_CANCEL] = "Cancel",
    [STR_OK] = "OK",
    [STR_PREPARING] = "Preparing..."
};

const char * const lang_zh_cn[STR_COUNT] = {
    [STR_EMPTY] = "",
    [STR_WELCOME] = "欢迎使用咖啡机！",
    [STR_ESPRESSO] = "浓缩咖啡",
    [STR_LATTE] = "拿铁",
    [STR_AMERICANO] = "美式咖啡",
    [STR_PREHEATING_IN_PROGRESS] = "正在预热中...",
    [STR_PREHEATING_TITLE] = "正在预热",
    [STR_PREHEATING_MESSAGE] = "咖啡机正在加热。请稍候。",
    [STR_FILL_WATER_TITLE] = "加水",
    [STR_FILL_WATER_MESSAGE] = "请加满水箱。",
    [STR_GRINDER_JAMMED_TITLE] = "磨豆器堵塞",
    [STR_GRINDER_JAMMED_MESSAGE] = "磨豆器堵塞。请清洁咖啡豆容器。",
    [STR_WATER_TANK_EMPTY_TITLE] = "水箱空",
    [STR_WATER_TANK_EMPTY_MESSAGE] = "请加满水箱以继续操作。",
    [STR_CANCEL] = "取消",
    [STR_OK] = "确定",
    [STR_PREPARING] = "准备中..."
};