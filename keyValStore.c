#include "keyValStore.h"
#include <string.h>
#include "main.h"

int put(char* key, char* value) {
    sem_wait(&shared_data->semaphore);  // Lock

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            strcpy(shared_data->store[i].value, value);
            sem_post(&shared_data->semaphore);  // Unlock
            // Key already exists, updated successfully
            return 0;
        }
    }

    if (shared_data->store_size < 1024) {
        strcpy(shared_data->store[shared_data->store_size].key, key);
        strcpy(shared_data->store[shared_data->store_size].value, value);
        shared_data->store_size++;
        sem_post(&shared_data->semaphore);  // Unlock
        // Key added successfully
        return 0;
    }

    sem_post(&shared_data->semaphore);  // Unlock
    // Store is full
    return -1;
}

int get(char* key, char* res) {
    sem_wait(&shared_data->semaphore);  // Lock

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            strcpy(res, shared_data->store[i].value);
            sem_post(&shared_data->semaphore);  // Unlock
            return 0;  // Key found
        }
    }

    sem_post(&shared_data->semaphore);  // Unlock
    // Key not found
    return -1;
}

int del(char* key) {
    sem_wait(&shared_data->semaphore);  // Lock

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            for (int j = i; j < shared_data->store_size - 1; j++) {
                shared_data->store[j] = shared_data->store[j + 1];
            }
            shared_data->store_size--;
            sem_post(&shared_data->semaphore);  // Unlock
            // Key deleted successfully
            return 0;
        }
    }

    sem_post(&shared_data->semaphore);  // Unlock
    // Key not found
    return -1;
}
