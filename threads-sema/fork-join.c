#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "common_threads.h"

my_sem_t s; 

void *child(void *arg) {
    printf("child\n");
    my_sem_post(&s);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p;
    printf("parent: begin\n");
    my_sem_init(&s,0,0);
    Pthread_create(&p, NULL, child, NULL);
    my_sem_wait(&s);
    printf("parent: end\n");
    return 0;
}

