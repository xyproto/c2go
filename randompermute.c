/* randompermute.c - A program will generate "random permutations of n elements"
   if at all points the n! possible permutations have all the same probability 
   of being generated.
*/

#include <stdio.h>
#include <stdlib.h>

// It returns a random permutation of 0..n-1
int * rpermute(int n) {
    // a := make([]int, n)
    int *a = malloc(n*sizeof(int));
    int k;
    for (k = 0; k < n; k++)
	a[k] = k;
    for (k = n-1; k > 0; k--) {
	int j = rand() % (k+1);
	int temp = a[j];
	a[j] = a[k];
	a[k] = temp;
    }
    return a;
}

// Print a 8 elements per line
void printarray(int n, int a[n]) {
    int k = 0;
    for (k = 0; k < n; k++) {
	printf("%6d   ", a[k]);
	if (k % 8 == 7)
	    printf("\n");
    } 
}

int main(void) {
    int limit = 6;
    int *a;
    int k;
    // Print 7 permutations
    for (k = 0; k < 7; k++) {
	a = rpermute(limit);
	printarray(limit, a);
	printf("\n");
    }
    
    return 0;
}
