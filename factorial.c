/* factorial.c -- It computes repeatedly the factorial of an integer entered 
 *        by the user. It terminates when the integer entered is not
 *        positive.
 */

#include <stdio.h>

int fact(int n);

int main(void) {
  int current;

  printf("Enter a positive integer [to terminate enter non-positive] > ");
  scanf("%d", &current);
  while (current > 0) {
    printf("The factorial of %d is %d\n", current, fact(current)); 
    printf("Enter a positive integer [to terminate enter non-positive] > ");
    scanf("%d", &current);
  }
}

/* n is a positive integer. The function returns its factorial */
int fact(int n) {
  int lcv;    /* loop control variable */
  int p;      /* set to the product of the first lcv positive integers */

  for(p=1, lcv=2; lcv <= n; p=p*lcv, lcv++);
  return p;
}
