/**
 * Sets a lock and message string to shared memory
 */

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

  printf("Setting lock...\n");
  if (set_lock(LOCK) == -1) {
    return -1;
  }

  printf("Setting message...\n");
  if (set_message(MESSAGE, message) == -1) {
    return -1;
  }

  return 0;
}
