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
	//reg[1] += reg[2];
	__asm {
		mov eax, reg[1];
		mov ebx, reg[2];
		add eax, ebx;
		mov reg[1], eax;
	}
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
	DBG_P("function_arg_stack[stack_count] = [%d]", function_arg_stack[stack_count]);
	cur_arg = r->function_arg_stack;
	reg[0] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPBPOP) {
	DBG_P("OPBPOP : ");
	DBG_P("function_arg_stack[stack_count] = [%d]", function_arg_stack[stack_count]);
	cur_arg = r->function_arg_stack;
	reg[1] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPCPOP) {
	DBG_P("OPCPOP : ");
	DBG_P("function_arg_stack[stack_count] = [%d]", function_arg_stack[stack_count]);
	cur_arg = r->function_arg_stack;
	reg[2] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPDPOP) {
	DBG_P("OPDPOP : ");
	DBG_P("function_arg_stack[stack_count] = [%d]", function_arg_stack[stack_count]);
	cur_arg = r->function_arg_stack;
	reg[3] = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPAPUSH) {
	DBG_P("OPAPUSH : ");
	//stack_count++;
	r++;
	r->function_arg_stack = reg[0];
	pc++; goto NEXTOP;
}
CASE(OPBPUSH) {
	DBG_P("OPBPUSH : ");
	//stack_count++;
	r++;
	r->function_arg_stack = reg[1];
	pc++; goto NEXTOP;
}
CASE(OPCPUSH) {
	DBG_P("OPCPUSH : ");
	//stack_count++;
	r++;
	r->function_arg_stack = reg[2];
	pc++; goto NEXTOP;
}
CASE(OPDPUSH) {
	DBG_P("OPDPUSH : ");
	//stack_count++;
	r++;
	r->function_arg_stack = reg[3];
	pc++; goto NEXTOP;
}
CASE(OPAiPUSHC) {
	DBG_P("OPAiPUSHC : ");
	//stack_count++;
	r++;
	r->function_arg_stack = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPBiPUSHC) {
	DBG_P("OPBiPUSHC : ");
	//stack_count++;
	r++;
	r->function_arg_stack = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCiPUSHC) {
	DBG_P("OPCiPUSHC : ");
	//stack_count++;
	r++;
	r->function_arg_stack = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDiPUSHC) {
	DBG_P("OPDiPUSHC : ");
	//stack_count++;
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
	DBG_P("stack_count = [%d]", stack_count);
	pc = r->pc_stack;
	memcpy(reg, r->reg_stack, 4 * 4);
	//stack_count--;
	r--;
	DBG_P("L_AFASTCALLAFTER: res = [%d]", res);
	//reg[0] = res;
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
	DBG_P("stack_count = [%d]", stack_count);
	pc = r->pc_stack;
	int res = reg[0];
	memcpy(reg, r->reg_stack, 16);
	//stack_count--;
	r--;
	DBG_P("L_BFASTCALLAFTER: res = [%d]", res);
	reg[1] = res;
	//reg[0] = reg[1];
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
	DBG_P("stack_count = [%d]", stack_count);
	pc = r->pc_stack;
	int res = reg[0];
	memcpy(reg, r->reg_stack, 16);
	//stack_count--;
	r--;
	DBG_P("L_CFASTCALLAFTER: res = [%d]", res);
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
	DBG_P("stack_count = [%d]", stack_count);
	pc = r->pc_stack;
	int res = reg[0];
	memcpy(reg, r->reg_stack, 16);
	//stack_count--;
	r--;
	DBG_P("L_DFASTCALLAFTER: res = [%d]", res);
	reg[3] = res;
	pc++;
	goto NEXTOP;
}
CASE(OPARET) {
	DBG_P("OPARET : ");
	//reg[0] = reg[0];
	DBG_P("stack_count = [%d]", stack_count);
	DBG_P("reg[0] = [%d]", reg[0]);
	goto *r->ret_label_stack;
}
CASE(OPBRET) {
	DBG_P("OPBRET : ");
	reg[0] = reg[1];
	DBG_P("stack_count = [%d]", stack_count);
	DBG_P("reg[0] = [%d]", reg[0]);
	goto *r->ret_label_stack;
}
CASE(OPCRET) {
	DBG_P("OPCRET : ");
	reg[0] = reg[2];
	DBG_P("stack_count = [%d]", stack_count);
	DBG_P("reg[0] = [%d]", reg[0]);
	goto *r->ret_label_stack;
}
CASE(OPDRET) {
	DBG_P("OPDRET : ");
	reg[0] = reg[3];
	DBG_P("stack_count = [%d]", stack_count);
	DBG_P("reg[0] = [%d]", reg[0]);
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
