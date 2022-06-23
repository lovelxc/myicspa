def_EHelper(jal) {
  // ddest <- pc + imm
  *ddest = s->snpc;
  rtl_j(id_src1->simm);
}

def_EHelper(jalr) {
  *ddest = s->snpc;
  *s0 = id_src2->imm + id_src1->simm;
  rtl_j(s0);
}