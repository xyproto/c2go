/* scope1.c -- Simple example showing effects of the scope rules
 */

#include <stdio.h>

int a=0;    /* This is a global variable */

void foo(void);

int main(void) {
  int a=2;  /* This is a variable local to main */
  int b=3;  /* This is a variable local to main */

  printf("1. main_b = %d\n", b);
  printf("main_a = %d\n", a);
  foo();
  printf("2. main_b = %d\n", b);
}

void foo(void){
  int b=4;  /* This is a variable local to foo */

  printf("foo_a = %d\n", a);
  printf("foo_b = %d\n", b);
}
