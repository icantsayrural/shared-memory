#include "shmhelper.h"
#include <sys/shm.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h> 
#include <unistd.h>

void *SHARED_MEM_INVALID = (void *) -1;

void error(char *message) {
  printf("%s", message);
  printf("\n");
  printf("%s\n", strerror(errno));
}

void *create_shared_memory(char *name, int size) {
  int shm_fd;
  void *ptr;

  // in OSX, ftruncate only works if I put an shm_unlink before shm_open
  // i.e. destroy POSIX shared memory object
  // see: https://stackoverflow.com/a/25510361
  shm_unlink(name);
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  if (shm_fd == -1) {
    error("Failed to create shared memory object.");
    return SHARED_MEM_INVALID;
  }

  if (ftruncate(shm_fd, size) != 0) {
    error("Failed to configure the size of the shared memory object.");
    return SHARED_MEM_INVALID;
  }

  ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (ptr == MAP_FAILED) {
    error("Failed to memory map the shared object.");
    return SHARED_MEM_INVALID;
  }

  return ptr;
}

void *get_shared_memory(char *name, int size, bool write) {
  int shm_fd;
  void *ptr;

  shm_fd = shm_open(name, O_RDWR, 0666);
  if (shm_fd == -1) {
    error("Failed to open shared memory object.");
    return SHARED_MEM_INVALID;
  }

  int flags = PROT_READ;
  if (write) {
    flags = flags | PROT_WRITE;
  }
  ptr = mmap(0, size, flags, MAP_SHARED, shm_fd, 0);
  if (ptr == MAP_FAILED) {
    error("Failed to memory map the shared object.");
    return SHARED_MEM_INVALID;
  }

  return ptr;
}

int create_mutex(char *name) {
  pthread_mutexattr_t attr;
  pthread_mutex_t *ptr;

  ptr = create_shared_memory(name, sizeof(pthread_mutex_t));
  if (ptr == SHARED_MEM_INVALID) {
    return -1;
  }

  if (pthread_mutexattr_init(&attr) != 0) {
    error("Failed to initialize mutex attribute object.");
    return -1;
  }

  if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
    error("Failed to set shared to mutex attribute object.");
    return -1;
  }

  if (pthread_mutex_init(ptr, &attr) != 0) {
    error("Failed to initialize mutex.");
    return -1;
  }

  return 0;
}

pthread_mutex_t *get_mutex(char *name) {
  return (pthread_mutex_t *) get_shared_memory(name, sizeof(pthread_mutex_t), true);
}

int create_string(char *name, char *message) {
  void *ptr = create_shared_memory(name, sizeof(message));
  if (ptr == SHARED_MEM_INVALID) {
    return -1;
  }

  sprintf(ptr, "%s\n", message);
  return 0;
}

char *get_string(char *name) {
  return (char *) get_shared_memory(name, sizeof(name), false);
}
