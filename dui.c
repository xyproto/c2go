#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <ctype.h>

/*
 * "Delete unversioned items"
 *
 * This file does not convert too well, so far
 */

#define STRING_LENGTH 2000
#define True 0
#define False -1

#if STAT_MACROS_BROKEN
    #undef S_ISDIR
#endif

#if !defined S_ISDIR && defined S_IFDIR
    #define S_ISDIR(Mode) (((Mode) & S_IFMT) == S_IFDIR)
#endif

int yes(void) {
	return 1;
}

/*  Checks if a file/path is a directory, returns 0 or 1 */
int isdir(char* filename)
{
    static struct stat stats;
    return (stat(filename, &stats) == True) && (S_ISDIR(stats.st_mode));
}

/*  Checks if a file/path can be read, returns 0 or 1 */
int canread(char* filename)
{
    return access(filename, R_OK) == True;
}

/*  Checks if a file/directory can be executed, returns 0 or 1 */
int canx(char* filename)
{
    return access(filename, X_OK) == True;
}

/*  Runs the svn st command and deletes the files that fit */
void run(char* command)
{
    FILE *fp;

    char cmd[STRING_LENGTH];
    char filename[STRING_LENGTH];
    char line[STRING_LENGTH];
    char curdir[STRING_LENGTH];
    char short_filename[STRING_LENGTH];
    char slash[2];

    int result;
    int i;
    int firstletterpos;

    #ifdef __linux__
        strcpy(slash, "/");
    #else
        strcpy(slash, "\\");
    #endif

    getcwd(curdir, STRING_LENGTH);
    /* curdir = (curdir += slash) */
    strcat(curdir, slash);
    
    fp = popen(command, "r");
    while (fgets(line, sizeof line, fp))
    {
        if ((line[0] == '?') || (line[0] == 'M') || (line[0] == 'I'))
        {
            /* Remove the newline at the end of the string */
            line[strlen(line) - 1] = '\0';
            /* filename (points to filename) = curdir (points to curdir) */
            strcpy(filename, curdir);
            /* find the first letter */
            firstletterpos = 8;
            for (i=1; i <= 8; i++)
            {
                if (line[i] != ' ')
                {
                    #ifdef CENTOS
                        firstletterpos = i - 1;
                    #else
                        firstletterpos = i;
                    #endif
                }
            }
            /* short filename */
            strcpy(short_filename, line + firstletterpos);
            /* filename (points to filename) += line[8:] */
            strcat(filename, short_filename);
            if (canread(filename) && !isdir(filename))
            {
                printf("Deleting %s\n", short_filename);
                #ifdef __linux__
                    sprintf(cmd, "rm %s", filename);
                #else
                    sprintf(cmd, "del %s", filename);
                #endif
                /* printf("cmd #1: %s\n", cmd); */
                result = system(cmd);
            }
            else
            {
                printf("Deleting %s%s\n", short_filename, slash);
                /* Try deleting the directory */
                #ifdef __linux__
                    sprintf(cmd, "rm -rf %s", filename);
                #else
                    sprintf(cmd, "rd %s /S /Q", filename);
                #endif
                result = system(cmd);
                if (result != True)
                {
                    printf("Deleting %s failed.\n", filename);
                }
            }
        }
    }
    pclose(fp);
}

/*  Test function */
void test_canread_filename(char* filename)
{
    if (canread(filename) == True)
        printf("Can read %s (access)\n", filename);
    else
        printf("Can not read %s (access)\n", filename);
}

/*  Test function */
void test_canread()
{
    #ifdef __linux__
        test_canread_filename("/proc/cpuinfo");
    #else
        test_canread_filename("c:\\windows\\explorer.exe");
    #endif
}

/*  Test function */
void test_macro_linux()
{
    #ifdef __linux__
        printf("linux\n");
    #else
        printf("not linux\n");
    #endif
}

/*  The main function, asks the user for confirmation */
int main()
{
    char answer;
    printf("Delete unversioned items? [Yn]");
    answer = tolower(getc(stdin));
    if (answer != 'n')
    {
        run("svn st --non-interactive --no-ignore");
        return True;
    }
    else
    {
        return False;
    }
}

