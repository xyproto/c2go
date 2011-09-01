/* addn.c -- Read a positive number N. Then read N integers and
 *           print them out together with their sum.
 */

#include <stdio.h>

int main(void) {
  int n;       /* The number of numbers to be read */
  int sum;     /* The sum of numbers already read  */
  int current; /* The number just read             */
  int lcv;     /* Loop control variable, it counts the number
                  of numbers already read */

  printf("Enter a positive number n > "); 
  scanf("%d",&n); /* We should check that n is really positive*/
  sum = 0;
  for (lcv=0; lcv < n; lcv++) {
    printf("\nEnter an integer > ");
    scanf("%d",&current);
    /*    printf("\nThe number was %d\n", current); */
    sum = sum + current;
  }
  printf("The sum is %d\n", sum);
  return 0;
}
