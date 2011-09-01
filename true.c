/* true.c  -- What are in C the values of TRUE and FALSE? 
 */

 #include <stdio.h>

 int main(void) {
    printf("The value of 1<2 is %v\n", (1<2));
    printf("The value of 2<1 is %v\n", (2<1));
 }

 /* The program prints out

 The value of 1<2 is 1
 The value of 2<1 is 0

*/
