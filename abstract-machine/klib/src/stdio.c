#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
  if(fmt == NULL) return -1;
  char *res = out;
  va_list ap;
  int d;
  char c;
  char *s;
  char buf[34];
  va_start(ap, fmt);
  while (*fmt){
    switch (*fmt){
      case 's': /* string */
        s = va_arg(ap, char *);
        while(*s){
          *out++ = *s++;
        }
        break;
      case 'd': /* int */
        d = va_arg(ap, int);
        memset(buf, 0, sizeof(buf));
        if(d < 0){
          *out++ = '-';
          d = -d;
        }
        if(d == 0) *out++ = '0';
        else{
          int len = 0;
          while(d != 0){
            buf[len++] = d%10 + '0';
            d /= 10;
          }
          while(len > 0){
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
        *out++ = *fmt;
    }
    ++fmt;
  }
  va_end(ap);
  return strlen(res);
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
