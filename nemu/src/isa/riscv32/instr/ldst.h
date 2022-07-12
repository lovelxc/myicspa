def_EHelper(lw) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(sw) {
  rtl_sm(s, ddest, dsrc1, id_src2->imm, 4);
}

def_EHelper(lhu) {
  *s0 = id_src2->imm;
//  rtl_sext(s, s1, s0, 12);
  rtl_lm(s, ddest, dsrc1, id_src2->imm, 2);
}
