def_EHelper(beq) {
  rtl_jrelop(s, RELOP_EQ, dsrc1, ddest, s->pc + id_src2->simm);
}