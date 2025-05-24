// app/coffee_logic.h
#ifndef COFFEE_LOGIC_H
#define COFFEE_LOGIC_H

#include "app_core.h"

#ifdef __cplusplus
extern "C" {
#endif

void coffee_logic_start_making(app_coffee_type_t type);
void coffee_logic_cancel_making(void);
void coffee_logic_simulate_progress_update(void); // 模拟进度更新

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // COFFEE_LOGIC_H