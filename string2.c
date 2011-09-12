/* string2.c  -- Compacting sequences of spaces in a string.
                 We use two different methods
 */

#include <stdio.h>
#define MAXBUFF 128

int getline(char line[], int nmax);
int compact1(char line[]);
int compact2(char line[]);

int main(void) {
  char buffer1[MAXBUFF];
  char buffer2[MAXBUFF];
  int len;

  len = getline(buffer1, MAXBUFF);
  printf("You entered : %s\n", buffer1);
  strcpy(buffer2,buffer1);
  printf("Which is : %s\n", buffer2);

  len=compact1(buffer1);
  printf("compact1: len=%d,  %s\n",len, buffer1);
  len=compact2(buffer2);
  printf("compact2: len=%d,  %s\n",len, buffer2);
}

int getline(char line[], int nmax)
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

int compact1(char line[])
     /* It replaces streaks of spaces in line by a 
      * single space. It returns lenght of resulting string.
      */
{
  int cursor=0;      /* Cursor on the line */
  int prevspace = 0; /* True iff preceding position was with a space */
  int lcv=0;         /* Other cursor */

  if(line[cursor]=='\0')
    return 0;
  do{
    if((line[cursor]==' ')&&prevspace){
      /*If we have a space preceded by a space, move rest of string
	left one position */
      for(lcv=cursor;line[lcv];lcv++)
	line[lcv]=line[lcv+1];
    }else
      prevspace=(line[cursor++]==' ');
  }while(line[cursor]);
  return cursor;
}

int compact2(char line[])
     /* It replaces streaks of spaces in line by a 
      * single space. It returns lenght of resulting string.
      */
{
  int cursor=0;      /* Cursor on the line */
  int prevspace = 0; /* True iff preceding position was with a space */
  int lcv = 0;       /* Where we copy characters to */

  do{
    if(!((line[cursor]==' ')&&prevspace)){
      line[lcv++]=line[cursor];
      prevspace=(line[cursor]==' ');
    }
  }while(line[cursor++]);
  return(lcv-1); /*We need the -1 since it counts also the '\0' */
}
