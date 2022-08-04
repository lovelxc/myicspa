#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if(s == NULL) return 0;
  size_t i = 0;
  while(*s != '\0') ++s, ++i;
  return i;
}

char *strcpy(char *dst, const char *src) {
  char *d = dst;
  while(*src != '\0'){
    *d = *src;
    ++d, ++src;
  }
  *d = '\0';
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  char *d = dst;
  while(*d != '\0') ++d;
  for(const char *s = src; *s != '\0'; s++, d++){
    *d = *src;
  }
  *d = '\0';
  return dst;
}

int strcmp(const char *s1, const char *s2) {
  for(const char *tmp1 = s1, *tmp2 = s2; !(*tmp1 == '\0' && *tmp2 == '\0'); ++tmp1, ++tmp2){
    if(*tmp1 == '\0') return -1;
    else if (*tmp2 == '\0') return 1;
    if(*tmp1 != *tmp2) return (*tmp1 < *tmp2) ? -1 : 1;
  }
  return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  if(s == NULL || n <= 0) return 0;
  unsigned char *tmp = s, t = c;
  while(n--){
    *tmp = t, ++tmp;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  if(n <= 0) return 0;
  size_t i = 0;
  for(const unsigned char *tmp1 = s1, *tmp2 = s2; i < n; ++tmp1, ++tmp2, ++i){
    if(*tmp1 != *tmp2) return (*tmp1 < *tmp2) ? -1 : 1;
  }
  return 0;
}

#endif
