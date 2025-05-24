// data_manager/storage_backend/storage_backend.h
#ifndef STORAGE_BACKEND_H
#define STORAGE_BACKEND_H

#include <stdbool.h>
#include <stddef.h> // For size_t

#ifdef __cplusplus
extern "C" {
#endif

void storage_backend_init(void);
bool storage_backend_write(const char *key, const char *value);
bool storage_backend_read(const char *key, char *buffer, size_t buffer_size);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // STORAGE_BACKEND_H