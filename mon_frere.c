#define XOPEN_SOURCE 700

#define NB_PARAMETERS 1
#define BAD_PARAMETERS -1
#define CHILD 0
#define CHILD_SUCCESS 0

#define NB_CHILD 2

#include <unistd.h> /* fork, exit, sleep, execv, execl */
#include <stdio.h> /* perror, fprintf */
#include <errno.h> /* errno */
#include <stdlib.h> /* atoi, EXIT_SUCCESS, EXIT_FAILLURE */
#include <sys/types.h> /* wait */
#include <sys/wait.h> /* wait */

int main(int argc, char** argv) {
	pid_t fork_return = NULL;
	pid_t wait_return = NULL;
	pid_t fils1 = NULL;
	int error = 0;
	int wait_status = 0;
	int fprintf_return = 0;
	int main_nb_fork = 0;
	int child_nb_fork = 0;

	if (argc != NB_PARAMETERS) {
		fprintf_return = fprintf(stdout, "Usage: %s \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}

	for (main_nb_fork = 0; main_nb_fork < NB_CHILD; main_nb_fork++) {
		fork_return = fork();
		if (fork_return == -1) {
			error = errno;
			perror("Error fork");
			return error;
		}
		else if (fork_return == CHILD) {
			for (child_nb_fork = 0; child_nb_fork < NB_CHILD; child_nb_fork++) {
				fork_return = fork();
				if (fork_return == -1) {
					error = errno;
					perror("Error fork");
					return error;
				}
				else if (fork_return == CHILD) {
					fprintf_return = fprintf(stdout, "Petit fils PID, PPID: %d, %d \n", getpid(), getppid());
					if (fprintf_return < 0) {
						perror("Error fprintf");
						return fprintf_return;
					}
					exit(CHILD_SUCCESS);
				}
			}
			for (child_nb_fork = 0; child_nb_fork < NB_CHILD; child_nb_fork++)
			{
				wait_return = wait(&wait_status);
				if (wait_return == -1) {
					error = errno;
					perror("Error wait");
					return error;
				}
			}
			if (main_nb_fork == 1) {
				fprintf_return = fprintf(stdout, "Fils PID, PPID, PID de mon frere: %d, %d, %d \n", getpid(), getppid(), fils1);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
			}
			else {
				fprintf_return = fprintf(stdout, "Fils PID, PPID: %d, %d \n", getpid(), getppid());
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
			}
			exit(CHILD_SUCCESS);
		}
		if (main_nb_fork == 0) {
			fils1 = fork_return;
		}
	}
	for (main_nb_fork = 0; main_nb_fork < NB_CHILD; main_nb_fork++)
	{
		wait_return = wait(&wait_status);
		if (wait_return == -1) {
			error = errno;
			perror("Error wait");
			return error;
		}
	}
	fprintf_return = fprintf(stdout, "Pere PID: %d \n", getpid());
	if (fprintf_return < 0) {
		perror("Error fprintf");
		return fprintf_return;
	}
	return EXIT_SUCCESS;
}