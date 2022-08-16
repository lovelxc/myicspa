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
    *d++ = *src++;
  }
  *d = '\0';
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  char *d = dst;
  size_t i = 0;
  // 复制n个字符(不特判\0), 不过只复制一个字符串, 多的(长度不及n)则补0.
  for(const char *s = src; i < n; ++d, ++s, ++i){
    if(*s == '\0'){ 
      *d = '\0';
      ++d, ++i;
      break;
      }
    *d = *s;
  }
  if(i < n){
    memset(d, 0, n - i);
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  char *d = dst;
  while(*d != '\0') ++d;
  for(const char *s = src; *s != '\0'; s++, d++){
    *d = *s;
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
  size_t i = 0;
  for(const char *tmp1 = s1, *tmp2 = s2; !(*tmp1 == '\0' && *tmp2 == '\0') && i < n; ++tmp1, ++tmp2){
    if(*tmp1 == '\0') return -1;
    else if (*tmp2 == '\0') return 1;
    if(*tmp1 != *tmp2) return (*tmp1 < *tmp2) ? -1 : 1;
  }
  return 0;
}

void *memset(void *s, int c, size_t n) {
  if(s == NULL || n <= 0) return s;
  unsigned char *tmp = s, t = c;
  while(n--){
    *tmp++ = t;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented"); // 允许有重叠的空间
  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  size_t i = 0;
  const unsigned char *s = in;
  for(unsigned char *d = out; i < n; ++d, ++s, ++i){
    *d = *s;
  }
  return out;
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
