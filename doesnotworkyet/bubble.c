/* bubble.c -- Read an integer array, print it, then sort it and
 * print it. Use the bubble sort method.
 */

#include <stdio.h>

#define NMAX 10

int getIntArray(int a[], int nmax, int sentinel);
void printIntArray(int a[], int n);
void bubbleSort(int a[], int n);

int main(void) {
  int x[NMAX];
  int hmny;
  int who;
  int where;

  hmny = getIntArray(x, NMAX, 0);
  if (hmny==0)
    printf("This is the empty array!\n");
  else{
    printf("The array was: \n");
    printIntArray(x,hmny);
    bubbleSort(x,hmny);
    printf("The sorted array is: \n");
    printIntArray(x,hmny);
  }
}

void printIntArray(int a[], int n)
     /* n is the number of elements in the array a.
      * These values are printed out, five per line. */
{
  int i;

  for (i=0; i<n; ){
    printf("\t%d ", a[i++]);
    if (i%5==0)
      printf("\n");
  }
  printf("\n");
}

int getIntArray(int a[], int nmax, int sentinel)
     /* It reads up to nmax integers and stores then in a; sentinel 
      * terminates input. */
{
  int n = 0;
  int temp;

  do {
    printf("Enter integer [%d to terminate] : ", sentinel);
    scanf("%d", &temp);
    if (temp==sentinel) break;
    if (n==nmax)
      printf("array is full\n");
    else 
      a[n++] = temp;
  }while (1);
  return n;
}


void bubbleSort(int a[], int n)
/* It sorts in non-decreasing order the first N positions of A. It uses 
 * the bubble sort method.
 */
{
  int lcv;
  int limit = n-1;
  int temp;
  int lastChange;
  
  while (limit) {
    lastChange = 0;
    for (lcv=0;lcv<limit;lcv++)
      /* Notice that the values in positions LIMIT+1 .. N are in
       * their final position, i.e. they are sorted right */
	if (a[lcv]>a[lcv+1]) {
	  temp = a[lcv];
	  a[lcv] = a[lcv+1];
	  a[lcv+1] = temp;
	  lastChange = lcv;
	}
    limit = lastChange;
  }
}
