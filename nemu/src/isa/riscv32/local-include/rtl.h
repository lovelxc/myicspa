#ifndef __RISCV32_RTL_H__
#define __RISCV32_RTL_H__

#include <rtl/rtl.h>
#include "reg.h"

// no isa-dependent rtl instructions
// t0, t1, ... - 只能在RTL伪指令的实现过程中存放中间结果
// 要自己写
static inline def_rtl(setrelopiu, uint32_t relop, rtlreg_t *dest,
    const rtlreg_t *src1, word_t imm) {
  *dest = interpret_relop(relop, *src1, imm);
}

static inline def_rtl(muls_lo, rtlreg_t *dest,
    const rtlreg_t *src1, const rtlreg_t *src2) {
  *dest = (int32_t)(*src1) * (int32_t)(*src2);
}

static inline def_rtl(mulsu_hi, rtlreg_t *dest,
    const rtlreg_t *src1, const rtlreg_t *src2) {
  *dest = (((int64_t)(sword_t)(*src1) * (uint64_t)(*src2)) >> 32);
}
#endif
