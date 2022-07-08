// #include<stdio.h>
// J-type
def_EHelper(jal) {
  // ddest <- pc + 4
  *ddest = s->snpc;
  rtl_j(s, id_src1->simm + s->pc);
}

// I-type
def_EHelper(jalr) {
  *ddest = s->snpc;
  rtl_j(s, (*dsrc1 + id_src2->imm) & (~(sword_t)1));
}