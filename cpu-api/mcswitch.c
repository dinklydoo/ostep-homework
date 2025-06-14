#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>

#define TIMES 1000; // benchmark value

int main(int argc, char *argv[]) {
  int pipefd_1[2], pipefd_2[2]; // init pipes
  
  struct timeval t1, t2;
  
  gettimeofday(&t1, NULL); // get start time
  
  // for child and parent process run on the same cpu
  struct cpu_set_t set;
  int parentCPU, childCPU;
  
  char testChar = 'a';        /* Use for test */
  
  if (argc != 3) {
      fprintf(stderr, "Usage: %s parent-cpu child-cpu\n",
              argv[0]);
      exit(EXIT_FAILURE);
  }
  
  parentCPU = atoi(argv[1]); // param in[0] - parent cpu
  childCPU = atoi(argv[2]); // param in[1] - child cpu
  
  CPU_ZERO(&set); // clear cpu set
  
  int pid = fork();
  if (pid == 0){ // child
    CPU_SET(childCPU, &set);
    sched_setaffinity(getpid(), sizeof(set), &set);
    
    char read_charc;
  
    close(pipefd_1[0]); // pipefd1 - write only
    close(pipefd_2[1]); // pipefd2 - read only
  
    for (int i = 0; i < TIMES; i++){
      while (read(pipefd_2[0], &read_charc, 1) <= 0) {} // wait for write from parent
      write(pipefd_1[1], &read_charc, 1); // write to pipe 1
    }
  
    close(pipefd_2[0]);
    close(pipefd_1[1]);
    // close all pipes
  
    exit(EXIT_SUCCESS); // child process exits
  }
  else { // parent
    CPU_SET(parentCPU, &set);
    sched_setaffinity(getpid(), sizeof(set), &set);
  
    char read_charp;
  
    close(pipefd_1[1]); // pipefd1 - read only
    close(pipefd_2[0]); // pipefd2 - write only
  
    for (int i = 0; i < TIMES; i++){
      write(pipefd_2[1], &test_char, 1);
      while (read(pipefd_1[0], &read_charp, 1) <= 0){}
    }
  
    close(pipefd_2[1]);
    close(pipefd_1[0]);

  }
  gettimeofday(&t2, NULL); // get end time

  long double elapsed = t2.tv_usec - t1.tv_usec; // get elapsed time in us
  printf("Elapsed time: %Lf us", elapsed / TIMES);
  exit(EXIT_SUCCESS);
}