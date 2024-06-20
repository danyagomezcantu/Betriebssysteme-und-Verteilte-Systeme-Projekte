#ifndef MAIN_H
#define MAIN_H

#include <semaphore.h>

#define MAX_SUBSCRIPTIONS 1024

typedef struct {
    char key[256];
    int client_fds[MAX_SUBSCRIPTIONS];
    int count;
} Subscription;

typedef struct {
    char key[256];
    char value[256];
} KeyValue;

typedef struct {
    KeyValue store[1024];
    int store_size;
    sem_t semaphore;
    Subscription subscriptions[1024];
    int subscription_count;
} SharedMemoryStruct;

extern SharedMemoryStruct *shared_data;

#endif
