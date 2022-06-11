#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  // FILE * fp=freopen("in.txt","r",stdin);
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  engine_start();
  // fclose(fp);
  if(is_exit_status_bad()){
	  Log("exit_status is bad!");
  }
  return 0;
}
