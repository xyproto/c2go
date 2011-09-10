/* sortmerge.c - Reads a file containing a sequence of records
 *                representing students and places them into an
 *                array table1. It does the same for a second file
 *                placing result in table2. It then sorts table1 
 *                and table2 in non descending order on the basis 
 *                of their name fields. It now stores the merge of
 *                the two sorted sequences in an array table3.
 *                Finally it writes the result to a third file.
 *                The names of the three files are passed as
 *                command line parameters
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
      * WARNING: We assume that the name of the students contain no spaces.
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

void sortStudentArray(student table[], int n){
  /* It sorts in nondecreasing order on the basis of their
   * names the first n records of table.
   * WARNING: THIS IS A NULL STUB.
   */
}

int mergeStudentArray(student table3[], student table1[], int n1, 
		      student table2[], int n2){
  /* It merges into table3 the first n1 student records of table1
   * and the first n2 student records of table2. The records in
   * table1 and table2 are sorted in nondecreasing order of their 
   * name fields. It returns n1+n2.
   * WARNING: THIS IS A STUB WHERE WE JUST COPY, NOT MERGE.
   */
  int i,j=0;

  for(i=0;i<n1;i++){
    table3[j++] = table1[i];
  }
  for(i=0;i<n2;i++){
    table3[j++] = table2[i];
  }
  return n1+n2;
}

int main(int argc, char *argv[]){
  int n1,     /* Number of students in first sequence */
    n2,       /* Number of students in second sequence */
    n3;       /* Number of students in resulting sequence */

  student table1[SIZE],  /* Students in first sequence */
    table2[SIZE],        /* Students in second sequence */
    table3[SIZE+SIZE];   /* Students in merged sequence */

  if(argc!=4){
    printf("Usage: %s unsfile1 unsfile2 outfile\n", argv[0]);
    exit(0);
  }
  n1 = readStudentArray(argv[1],table1,SIZE);
  sortStudentArray(table1,n1);
  n2 = readStudentArray(argv[2],table2,SIZE);
  sortStudentArray(table2,n2);
  n3 = mergeStudentArray(table3, table1, n1, table2, n2);
  writeStudentArray(argv[3],table3,n3);
}
