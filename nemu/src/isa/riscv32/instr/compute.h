def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
}

def_EHelper(auipc) {
  // ddest <- pc + imm
  *s0 = s->pc;
  rtl_addi(s, ddest, s0, id_src1->imm);
}

def_EHelper(addi) {
  rtl_addi(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slti) {
  rtl_setrelopi(s, RELOP_LT, ddest, dsrc1, id_src2->simm);
}

def_EHelper(sltiu) {
  rtl_setrelopiu(s, RELOP_LTU, ddest, dsrc1, id_src2->imm);
}