/* hmw10.c -- Outline of solution for homework 10
 */

#include <stdio.h>

#define ARRAYSIZE 2000
#define LINESIZE 65

typedef char linetype[LINESIZE];

int getline(FILE * fd, char buff[], int nmax){
  /* It reads a line from fd and stores up to nmax of 
   * its characters to buff.
   */
  char c;
  int n=0;

  while ((c=getc(fd))!='\n'){
    if(c==EOF)return EOF;
    if(n<nmax)
      buff[n++]=c;
  }
  buff[n]='\0';
  return n;
}

int getStringArray(char *filename, linetype table[], int nmax)
     /* It reads up to nmax lines from file called filename and
      * stores them into table. It returns the numbers of lines read
      */
{
  FILE *fdu;  /* File descriptor used for filename */
  int i, j;
  char c;

  if((fdu=fopen(filename,"r"))==NULL){
    perror("fopen");
    exit(1);
  }
  i=j=0;
  while((c=getc(fdu))!=EOF){
    if(i==nmax)break;  /* We have filled up the table */
    if (c=='\n'){ /* We have finished reading a line */
      table[i++][j]='\0';
      j=0;
    }else if(j<LINESIZE)
      table[i][j++]=c;
  }
  fclose(fdu);
  return i;
}

void writeStringArray(char *filename, linetype table[], int n)
     /* It writes the first n lines of table into the file
      * called filename
      */
{
  FILE *fdu;  /* File descriptor used for filename */
  int i;
  
  if((fdu=fopen(filename,"w"))==NULL){
    perror("fopen");
    exit(1);
  }
  for(i=0;i<n;i++)
    fprintf(fdu, "%s\n", table[i]);
  fclose(fdu);
}

void stringBubble(linetype table[], int n){
  /* This function sorts the first n lines of table.
   * Here you see just the prototype, or stub, of the function.
   */
}

int stringBinary(linetype table[], int n, char who[]){
  /* It searches the first n positions of table looking for who.
   * It uses binary search. It returns the position where who is
   * found, or -1 if not there. This is just the prototype, or stub, 
   * of the function.
   */
  return -1;
}

int main(void){
  int n;
  linetype linearray[ARRAYSIZE];
  linetype who;
  int where;
  FILE *fdw;  /* FILE descriptor used for "who.dat" */
  FILE *fda;  /* File descriptor used for "answers.dat" */
  
  /* Read unsorted.dat into linearray and return number of lines read */
  n = getStringArray("unsorted.dat", linearray, ARRAYSIZE);
  if (n==0){
    printf("The unsorted.dat file is empty. Good bye.\n");
    exit(0);
  }
  /* Sort the first n lines of linearray */
  stringBubble(linearray,n);
  /* Save the sorted array to sorted.dat */
  writeStringArray("sorted.dat",linearray,n);
  /* Open for reading who.dat */
  if((fdw=fopen("who.dat","r"))==NULL){
    perror("fopen");
    exit(1);
  }
  /* Open for writing answers.dat */
  if((fda=fopen("answers.dat","w"))==NULL){
    perror("fopen");
    exit(1);
  }
  while (getline(fdw,who,LINESIZE-1)!=EOF){   /* Read a line from "who.dat" */
    where = stringBinary(linearray,n,who);    /* See where it is in table */
    fprintf(fda,"%d\n",where);            /* Write position to "answer.dat*/
  }
  fclose(fdw);
  fclose(fda);
}
