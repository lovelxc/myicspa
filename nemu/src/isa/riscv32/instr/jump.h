// 如果dst和src1或src2中的某个是同一个寄存器，那么先对*ddest赋值会造成src1或src2的值不是原来的值
// J-type
def_EHelper(jal) {
  // ddest <- pc + 4
  *s0 = id_src1->simm;
  *ddest = s->snpc;
  rtl_j(s, (*s0) + s->pc);
}

// I-type
def_EHelper(jalr) {
  *s0 = *dsrc1;
  *s1 = id_src2->simm;
  *ddest = s->snpc;
  rtl_j(s, (*s0 + *s1) & (~(sword_t)1));
}