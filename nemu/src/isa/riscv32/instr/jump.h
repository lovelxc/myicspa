// #include<stdio.h>
def_EHelper(jal) {
  // ddest <- pc + 4
  *ddest = s->snpc;
  *s0 = id_src1->simm;
  rtl_sext(s, s1, s0, 21);
  printf("in jal:%x %x\n", (*s1) , s->pc);
  rtl_j(s, (*s1) + s->pc);
}

def_EHelper(jalr) {
  *ddest = s->snpc;
  *s0 = id_src2->imm + id_src1->simm;
  rtl_j(s, *s0);
}