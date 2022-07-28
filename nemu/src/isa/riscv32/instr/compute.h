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

def_EHelper(slti) {
  rtl_setrelopi(s, RELOP_LT, ddest, dsrc1, id_src2->simm);
}

def_EHelper(sltiu) {
  rtl_setrelopiu(s, RELOP_LTU, ddest, dsrc1, id_src2->imm);
}

def_EHelper(xori) {
  rtl_xori(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(ori) {
  rtl_ori(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(andi) {
  rtl_andi(s, ddest, dsrc1, id_src2->simm);
}

def_EHelper(slli) {
  rtl_slli(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(srli) {
  rtl_slli(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(srai) {
  rtl_srai(s, ddest, dsrc1, id_src2->imm);
}

def_EHelper(add) {
  rtl_add(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sub) {
  rtl_sub(s, ddest, dsrc1, dsrc2);
}

def_EHelper(sll) {
  rtl_sll(s, ddest, dsrc1, dsrc2);
}

def_EHelper(slt) {
  rtl_setrelop(s, RELOP_LT, ddest, dsrc1, dsrc2);
}

def_EHelper(sltu) {
  rtl_setrelop(s, RELOP_LTU, ddest, dsrc1, dsrc2);
}