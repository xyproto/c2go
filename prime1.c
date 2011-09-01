/* prime1.c  It prompts the user to enter an integer N. It prints out
 *           if it is a prime or not. If not, it prints out a factor of N.
 */

#include <stdio.h>

int main(void) {
  int n;
  int i;
  int flag;

  printf("Enter value of N > ");
  scanf("%d", &n);
  flag = 1;
  for (i=2; (i<(n/2)) && (flag > 0); ) { /* May be we do not need to test
			values of i greater than the square root of n? */
    if ((n % i) == 0) /* If true n is divisible by i */
      flag = 0;
    else
      i++;
  }
 
  if (flag)
    printf("%d is prime\n", n);
  else
    printf("%d has %d as a factor\n", n, i);
  return 0;
}
