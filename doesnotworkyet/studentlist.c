/* studentlist.c -- Reads a file containing a sequence of records
 *                  representing students, places them into a linked
 *                  list (a queue), then writes that  out to a new files.
 *                  The names of the files are passed in as command
 *                  line parameters.
 */

#include <stdio.h>

#define NAMESIZE 25

typedef struct {
  char name[NAMESIZE];
  int  midterm;
  int final;
  int homeworks;
} student;

typedef student *studentptr;

/***********THE QUEUE ABSTRACT DATA TYPE************/

typedef struct node {
  struct node *next;
  student value;
}node;

typedef node *nodeptr;

typedef struct {
  nodeptr head;  /* Here we point to the oldest element in the queue */
  nodeptr tail;  /* Here we point to the most recent element in queue*/
} queue;


queue *init(void){
  /* It creates, initializes, and returns a queue */
  queue *q;

  if((q=(queue *)malloc(sizeof(queue)))==NULL){
    perror("malloc");
    exit(1);
  }
  q->head=NULL;
  q->tail=NULL;
  return q;
}

void final(queue *q){
  /* It frees the queue q */
  student s;

  while(get(q,&s));
  free(q);
}

void put(queue *q, studentptr s){
  /* It inserts a new value *s in the queue q */
  nodeptr p;
  
  if ((p=(nodeptr)malloc(sizeof(node)))==NULL){
    perror("malloc");
    exit(1);
  }
  p->value=*s;
  p->next = NULL;
  if (q->head == NULL)
    q->head = p;
  else
    q->tail->next = p;
  q->tail = p;
}

int get(queue *q, studentptr s){
  /*It obtains in s a student record from queue and returns true
    if such a record existed */
  nodeptr p;
  
  if(p=q->head){
    q->head = q->head->next;
    if((q->head)==NULL)
      q->tail = NULL;
    *s=p->value;
    free(p);
    return 1;
  }else
    return 0;
}

/*******END OF QUEUE ABSTRACT DATA TYPE******************/

void writeStudentList(char filename[], queue *q)
     /* filename is the name of the file where we will
      * write sequentially the list accessed from q.
      */
{
  FILE *fd;  /* File descriptor used for filename */
  student s;

  if((fd=fopen(filename,"w"))==NULL){
    perror("fopen");
    exit(1);
  }
  while(get(q, &s)){
    fprintf(fd,"%s %d %d %d\n",
	    s.name, 
	    s.midterm, 
	    s.final, 
	    s.homeworks);
  }
  fclose(fd);
}

int readStudentList(char filename[], queue *q)
     /* It reads student records from file filename 
      * and stores them in q.
      * It returns the number of records actually read.
      */
{
  FILE *fd;  /* File descriptor used for filename */
  int i=0;
  student temp;
  
  if((fd=fopen(filename,"r"))==NULL){
    perror("fopen");
    exit(1);
  }
  while(fscanf(fd,"%s %d %d %d",
	       temp.name, 
	       &temp.midterm,
	       &temp.final, 
	       &temp.homeworks)!=EOF){
    put(q,&temp);
    i++;
  }
  fclose(fd);
  return i;
}

int main(int argc, char *argv[]){
  int n;
  queue *q;

  q = init();
  if(argc!=3){
    printf("Usage: %s infile outfile\n", argv[0]);
    exit(0);
  }
  n = readStudentList(argv[1],q);
  writeStudentList(argv[2],q);
  final(q);
}
