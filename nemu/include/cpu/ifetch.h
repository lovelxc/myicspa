#ifndef __CPU_IFETCH_H__

#include <memory/vaddr.h>

static inline uint32_t instr_fetch(vaddr_t *pc, int len) {
  // 其实就是读内存，(*pc)指指令所在的地址
  uint32_t instr = vaddr_ifetch(*pc, len);
  (*pc) += len; // 传入的s->snpc自动+4 (64位+8)
  return instr;
}

#endif
