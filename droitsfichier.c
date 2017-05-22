#define XOPEN_SOURCE 700

#define MAX_PARAMETERS 4
#define MIN_PARAMETERS 3
#define BAD_PARAMETERS -1

#define EFFACER 'E'
#define RENOMMER 'R'
#define DROITS 'C'
#define READ_ONLY 'R'
#define READ_WRITE 'W'
#define RO 00600
#define RW 00666

#include <stdio.h> /* perror, fprintf, sprintf */
#include <errno.h> /* errno */
#include <sys/types.h> /* open, lseek, lstat, stat*/
#include <sys/stat.h> /* open, fchmod, lstat, stat, chmod */
#include <unistd.h> /* read, write, lseek, dup, stat, lstat, unlink */
#include <stdlib.h> /* atoi, EXIT_SUCCESS, EXIT_FAILLLURE, rand */
#include <string.h> /* memset, strcmp, strlen */
#include <ctype.h> /* toupper */

int main(int argc, char** argv) {
	int error = 0;
	int command = 0;
	int options = 0;
	int fprintf_return = 0;
	int lstat_return = 0;
	int unlink_return = 0;
	int rename_return = 0;
	int chmod_return = 0;
	struct stat lstat_file;

	if (argc < MIN_PARAMETERS || argc > MAX_PARAMETERS) {
		fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}
	if (strlen(argv[1]) != 1) {
		fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}
	lstat_return = lstat(argv[2], &lstat_file);
	if (lstat_return == -1) {
		error = errno;
		perror("Error lstat");
		return error;
	}
	if (S_ISDIR(lstat_file.st_mode)) {
		fprintf_return = fprintf(stdout, "C'est un repertoire ! \n");
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return EXIT_SUCCESS;
	}
	else {
		command = toupper((char)argv[1][0]);
		switch(command) {
		case EFFACER:
			if (argc != MIN_PARAMETERS) {
				fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
				return BAD_PARAMETERS;
			}
			unlink_return = unlink(argv[2]);
			if (unlink_return == -1) {
				error = errno;
				perror("Error unlink");
				return error;
			}
			fprintf_return = fprintf(stdout, "Fichier efface ! \n");
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
			return EXIT_SUCCESS;
		case RENOMMER:
			if (argc != MAX_PARAMETERS) {
				fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
				return BAD_PARAMETERS;
			}
			rename_return = rename(argv[2], argv[3]);
			if (rename_return == -1) {
				error = errno;
				perror("Error rename");
				return error;
			}
			fprintf_return = fprintf(stdout, "Fichier renomme ! \n");
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
			return EXIT_SUCCESS;
		case DROITS:
			if (argc != MAX_PARAMETERS) {
				fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
				return BAD_PARAMETERS;
			}
			if (strlen(argv[3]) != 1) {
				fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
				return BAD_PARAMETERS;
			}
			options = toupper(argv[3][0]);
			switch (options) {
			case READ_ONLY:
				chmod_return = chmod(argv[2], RO);
				if (chmod_return == -1) {
					error = errno;
					perror("Error chmod");
					return error;
				}
				fprintf_return = fprintf(stdout, "Droits modifie ! \n");
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
				return EXIT_SUCCESS;
			case READ_WRITE:
				chmod_return = chmod(argv[2], RW);
				if (chmod_return == -1) {
					error = errno;
					perror("Error chmod");
					return error;
				}
				fprintf_return = fprintf(stdout, "Droits modifie ! \n");
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
				return EXIT_SUCCESS;
			default:
				fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
				if (fprintf_return < 0) {
					perror("Error fprintf");
					return fprintf_return;
				}
				return EXIT_SUCCESS;
			}
		default:
			fprintf_return = fprintf(stdout, "Usage: %s <type operation: eE, rR, cC> <fichier a traiter> [<options: nom du fichier a renommer, rR, wW>] \n", argv[0]);
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
			return EXIT_SUCCESS;
		}
	}
}
