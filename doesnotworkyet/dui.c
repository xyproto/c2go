/*
 * Delete unversioned items
 *
 * Small utility that really should have been a script instead.
 * It has worked fine for five years, though.
 * Works on both Linux and Windows.
 *
 * Alexander F Rødseth <xyproto@archlinux.org>, 2015-10-15
 *
 * Public Domain
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#define STRING_LENGTH 2000

#if STAT_MACROS_BROKEN
#undef S_ISDIR
#endif

#if !defined S_ISDIR && defined S_IFDIR
#define S_ISDIR(Mode) (((Mode) & S_IFMT) == S_IFDIR)
#endif

// Checks if a file/path is a directory, returns 0 or 1
int isdir(char *filename)
{
	static struct stat stats;
	return (stat(filename, &stats) == true) && (S_ISDIR(stats.st_mode));
}

// Checks if a file/path can be read, returns 0 or 1
int canread(char *filename)
{
	return access(filename, R_OK) == true;
}

// Checks if a file/directory can be executed, returns 0 or 1
int canx(char *filename)
{
	return access(filename, X_OK) == true;
}

// Runs the svn st command and deletes the files that fit
void run(char *command)
{
	FILE *fp;

	char cmd[STRING_LENGTH];
	char filename[STRING_LENGTH];
	char line[STRING_LENGTH];
	char curdir[STRING_LENGTH];
	char short_filename[STRING_LENGTH];
	char slash[2];

	int result;

#ifdef __linux__
	strcpy(slash, "/");
#else
	strcpy(slash, "\\");
#endif

	getcwd(curdir, STRING_LENGTH);
	strcat(curdir, slash);

	fp = popen(command, "r");
	while (fgets(line, sizeof line, fp)) {
		if ((line[0] == '?') || (line[0] == 'M') || (line[0] == 'I')) {
			// Remove the newline at the end of the string
			line[strlen(line) - 1] = '\0';
			// filename (points to filename) = curdir (points to curdir)
			strcpy(filename, curdir);
			// short filename
			strcpy(short_filename, line + 8);
			// filename (points to filename) += line[8:]
			strcat(filename, short_filename);
			if (canread(filename) && !isdir(filename)) {
				printf("Deleting %s\n", short_filename);
#ifdef __linux__
				sprintf(cmd, "rm %s", filename);
#else
				sprintf(cmd, "del %s", filename);
#endif
				result = system(cmd);
			} else {
				printf("Deleting %s%s\n", short_filename, slash);
				// Try deleting the directory
#ifdef __linux__
				sprintf(cmd, "rm -rf %s", filename);
#else
				sprintf(cmd, "rd %s /S /Q", filename);
#endif
				result = system(cmd);
				if (result != true) {
					printf("Deleting %s failed.\n", filename);
				}
			}
		}
	}
	pclose(fp);
}

int main(int const argc, const char **const argv)
//  The main function, asks the user for confirmation
{
	printf("Delete unversioned items? [Yn]");
	char answer = tolower(getc(stdin));
	if (answer != 'n') {
		run("svn st --non-interactive --no-ignore");
		return true;
	}
	return false;
}

