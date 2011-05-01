CASE(OPABADD) {
	DBG_P("OPABADD : ");
	eax += ebx;
	pc++; goto NEXTOP;
}
CASE(OPACADD) {
	DBG_P("OPACADD : ");
	eax += ecx;
	pc++; goto NEXTOP;
}
CASE(OPADADD) {
	DBG_P("OPADADD : ");
	eax += edx;
	pc++; goto NEXTOP;
}
CASE(OPBCADD) {
	DBG_P("OPBCADD : ");
	asm volatile("header:");
	ebx += ecx;
	asm volatile("footer:");
	/*
	__asm {
		mov eax, ebx;
		mov ebx, ecx;
		add eax, ebx;
		mov ebx, eax;
	}
	*/
	//asm volatile ("mov ebx, %eax");
	//asm volatile ("mov ecx, %ebx");
	//asm volatile ("addl %eax, %ebx");
	//asm volatile ("mov %eax, ebx");
	pc++; goto NEXTOP;
}
CASE(OPBDADD) {
	DBG_P("OPBDADD : ");
	ebx += edx;
	pc++; goto NEXTOP;
}
CASE(OPCDADD) {
	DBG_P("OPCDADD : ");
	ecx += edx;
	pc++; goto NEXTOP;
}
CASE(OPAiADDC) {
	DBG_P("OPAiADDC : ");
	eax += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPBiADDC) {
	DBG_P("OPBiADDC : ");
	ebx += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCiADDC) {
	DBG_P("OPCiADDC : ");
	ecx += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDiADDC) {
	DBG_P("OPDiADDC : ");
	edx += pc->src;
	pc++; goto NEXTOP;
}
CASE(OPAiSUBC) {
	DBG_P("OPAiSUBC : ");
	eax -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPBiSUBC) {
	DBG_P("OPBiSUBC : ");
	ebx -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCiSUBC) {
	DBG_P("OPCiSUBC : ");
	ecx -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDiSUBC) {
	DBG_P("OPDiSUBC : ");
	edx -= pc->src;
	pc++; goto NEXTOP;
}
CASE(OPAPOP) {
	DBG_P("OPAPOP : ");
	cur_arg = r->function_arg_stack;
	eax = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPBPOP) {
	DBG_P("OPBPOP : ");
	cur_arg = r->function_arg_stack;
	ebx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPCPOP) {
	DBG_P("OPCPOP : ");
	cur_arg = r->function_arg_stack;
	ecx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPDPOP) {
	DBG_P("OPDPOP : ");
	cur_arg = r->function_arg_stack;
	edx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPAPUSH) {
	DBG_P("OPAPUSH : ");
	r++;
	r->function_arg_stack = eax;
	pc++; goto NEXTOP;
}
CASE(OPBPUSH) {
	DBG_P("OPBPUSH : ");
	r++;
	r->function_arg_stack = ebx;
	pc++; goto NEXTOP;
}
CASE(OPCPUSH) {
	DBG_P("OPCPUSH : ");
	r++;
	r->function_arg_stack = ecx;
	pc++; goto NEXTOP;
}
CASE(OPDPUSH) {
	DBG_P("OPDPUSH : ");
	r++;
	r->function_arg_stack = edx;
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
	eax = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPBCOPY) {
	DBG_P("OPBCOPY : ");
	ebx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPCCOPY) {
	DBG_P("OPCCOPY : ");
	ecx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPDCOPY) {
	DBG_P("OPDCOPY : ");
	edx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPAiJLC) {
	DBG_P("OPAiJLC : ");
	if (eax < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPBiJLC) {
	DBG_P("OPBiJLC : ");
	if (ebx < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPCiJLC) {
	DBG_P("OPCiJLC : ");
	if (ecx < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPDiJLC) {
	DBG_P("OPDiJLC : ");
	if (edx < pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPAiJGC) {
	DBG_P("OPAiJGC : ");
	if (eax > pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPBiJGC) {
	DBG_P("OPBiJGC : ");
	if (ebx > pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPCiJGC) {
	DBG_P("OPCiJGC : ");
	if (ecx > pc->src) {
		pc += pc->jmp;
	} else {
		pc++;
	}
	goto NEXTOP;
}
CASE(OPDiJGC) {
	DBG_P("OPDiJGC : ");
	if (edx > pc->src) {
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
	int res = eax;
	memcpy(reg, r->reg_stack, 16);
	r--;
	ebx = res;
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
	int res = eax;
	memcpy(reg, r->reg_stack, 16);
	r--;
	ecx = res;
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
	int res = eax;
	memcpy(reg, r->reg_stack, 16);
	r--;
	edx = res;
	pc++;
	goto NEXTOP;
}
CASE(OPAPFASTCALL) {
	DBG_P("OPAPFASTCALL : ");
	//=========APUSH=========//
	r++;
	r->function_arg_stack = eax;
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
	r->function_arg_stack = ebx;
	//=======================//
	DBG_P("arg = [%d]", ebx);
	r->ret_label_stack = &&L_BPFASTCALLAFTER;
	r->pc_stack = pc;
	//memcpy(r->reg_stack, reg, 16);
	r->reg_stack[0] = eax;
	r->reg_stack[1] = ebx;
	r->reg_stack[2] = ecx;
	r->reg_stack[3] = edx;
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_BPFASTCALLAFTER:
	DBG_P("L_BPFASTCALLAFTER");
	pc = r->pc_stack;
	int res = eax;
	DBG_P("res = [%d]", res);
	//memcpy(reg, r->reg_stack, 16);
	eax = r->reg_stack[0];
	//ebx = r->reg_stack[1];
	ecx = r->reg_stack[2];
	edx = r->reg_stack[3];
	r--;
	ebx = res;
	pc++;
	goto NEXTOP;
}
CASE(OPCPFASTCALL) {
	DBG_P("OPCPFASTCALL : ");
	//=========CPUSH=========//
	r++;
	r->function_arg_stack = ecx;
	//=======================//
	DBG_P("arg = [%d]", ecx);
	r->ret_label_stack = &&L_CPFASTCALLAFTER;
	r->pc_stack = pc;
	//memcpy(r->reg_stack, reg, 16);
	r->reg_stack[0] = eax;
	r->reg_stack[1] = ebx;
	r->reg_stack[2] = ecx;
	r->reg_stack[3] = edx;
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_CPFASTCALLAFTER:
	//asm("int3");
	DBG_P("L_CPFASTCALLAFTER");
	pc = r->pc_stack;
	int res = eax;
	DBG_P("res = [%d]", res);
	//memcpy(reg, r->reg_stack, 16);
	eax = r->reg_stack[0];
	ebx = r->reg_stack[1];
	//ecx = r->reg_stack[2];
	edx = r->reg_stack[3];
	r--;
	ecx = res;
	pc++;
	goto NEXTOP;
}
CASE(OPDPFASTCALL) {
	DBG_P("OPDPFASTCALL : ");
	//=========DPUSH=========//
	r++;
	r->function_arg_stack = edx;
	//=======================//
	r->ret_label_stack = &&L_DPFASTCALLAFTER;
	r->pc_stack = pc;
	memcpy(r->reg_stack, reg, 16);
	pc = local_cache_func_vmcode;
	goto NEXTOP;
L_DPFASTCALLAFTER:
	DBG_P("L_DPFASTCALLAFTER");
	pc = r->pc_stack;
	int res = eax;
	memcpy(reg, r->reg_stack, 16);
	r--;
	edx = res;
	pc++;
	goto NEXTOP;
}
CASE(OPARET) {
	DBG_P("OPARET : ");
	goto *r->ret_label_stack;
}
CASE(OPBRET) {
	DBG_P("OPBRET : ");
	eax = ebx;
	goto *r->ret_label_stack;
}
CASE(OPCRET) {
	DBG_P("OPCRET : ");
	eax = ecx;
	goto *r->ret_label_stack;
}
CASE(OPDRET) {
	DBG_P("OPDRET : ");
	eax = edx;
	goto *r->ret_label_stack;
}
CASE(OPAMOV) {
	DBG_P("OPAMOV : ");
	eax = pc->src;
	pc++; goto NEXTOP;	
}
CASE(OPBMOV) {
	DBG_P("OPBMOV : ");
	ebx = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCMOV) {
	DBG_P("OPCMOV : ");
	ecx = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDMOV) {
	DBG_P("OPDMOV : ");
	edx = pc->src;
	pc++; goto NEXTOP;
}
