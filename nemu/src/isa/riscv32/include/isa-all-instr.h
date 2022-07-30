#include <cpu/decode.h>
#include "../local-include/rtl.h"

#define INSTR_LIST(f) f(lui) f(auipc) f(jal) f(jalr) f(inv) f(nemu_trap)\
f(beq) f(bne) f(blt) f(bge) f(bltu) f(bgeu)\
f(lw) f(lb) f(lh) f(lbu) f(lhu) f(sw) f(sb) f(sh)\
f(addi) f(slti) f(sltiu) f(xori) f(ori) f(andi) f(slli) f(srli) f(srai)\
f(add) f(sub) f(sll) f(slt) f(sltu) f(xor) f(srl) f(sra) f(or) f(and)\
f(mul) f(mulh) f(mulhsu) f(mulhu) f(div) f(divu) f(rem) f(remu)

def_all_EXEC_ID(); // -> enum { MAP(INSTR_LIST, def_EXEC_ID) TOTAL_INSTR }
