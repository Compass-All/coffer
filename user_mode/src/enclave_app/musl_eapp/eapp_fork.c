#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	pid_t pid = 0;
	pid = fork();
    printf("fork return val = %d\n\r", pid);
    fflush(stdout);
	exit(0);
	// _exit(0);
	// return 0;
}
