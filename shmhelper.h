#ifndef __SHMHELPER_H__
#define __SHMHELPER_H__

#include <pthread.h>
#include <stdbool.h>

extern void *SHARED_MEM_INVALID;

void error(char *message);

void *create_shared_memory(char *name, int size);

void *get_shared_memory(char *name, int size, bool write);

int create_mutex(char *name);

pthread_mutex_t *get_mutex(char *name);

int create_string(char *name, char *message);

char *get_string(char *name);

#endif
