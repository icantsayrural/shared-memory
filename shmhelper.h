#ifndef __SHMHELPER_H__
#define __SHMHELPER_H__

#include <pthread.h>

// Error code for failed shared memory allocation or mapping
extern void *SHARED_MEM_INVALID;

// Creates a new shared memory object with defined
// size and name then returns a pointer to it
void *create_shared_memory(char *name, int size);

// Same as create_shared_memory except shared memory is
// opened without O_CREAT flag and ftruncate is not performed
void *get_shared_memory(char *name, int size);

// Creates a pthread lock in shared memory space
// with specified name
int set_lock(char *name);

// Returns a pointer to a pthread lock in shared memory space
// with specified name
pthread_mutex_t *get_lock(char *name);

// Creates a message string in shared memory space
// with specified name
int set_message(char *name, char *message);

// Returns an existing message string in shared memory space
// with specified name
char *get_message(char *name);

// Prints message and errno with newline
void error(char *message);

#endif
