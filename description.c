#define XOPEN_SOURCE 700

#define CHILD 0
#define CHILD_SUCCESS 0
#define CHILD_FAIL -1

#include <unistd.h> /* fork, exit */
#include <stdio.h> /* perror, fprintf */
#include <errno.h> /* errno */
#include <stdlib.h> /* atoi, EXIT_SUCCESS, EXIT_FAILLURE */
#include <sys/types.h> /* wait */
#include <sys/wait.h> /* wait */

int nfork(int n) {
	pid_t fork_return = NULL;
	pid_t wait_return = NULL;
	int error = 0;
	int wait_status = 0;
	int fprintf_return = 0;
	int nb_fork = 0;

	for (nb_fork = 0; nb_fork < n; nb_fork++) {
		fork_return = fork();
		if (fork_return == -1) {
			perror("Error fork");
			if (nb_fork == 0)
				return CHILD_FAIL;
			return nb_fork;
		}
		else if (fork_return == CHILD) {
			fprintf_return = fprintf(stdout, "Fils PID, numero de fils: %d, %d \n", getpid(), nb_fork + 1);
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
			exit(CHILD_SUCCESS);
		}
		fprintf_return = fprintf(stdout, "Pere PID, nb de fils crees: %d, %d \n", getpid(), nb_fork + 1);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
	}
	for (nb_fork = 0; nb_fork < n; nb_fork++) {
		wait_return = wait(&wait_status);
		if (wait_return == -1) {
			error = errno;
			perror("Error wait");
			return error;
		}
	}
	fprintf_return = fprintf(stdout, "J'ai cree un total de: %d fils \n", nb_fork);
	if (fprintf_return < 0) {
		perror("Error fprintf");
		return fprintf_return;
	}
	return nb_fork;
}

/* REPEAT_NFORK */
int main(void) {
	int p;
	int i = 1; int N = 3;
	int fprintf_return = 0;
	int nb_processus = 1;
	do {
		p = nfork(i);
		nb_processus += p;
		if (p != 0)
			printf("%d \n", p);
	} while ((p == 0) && (++i <= N));
	fprintf_return = fprintf(stdout, "nb de processus: %d \n", nb_processus);
	if (fprintf_return < 0) {
		perror("Error fprintf");
		return fprintf_return;
	}
	printf("FIN\n");
	return EXIT_SUCCESS;
}
