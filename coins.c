/* FILE: coins.c
 * DETERMINES THE VALUE OF A COIN COLLECTION
 * A Variation of the Hanly/Koffman book's example
 */

#include <stdio.h>

void main ()
{
   // Local data ...
   int pennies;              // input: count of pennies
   int nickels;              // input: count of nickels
   int dimes;                // input: count of dimes
   int quarters;             // input: count of quarters
   int temp, left;           // temporaries for various
                             // computations 

   // Read in the count of quarters, dimes, nickels and pennies.
   printf("Enter the number of quarters, dimes, nickels, and pennies: ");
   scanf("%d %d %d %d", &quarters, &dimes, &nickels, &pennies);

   // Compute the total value in cents.
   left = 25 * quarters + 10 * dimes + 5 * nickels + pennies;

   // Find and display the value in dollars
   printf("Your collection is worth\n "); 
   temp = left / 100;
   printf("\t%d dollar", temp);
   if (temp==1) 
      printf(", ");
   else
      printf("s, ");
   left = left % 100;

   // Find and display the value left in quarters
   temp = left / 25;
   printf("%d quarter", temp);
   if (temp==1) 
      printf(", ");
   else
      printf("s, ");
   left = left % 25;

   // Find and display the value left in dimes
   temp = left / 10;
   printf("%d dime", temp);
   // Here, just for fun, instead of using a conditional statement, 
   // I use a conditional expression and string concatenation
   printf ((temp==1) ? ", " : "s, ");
   left = left % 10;

   // Find and display the value left in nickels
   temp = left / 5;
   printf("%d nickel", temp);
   if (temp==1) 
      printf(", and ");
   else
      printf("s, and ");
   left = left % 5;

   // Find and display the value left in pennies
   printf("%d penn", left);
   if (left==1) 
      printf("y\n");
   else
      printf("ies\n");
}


