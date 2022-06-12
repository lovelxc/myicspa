#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
//   /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  // engine_start();
  
  #include<../src/monitor/sdb/sdb.h>
  char s[655035];
  unsigned t;
  bool zc;
  while(scanf("%u %s", &t, s)){
    unsigned cc = expr(s, &zc);
    if(cc!=t){
      printf("correct = %u, error = %u\n", t, cc);
    }else puts("OK");
  }
  if(is_exit_status_bad()){
	  Log("exit_status is bad!");
  }
  return 0;
}
