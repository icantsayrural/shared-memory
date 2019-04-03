#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include "shmhelper.h"
#include "shared.h"

int main()
{
  char *message = "Hello World!\n";

  printf("Creating lock...\n");
  if (create_mutex(MUTEX) == -1) {
    return -1;
  }

  printf("Creating value...\n");
  if (create_string(VALUE, message) == -1) {
    return -1;
  }

  return 0;
}
