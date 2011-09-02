/* shift.c -- It reads a sequence of positive integers. It stores it
 *            in an array, then it prompts the user to enter an
 *            integer, and then rotates clockwise the content of the 
 *            array a corresponding number of positions. The contents 
 *            the array are printed out whenever it changes.
 *            For example, if the sequence was
 *                 1,2,3,4,5
 *            and the number 3 is entered, the array becomes
 *                 3,4,5,1,2
 *            and if the number 11 is now entered, the array becomes
 *                 2,3,4,5,1
 *            and if the number -4 is now entered, the array becomes
 *                 1,2,3,4,5
 */

#include <stdio.h>
#define MAXN 8


/* It prints the current content on the array a with n elements */
void printarray(int a[8], int n);
/* I will assume first that I have a number of functions then I will 
 * write them.
 */

/* Given an array a with n values, shift rotates
 * its content m positions, clockwise.
 */
void shift(int a[8], int n, int m); 
                 
/* getint prompts the user to enter a positive integer and returns 
 * its value. 
 */                
int getint(void);

/* It requests the user to enter positive integers and stores
 * them in a. It returns in n the number of integers stored
 * in the array.
 */
void getarray(int a[8], int *n);

int main(void) {
  int table[MAXN];  /* array where we store the sequence */
  int howmany=0;    /* number of elements in sequence */
  int amount;       /* amount of shift */

  getarray(table, &howmany);
  if (howmany==0) {
    printf("Sorry, you entered the null sequence. Good bye.\n");
  }else {
    do {
      printarray(table,howmany);
      printf("By how much do you want to shift[0 to terminate]? ");
      scanf("%d",&amount);
      if (amount!=0)
	shift(table,howmany,amount);
    }while(amount!=0);
  }
}

/* Given an array a with n values, shift rotates
 * its content m positions, clockwise.
 */
  void shift(int a[8], int n, int m){
    int temp[MAXN];
    int lcv;

    if (m<0)
      m = n-(abs(m)%n);
    for (lcv=0;lcv<n;lcv++)
      temp[(lcv+m)%n] = a[lcv];
    for (lcv=0; lcv<n;lcv++)
      a[lcv]=temp[lcv];
  }
                 
/* getint prompts the user to enter a positive integer and returns 
 * its value. 
 */                
  int getint(void){
    int answer;

    printf("Please enter a positive integer [<=0 to terminate] : ");
    scanf("%d", &answer);
    return answer;
  }

/* It requests the user to enter positive integers and stores
 * them in a. It returns in n the number of integers stored
 * in the array.
 */
  void getarray(int a[8], int *n) {
    int answer;
    int i=0;

    do {
      answer = getint();
      if (answer>0 && (i<MAXN))
	a[i++]=answer;
    }while(answer>0 && (i<MAXN));
    *n = i;
  }

/* It prints the current content of the array a with n elements */
  void printarray(int a[8], int n) {
    int lcv;

    for (lcv=0;lcv<n;lcv++){
      printf("   %d",a[lcv]);
    }
    printf("\n");
  }
