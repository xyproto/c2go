/* enum2.c  -- Starting to use enumerated types: Printing for each 
 *             day of the week, today, yesterday, and tomorrow, both
 *             as a string and as a number. We use typedef
 */

#include <stdio.h>

/* Introducing an enumerated data type */
typedef enum {monday,tuesday,wednesday,thursday,friday,saturday,sunday} days;

/* Two useful functions */
days yesterday(days today);
days tomorrow(days today);

char *thedays[] = {"monday", "tuesday", "wednesday", "thursday",
		       "friday", "saturday", "sunday"};

int main(void){
  days today;

  printf("today    \tyesterday  \ttomorrow\n"
         "============================================\n");
  for (today=monday;today<=sunday;today++)
    printf("%s = %d \t %s = %d \t %s = %d\n", 
	   thedays[today], today, 
	   thedays[yesterday(today)], yesterday(today),
	   thedays[tomorrow(today)], tomorrow(today));
}

days yesterday(days today){
  return (today+6)%7;
}
days tomorrow(days today){
  return (today+1)%7;
}

/*
 The output is:

today    	yesterday  	tomorrow
============================================
monday = 0 	 sunday = 6 	 tuesday = 1
tuesday = 1 	 monday = 0 	 wednesday = 2
wednesday = 2 	 tuesday = 1 	 thursday = 3
thursday = 3 	 wednesday = 2 	 friday = 4
friday = 4 	 thursday = 3 	 saturday = 5
saturday = 5 	 friday = 4 	 sunday = 6
sunday = 6 	 saturday = 5 	 monday = 0

*/
