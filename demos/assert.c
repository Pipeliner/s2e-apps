#include <s2e.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define LEN 256
int main(int argc, char **argv)
{
	char msg[LEN];
	if (argc != 2) {
		printf("usage: %s \"COMMAND\"\n", argv[0]);
		return 1;
	}
	int ret = system(argv[1]);
	if (WIFEXITED(ret)) {
		snprintf(msg, LEN, "Normally returned (%d): %s\n", WEXITSTATUS(ret), argv[1]);
		printf("%s", msg);
		_s2e_assert(WEXITSTATUS(ret), msg);
	}
	if (WIFSIGNALED(ret)) {
		snprintf(msg, LEN, "Signaled (%d): %s\n", WTERMSIG(ret), argv[1]);
		printf("%s", msg);
		_s2e_assert(WEXITSTATUS(ret), msg);
	}
	return ret;

}
