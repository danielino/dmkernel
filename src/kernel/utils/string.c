#include "kernel/utils/string.h"
#include <stddef.h>
#include <stdint.h>

size_t strlen(const char *s){
    size_t len = 0;
    while(s[len] != '\0'){
        len++;
    }
    return len;
}

int strcmp(const char *a, const char *b){
    while(*a && *a == *b) {
        a++;
        b++;
    }
    return *a - *b;
}

int strncmp(const char *a, const char *b, uint32_t n) {
      while (n && *a && *a == *b) {
          a++; b++; n--;
      }
      if (n == 0) return 0;
      return *a - *b;
  }

void strcpy(char *dst, const char *b){
    while((*dst++ = *b++));
}

void memset(void *ptr, int val, uint32_t size){
    uint8_t *p = ptr;
    while(size--)
        *p++ = (uint8_t)val;
}

void memcpy(void *dst, const void *src, uint32_t size){
    uint8_t *d = dst;
    const uint8_t *s = src;
    while(size--)
        *d++ = *s++;
}