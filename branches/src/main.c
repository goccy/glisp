#include <glisp.h>

int main(int argc, char **argv)
{
	if (argc < 2) {
		glisp_start_shell();
	} else {
		glisp_start_script(argv[1]);
	}
	return 0;
}
