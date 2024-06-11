#ifndef MAIN_H
#define MAIN_H

#include <semaphore.h>

typedef struct {
    char key[256];
    char value[256];
} KeyValue;

typedef struct {
    int client_fds[10];  // Array of client file descriptors interested in the key
    int num_clients;     // Number of clients subscribed to the key
} Subscription;

typedef struct {
    KeyValue store[1024];
    int store_size;
    sem_t semaphore;
    Subscription subscriptions[1024];  // Subscriptions for each key
} SharedMemoryStruct;

extern SharedMemoryStruct *shared_data;

#endif
