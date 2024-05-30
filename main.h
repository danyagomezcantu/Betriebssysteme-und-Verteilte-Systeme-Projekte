#ifndef MAIN_H
#define MAIN_H

#include <semaphore.h>

typedef struct {
    char key[256];
    char value[256];
} KeyValue;

typedef struct {
    KeyValue store[1024];
    int store_size;
    sem_t semaphore;
} SharedMemoryStruct;

extern SharedMemoryStruct *shared_data;

#endif
