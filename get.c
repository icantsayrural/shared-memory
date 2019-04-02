// C program for Consumer process illustrating 
// POSIX shared-memory API. 
#include <stdio.h> 
#include <stdlib.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <sys/mman.h> 
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main() 
{ 
  /* the size (in bytes) of shared memory object */
  const int SIZE = 4096; 

  /* name of the shared memory object */
  const char* name = "os"; 

  /* shared memory file descriptor */
  int shm_fd; 

  /* pointer to shared memory object */
  void* ptr; 

  /* pthread mutex attr and mutex*/
  const char* mutex_name = "mymutex";
  pthread_mutexattr_t attr;
  int mutex_shm_fd;
  pthread_mutex_t* mutex_ptr;

  mutex_shm_fd = shm_open(mutex_name, O_RDWR, 0666);
  if (mutex_shm_fd == -1) {
    printf("Failed to open shared memory.\n");
    return 1;
  }

  mutex_ptr = (pthread_mutex_t*) mmap(0, sizeof(pthread_mutex_t), PROT_READ|PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
  if (mutex_ptr == MAP_FAILED) {
    printf("Failed to map mutex to a ptr.\n");
    return 1;
  }

  printf("Waiting for lock...\n");
  if (pthread_mutex_lock(mutex_ptr) != 0) {
    printf("Failed to obtain lock!\n");
    printf("%s\n", strerror(errno));
    return 1;
  }
  printf("Lock obtained!\n");

  /* open the shared memory object */
  shm_fd = shm_open(name, O_RDONLY, 0666); 

  /* memory map the shared memory object */
  ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0); 

  /* read from the shared memory object */
  printf("%s\n", (char*)ptr);
  sleep(5);

  if (pthread_mutex_unlock(mutex_ptr) != 0) {
    printf("Failed to relase lock!\n");
    return 1;
  }
  printf("Lock released!\n");
  /* remove the shared memory object */
  //shm_unlink(name);
  return 0; 
}
