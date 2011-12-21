#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "s2e.h"
int main(int argc, char **argv)
{
	int ARGSIZE = 4;
	char symarg[ARGSIZE];
	char program[] = "./md5test.sh ";
	int PROGSIZE = sizeof(program);
	s2e_make_symbolic(&symarg, sizeof(symarg), "Symbolic argument");

	int CMDSIZE = PROGSIZE + ARGSIZE;
	char cmd[CMDSIZE];
	strncpy(cmd, program, PROGSIZE);
	strncat(cmd, symarg, ARGSIZE);

	system(cmd);
}
