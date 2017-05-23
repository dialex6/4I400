#define XOPEN_SOURCE 700

#define NB_PARAMETERS 3
#define BAD_PARAMETERS -1

#include <stdio.h> /* perror, fprintf, sprintf, fopen, fgetc */
#include <errno.h> /* errno */
#include <sys/stat.h> /* open, fchmod, lstat, stat */
#include <sys/types.h> /* open, lseek, lstat, stat, wait */
#include <unistd.h> /* read, write, lseek, dup, lstat, stat, fork, exit, sleep, execv, execl */
#include <stdlib.h> /* atoi, EXIT_SUCCESS, EXIT_FAILLURE, rand */
#include <string.h> /* memset, strcmp */

int main(int argc, char**argv) {
	int error = 0;
	int fprintf_return = 0;
	int stat_return = 0;
	int lstat_return = 0;
	struct stat stat_file1;
	struct stat stat_file2;
	struct stat lstat_file1;
	struct stat lstat_file2;
	if (argc != NB_PARAMETERS) {
		fprintf(stdout, "Usage: %s <fichier 1 a comparer> <fichier 2 a comparer> \n", argv[0]);
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		return BAD_PARAMETERS;
	}
	lstat_return = lstat(argv[1], &lstat_file1);
	if (lstat_return == -1) {
		error = errno;
		perror("Error lstat");
		return error;
	}
	lstat_return = lstat(argv[2], &lstat_file2);
	if (lstat_return == -1) {
		error = errno;
		perror("Error lstat");
		return error;
	}
	if ((lstat_file1.st_dev == lstat_file2.st_dev) && (lstat_file1.st_ino == lstat_file2.st_ino)) {
		fprintf_return = fprintf(stdout, "C'est le meme fichier ! \n");
		if (fprintf_return < 0) {
			perror("Error fprintf");
			return fprintf_return;
		}
		if (strcmp(argv[1],argv[2]) != 0) {
			fprintf_return = fprintf(stdout, "C'est un lien physique ! \n");
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
		}
		return EXIT_SUCCESS;
	}
	else if (S_ISLNK(lstat_file1.st_mode) && ~(S_ISLNK(lstat_file2.st_mode))) {
		stat_return = stat(argv[1], &stat_file1);
		if (stat_return == -1) {
			error = errno;
			perror("Error stat");
			return error;
		}
		if ((stat_file1.st_dev == lstat_file2.st_dev) && (stat_file1.st_ino == lstat_file2.st_ino)) {
			fprintf_return = fprintf(stdout, "%s est un lien symbolique vers %s ! \n", argv[1], argv[2]);
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
		}
		return EXIT_SUCCESS;
	}
	else if (S_ISLNK(lstat_file2.st_mode) && ~(S_ISLNK(lstat_file2.st_mode))) {
		stat_return = stat(argv[2], &stat_file2);
		if (stat_return == -1) {
			error = errno;
			perror("Error stat");
			return error;
		}
		if ((lstat_file1.st_dev == stat_file2.st_dev) && (lstat_file1.st_ino == stat_file2.st_ino)) {
			fprintf_return = fprintf(stdout, "%s est un lien symbolique vers %s ! \n", argv[2], argv[1]);
			if (fprintf_return < 0) {
				perror("Error fprintf");
				return fprintf_return;
			}
		}
		return EXIT_SUCCESS;
	}
	else if (S_ISLNK(lstat_file1.st_mode) && S_ISLNK(lstat_file2.st_mode)) {
		if ((stat_file1.st_dev == stat_file2.st_dev) && (stat_file1.st_ino == stat_file2.st_ino)) {
			fprintf_return = fprintf(stdout, "Ce sont des liens symboliques sur le meme fichier ! \n");
			if (fprintf_return < 0) {
				perror("Error fpintf");
				return fprintf_return;
			}
		}
		return EXIT_SUCCESS;
	}
	fprintf_return = fprintf(stdout, "Ce n'est pas le meme fichier ! \n");
	if (fprintf_return < 0) {
		perror("Error fpintf");
		return fprintf_return;
	}
	return EXIT_SUCCESS;
}