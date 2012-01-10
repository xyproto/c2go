/* array.c  -- Operations on arrays
 */

#include <stdio.h>

int main(void) {
  int a[2] = {1,2};  /* The aggregates like {1,2} are literals for arrays */
  int b[2] = {2,3};
  int i;

  /* It is legal to use subscripts on arrays, both on the left and on 
   * the right hand side of assignments. */
  for(i=0;i<2;i++)
    a[i]=b[i];

  /* It is not legal to assign arrays, like in a=b; */

  /* The comparison of two distinct arrays with the same content
   * results in FALSE. So below we print "They are not equal"
   */
  if(0 /*a==b*/)
    printf("They are equal\n");
  else
    printf("They are not equal\n");

  /* The following comparison results in TRUE. */
  if(1 /*a==a*/)
    printf("Of course a is equal to a\n");
  else
    printf("No, a is not equal to a\n");
  /* The behavior of comparison is explained when we note that the
   * comparison is a comparison of addresses, not contents.
   */
  /* We cannot print out an array as a single unit. We have to print out
   * its elements one at a time.
   */
  for(i=0;i<2;i++)
    printf("a[%1d] = %3d\n", i, a[i]);
}
