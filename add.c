/* add.c -- Read a sequence of positive integers and print them 
 *          out together with their sum. Use a Sentinel value
 *          (say 0) to determine when the sequence has terminated.
 */

#include <stdio.h>
#define SENTINEL 0

int main(void) {
  int sum = 0; /* The sum of numbers already read */
  int current; /* The number just read */

  do {
    printf("\nEnter an integer > ");
    scanf("%d", &current);
    if (current > SENTINEL)
      sum = sum + current;
  } while (current > SENTINEL);
  printf("\nThe sum is %d\n", sum);
}
