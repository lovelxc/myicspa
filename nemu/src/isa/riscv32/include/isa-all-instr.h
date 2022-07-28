#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(jal) f(inv) f(nemu_trap)\
f(auipc) f(jalr) f(add) f(sub)\
f(addi) f(slti) f(sltiu)\
f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu)\
f(lw) f(lb) f(lh) f(lbu) f(lhu)\
f(sw) f(sb) f(sh)\

def_all_EXEC_ID(); // -> enum { MAP(INSTR_LIST, def_EXEC_ID) TOTAL_INSTR }
