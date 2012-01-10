/* Generating random number sequences using the formula (linear congruence)
	x[k+1] = (a*x[k] + c)mod m
   where a, c, and m are parameters set by the user and passed as command line 
   parameters together with a seed i.e. x[0]
   As a simple example try  a=7, c=1, m=13, and seed=5
   A more sophisticated selection would be a=69069, c=0, 
   m=2^32=4294967296, and seed=31
   It will print out, in a sort of random order, up to m-1 distinct values. 
   Then it loops.
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static long seed = 13;
static long a;
static long c;
static long m;

void random_init(long s) {
    if (s != 0) seed = s;
}

long random() {
	seed = (a*seed + c)%m;
	return seed;
    }

int main(int argc, char * argv[]) {
    if (argc != 5) {
	printf("usage: %s a, c, m, seed\n", argv[0]);
	return 1;
    }
    a = atoi(argv[1]);
    c = atoi(argv[2]);
    m = atoi(argv[3]);
    int s = atoi(argv[4]);
    random_init(s);
    int k;
    for (k = 0; k < m-1; k++) { 
	printf("%8d", random());
	if (k % 8 == 7) { // after 8 elements go to a new line
	    printf("\n");
	    sleep(1); // sleep for a second
	} 
    }
    printf("\n");
    return 0;
}
