// macos free :: vm_stat
// pmap :: /usr/bin/vmmap pid
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  printf("Process Id: %d\n", getpid());
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <megabytes>\n", argv[0]);
    return 1;
  }
  int mb = atoi(argv[1]);
  if (mb <= 0) {
    fprintf(stderr, "Please provide a positive number of megabytes.\n");
    return 1;
  }
  size_t num_bytes = (size_t) mb * 1000000;

  char* arr = calloc(num_bytes, sizeof(char)); // allocate argv MB of memory

  size_t idx = 0;
  while (1) { // touch each element
    arr[idx] = (char)(arr[idx] + 1);
    idx = (idx + 1) % num_bytes;
  }

  return 0;
}