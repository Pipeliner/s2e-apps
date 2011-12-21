#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "s2e.h"


int main(int argc, char **argv)
{
	int SIZE = 5;
	char *name = 0;
	if (argc > 1) {
		name = argv[1];
	} else {
		name = "/mnt/symbolic";
	}
	char data[SIZE];
	s2e_make_symbolic(&data, sizeof(data), "File data");
#ifdef _WIN32
	int fd = open(name, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, S_IRWXU);
#else
	int fd = creat(name, S_IRWXU);
#endif

	if (fd == -1) {
		fprintf(stderr, "Cannot create file %s\n", name);
		exit(1);
	}

	int written = write(fd, data, SIZE);
	if (written != SIZE) {
		fprintf(stderr, "Can not write to file\n");
		exit(1);
	}

	printf("Symbolic file %s of size %d was written successfully\n", name, SIZE);
}
