#ifndef __RISCV32_RTL_H__
#define __RISCV32_RTL_H__

#include <rtl/rtl.h>
#include "reg.h"

// no isa-dependent rtl instructions
// t0, t1, ... - 只能在RTL伪指令的实现过程中存放中间结果
// 要自己写
// static inline def_rtl(xxx, rtlreg_t* dest, const rtlreg_t imm) {
//   rtl_xxx(s, dest, rz, imm);
// }
#endif
