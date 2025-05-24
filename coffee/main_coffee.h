// coffee/main_coffee.h
#ifndef MAIN_COFFEE_H
#define MAIN_COFFEE_H

#ifdef __cplusplus
extern "C" {
#endif

void coffee_app_init(void);
void coffee_app_run(void); // 用于主循环中调用

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // MAIN_COFFEE_H