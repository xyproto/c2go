/* clean.c -- Given as command line parameter a filename,
 *            it removes from that file all occurrences of ^M
 *            If 'clean' is the executable image of this
 *            program, you can use it as follows:
 *                 % clean dirtyfile > cleanfile
 */

#include <stdio.h>
#define CONTROLM 13

int main(int argc, char *argv[]){
  char c;
  FILE *fd;

  if(argc!=2){
    printf("Usage: %s filename\n", argv[0]);
    exit(0);
  }
  if((fd = fopen(argv[1],"r"))==NULL){
    perror("fopen");
    exit(1);
  }
  while((c=getc(fd))!=EOF)
    if (c!=CONTROLM)
      putchar(c);
  fclose(fd);
}
