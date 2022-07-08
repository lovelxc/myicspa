// #include<stdio.h>
// J-type
def_EHelper(jal) {
  // ddest <- pc + 4
  *ddest = s->snpc;
  *s0 = id_src1->simm;
  rtl_sext(s, s1, s0, 21);
  rtl_j(s, (*s1) + s->pc);
}

// I-type
def_EHelper(jalr) {
  *ddest = s->snpc;
  *s0 = *dsrc2;
  rtl_sext(s, s1, s0, 12); // sext(offset))
  *s1 = (*dsrc1 + *dsrc2) & (~(sword_t)1);
  printf("%x", (word_t)*s1);
  rtl_jr(s, s1);
}