/* string1.c  -- Simple string operations
                 String literals. 
		 printf, scanf, %s, %c
		 strlen
		 strcpy
		 strcmp
 */

#include <stdio.h>
#define MAXBUFF 128

int main(void) {
  char c[] = "012345";
  char line[MAXBUFF];
  int lcv;
  int cmp;

  printf("sizeof(c)= %d\n", sizeof(c));
  printf("sizeof(line)= %d\n", sizeof(line));
  for (lcv=0; lcv<=strlen(c); lcv++)
    printf("c[lcv]= %d = %c\n",c[lcv],c[lcv]);
  printf("Please enter a string : ");
  scanf("%s",line);
  printf("strlen(line) = %d\n", strlen(line));
  printf("line = [%s]\n",line);
  cmp = strcmp(c,line);
  if(cmp<0)
    printf("%s is less than %s\n", c, line);
  else if (cmp==0)
    printf("%s is equal to %s\n", c, line);
  else
    printf("%s is greater than %s\n", c, line);
  strcpy(line,c);   /*copy the string c into line */
  cmp = strcmp(c,line);
  if(cmp<0)
    printf("%s is less than %s\n", c, line);
  else if (cmp==0)
    printf("%s is equal to %s\n", c, line);
  else
    printf("%s is greater than %s\n", c, line); 
}

/* The output of this program:

sizeof(c)= 7
sizeof(line)= 128
c[lcv]= 48 = 0
c[lcv]= 49 = 1
c[lcv]= 50 = 2
c[lcv]= 51 = 3
c[lcv]= 52 = 4
c[lcv]= 53 = 5
c[lcv]= 0 = 
Please enter a string : roses are red
strlen(line) = 5
line = [roses]
012345 is less than roses
012345 is equal to 012345


 */
