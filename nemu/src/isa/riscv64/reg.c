#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  // bool t;
  printf("reg_name  value(16)   value(10)\n");
  for(int i = 0; i < 32; ++i){
    check_reg_idx(i);
    printf("%-8s  0x%08lx  %ld\n", reg_name(i,4), gpr(i), gpr(i));
  }
  printf("PC:       0x%08lx  %ld\n", (uint64_t)cpu.pc, (uint64_t)cpu.pc);
}

word_t isa_reg_str2val(const char *s, bool *success) {

  return 0;
}
