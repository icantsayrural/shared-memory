#include "shmhelper.h"
#include "shared.h"
#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <pthread.h>
#include <string.h>
#include <unistd.h>

int main() 
{
  pthread_mutex_t *mutex;
  char *message;

  printf("Getting lock...\n");
  mutex = get_mutex(MUTEX);
  if (mutex == SHARED_MEM_INVALID) {
    return -1;
  }

  printf("Obtaining lock...\n");
  if (pthread_mutex_lock(mutex) != 0) {
    error("Failed to obtain lock.");
    return -1;
  }

  printf("Locally save value...\n");
  message = get_string(VALUE);
  printf("Sleep...\n");
  sleep(5);

  printf("Releasing lock...\n");
  if (pthread_mutex_unlock(mutex) != 0) {
    error("Failed to release lock.");
    return -1;
  }

  if (message == SHARED_MEM_INVALID) {
    return -1;
  }

  printf("%s", message);
  return 0;
}
