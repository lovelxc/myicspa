#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char *expr;
  word_t value;
} WP;
// static 是为了防止其他地方修改这些数据，强制其他代码只能从函数调用中修改数据
static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

WP* new_wp();
void free_wp(WP *wp);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
// 从free_链表中返回一个空闲的监视点结构
WP* new_wp(){
  if(!free_){
    Assert(0,"No watchpoint can be allocated");
  }
  // 头插法
  WP *p = head; // 记录头节点
  head = free_;
  free_ = free_->next;
  head->next = p;
  return head;
}

// 将wp归还到free_链表中
void free_wp(WP *wp){
  if(!wp){
    Assert(0,"error watchpoint");
  }
  // wp_front 为 head时，要特殊处理
  WP *wp_front = head;
  if(wp != head){
    while(wp_front->next != wp) wp_front = wp_front->next;
    wp_front->next = wp->next;
  }else{
    head = head->next;
  }
  wp->next = free_;
  free_ = wp;
}

