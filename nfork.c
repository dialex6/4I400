#define XOPEN_SOURCE 700

#define NB_PARAMETERS 2
#define BAD_PARAMETERS -1
#define NO_CHILD -1
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
			fprintf_return = fprintf(stdout, "Fils: %d \n", nb_fork + 1);
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
			exit(CHILD_SUCCESS);
		}
		fprintf_return = fprintf(stdout, "J'ai cree: %d fils \n", nb_fork + 1);
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
	return nb_fork;
}

int main(int argc, char** argv) {
	int user_nb_fork = 0;
	int nfork_return = 0;
	int fprintf_return = 0;

	if (argc != NB_PARAMETERS) {
		fprintf_return = fprintf(stdout, "Usage: %s <nb de processus fils> \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}
	user_nb_fork = atoi(argv[1]);
	if (user_nb_fork == 0) {
		return NO_CHILD;
	}
	nfork_return = nfork(user_nb_fork);
	if (nfork_return <= 0)
		return EXIT_FAILURE;
	return nfork_return;
}