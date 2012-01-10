/* sieve.c - It prompts the user to enter an integer N. It prints out
 *           It prints out all the primes up to N included.
 *           It uses a sieve method. As primes are found they are
 *           stored in an array PRIMES and used as possible factors
 *           for the next potential prime.
 */

#include <stdio.h>

#define NPRIMES  1000
#define FALSE 0
#define TRUE  1


int main(void) {
  int n;
  int i,j;
  int flag;
  int primes[NPRIMES]; /*It will contain the primes smaller than n
                        *that we have already encountered*/
  int level;           /*1+Number of primes currently in PRIMES*/

  /*Introduction*/
  printf("Enter value of N > ");
  scanf("%d",&n);
  level = 0;

  /*Main body*/
  for(i=2;i<=n;i++) {
    for(j = 0, flag = TRUE; j<level && flag; j++)
      flag = (i%primes[j]);
    if (flag) { /*I is a prime */
      printf("%12d\n", i);
      if (level < NPRIMES)
	primes[level++] = i;
    }
  }
}
