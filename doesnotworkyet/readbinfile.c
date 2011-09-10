/* readbinfile.c - Reads a file containing a sequence of binary records
 *                 and writes it out to standard output.  [The input
 *                 file was created by makebinfile.c]
 */

#include <stdio.h>

#define SIZE 10
#define NAMESIZE 25

typedef struct {
  char name[NAMESIZE];
  int  midterm;
  int final;
  int homeworks;
} student;

int readastudent(FILE *fdin, student * who){
  /* Read from an open binary file fdin into who
   * and return the number of bytes read or EOF if at end
   * of file 
   */
  char * p;        /* Cursor in inputting a byte at a time */
  char * limit = ((char *)who)+sizeof(student); /*Address just past who */

  for (p=(char *)who; p<limit; p++){
    if(feof(fdin)) return EOF;
    *p = fgetc(fdin);
  }
  return (p - (char *)who);
}

int main(int argc, char *argv[]){
  int n = 0;         /* Number of records read */
  student who;       /* Buffer for a record */
  int val;
 
  FILE *fdin;  /* File descriptor for input file */
  
  if(argc!=2){
    printf("Usage: %s infile\n", argv[0]);
    exit(0);
  }
  
  if((fdin=fopen(argv[1],"r"))==NULL){
    perror("fopen");
    exit(1);
  }

  while((val=readastudent(fdin, &who))!=EOF){
    printf("%s %d %d %d\n", 
	   who.name, who.midterm, who.final, who.homeworks);
    n++;
  }

  printf("n=%d\n", n);

  fclose(fdin);
}
