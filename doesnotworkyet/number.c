/* number.c -- Reading and writing integer as character sequences .
 *             Notice that the base for the digits is the constant
 *             BASE that I have chosen to be 10. Of course, you
 *             use a different value. You might even chose to
 *             have the base as a function parameter.
 */

#include <stdio.h>
#include <assert.h>

#define BASE 10

// a is a string. cursor is a position in a such that a[cursor] is a digit.
// Return the integer value represented by the longest sequence of digits
// in a starting at cursor. On exit cursor will be at the first position
// in a that does not contain a digit. Note that we assume that the base for 
// the number is 10. 
int getNumberToken(char a[], int *cursor)
{
  int value = 0;
  assert((*cursor) >= 0 && (*cursor) < strlen(a));
  do
    {
	value = value*BASE + (int)(a[*cursor]) - (int)('0');
	(*cursor)++;
    }
  while (isdigit(a[*cursor]));
  return value;
}

// value is a positive integer. a is a character array with n positions.
// Store in a as a '\0' terminated string the representation of value as
// a string in base BASE.
void putNumberToken(int value, char a[], int n)
{
  int k;
  int h, temp;
  assert(value >= 0 && n > 0);
  // We collect the desired string in reverse order
  for (k = 0; k < n-1; ++k)
    {
      a[k] = (char)(value%BASE + (int)'0');
      value = value/BASE;
      if (value == 0) break;
    }
  if (k == 0)     // If value was 0 we still return a '0'
    a[k] = '0';
  a[++k] = '\0';
  // Now we put the string in the right order
  for (h = 0; h < k/2; ++h)
    {
      temp = a[h];
      a[h] = a[k-1-h];
      a[k-1-h] = temp;
    }
}

void main (void)
{
  char s[] = "5432";
  int k = 0;
  int val = getNumberToken(s, &k);
  char outs[256];

  printf("val = %d, k = %d\n", val, k);
  putNumberToken(val, outs, 256);
  printf("val = %s\n", outs);
}
