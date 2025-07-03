#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#define OPS_PER_THREAD 1000

typedef struct node_c { // complex node structure (hand over hand llist)
  int value;
  struct node_c* next;
  pthread_mutex_t lock;
}node_c;

typedef struct llist_c {
  node_c* head;
} llist_c;

node_c* node_c_init(int value){
  node_c* n = malloc(sizeof(node_c));
  n->value = value;
  n->next = NULL;
  pthread_mutex_init(&n->lock, NULL);
  return n;
}

// void node_c_add(node_c* n, int v){
//   pthread_mutex_lock(&n->lock);
//   node* new = node_c_init(v);
//   new->next = n->next;
//   n->next = new;
//   pthread_mutex_unlock(&n->lock);
// }

llist_c* llist_c_init(int value){
  llist_c* l = malloc(sizeof(llist_c));
  l->head = node_c_init(value);
  return l;
}

void llist_c_add(llist_c* l, int value){
  node_c* temp = l->head;
  pthread_mutex_lock(&temp->lock);
  while (temp->next){
    pthread_mutex_lock(&temp->next->lock);
    pthread_mutex_unlock(&temp->lock);

    temp = temp->next;
  }
  temp->next = node_c_init(value);
  pthread_mutex_unlock(&temp->lock);
}


typedef struct node{
  int value;
  struct node* next;
} node;

typedef struct llist{
  node* head;
  pthread_mutex_t lock;
} llist;

node* node_init(int value){
  node* n = malloc(sizeof(node));
  n->value = value;
  n->next =NULL;
  return n;
}

llist* llist_init(int value){
  llist* l = malloc(sizeof(llist));
  l->head = node_init(value);
  pthread_mutex_init(&l->lock, NULL);
  return l; 
}

void llist_add(llist* l, int value){
  pthread_mutex_lock(&l->lock);
  node* current = l->head;
  while (current->next){
    current=current->next;
  }
  current->next = node_init(value);
  pthread_mutex_unlock(&l->lock);
}

void* biglock_test(void* arg){
  llist* l = (llist*) arg;
  for (int i = 0; i < OPS_PER_THREAD; i++){
    llist_add(l, i);
  }
  return NULL;
}

void* hoh_test(void* arg){
  llist_c* l = (llist_c*)arg;
  for (int i = 0; i < OPS_PER_THREAD; i++){
    llist_c_add(l, i);
  }
  return NULL;
}

int main(int argc, char** argv){
  if (argc != 2){
    printf("Expected one argument equal to number of threads");
    exit(1);
  }
  int t = atoi(argv[1]);
  pthread_t* threads = malloc(sizeof(pthread_t) * t);

  // TESTING FOR ONE BIG LOCK

  struct timeval t1, t2;
  gettimeofday(&t1, NULL);

  llist* l = llist_init(0);
  for (int i = 0; i < t; i++){
    pthread_create(&threads[i], NULL, biglock_test, l);
  }
  for (int i = 0; i < t; i++){
    pthread_join(threads[i], NULL);
  }
  gettimeofday(&t2, NULL);
  long elapsed = (t2.tv_sec - t1.tv_sec) * 1000000;
  elapsed += t2.tv_usec - t1.tv_usec;
  printf("Time elapsed for big lock: %ld\n", elapsed);

  // TESTING FOR HAND OVER HAND LOCKING

  for (int i = 0; i < t; i++){
    pthread_create(&threads[i], NULL, hoh_test, l);
  }
  for (int i = 0; i < t; i++){
    pthread_join(threads[i], NULL);
  }
  gettimeofday(&t2, NULL);
  elapsed = (t2.tv_sec - t1.tv_sec) * 1000000;
  elapsed += t2.tv_usec - t1.tv_usec;
  printf("Time elapsed for HOH lock: %ld\n", elapsed);

  return 0;
}