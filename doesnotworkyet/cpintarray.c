/* cpintarray.c -- Example showing how addresses and arrays are alike
 */

#include <stdio.h>
#define SIZE 8

void cpIntArray(int *a, int *b, int n)
/*It copies n integers starting at b into a*/
{
  for(;n>0;n--)
    *a++=*b++;
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

int main(void){
  int x[SIZE], nx;
  int y[SIZE], ny;

  printf("Read the x array:\n");
  nx = getIntArray(x,SIZE,0);
  printf("The x array is:\n");
  printIntArray(x,nx);

  printf("Read the y array:\n");
  ny = getIntArray(y,SIZE,0);
  printf("The y array is:\n");
  printIntArray(y,ny);

  cpIntArray(x+2,y+3,4);
  /*Notice the expression 'x+2'. x is interpreted as the address for
    the beginning of the x array. +2 sais to increment that address
    by two units, in accordance with the type of x, which is
    an integer array. Thus we move from x to two integer locations
    past it, that is to the location of x[2]. The same reasoning applied
    to 'y+3'.
    */
  printf("Printing x after having copied 4 elements\n"
         "from y starting at y[3] into x starting at x[2]\n");
  printIntArray(x,nx);
}

/* Here is the interaction in a run of this program:

Read the x array:
Enter integer [0 to terminate] : 1
Enter integer [0 to terminate] : 3
Enter integer [0 to terminate] : 5
Enter integer [0 to terminate] : 7
Enter integer [0 to terminate] : 9
Enter integer [0 to terminate] : 11
Enter integer [0 to terminate] : 13
Enter integer [0 to terminate] : 15
Enter integer [0 to terminate] : 0
The x array is:
	1 	3 	5 	7 	9 
	11 	13 	15 
Read the y array:
Enter integer [0 to terminate] : 2
Enter integer [0 to terminate] : 4
Enter integer [0 to terminate] : 6
Enter integer [0 to terminate] : 8
Enter integer [0 to terminate] : 10
Enter integer [0 to terminate] : 12
Enter integer [0 to terminate] : 14
Enter integer [0 to terminate] : 16
Enter integer [0 to terminate] : 0
The y array is:
	2 	4 	6 	8 	10 
	12 	14 	16 
Printing x after having copied 4 elements
from y starting at y[3] into x starting at x[2]
	1 	3 	8 	10 	12 
	14 	13 	15 

 */
