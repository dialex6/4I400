#define XOPEN_SOURCE 700

#define NB_PARAMETERS 4
#define BAD_PARAMETERS -1

#define RW 00600
#define BUFFER_LEN 1
#define SEPARATEUR ' '
#define FIN_DE_CHAINE '\0'

#include <stdio.h> /* perror, fprintf, sprintf */
#include <errno.h> /* errno */
#include <sys/types.h> /* open, lseek, lstat, stat */
#include <sys/stat.h> /* open, fchmod, lstat, stat, chmod */
#include <fcntl.h> /* open */
#include <string.h> /* memset, strcmp, strlen */
#include <stdlib.h> /* atoi, EXIT_SUCCESS, EXIT_FAILLLURE, rand */
#include <unistd.h> /* read, write, lseek, dup, stat, lstat, unlink */

int main(int argc, char** argv) {
	int error = 0;
	int arg_i = 0;
	int fprintf_return = 0;
	int read_return = 0;
	int write_return = 0;
	int close_return = 0;
	int file_to_write = 0;
	int file_to_read = 0;
	int char_i = 0;
	char read_buffer[BUFFER_LEN + 1] = "";
	off_t lseek_return = 0;

	if (argc < NB_PARAMETERS) {
		fprintf_return = fprintf(stdout, "Usage: %s <fichier a creer> <mot de remplacement> <suite de mots a ecrire> \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}
	file_to_write = open(argv[1], O_EXCL | O_CREAT | O_WRONLY, RW);
	if (file_to_write == -1) {
		error = errno;
		perror("Error open");
		return error;
	}
	arg_i = 3;
	while (arg_i < argc) {
		write_return = write(file_to_write, (const void*)argv[arg_i], strlen(argv[arg_i]));
		if (write_return == -1) {
			error = errno;
			perror("Error write");
			return error;
		}
		write_return = write(file_to_write, " ", strlen(" "));
		if (write_return == -1) {
			error = errno;
			perror("Error write");
			return error;
		}
		fprintf_return = fprintf(stdout, "%s ", argv[arg_i]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		arg_i += 1;
	}
	fprintf(stdout, "\n");
	close_return = close(file_to_write);
	if (close_return == -1) {
		error = errno;
		perror("Error close");
		return error;
	}
	file_to_read = open(argv[1], O_RDWR);
	if (file_to_read == -1) {
		error = errno;
		perror("Error open");
		return error;
	}
	read_buffer[BUFFER_LEN + 1] = FIN_DE_CHAINE;
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
		for (char_i = 0; char_i < BUFFER_LEN; char_i++) {
			if ((unsigned char)read_buffer[char_i] == SEPARATEUR) {
				file_to_write = dup(file_to_read);
				if (file_to_read == -1) {
					error = errno;
					perror("Error dup");
					return error;
				}
				close_return = close(file_to_read);
				if (close_return == -1) {
					error = errno;
					perror("Error close");
					return error;
				}
				lseek_return = lseek(file_to_write, (off_t)char_i + 1, SEEK_SET);
				if (lseek_return == -1) {
					error = errno;
					perror("Error lseek");
					return error;
				}
				write_return = write(file_to_write, (const void*)argv[2], strlen(argv[2]));
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
				return EXIT_SUCCESS;
			}
		}
	}
	return EXIT_SUCCESS;	
}