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
void f1() {
	i++;
	i++;
	i++;
	MSG("Hi from f1()\n");
	char shortbuf[40];
	memcpy(shortbuf, longbuf, 1000);
	i--;

}
int main(int argc, char **argv) {
	MSG("main() start\n");
	i++;
	i++;
	i++;
	i++;
	i++;
	i++;
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
}

