/* array1.c -- Simple operations with arrays.
 */

#include <stdio.h>
#define N 10

void oneWay(void);
void anotherWay(void);

int main(void) {
  printf("\noneWay:\n");
  oneWay();
  printf("\nantherWay:\n");
  anotherWay();
}

/*Array initialized with aggregate */
void oneWay(void) {
  int vect[N] = {1,2,3,4,5,6,7,8,9,0};
  int i;
  
  for (i=0; i<N; i++)
    printf("i = %2d  vect[i] = %2d\n", i, vect[i]);
} 

/*Array initialized with loop */
void anotherWay(void) {
  int vect[N];
  int i;
  
  for (i=0; i<N; i++)
    vect[i] = i+1;

  for (i=0; i<N; i++)
    printf("i = %2d  vect[i] = %2d\n", i, vect[i]);
} 

/* The output of this program is

oneWay:
i =  0  vect[i] =  1
i =  1  vect[i] =  2
i =  2  vect[i] =  3
i =  3  vect[i] =  4
i =  4  vect[i] =  5
i =  5  vect[i] =  6
i =  6  vect[i] =  7
i =  7  vect[i] =  8
i =  8  vect[i] =  9
i =  9  vect[i] =  0

antherWay:
i =  0  vect[i] =  1
i =  1  vect[i] =  2
i =  2  vect[i] =  3
i =  3  vect[i] =  4
i =  4  vect[i] =  5
i =  5  vect[i] =  6
i =  6  vect[i] =  7
i =  7  vect[i] =  8
i =  8  vect[i] =  9
i =  9  vect[i] = 10

 */
