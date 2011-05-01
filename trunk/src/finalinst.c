CASE(OPABADD) {
	DBG_P("OPABADD : ");
	reg[0] += reg[1];
	pc++; goto NEXTOP;
}
CASE(OPACADD) {
	DBG_P("OPACADD : ");
	reg[0] += reg[2];
	pc++; goto NEXTOP;
}
CASE(OPADADD) {
	DBG_P("OPADADD : ");
	reg[0] += reg[3];
	pc++; goto NEXTOP;
}
CASE(OPBCADD) {
	DBG_P("OPBCADD : ");
	reg[1] += reg[2];
	/*
	__asm {
		mov eax, reg[1];
		mov ebx, reg[2];
		add eax, ebx;
		mov reg[1], eax;
	}
	*/
	//asm volatile ("mov reg[1], %eax");
	//asm volatile ("mov reg[2], %ebx");
	//asm volatile ("addl %eax, %ebx");
	//asm volatile ("mov %eax, reg[1]");
	pc++; goto NEXTOP;
}
CASE(OPBDADD) {
	DBG_P("OPBDADD : ");
	reg[1] += reg[3];
	pc++; goto NEXTOP;
}
CASE(OPCDADD) {
	DBG_P("OPCDADD : ");
	reg[2] += reg[3];
	pc++; goto NEXTOP;
}
CASE(OPAiADDC) {
	DBG_P("OPAiADDC : ");
	reg[0] += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPBiADDC) {
	DBG_P("OPBiADDC : ");
	reg[1] += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCiADDC) {
	DBG_P("OPCiADDC : ");
	reg[2] += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDiADDC) {
	DBG_P("OPDiADDC : ");
	reg[3] += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPAiSUBC) {
	DBG_P("OPAiSUBC : ");
	reg[0] -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPBiSUBC) {
	DBG_P("OPBiSUBC : ");
	reg[1] -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCiSUBC) {
	DBG_P("OPCiSUBC : ");
	reg[2] -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDiSUBC) {
	DBG_P("OPDiSUBC : ");
	reg[3] -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPAPOP) {
	DBG_P("OPAPOP : ");
	cur_arg = r->function_arg_stack;
	reg[0] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPBPOP) {
	DBG_P("OPBPOP : ");
	cur_arg = r->function_arg_stack;
	reg[1] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPCPOP) {
	DBG_P("OPCPOP : ");
	cur_arg = r->function_arg_stack;
	reg[2] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPDPOP) {
	DBG_P("OPDPOP : ");
	cur_arg = r->function_arg_stack;
	reg[3] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPAPUSH) {
	DBG_P("OPAPUSH : ");
	r++;
	r->function_arg_stack = reg[0];
	pc++; goto NEXTOP;
}
CASE(OPBPUSH) {
	DBG_P("OPBPUSH : ");
	r++;
	r->function_arg_stack = reg[1];
	pc++; goto NEXTOP;
}
CASE(OPCPUSH) {
	DBG_P("OPCPUSH : ");
	r++;
	r->function_arg_stack = reg[2];
	pc++; goto NEXTOP;
}
CASE(OPDPUSH) {
	DBG_P("OPDPUSH : ");
	r++;
	r->function_arg_stack = reg[3];
	pc++; goto NEXTOP;
}
CASE(OPAiPUSHC) {
	DBG_P("OPAiPUSHC : ");
	r++;
	r->function_arg_stack = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPBiPUSHC) {
	DBG_P("OPBiPUSHC : ");
	r++;
	r->function_arg_stack = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCiPUSHC) {
	DBG_P("OPCiPUSHC : ");
	r++;
	r->function_arg_stack = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDiPUSHC) {
	DBG_P("OPDiPUSHC : ");
	r++;
	r->function_arg_stack = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPACOPY) {
	DBG_P("OPACOPY : ");
	reg[0] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPBCOPY) {
	DBG_P("OPBCOPY : ");
	reg[1] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPCCOPY) {
	DBG_P("OPCCOPY : ");
	reg[2] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPDCOPY) {
	DBG_P("OPDCOPY : ");
	reg[3] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPAiJLC) {
	DBG_P("OPAiJLC : ");
	if (reg[0] < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPBiJLC) {
	DBG_P("OPBiJLC : ");
	if (reg[1] < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPCiJLC) {
	DBG_P("OPCiJLC : ");
	if (reg[2] < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPDiJLC) {
	DBG_P("OPDiJLC : ");
	if (reg[3] < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPAiJGC) {
	DBG_P("OPAiJGC : ");
	if (reg[0] > pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPBiJGC) {
	DBG_P("OPBiJGC : ");
	if (reg[1] > pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPCiJGC) {
	DBG_P("OPCiJGC : ");
	if (reg[2] > pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPDiJGC) {
	DBG_P("OPDiJGC : ");
	if (reg[3] > pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPAFASTCALL) {
	DBG_P("OPAFASTCALL : ");
	r->ret_label_stack = &&L_AFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 4 * 4);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_AFASTCALLAFTER:
	DBG_P("L_AFASTCALLAFTER");
	pc = r->pc_stack;
	memcpy(reg, r->reg_stack, 4 * 4);
	r--;
	pc++;
	goto NEXTOP;
}
CASE(OPBFASTCALL) {
	DBG_P("OPBFASTCALL : ");
	r->ret_label_stack = &&L_BFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 16);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_BFASTCALLAFTER:
	DBG_P("L_BFASTCALLAFTER");
	pc = r->pc_stack;
	int res = reg[0];
	memcpy(reg, r->reg_stack, 16);
	r--;
	reg[1] = res;
	pc++;
	goto NEXTOP;
}
CASE(OPCFASTCALL) {
	DBG_P("OPCFASTCALL : ");
	r->ret_label_stack = &&L_CFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 16);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_CFASTCALLAFTER:
	DBG_P("L_CFASTCALLAFTER");
	pc = r->pc_stack;
	int res = reg[0];
	memcpy(reg, r->reg_stack, 16);
	r--;
	reg[2] = res;
	pc++;
	goto NEXTOP;
}
CASE(OPDFASTCALL) {
	DBG_P("OPDFASTCALL : ");
	r->ret_label_stack = &&L_DFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 16);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_DFASTCALLAFTER:
	DBG_P("L_DFASTCALLAFTER");
	pc = r->pc_stack;
	int res = reg[0];
	memcpy(reg, r->reg_stack, 16);
	r--;
	reg[3] = res;
	pc++;
	goto NEXTOP;
}
CASE(OPAPFASTCALL) {
	DBG_P("OPAPFASTCALL : ");
	//=========APUSH=========//
	r++;
	r->function_arg_stack = reg[0];
	//=======================//
	r->ret_label_stack = &&L_APFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 4 * 4);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_APFASTCALLAFTER:
	DBG_P("L_APFASTCALLAFTER");
	pc = r->pc_stack;
	memcpy(reg, r->reg_stack, 4 * 4);
	r--;
	pc++;
	goto NEXTOP;
}
CASE(OPBPFASTCALL) {
	DBG_P("OPBPFASTCALL : ");
	//=========BPUSH=========//
	r++;
	r->function_arg_stack = reg[1];
	//=======================//
	DBG_P("arg = [%d]", reg[1]);
	r->ret_label_stack = &&L_BPFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 16);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_BPFASTCALLAFTER:
	DBG_P("L_BPFASTCALLAFTER");
	pc = r->pc_stack;
	int res = reg[0];
	DBG_P("res = [%d]", res);
	memcpy(reg, r->reg_stack, 16);
	r--;
	reg[1] = res;
	pc++;
	goto NEXTOP;
}
CASE(OPCPFASTCALL) {
	DBG_P("OPCPFASTCALL : ");
	//=========CPUSH=========//
	r++;
	r->function_arg_stack = reg[2];
	//=======================//
	DBG_P("arg = [%d]", reg[2]);
	r->ret_label_stack = &&L_CPFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 16);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_CPFASTCALLAFTER:
	//asm("int3");
	DBG_P("L_CPFASTCALLAFTER");
	pc = r->pc_stack;
	int res = reg[0];
	DBG_P("res = [%d]", res);
	memcpy(reg, r->reg_stack, 16);
	r--;
	reg[2] = res;
	pc++;
	goto NEXTOP;
}
CASE(OPDPFASTCALL) {
	DBG_P("OPDPFASTCALL : ");
	//=========DPUSH=========//
	r++;
	r->function_arg_stack = reg[3];
	//=======================//
	r->ret_label_stack = &&L_DPFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 16);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_DPFASTCALLAFTER:
	DBG_P("L_DPFASTCALLAFTER");
	pc = r->pc_stack;
	int res = reg[0];
	memcpy(reg, r->reg_stack, 16);
	r--;
	reg[3] = res;
	pc++;
	goto NEXTOP;
}
CASE(OPARET) {
	DBG_P("OPARET : ");
	goto *r->ret_label_stack;
}
CASE(OPBRET) {
	DBG_P("OPBRET : ");
	reg[0] = reg[1];
	goto *r->ret_label_stack;
}
CASE(OPCRET) {
	DBG_P("OPCRET : ");
	reg[0] = reg[2];
	goto *r->ret_label_stack;
}
CASE(OPDRET) {
	DBG_P("OPDRET : ");
	reg[0] = reg[3];
	goto *r->ret_label_stack;
}
CASE(OPAMOV) {
	DBG_P("OPAMOV : ");
	reg[0] = pc->src;
	pc++; goto NEXTOP;	
}
CASE(OPBMOV) {
	DBG_P("OPBMOV : ");
	reg[1] = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCMOV) {
	DBG_P("OPCMOV : ");
	reg[2] = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDMOV) {
	DBG_P("OPDMOV : ");
	reg[3] = pc->src;
	pc++; goto NEXTOP;
}
