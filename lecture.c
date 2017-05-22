#define XOPEN_SOURCE 700

#define NB_PARAMETERS 3
#define BAD_PARAMETERS -1
#define CHILD 0
#define CHILD_SUCCESS 0

#define POSIX_IO "-p"
#define LIBC_IO "-c"
#define BUFFER_LEN 1
#define READ "r"
#define FIN_DE_CHAINE '\0'

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char** argv) {
	int error = 0;
	int fprintf_return = 0;
	int file_to_read = 0;
	int fgetc_return = 0;
	FILE* file_to_fread = NULL;
	int close_return = 0;
	pid_t fork_return = NULL;
	ssize_t read_return = 0;
	unsigned char read_buffer[BUFFER_LEN + 1] = "";

	if (argc != NB_PARAMETERS) {
		fprintf_return = fprintf(stdout, "Usage: %s <option: -p -c> <fichier a lire> \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}
	read_buffer[BUFFER_LEN + 1] = FIN_DE_CHAINE;
	if (strcmp(argv[1], POSIX_IO) == 0) {
		file_to_read = open(argv[2], O_RDONLY);
		if (file_to_read == -1) {
			error = errno;
			perror("Error open");
			return error;
		}
		fork_return = fork();
		if (fork_return == -1) {
			error = errno;
			perror("Error fork");
			return error;
		}
		if (fork_return == CHILD) {
			fork_return = fork();
			if (fork_return == -1) {
				error = errno;
				perror("Error fork");
				return error;
			}
			if (fork_return == CHILD) {
				while ((read_return = read(file_to_read, (void*)memset((void*)read_buffer, 0, (size_t)BUFFER_LEN), (size_t)BUFFER_LEN)) != 0) {
					if (read_return == -1) {
						error = errno;
						perror("Error read");
						return error;
					}
					fprintf_return = fprintf(stdout, "chaine lue: %s \n", (unsigned char*)read_buffer);
					if (fprintf_return < 0) {
						perror("Error fprintf");
						return fprintf_return;
					}
				}
				return CHILD_SUCCESS;
			}
			while ((read_return = read(file_to_read, (void*)memset((void*)read_buffer, 0, (size_t)BUFFER_LEN), (size_t)BUFFER_LEN)) != 0) {
				if (read_return == -1) {
					error = errno;
					perror("Error read");
					return error;
				}
				fprintf_return = fprintf(stdout, "chaine lue: %s \n", (unsigned char*)read_buffer);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
			}
			return CHILD_SUCCESS;
		}
		while ((read_return = read(file_to_read, (void*)memset((void*)read_buffer, 0, (size_t)BUFFER_LEN), (size_t)BUFFER_LEN)) != 0) {
			if (read_return == -1) {
				error = errno;
				perror("Error read");
				return error;
			}
			fprintf_return = fprintf(stdout, "chaine lue: %s \n", (unsigned char*)read_buffer);
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
		}
		close_return = close(file_to_read);
		if (close_return == -1) {
			error = errno;
			perror("Error close");
			return error;
		}
	}
	if (strcmp(argv[1], LIBC_IO) == 0) {
		file_to_fread = fopen(argv[2], READ);
		if (file_to_fread == NULL) {
			error = errno;
			perror("Error open");
			return error;
		}
		fork_return = fork();
		if (fork_return == -1) {
			error = errno;
			perror("Error fork");
			return error;
		}
		if (fork_return == CHILD) {
			fork_return = fork();
			if (fork_return == -1) {
				error = errno;
				perror("Error fork");
				return error;
			}
			if (fork_return == CHILD) {
				while ((fgetc_return = fgetc(file_to_fread)) != EOF) {
					fprintf_return = fprintf(stdout, "Petit fils, Caractere lu: %c \n", (unsigned char)fgetc_return);
					if (fprintf_return < 0) {
						perror("Error fprintf");
						return fprintf_return;
					}
				}
				return CHILD_SUCCESS;
			}
			while ((fgetc_return = fgetc(file_to_fread)) != EOF) {
				fprintf_return = fprintf(stdout, "Fils, caractere lu: %c \n", (unsigned char)fgetc_return);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
			}
			return CHILD_SUCCESS;
		}
		while ((fgetc_return = fgetc(file_to_fread)) != EOF) {
			fprintf_return = fprintf(stdout, "Pere, caractere lu: %c \n", (unsigned char)fgetc_return);
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
		}
		close_return = close(file_to_read);
		if (close_return == -1) {
			error = errno;
			perror("Error close");
			return error;
		}
	}
	return EXIT_SUCCESS;
}