#include <stdio.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>


int main(int argc, char *argv[]){
  struct timeval t1, t2;
  long double elapsed;

  int fd = open("buffer.txt", O_RDONLY);
  void* buf;

  gettimeofday(&t1, NULL);


  for (int i = 0; i < 999999; i++){ read(fd, buf, 0);}

  gettimeofday(&t2, NULL);

  //elapsed = (t2.tv_sec - t1.tv_sec) * 1000000;
  elapsed = (t2.tv_usec - t1.tv_usec); // unlikely the syscall takes seconds
  printf("Elapsed time: %Lf us", elapsed / 999999);
}