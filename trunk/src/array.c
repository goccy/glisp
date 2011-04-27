#include <glisp.h>

static void VirtualMachineCodeArray_add(VirtualMachineCodeArray *array, VirtualMachineCode *c)
{
	array->a[array->size] = c;
	array->size++;
}

static VirtualMachineCodeArray *VirtualMachineCodeArray_copy(VirtualMachineCodeArray *from, int base_offset)
{
	VirtualMachineCodeArray *f = from;
	f->a += base_offset;
	VirtualMachineCodeArray *ret = new_VirtualMachineCodeArray();
	VirtualMachineCode *nop = new_VirtualMachineCode(NULL, 0);
	nop->op = OPNOP;
	nop->opnext = from->a[-2]->opnext;
	ret->add(ret, nop);
	size_t i = 0;
	VirtualMachineCode *r = new_VirtualMachineCode(NULL, 0);//OPRET
	r->opnext = from->a[-1]->opnext;
	ret->add(ret, r);
	for (i = 1; i < from->size - 1; i++) {
		ret->add(ret, f->a[i]);
	}
	ret->size--;
	return ret;
}

static void VirtualMachineCodeArray_reverse(VirtualMachineCodeArray *array)
{
	size_t i, j = 0;
	VirtualMachineCode *tmp[MAX_STACK_SIZE] = {NULL};
	for (i = 0; i < array->size; i++) {
		tmp[i] = array->a[i];
	}
	for (i = 0, j = array->size - 1; i < array->size; i++, j--) {
		array->a[i] = tmp[j];
	}
}

static void VirtualMachineCodeArray_dump(VirtualMachineCodeArray *array)
{
	int i = 0;
	while (array->a[i] != NULL) {
		DBG_PL("L%d : ", i);
		array->a[i]->api->dump(array->a[i]);
		i++;
	}
}

static void VirtualMachineCodeArray_remove(VirtualMachineCodeArray *array, int num)
{
	(array->a[num])->api->delete(array->a[num]);
	size_t i = 0;
	for (i = num; i < array->size - 1; i++) {
		array->a[i] = array->a[i + 1];
	}
	array->a[i] = NULL;
	array->size--;
}

static VirtualMachineCode _code[MAX_STACK_SIZE];
static VirtualMachineCode *VirtualMachineCodeArray_getPureCode(VirtualMachineCodeArray *array)
{
	size_t i = 0;
	for (i = 0; i < array->size; i++) {
		_code[i] = *array->a[i];
	}
	VirtualMachineCode *code = (VirtualMachineCode *)gmalloc(sizeof(VirtualMachineCode) * MAX_STACK_SIZE);
	memcpy(code, _code, sizeof(VirtualMachineCode) * MAX_STACK_SIZE);
	memset(_code, 0, sizeof(VirtualMachineCode) * MAX_STACK_SIZE);
	return code;
}

VirtualMachineCodeArray *new_VirtualMachineCodeArray(void)
{
	VirtualMachineCodeArray *vm_array = (VirtualMachineCodeArray *)gmalloc(sizeof(VirtualMachineCodeArray));
	vm_array->a = (VirtualMachineCode **)gmalloc(sizeof(VirtualMachineCode) * MAX_STACK_SIZE);
	//vm_array->a[MAX_STACK_SIZE] = {NULL};
	vm_array->size = 0;
	vm_array->add = VirtualMachineCodeArray_add;
	vm_array->copy = VirtualMachineCodeArray_copy;
	vm_array->dump = VirtualMachineCodeArray_dump;
	vm_array->remove = VirtualMachineCodeArray_remove;
	vm_array->reverse = VirtualMachineCodeArray_reverse;
	vm_array->getPureCode = VirtualMachineCodeArray_getPureCode;
	return vm_array;
}
