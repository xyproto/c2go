/* From http://www.java2s.com/Code/C/Small-Application/Acalculatorthatallowsmultiplecalculations.htm */

#include <stdio.h>

int main()
{
  double number1 = 1.0;
  double number2 = 2.0;
  char operation = '+';
  char answer = 0;



  switch(operation)
  {
    case '+':
      printf("= %lf\n", number1 + number2);
      break;

    case '-':
      printf("= %lf\n", number1 - number2);
      break;

    case '*':
      printf("= %lf\n", number1 * number2);
      break;

    case '/':
      if(number2 == 0)
        printf("\n\n\aDivision by zero error!\n");
      else
        printf("= %lf\n", number1 / number2);
      break;

    case '%':
      if((long)number2 == 0)
        printf("\n\n\aDivision by zero error!\n");
      else
        printf("= %ld\n", (long)number1 % (long)number2);
      break;

    default:
      printf("\n\n\aIllegal operation!\n");
  }
}
