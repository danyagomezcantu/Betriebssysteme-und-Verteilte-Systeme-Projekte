#include "keyValStore.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "main.h"

void notify_subscribers(char* key, char* value, char* operation) {
    for (int i = 0; i < shared_data->subscription_count; i++) {
        if (strcmp(shared_data->subscriptions[i].key, key) == 0) {
            for (int j = 0; j < shared_data->subscriptions[i].count; j++) {
                int client_fd = shared_data->subscriptions[i].client_fds[j];
                char message[512];
                snprintf(message, sizeof(message), "%s:%s:%s\n", operation, key, value);
                write(client_fd, message, strlen(message));
            }
        }
    }
}

int subscribe(char* key, int connfd) {
    sem_wait(&shared_data->semaphore);  // Lock

    for (int i = 0; i < shared_data->subscription_count; i++) {
        if (strcmp(shared_data->subscriptions[i].key, key) == 0) {
            if (shared_data->subscriptions[i].count < MAX_SUBSCRIPTIONS) {
                shared_data->subscriptions[i].client_fds[shared_data->subscriptions[i].count++] = connfd;
                sem_post(&shared_data->semaphore);  // Unlock
                return 0;
            } else {
                sem_post(&shared_data->semaphore);  // Unlock
                // Subscription list is full
                return -1;
            }
        }
    }

    if (shared_data->subscription_count < 1024) {
        strcpy(shared_data->subscriptions[shared_data->subscription_count].key, key);
        shared_data->subscriptions[shared_data->subscription_count].client_fds[0] = connfd;
        shared_data->subscriptions[shared_data->subscription_count].count = 1;
        shared_data->subscription_count++;
        sem_post(&shared_data->semaphore);  // Unlock
        return 0;
    }

    sem_post(&shared_data->semaphore);  // Unlock
    // Subscription list is full
    return -1;
}


int put(char* key, char* value) {
    sem_wait(&shared_data->semaphore);  // Lock

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            strcpy(shared_data->store[i].value, value);
            notify_subscribers(key, value, "PUT");
            sem_post(&shared_data->semaphore);  // Unlock
            // Key already exists, updated successfully
            return 0;
        }
    }

    if (shared_data->store_size < 1024) {
        strcpy(shared_data->store[shared_data->store_size].key, key);
        strcpy(shared_data->store[shared_data->store_size].value, value);
        notify_subscribers(key, value, "PUT");
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
            notify_subscribers(key, "key_deleted", "DEL");
            sem_post(&shared_data->semaphore);  // Unlock
            // Key deleted successfully
            return 0;
        }
    }

    sem_post(&shared_data->semaphore);  // Unlock
    // Key not found
    return -1;
}




