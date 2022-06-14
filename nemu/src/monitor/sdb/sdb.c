#include <isa.h>
#include <cpu/cpu.h>
#include <cpu/exec.h>
#include <memory/paddr.h>
#include <utils.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char *expr;
  word_t value;
} WP;
WP* new_wp();
void free_wp(WP *wp);
void print_watchpoints();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_si(char *args) {
  int n = 1;
  // args不为空时才去读，否则默认执行1步
  if(args) sscanf(args, "%d", &n);
  Log("execute %d line.", n);
  cpu_exec(n);
  return 0;
}

static int cmd_info(char *args) {
  if(args==NULL) {
    printf("r: print all registers\nw: print all watchpoints\n");
    return 0;
  }
  char *op = strtok(args, " ");
  if(strcmp("r", op) == 0){
    isa_reg_display();
  } else if(strcmp("w", op) == 0){
    // print watchpoints
    print_watchpoints();
  }
  return 0;
}

static int cmd_x(char *args) {
  char *_expr = strtok(args, " ");
  int n = atoi(_expr);
  // 表达式可能含空格
  _expr = args + strlen(_expr) + 1;
  
  bool bl = true;
  word_t ans = expr(_expr, &bl);
  
  if(!bl) {
    Log("Bad expression: %s", _expr);
    return 0;
  }
  for (int i = 0; i < n; ++i){
    uint32_t t;
    memcpy(&t, guest_to_host(ans), sizeof(uint32_t));
    printf("0x%08x\n", t);
    _expr += sizeof(uint32_t);
  }
  return 0;
}

static int cmd_w(char *args) {
  bool bl;
  word_t ans = expr(args, &bl);
  
  if(!bl) {
    Log("Bad expression: %s", args);
    return 0;
  }
  WP *p = new_wp();
  (*p).value = ans;
  printf("%d %d\n", sizeof (*p).expr,sizeof args);
  strcpy((*p).expr, args);
  
  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Execute next or next n program line", cmd_si},
  { "info", "List of all registers and their contents, for selected stack frame", cmd_info},
  { "x", "Print the value of EXP", cmd_x},
  { "w", "Add watchpoint", cmd_w},
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
