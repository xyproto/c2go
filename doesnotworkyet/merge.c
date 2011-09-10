/* merge.c -- Given two sorted sequences of integers, it creates
 *            a sorted sequence consisting of all their numbers.
 */

#include <stdio.h>

#define NMAX 10

void printIntArray(int a[], int n);
void merge(int c[], int *nc, int a[], int na, int b[], int nb);

int main(void) {
  int x[NMAX] = {1,3,5,6,7}; /* The first sorted sequence */
  int y[NMAX] = {2,3,4}; /* The second sorted sequence */
  int z[NMAX+NMAX]; /* The merge sequence */
  int nz;

  merge(z,&nz,x,5,y,3);
  printIntArray(z,nz);
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

void merge(int c[], int *nc, int a[], int na, int b[], int nb){
  /* Given sorted sequences a and b, respectively with na and nb
   * elements, it stores their merge sequence in c and returns 
   * the total number of elements in nc
   */
  int cursora, cursorb, cursorc;

  cursora=cursorb=cursorc=0;

  while((cursora<na)&&(cursorb<nb))
    if (a[cursora]<=b[cursorb])
      c[cursorc++]=a[cursora++];
    else
      c[cursorc++]=b[cursorb++];

  while(cursora<na)
    c[cursorc++]=a[cursora++];

  while(cursorb<nb)
    c[cursorc++]=b[cursorb++];

  *nc = cursorc;
}
