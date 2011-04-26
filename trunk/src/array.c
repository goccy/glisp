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
	size_t i = 0;
	ret->add(ret, new_VirtualMachineCode(NULL, 0));//OPRET
	for (i = 1; i < from->size - 1; i++) {
		ret->add(ret, f->a[i]);
	}
	return ret;
}

static void VirtualMachineCodeArray_dump(VirtualMachineCodeArray *array)
{
	int i = 0;
	while (array->a[i] != NULL) {
		DBG_P("L%d : ", i);
		array->a[i]->dump(array->a[i]);
		i++;
	}
}

static void VirtualMachineCodeArray_remove(VirtualMachineCodeArray *array, int num)
{
	(array->a[num])->delete(array->a[num]);
	size_t i = 0;
	for (i = num; i < array->size - 1; i++) {
		array->a[i] = array->a[i + 1];
	}
	array->a[i] = NULL;
	array->size--;
}

VirtualMachineCodeArray *new_VirtualMachineCodeArray(void)
{
	VirtualMachineCodeArray *vm_array = (VirtualMachineCodeArray *)gmalloc(sizeof(VirtualMachineCodeArray));
	vm_array->a = (VirtualMachineCode **)gmalloc(sizeof(VirtualMachineCode) * MAX_STACK_SIZE);
	vm_array->size = 0;
	vm_array->add = VirtualMachineCodeArray_add;
	vm_array->copy = VirtualMachineCodeArray_copy;
	vm_array->dump = VirtualMachineCodeArray_dump;
	vm_array->remove = VirtualMachineCodeArray_remove;
	return vm_array;
}
