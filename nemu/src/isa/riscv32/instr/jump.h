// #include<stdio.h>
// J-type
def_EHelper(jal) {
  // ddest <- pc + 4
  *ddest = s->snpc;
  printf("jump to:%x\n", (word_t)(id_src1->simm + s->pc));
  rtl_j(s, id_src1->simm + s->pc);
}

// I-type
def_EHelper(jalr) {
  *ddest = s->snpc;
  *s0 = *dsrc2;
  rtl_sext(s, s1, s0, 12); // sext(offset))
  *s1 = (*dsrc1 + id_src2->imm) & (~(sword_t)1);
  printf("%x\n", (word_t)*s1);
  rtl_jr(s, s1);
}