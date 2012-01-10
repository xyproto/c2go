/* fibo.c -- It prints out the first N Fibonacci
 *           numbers.
 */

#include <stdio.h>

int main(void) {
    int n;        /* The number of fibonacci numbers we will print */
    int i;        /* The index of fibonacci number to be printed next */ 
    int current;  /* The value of the (i)th fibonacci number */
    int next;     /* The value of the (i+1)th fibonacci number */
    int twoaway;  /* The value of the (i+2)th fibonacci number */

    printf("How many Fibonacci numbers do you want to compute? ");
    scanf("%d", &n);
    if (n<=0)
       printf("The number should be positive.\n");
    else {
      printf("\n\n\tI \t Fibonacci(I) \n\t=====================\n");
      next = current = 1;
      for (i=1; i<=n; i++) {
	printf("\t%d \t   %d\n", i, current);
	twoaway = current+next;
	current = next;
	next    = twoaway;
      }
    }
}

/* The output from a run of this program was:

How many Fibonacci numbers do you want to compute? 9

	I 	 Fibonacci(I) 
	=====================
	1 	   1
	2 	   1
	3 	   2
	4 	   3
	5 	   5
	6 	   8
	7 	   13
	8 	   21
	9 	   34

*/
