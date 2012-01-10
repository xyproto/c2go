/* factor1.c -- It prompts the user to enter an integer N. It prints out
 *        if it is a prime or not. If not, it prints out all of its
 *        proper factors.
 */

#include <stdio.h>

int main(void) {
  int n, 
    lcv, 
    flag; /* flag initially is 1 and becomes 0 if we determine that n
	     is not a prime */
 
  printf("Enter value of N > ");
  scanf("%d", &n);
  for (lcv=2, flag=1; lcv <= (n / 2); lcv++) {
    if ((n % lcv) == 0) {
      if (flag)
	printf("The non-trivial factors of %d are: \n", n);
      flag = 0;
      printf("\t%d\n", lcv);
    }
  }
  if (flag)
    printf("%d is prime\n", n);
}
