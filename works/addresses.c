/* addresses.c -- Playing with addresses of variables and their contents:
 *                what is done by C with variables, addresses, and values.
 */

#include <stdio.h>

void moo(int a, int * b);

int main(void) {
  int x;
  int *y;

  x=1;
  y=&x;
  printf("Address of x = %d, value of x = %d\n", &x, x);
  printf("Address of y = %d, value of y = %d, value of *y = %d\n", &y, y, *y);
  moo(9,y);
}

void moo(int a, int *b){
  printf("Address of a = %d, value of a = %d\n", &a, a);
  printf("Address of b = %d, value of b = %d, value of *b = %d\n", &b, b, *b);
}

/* Output from running this program on my computer:

Address of x = 536869640, value of x = 1
Address of y = 536869632, value of y = 536869640, value of *y = 1
Address of a = 536869608, value of a = 9
Address of b = 536869600, value of b = 536869640, value of *b = 1

 */
