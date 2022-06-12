#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static int idx;
static unsigned choose(unsigned n){
  unsigned t = rand();
  t %= n;
  assert(t >= 0 && t < n);
  return t;
}

static void gen_num(){
  unsigned n = rand();
  char s[33];
  switch (choose(2))  {
    case 0: sprintf(s, "%u", n); break;
    case 1: sprintf(s, "%x", n); break;
  }
  strcpy(&buf[idx], s);
  idx += strlen(s);
}

inline static void gen(char c){
  buf[idx] = c; buf[++idx] = '\0';
}
static void gen_rand_op(){
  char c;
  switch (choose(4))  {
    case 0: c = '+'; break;
    case 1: c = '-'; break;
    case 2: c = '*'; break;
    case 3: c = '/'; break;
  }
  buf[idx] = c; buf[++idx] = '\0';
}

static void gen_rand_expr() {
  buf[0] = '\0';
  switch (choose(3)) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    idx = 0;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    if(!fscanf(fp, "%d", &result)) assert(0);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
