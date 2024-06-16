#include "keyValStore.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "main.h"

void notify_subscribers(char* key, char* value, char* operation, int excluding_fd) {
    printf("Notifying subscribers for key: %s, operation: %s, value: %s\n", key, operation, value);
    for (int i = 0; i < shared_data->subscription_count; i++) {
        if (strcmp(shared_data->subscriptions[i].key, key) == 0) {
            printf("Found subscription for key: %s\n", key);
            for (int j = 0; j < shared_data->subscriptions[i].count; j++) {
                int client_fd = shared_data->subscriptions[i].client_fds[j];
                if (client_fd != excluding_fd) {
                    char message[512];
                    snprintf(message, sizeof(message), "%s:%s:%s\n", operation, key, value);
                    printf("Sending notification to fd %d: %s", client_fd, message);
                    if (write(client_fd, message, strlen(message)) < 0) {
                        perror("write");
                    }
                }
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
                printf("Client fd %d subscribed to key %s\n", connfd, key);
                return 0;
            } else {
                sem_post(&shared_data->semaphore);  // Unlock
                // Subscription list is full
                printf("Subscription list is full for key %s\n", key);
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
        printf("Client fd %d subscribed to new key %s\n", connfd, key);
        return 0;
    }

    sem_post(&shared_data->semaphore);  // Unlock
    // Subscription list is full
    printf("Global subscription list is full\n");
    return -1;
}

int put(char* key, char* value, int connfd) {
    sem_wait(&shared_data->semaphore);  // Lock
    printf("Executing PUT for key: %s, value: %s\n", key, value);

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            strcpy(shared_data->store[i].value, value);
            notify_subscribers(key, value, "PUT", connfd);
            sem_post(&shared_data->semaphore);  // Unlock
            printf("Key %s already exists, updated with value %s\n", key, value);
            return 0;
        }
    }

    if (shared_data->store_size < 1024) {
        strcpy(shared_data->store[shared_data->store_size].key, key);
        strcpy(shared_data->store[shared_data->store_size].value, value);
        notify_subscribers(key, value, "PUT", connfd);
        shared_data->store_size++;
        sem_post(&shared_data->semaphore);  // Unlock
        printf("Key %s added with value %s\n", key, value);
        return 0;
    }

    sem_post(&shared_data->semaphore);  // Unlock
    printf("Store is full, cannot add key %s\n", key);
    return -1;
}

int get(char* key, char* res) {
    sem_wait(&shared_data->semaphore);  // Lock
    printf("Executing GET for key: %s\n", key);

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            strcpy(res, shared_data->store[i].value);
            sem_post(&shared_data->semaphore);  // Unlock
            printf("Key %s found with value %s\n", key, res);
            return 0;  // Key found
        }
    }

    sem_post(&shared_data->semaphore);  // Unlock
    printf("Key %s not found\n", key);
    return -1;
}

int del(char* key, int connfd) {
    sem_wait(&shared_data->semaphore);  // Lock
    printf("Executing DEL for key: %s\n", key);

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            for (int j = i; j < shared_data->store_size - 1; j++) {
                shared_data->store[j] = shared_data->store[j + 1];
            }
            shared_data->store_size--;
            notify_subscribers(key, "key_deleted", "DEL", connfd);
            sem_post(&shared_data->semaphore);  // Unlock
            printf("Key %s deleted\n", key);
            return 0;
        }
    }

    sem_post(&shared_data->semaphore);  // Unlock
    printf("Key %s not found for deletion\n", key);
    return -1;
}
