#include "s2e.h"
#include <string.h>
#include <stdio.h>
//#define host
#ifdef host
	#define MSG(str) {printf(str); }
#else
	#define MSG(str) {printf(str); s2e_message(str); }
#endif
int i;
char longbuf[1000];
void dummy_my_first_function() {}
void f1() {
	i++;
	i++;
	i++;
	MSG("Hi from f1()\n");
	char shortbuf[40];
	memcpy(shortbuf, longbuf, 1000);
	i--;

}
void dummy_my_last_function();
int main(int argc, char **argv) {
	rc_enter_main();
	MSG("main() start\n");
	/*
	s2e_enable_forking();
	s2e_moduleexec_add_module("manually_instrumented", argv[0], 0);
	s2e_codeselector_select_module("manually_instrumented");
	s2e_rawmon_loadmodule2(argv[0], dummy_my_first_function, dummy_my_first_function, 0, dummy_my_last_function-dummy_my_first_function, 0);
	*/
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	//s2e_make_symbolic(&argc, sizeof(argc), "argc");
	if (argc > 1) {
		i++;
		i++;
		f1();
	}
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
	MSG("main() end\n");
	rc_leave_main();
}
void dummy_my_last_function() {}
