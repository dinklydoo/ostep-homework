#include "my_semaphore.h"

int my_sem_init(my_sem_t* sem, int pshared, int value){
  sem->value = value;
  pthread_mutex_init(&sem->lock, NULL);
  pthread_cond_init(&sem->cond, NULL);
  return 0;
}

int my_sem_wait(my_sem_t* sem){
  pthread_mutex_lock(&sem->lock);
  while (sem->value <= 0){
    pthread_cond_wait(&sem->cond, &sem->lock);
  }
  sem->value--;
  pthread_mutex_unlock(&sem->lock);
  return 0;
}

int my_sem_post(my_sem_t* sem){
  pthread_mutex_lock(&sem->lock);
  sem->value++;
  pthread_cond_signal(&sem->cond);
  pthread_mutex_unlock(&sem->lock);
  return 0;
}