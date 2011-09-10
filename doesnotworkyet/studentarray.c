/* studentarray.c - Reads a file containing a sequence of records
 *                  representing students, places them into an
 *                  array, then writes that array out to a new files.
 *                  The names of the files are passed in as command
 *                  line parameters.
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


void writeStudentArray(char filename[], student a[], int n)
     /* n is the number of elements in the array a.
      * filename is the name of the file where we will
      * write.
      */
{
  FILE *fd;  /* File descriptor used for filename */
  int i;
  
  if(n<=0)
    return;
  if((fd=fopen(filename,"w"))==NULL){
    perror("fopen");
    exit(1);
  }
  for (i=0;i<n;i++){
    fprintf(fd,"%s %d %d %d\n",
	    a->name, a->midterm, a->final, a->homeworks);
    a++;
  }
  fclose(fd);
}

int readStudentArray(char filename[], student a[], int nmax)
     /* It reads up to nmax student records 
      * from file filename and stores them in a.
      * It returns the number of records actually read.
      */
{
  FILE *fd;  /* File descriptor used for filename */
  int i=0;
  
  if((fd=fopen(filename,"r"))==NULL){
    perror("fopen");
    exit(1);
  }
  while(fscanf(fd,"%s %d %d %d",
	       a->name, &a->midterm, &a->final, &a->homeworks)!=EOF){
    if(++i==nmax)break;  /* We have filled up the table */
    a++;
  }
  fclose(fd);
  return i;
}

int main(int argc, char *argv[]){
  int n;
  student table[SIZE];

  if(argc!=3){
    printf("Usage: %s infile outfile\n", argv[0]);
    exit(0);
  }
  n = readStudentArray(argv[1],table,SIZE);
  writeStudentArray(argv[2],table,n);
}
