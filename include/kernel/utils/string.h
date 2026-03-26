#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <stdint.h>

size_t strlen(const char* s);
int strcmp(const char* a, const char* b);
int strncmp(const char* a, const char* b, uint32_t n);
void strcpy(char* dst, const char* b);
void memset(void* ptr, int val, uint32_t size);
void memcpy(void* dst, const void* src, uint32_t size);

#endif