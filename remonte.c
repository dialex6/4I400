#define XOPEN_SOURCE 700

#define NB_PARAMETERS 3
#define BAD_PARAMETERS -1

#define NO_CHILD -1
#define CHILD 0
#define CHILD_SUCCESS 0
#define CHILD_FAIL -1

#define BUFFER_LEN 1
#define RW 00600

#include <stdio.h> /* perror, fprintf, sprintf */
#include <errno.h> /* errno */
#include <stdlib.h>  /* atoi, EXIT_SUCCESS, EXIT_FAILLURE, rand */
#include <sys/types.h> /* open, lseek */
#include <sys/stat.h> /* open, fchmod */
#include <fcntl.h> /* open */
#include <unistd.h> /* read, write, lseek, dup */
#include <string.h> /* memset */

int main(int argc, char** argv) {
	pid_t fork_return = NULL;
	pid_t waitpid_return = NULL;
	int error = 0;
	int waitpid_status = 0;
	int fprintf_return = 0;
	int sprintf_return = 0;
	int nb_fork = 0;
	int close_return = 0;
	int file_to_read = 0;
	int file_to_write = 0;
	int user_nb_fork = 0;
	int valeur_aleatoire = 0;
	int somme = 0;
	ssize_t read_return = 0;
	ssize_t write_return = 0;
	char read_buffer[BUFFER_LEN] = "";
	char write_buffer[BUFFER_LEN] = "";
	off_t lseek_return = 0;

	if (argc != NB_PARAMETERS) {
		fprintf_return = fprintf(stdout, "Usage: %s <nb de processus fils> <fichier de communication> \n", argv[0]);
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
	file_to_write = open(argv[2], O_CREAT | O_RDWR | O_TRUNC, RW);
	if (file_to_write == -1) {
		error = errno;
		perror("Error open");
		return error;
	}
	for (nb_fork = 0; nb_fork < user_nb_fork; nb_fork++) {
		fork_return = fork();
		if (fork_return == -1) {
			error = errno;
			perror("Error fork");
			if (nb_fork == 0)
				return CHILD_FAIL;
			return error;
		}
		else if (fork_return == CHILD) {
			valeur_aleatoire = (int)(10 * (float)rand() / RAND_MAX);
			sprintf_return = sprintf(write_buffer, "%d", valeur_aleatoire);
			write_return = write(file_to_write, (const void*)write_buffer, (size_t)sprintf_return);
			if (write_return == -1) {
				error = errno;
				perror("Error write");
				return error;
			}
			write_return = write(file_to_write, "\0", strlen("\0"));
			if (write_return == -1) {
				error = errno;
				perror("Error write");
				return error;
			}
			close_return = close(file_to_write);
			if (close_return == -1) {
				error = errno;
				perror("Error close");
				return error;
			}
			exit(CHILD_SUCCESS);
		}
	}
	for (nb_fork = 0; nb_fork < user_nb_fork; nb_fork++) {
		waitpid_return = waitpid(-1, &waitpid_status, 0);
		if (waitpid_return == -1) {
			error = errno;
			perror("Error wait");
			return error;
		}
	}
	file_to_read = dup(file_to_write);
	if (file_to_read == -1) {
		error = errno;
		perror("Error dup");
		return error;
	}
	close_return = close(file_to_write);
	if (close_return == -1) {
		error = errno;
		perror("Error close");
		return error;
	}
	lseek_return = lseek(file_to_read, 0, SEEK_SET);
	if (lseek_return == -1) {
		error = errno;
		perror("Error lseek");
		return error;
	}
	while ((read_return = read(file_to_read, (void*)memset((void*)read_buffer, 0, (size_t)BUFFER_LEN), (size_t)BUFFER_LEN)) != 0) {
		if (read_return == -1) {
			error = errno;
			perror("Error read");
			return error;
		}
		valeur_aleatoire = atoi((const char*)read_buffer);
		somme += valeur_aleatoire;
	}
	close_return = close(file_to_read);
	if (close_return == -1) {
		error = errno;
		perror("Error close");
		return error;
	}
	fprintf_return = fprintf(stdout, "Somme: %d \n", somme);
	return EXIT_SUCCESS;
}