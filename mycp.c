#define XOPEN_SOURCE 700

#define NB_PARAMETERS 3
#define BAD_PARAMETERS -1

#define BUFFER_LEN 1024
#define RW 00600


#include <stdio.h> /* perror, fprintf */
#include <errno.h> /* errno */
#include <stdlib.h>  /* atoi, EXIT_SUCCESS, EXIT_FAILLURE */
#include <sys/types.h> /* open, wait */
#include <sys/stat.h> /* open, fchmod */
#include <fcntl.h> /* open */
#include <unistd.h> /* fork, exit, read, write, sleep, execv, execl */
#include <string.h> /* memset */

int main(int argc, char** argv) {
	int error = 0;
	int fprintf_return = 0;
	/* int fchmod_return = 0; */
	int close_return = 0;
	int file_to_read = 0;
	int file_to_write = 0;
	ssize_t read_return = 0;
	ssize_t write_return = 0;
	char read_buffer[BUFFER_LEN] = "";

	if (argc != NB_PARAMETERS) {
		fprintf_return = fprintf(stdout, "Usage: %s <fichier a lire> <fichier a ecrire> \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}
	file_to_read = open(argv[1], O_RDONLY);
	if (file_to_read == -1) {
		error = errno;
		perror("Error open");
		return error;
	}
	file_to_write = open(argv[2], O_CREAT | O_EXCL | O_WRONLY | O_TRUNC, RW);
	if (file_to_write == -1) {
		error = errno;
		perror("Error open");
		return error;
	}
	/* fchmod_return = fchmod(file_to_read, RW);
	if (chmod_return == -1) {
		error = errno;
		perror("Error fchmod");
		return error; 
	}*/
	while ((read_return = read(file_to_read, (void*)memset((void*)read_buffer, 0, (size_t)BUFFER_LEN), (size_t)BUFFER_LEN)) != 0) {
		if (read_return == -1) {
			error = errno;
			perror("Error read");
			return error;
		}
		write_return = write(file_to_write, (const void*)read_buffer, (size_t)read_return);
		if (write_return == -1) {
			error = errno;
			perror("Error write");
			return error;
		}
	}
	close_return = close(file_to_read);
	if (close_return == -1) {
		error = errno;
		perror("Error close");
		return error;
	}
	close_return = close(file_to_write);
	if (close_return == -1) {
		error = errno;
		perror("Error close");
		return error;
	}
	fprintf_return = fprintf(stdout, "Fichier copie ! \n");
	if (fprintf_return < 0) {
		perror("Error fprintf");
		return fprintf_return;
	}
	return EXIT_SUCCESS;
}