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
	ebx += ecx;
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
	eax += reg[pc->src];
	pc++; goto NEXTOP;
}
CASE(OPBiADDC) {
	DBG_P("OPBiADDC : ");
	ebx += reg[pc->src];
	pc++; goto NEXTOP;
}
CASE(OPCiADDC) {
	DBG_P("OPCiADDC : ");
	ecx += reg[pc->src];
	pc++; goto NEXTOP;
}
CASE(OPDiADDC) {
	DBG_P("OPDiADDC : ");
	edx += reg[pc->src];
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
	DBG_P("function_arg_stack[arg_stack_count] = [%d]", function_arg_stack[arg_stack_count]);
	cur_arg = function_arg_stack[arg_stack_count];
	eax = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPBPOP) {
	DBG_P("OPBPOP : ");
	DBG_P("function_arg_stack[arg_stack_count] = [%d]", function_arg_stack[arg_stack_count]);
	cur_arg = function_arg_stack[arg_stack_count];
	ebx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPCPOP) {
	DBG_P("OPCPOP : ");
	DBG_P("function_arg_stack[arg_stack_count] = [%d]", function_arg_stack[arg_stack_count]);
	cur_arg = function_arg_stack[arg_stack_count];
	ecx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPDPOP) {
	DBG_P("OPDPOP : ");
	DBG_P("function_arg_stack[arg_stack_count] = [%d]", function_arg_stack[arg_stack_count]);
	cur_arg = function_arg_stack[arg_stack_count];
	edx = cur_arg;
	pc++; goto NEXTOP;
}
CASE(OPAPUSH) {
	DBG_P("OPAPUSH : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = eax;
	pc++; goto NEXTOP;
}
CASE(OPBPUSH) {
	DBG_P("OPBPUSH : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = ebx;
	pc++; goto NEXTOP;
}
CASE(OPCPUSH) {
	DBG_P("OPCPUSH : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = ecx;
	pc++; goto NEXTOP;
}
CASE(OPDPUSH) {
	DBG_P("OPDPUSH : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = edx;
	pc++; goto NEXTOP;
}
CASE(OPAiPUSHC) {
	DBG_P("OPAiPUSHC : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPBiPUSHC) {
	DBG_P("OPBiPUSHC : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPCiPUSHC) {
	DBG_P("OPCiPUSHC : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = pc->src;
	pc++; goto NEXTOP;
}
CASE(OPDiPUSHC) {
	DBG_P("OPDiPUSHC : ");
	arg_stack_count++;
	function_arg_stack[arg_stack_count] = pc->src;
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
	int res = VirtualMachine_run(local_cache_func_vmcode);
	DBG_P("res = [%d]", res);
	arg_stack_count--;
	eax = res;
	pc++; goto NEXTOP;
}
CASE(OPBFASTCALL) {
	DBG_P("OPBFASTCALL : ");
	int res = VirtualMachine_run(local_cache_func_vmcode);
	DBG_P("res = [%d]", res);
	arg_stack_count--;
	ebx = res;
	pc++; goto NEXTOP;
}
CASE(OPCFASTCALL) {
	DBG_P("OPCFASTCALL : ");
	int res = VirtualMachine_run(local_cache_func_vmcode);
	DBG_P("res = [%d]", res);
	arg_stack_count--;
	ecx = res;
	pc++; goto NEXTOP;
}
CASE(OPDFASTCALL) {
	DBG_P("OPDFASTCALL : ");
	int res = VirtualMachine_run(local_cache_func_vmcode);
	DBG_P("res = [%d]", res);
	arg_stack_count--;
	edx = res;
	pc++; goto NEXTOP;
}
CASE(OPARET) {
	DBG_P("OPARET : ");
	//eax = eax;
	DBG_P("arg_stack_count = [%d]", arg_stack_count);
	DBG_P("eax = [%d]", eax);
	return eax;
}
CASE(OPBRET) {
	DBG_P("OPBRET : ");
	eax = ebx;
	DBG_P("arg_stack_count = [%d]", arg_stack_count);
	return eax;
}
CASE(OPCRET) {
	DBG_P("OPCRET : ");
	eax = ecx;
	DBG_P("arg_stack_count = [%d]", arg_stack_count);
	return eax;
}
CASE(OPDRET) {
	DBG_P("OPDRET : ");
	eax = edx;
	DBG_P("arg_stack_count = [%d]", arg_stack_count);
	return eax;
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
