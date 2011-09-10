/* binary.c - Binary search using two methods. The first is more intuitive, but it is
	slower.
*/

#include <stdio.h>
#include <sys/time.h>

/* Given a sorted array with n elements, we search for who using binary search.
   We return a position where found, or -1 if not there
*/
int binary1(int n, int a[n], int who) {
    int left = 0; 
    int right = n-1;
    while (left <= right) {
	int mid = left + (right-left)/2;
	if (who < a[mid])
	    right = mid - 1;
	else if (who > a[mid])
	    left = mid + 1;
	else
	    return mid;
    }
    return -1;
}

/* Given a sorted array with n elements, we search for who using binary search.
   We return a position where found, or -1 if not there
*/
int binary2(int n, int a[n], int who) {
    int p = n/2;
    while (n > 0) {
	n = n/2;
	if (who < a[p]) {
	    p -= n;
	} else if (who > a[p]) {
	    p += n;
	} else
	    return p;
    }
    return -1;
}

/* Returns the difference in microseconds between before and after */
long timediff(struct timeval before, struct timeval after) {
    long sec = after.tv_sec - before.tv_sec;
    long microsec = after.tv_usec - before.tv_usec;
    return 1000000*sec + microsec;
}

int main() {
    int a[] = {1,3,5,7,9,11,13,15,17,19,21,23,25,27,29,31,33};
    int n = sizeof(a)/sizeof(int);
    int where;
    struct timeval before;
    struct timeval after;
    int k;
    int j;
    gettimeofday(&before, NULL);
    for (j = 0; j < 1000000; j++)
    for (k = 0; k < 2*n+1; k++) {
	where = binary1(n, a, k);
//	printf("who = %d, \tvalue = %d\n", k, where); 
    }
    gettimeofday(&after, NULL);
    printf("before=[%ld,%ld], after=[%ld,%ld]\n", before.tv_sec, before.tv_usec,
			after.tv_sec, after.tv_usec);
    printf("The difference is %ld\n", timediff(before, after));
    printf("---------------------------------------------\n");
    gettimeofday(&before, NULL);
    for (j = 0; j < 1000000; j++)
    for (k = 0; k < 2*n+1; k++) {
	where = binary2(n, a, k);
//	printf("who = %d, \tvalue = %d\n", k, where); 
    }
    gettimeofday(&after, NULL);
    printf("before=[%ld,%ld], after=[%ld,%ld]\n", before.tv_sec, before.tv_usec,
			after.tv_sec, after.tv_usec);
    printf("The difference is %ld\n", timediff(before, after));
    return 0;
}
