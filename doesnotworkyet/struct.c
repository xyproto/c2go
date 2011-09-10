// struct.c - Simple uses of structures

#include <stdio.h>

struct student {
  int  mid;
  int final;
  int hmws;
};

void main(void){
   struct student sam = {85, 90, 88};
   struct student tom = {93, 88, 91};
   struct student *he = &tom;  // We can access a structure and its fields through a pointer

   // We cannot read or write directly a studentas we can integers, and reals, etc. 
   // Also, we cannot compare directly two students (i.e. we cannot say (sam == tom)].
   // But we can assign a student to another student. For example:
   tom = sam;
   // Then we can output the value of tom by printing its various fields:
   printf("tom = {%d, %d, %d}\n",  tom.mid, tom.final, tom.hmws); 
        // Notice the dot notation for accessing the fields, of tom. 
        // The output will be tom = {85, 90, 88}

   printf("*he = {%d, %d, %d}\n",  he->mid, he->final, he->hmws); 
        // Notice the arrow notation for accessing the fields from he. 
        // The output will be *he = {85, 90, 88} 
   // We would get the same output if we access the fields in a different way:
   printf("*he = {%d, %d, %d}\n",  (*he).mid, (*he).final, (*he).hmws); 
}






