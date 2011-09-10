/* getline.c -- Testing a function that reads a line 
 *              from input.
 */

#include <stdio.h>
#define MAXBUF 128

int getline2(char line[], int nmax);

int main(void){
  int len;
  char buffer[MAXBUF];

  while(1){
    len = getline2(buffer, MAXBUF);
    if (len==0)break;
    printf("len = %d, line = %s\n", len, buffer);
  };
}

int getline2(char line[], int nmax)
     /* It prompts user and reads up to nmax 
      * characters into line. It returns number 
      * of characters read. ['\n' terminates the line]
      */
{
  int len;
  char c;

  len = 0;
  printf("Enter a string [CR to exit]: ");
  while(((c=getchar())!='\n') && len<nmax-1)
    line[len++]=c;
  line[len]='\0';
  return len;
}
