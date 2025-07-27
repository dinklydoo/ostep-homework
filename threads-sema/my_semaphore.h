#ifndef SEMA_H
#define SEMA_H

#include <pthread.h>

typedef struct my_sem_t {
  pthread_cond_t cond;
  pthread_mutex_t lock;
  int value;
} my_sem_t;

int my_sem_init(my_sem_t* sem, int pshared, int value);
int my_sem_wait(my_sem_t* sem);
int my_sem_post(my_sem_t* sem);

#endif
