/* counts.c - It contains a program that is given as command parameter
 *            the name of a text file, say, temp.dat. It will read each line
 *            of temp.dat and print out in correspondence the number of
 *            characters and words on that line. At the end it will print
 *            out the number of lines that were read.
 */

#include <stdio.h>

int main (int argc, char *argv[]){
  FILE *fp;
  int nchars, nwords, nlines;
  int lastnblank;    /* 0 iff the last character was a space */
  char c;

  if(argc!=2){
    printf("Usage: %s filename\n", argv[0]);
    exit(0);
  }
  if((fp=fopen(argv[1],"r"))==NULL){
    perror("fopen");
    exit(0);
  }
  nchars=nwords=nlines=lastnblank=0;
  while((c=getc(fp))!=EOF){
    nchars++;
    if (c=='\n'){
      if (lastnblank)
	nwords++;
      printf("words=%d, characters=%d\n", nwords, nchars);
      nchars=nwords=lastnblank=0;
      nlines++;
    }else{
      if (((c==' ')||(c=='\t'))&(lastnblank))
	nwords++;
      lastnblank=((c!=' ')&&(c!='\t'));
    }
  }
  printf("lines=%d\n", nlines);
  fclose(fp);
}
