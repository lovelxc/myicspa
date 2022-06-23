#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(lw) f(sw) f(inv) f(nemu_trap)\
f(auipc) f(jal) f(jalr) 

def_all_EXEC_ID(); // -> enum { MAP(INSTR_LIST, def_EXEC_ID) TOTAL_INSTR }
