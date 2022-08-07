#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  // panic("Not implemented");
  int d;
  char c;
  char *s;
  char buf[34];
  char *res = out;
  while (*fmt){
    if(*fmt == '%'){
      switch(*(++fmt)){
        case 's': /* string */
          s = va_arg(ap, char *);
          if(s != NULL)
            while(*s){
              *out++ = *s++;
            }
          break;
        case 'd': /* int */
          d = va_arg(ap, int);
          if(d < 0){
            *out++ = '-';
            d = -d;
          }
          if(d == 0) *out++ = '0';
          else{
            memset(buf, '\0', sizeof(buf));
            int len = 0;
            while(d != 0){
              buf[len++] = d%10 + '0';
              d /= 10;
            }
            len--;
            while(len >= 0){
              *out++ = buf[len--];
            }
          }
          break;
        case 'c': /* char */
          /* need a cast here since va_arg only
            takes fully promoted types */
          c = (char)va_arg(ap, int);
          *out++ = c;
          break;
        default:
          break;
          panic("vsprintf: ");
      }
    }
    else{
      *out++ = *fmt;
    }
    ++fmt;
  }
  return strlen(res);
}

int sprintf(char *out, const char *fmt, ...) {
  if(fmt == NULL) return -1;
  va_list ap;
  va_start(ap, fmt);
  int n = vsprintf(out, fmt, ap);
  va_end(ap);
  return n;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
