/* homework1.c -- This is how the code for the first homework
 *                appears when we have a single block letter.
 *                In our Unix system you can compile, link,
 *                load, and run this program with the commands
 *                    % cc homework1.c
 *                    % a.out
 */

#include <stdio.h>

void blockg(void);   /*Prototype for blockg function */

int main (void) {
   printf("\n");
   blockg();
   printf("\n");
}

/* Print out the Block letter g */
void blockg(void) {
  printf("gggggg\n");
  printf("g    g\n");
  printf("g\n");
  printf("g  ggg\n");
  printf("g    g\n");
  printf("gggggg\n");
}
  
/* It prints out:

gggggg
g    g
g
g  ggg
g    g
gggggg

*/
