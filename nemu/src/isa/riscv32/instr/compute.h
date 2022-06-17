def_EHelper(lui) {
  // rtl_addi(s, ddest, 0, id_src1->imm);
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  // rtl_addi(s, ddest, 0, id_src1->imm);
  rtl_li(s, ddest, id_src1->imm);
}