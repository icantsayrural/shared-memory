#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/shm.h> 
#include <sys/stat.h> 
#include <sys/mman.h> 
#include <pthread.h>
#include <errno.h>
#include <unistd.h>

int main() 
{ 
  /* the size (in bytes) of shared memory object */
  const int SIZE = 4096; 

  /* name of the shared memory object */
  const char* name = "os";

  /* strings written to shared memory */
  const char* message_0 = "Hello World!\n"; 

  /* shared memory file descriptor */
  int shm_fd; 

  /* pointer to shared memory obect */
  void* ptr;

  /* pthread mutex attr and mutex*/
  const char* mutex_name = "mymutex";
  pthread_mutexattr_t attr;
  pthread_mutex_t* mutex;
  int mutex_shm_fd;

  /* create the shared memory object */
  shm_unlink(name);
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 

  /* configure the size of the shared memory object */
  if (ftruncate(shm_fd, sizeof(message_0)) != 0) {
    printf("Failed to truncate shared memory.\n");
    printf("%s\n", strerror(errno));
    return 1;
  }

  /* memory map the shared memory object */
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0); 

  /* write to the shared memory object */
  sprintf(ptr, "%s", message_0);

  /* write mutex to shared memory */
  shm_unlink(mutex_name);
  mutex_shm_fd = shm_open(mutex_name, O_CREAT | O_RDWR, 0666);
  if (mutex_shm_fd == -1) {
    printf("Failed to open shared memory.\n");
    return 1;
  }
  if (ftruncate(mutex_shm_fd, sizeof(pthread_mutex_t)) != 0) {
    printf("Failed to truncate shared memory.\n");
    printf("%s\n", strerror(errno));
    return 1;
  }
  mutex = (pthread_mutex_t *) mmap(0, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE, MAP_SHARED, mutex_shm_fd, 0);
  if (pthread_mutexattr_init(&attr) != 0) {
    printf("failed to initialize mutex attribute object.\n");
    return 1;
  }
  if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
    printf("failed to set shared to mutex attribute object.\n");
    return 1;
  }
  if (pthread_mutex_init(mutex, &attr) != 0) {
    printf("failed to init mutex.\n");
    return 1;
  }
  return 0; 
}
