/* linear.c -- Read an integer array and then do linear searches.
 */

#include <stdio.h>

#define NMAX 10

int getIntArray(int a[10], int nmax, int sentinel);
void printIntArray(int a[10], int n);
int linear(int a[], int n, int who);

int main(void) {
  int x[NMAX];
  int hmny;
  int who;
  int where;

  hmny = getIntArray(x, NMAX, 0);
  printf("The array was: \n");
  printIntArray(x,hmny);
  printf("Now we do linear searches on this data\n");
  do{
    printf("Enter integer to search for [0 to terminate] : ");
    scanf("%d", &who);
    if(who==0)break;
    where = linear(x,hmny,who);
    if (where<0){
      printf("Sorry, %d is not in the array\n",who);
    }else
      printf("%d is at position %d\n",who,where);
  }while(1);
}

void printIntArray(int a[10], int n)
     /* n is the number of elements in the array a.
      * These values are printed out, five per line. */
{
  int i;

  for (i=0; i<n; ){
    printf("\t%d ", a[++i]);
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
    else 
      a[n++] = temp;
  }while (1);
  return n;
}

int linear(int a[10], int n, int who)
     /* Given the array a with n elements, searches for who.
      * It returns its position if found, otherwise it returns
      * -1.
      */
{
  int lcv;
  for (lcv=0;lcv<n;lcv++)
    if(who == a[lcv]) return lcv;
  return (-1);
}
