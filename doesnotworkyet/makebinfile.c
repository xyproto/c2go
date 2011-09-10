/* makebinfile.c - Reads a file containing a sequence of text records
 *                 and writes it out to a new binary files.
 *                 The names of the files are passed in as command
 *                 line parameters.
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

int writeastudent(FILE *fdout, student * who){
  /* Write to an open binary file fdout the content of who.
   * Return the number of bytes that were written out.
   */

  char * p;          /* Cursor in outputting a byte at a time */
  char * limit = ((char *)who)+sizeof(student); /*Address just past who */
  
  for (p=(char *)who;p<limit;p++){
    fputc(*p, fdout);
  }
  return (limit - (char *)who);
}

int main (int argc, char *argv[]){
  int n = 0;         /* Number of records read */
  int m;             /* Number of bytes in a record */
  student who;       /* Buffer for a record */
  
  FILE *fdin;  /* File descriptor for input file */
  FILE *fdout; /* File descriptor for output file */
  
  if(argc!=3){
    printf("Usage: %s infile outfile\n", argv[0]);
    exit(0);
  }
  
  if((fdin=fopen(argv[1],"r"))==NULL){
    perror("fopen");
    exit(1);
  }

  if((fdout=fopen(argv[2],"w"))==NULL){
    perror("fopen");
    exit(1);
  }

  while(fscanf(fdin,"%s %d %d %d",
	       who.name, &who.midterm, &who.final, &who.homeworks)!=EOF){
    m = writeastudent(fdout, &who);
    printf("m=%d\n", m);
    n++;
  }

  printf("n=%d\n", n);

  fclose(fdin);
  fclose(fdout);

}
