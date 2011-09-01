/* array2.c -- Read/writing/reversing integer arrays
 */

#include <stdio.h>

#define NMAX 10

void intSwap(int *x, int *y);
int getIntArray(int a[], int nmax, int sentinel);
void printIntArray(int a[], int n);
void reverseIntArray(int a[], int n);

int main(void) {
  int x[NMAX];
  int hmny;

  /* C2GO */
  printf("This program compiles, but the array-conversion needs more work!\n\n");

  hmny = getIntArray(x, NMAX, 0);
  printf("The array was: \n");
  printIntArray(x,hmny);
  reverseIntArray(x,hmny);
  printf("after reverse it is:\n");
  printIntArray(x,hmny);
}

void intSwap(int *x, int *y)
     /* It swaps the content of x and y */
{
  int temp = *x;
  *x = *y;
  *y = temp;
}

void printIntArray(int a[10], int n)
     /* n is the number of elements in the array a.
      * These values are printed out, five per line. */
{
  int i;

  for (i=0; i<n; ){
    printf("\t%d ", a[i]);
    i++;
    if (i%5==0)
      printf("\n");
  }
  printf("\n");
}

int getIntArray(int a[10], int nmax, int sentinel)
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
    else {
      a[n] = temp;
      n++;
    }
  }while (1);
  return n;
}

void reverseIntArray(int a[10], int n)
     /* It reverse the order of the first n elements of a */
{
  int i;

  for(i=0;i<n/2;i++){
    intSwap(&a[i],&a[n-i-1]);
  }
}
