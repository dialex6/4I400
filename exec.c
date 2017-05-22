#define XOPEN_SOURCE 700

#include <unistd.h> /* fork, exit */
#include <stdio.h> /* perror, fprintf */
#include <errno.h> /* errno */
#include <stdlib.h> /* atoi, EXIT_SUCCESS, EXIT_FAILLURE */
#include <sys/types.h> /* wait */
#include <sys/wait.h> /* wait */

int main(void) {
	pid_t pid;
	pid_t main_pid = NULL;
	int  j = 0; int i = 0;
	/* int error = 0; */
	/* int exec_return = 0; */
	int fprintf_return = 0;
	int nb_processus = 1;
	main_pid = getpid();
	while (i < 2) {
		i++;
		if ((pid = fork()) == -1) {
			perror("fork");
			return errno;
		}
		else if (pid == 0) {
			j = i;
			fprintf_return = fprintf(stdout, "Fils PID, PPID: %d, %d \n", getpid(), getppid());
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
		}
		if (getpid() == main_pid) {
			nb_processus = 2 * nb_processus;
		}
	}
	if (j == 2) {
		sleep(2);
		/* exec_return = execv("/bin/sleep", (char*[]){"/bin/sleep", "2", NULL})
		if (exec_return == -1) {
			error = errno;
			perror("Error execv");
			return error; */
		/* j = execl("/bin/sleep", "sleep", "2", NULL);
		if (j == -1)
			perror("execl");
		printf("%d retour sleep %d\n", getpid(), errno); */
	}
	else {
		printf("%d fils\n", (i - j));
		while (j < i) {
			j++;
			wait(NULL);
		}
	}
	if (getpid() == main_pid) {
		fprintf_return = fprintf(stdout, "nb de processus: %d \n", nb_processus);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
	}
	return EXIT_SUCCESS;
}