/* misc.c -- various C constructs */

#include <stdio.h>



int main(void) {
  int answer;
  short x = 1;
  long y = 2;
  float u = 3.0;
  double v = 4.4;
  long double w = 5.54;
  char c = 'p';;

  /* __DATE__, __TIME__, __FILE__, __LINE__ are predefined symbols */
  printf("Date : %s\n", __DATE__);
  printf("Time : %s\n", __TIME__);
  printf("File : %s\n", __FILE__);
  printf("Line : %d\n", __LINE__);
  printf("Enter 1 or 0 : ");
  scanf("%d", &answer);

  /* answer?"you said yes":"You said no" is a conditional expression */
  printf("%s\n", answer?"You said YES":"You said NO");

  /* The size of various types */
  printf("The size of int %d\n", sizeof(answer));
  printf("The size of short %d\n", sizeof(x));
  printf("The size of long %d\n", sizeof(y));
  printf("The size of float %d\n", sizeof(u));
  printf("The size of double %d\n", sizeof(v));
  printf("The size of long double %d\n", sizeof(w));
  printf("The size of char %d\n", sizeof(c));

}

/*
  The output from a run was:

Date : Feb 11 1997
Time : 13:51:31
File : white.c
Line : 20
Enter 1 or 0 : 1 
You sayd YES
The size of int 4
The size of short 2
The size of long 8
The size of float 4
The size of double 8
The size of long double 8
The size of char 1


 */
