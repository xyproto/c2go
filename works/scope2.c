/* scope2.c -- Example on scope rules
 */

#include <stdio.h>
int x = 2;
int y = 3;
int z = 4;
void moo(int x, int *y){
  int z;
  x = x+3;
  *y = *y+3;
  z = z+3;  /*Here z is the local z. Notice that it has not been
              initialized. As you see from the output below
              in this case it was implicitly initialized to 0.
              In general that is not the case and the compiler 
	      should give you a warning
	      */
  printf("moo :  x = %1d, *y = %1d, y = %1d, z = %1d\n", x,*y,y,z);
}
int main(void){
  moo(x, &y);
  printf("main: x = %1d1, y = %1d, z = %1d\n", x,y,z);
}

/* The output is

moo :  x = 5, *y = 6, y = 1073742056, z = 3
main: x = 21, y = 6, z = 4

*/
