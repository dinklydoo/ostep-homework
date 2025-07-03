#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct counter{
  int count;
  pthread_mutex_t lock;
}counter;

counter* counter_init(int start){
  counter* c = malloc(sizeof(counter));
  c->count = start;
  pthread_mutex_init(&c->lock, NULL);
  return c;
}

void increment(counter *c){
  pthread_mutex_lock(&c->lock);
  c->count++;
  pthread_mutex_unlock(&c->lock);
}

void decrement(counter *c){
  pthread_mutex_lock(&c->lock);
  c->count--;
  pthread_mutex_unlock(&c->lock);
}

int get(counter* c){
  return c->count;
}

void* increment_thread(void* arg){
  int target = 1000000;
  pthread_t tid = pthread_self();
  counter* c = (counter*) arg;
  while (1){
    pthread_mutex_lock(&c->lock); // acquire lock for this thread
    if (c->count >= target){
      pthread_mutex_unlock(&c->lock);
      break;
    }
    c->count++;

    pthread_mutex_unlock(&c->lock);
    printf("Thread: %lu\nCounter: %d\n\n", tid, get(c));
  }
  return NULL;
}

int main(int argc, char** argv){
  if (argc != 2){ // expect 2 args, 1 arg for # of threads
    printf("Expected one argument for number of threads");
    exit(1);
  }

  struct timeval t1, t2;
  gettimeofday(&t1, NULL);

  counter* c = counter_init(0);
  int n = atoi(argv[1]);

  pthread_t* threads = alloca(sizeof(pthread_t) * n);
  for (int i = 0; i < n; i++){
    pthread_create(&threads[i], NULL, increment_thread, c);
    printf("Thread %d: %lu\n", i, threads[i]); // print out the threads
  }
  // STDOUT NOT ATOMIC SO PRINTS ARE WEIRD

  for (int i = 0; i < n; i++){
    pthread_join(threads[i], NULL);
  }

  gettimeofday(&t2, NULL);
  long elapsed = (t2.tv_sec - t1.tv_sec) * 1000000;
  elapsed += t2.tv_usec - t1.tv_usec;

  printf("Final Count %d", get(c));
  printf("Time Taken: %ld us", elapsed);
  return 0;
}
