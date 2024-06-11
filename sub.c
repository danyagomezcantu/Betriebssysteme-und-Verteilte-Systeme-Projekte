#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <semaphore.h>
#include "main.h"

#include "keyValStore.h"

void notify_subscribers(const char* key, const char* message) {
    sem_wait(&shared_data->semaphore);  // Lock

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            Subscription *subscription = &shared_data->subscriptions[i];
            for (int j = 0; j < subscription->num_clients; j++) {
                send(subscription->client_fds[j], message, strlen(message), 0);
            }
            break;
        }
    }

    sem_post(&shared_data->semaphore);  // Unlock
}


void subscribe(const char* key, int connfd) {
    sem_wait(&shared_data->semaphore);  // Lock

    for (int i = 0; i < shared_data->store_size; i++) {
        if (strcmp(shared_data->store[i].key, key) == 0) {
            Subscription *subscription = &shared_data->subscriptions[i];
            if (subscription->num_clients < 10) {
                subscription->client_fds[subscription->num_clients++] = connfd;
                char response[256];
                snprintf(response, sizeof(response), "SUB:%s:subscribed\n", key);
                send(connfd, response, strlen(response), 0);
            } else {
                send(connfd, "SUB:error:max_clients\n", strlen("SUB:error:max_clients\n"), 0);
            }
            sem_post(&shared_data->semaphore);  // Unlock
            return;
        }
    }

    sem_post(&shared_data->semaphore);  // Unlock
    send(connfd, "SUB:error:key_not_found\n", strlen("SUB:error:key_not_found\n"), 0);
}


void process_command(char* buffer, int connfd) {
    char command[256], key[256], value[256];
    sscanf(buffer, "%s %s %s", command, key, value);

    if (strcmp(command, "GET") == 0) {
        char res[256];
        int ret = get(key, res);
        if (ret == 0) {
            send(connfd, res, strlen(res), 0);
            send(connfd, "\n", 1, 0);
        } else {
            send(connfd, "key_nonexistent\n", strlen("key_nonexistent\n"), 0);
        }
    } else if (strcmp(command, "PUT") == 0) {
        int ret = put(key, value);
        if (ret == 0) {
            char response[256];
            snprintf(response, sizeof(response), "PUT:%s:%s\n", key, value);
            send(connfd, response, strlen(response), 0);
            notify_subscribers(key, response);
        } else {
            send(connfd, "error_put", strlen("error_put"), 0);
        }
    } else if (strcmp(command, "DEL") == 0) {
        int ret = del(key);
        if (ret == 0) {
            char response[256];
            snprintf(response, sizeof(response), "DEL:%s:key_deleted\n", key);
            send(connfd, response, strlen(response), 0);
            notify_subscribers(key, response);
        } else {
            char response[256];
            snprintf(response, sizeof(response), "DEL:%s:key_nonexistent\n", key);
            send(connfd, response, strlen(response), 0);
        }
    } else if (strcmp(command, "SUB") == 0) {
        subscribe(key, connfd);
    } else if (strcmp(command, "QUIT") == 0) {
        close(connfd);
    } else {
        send(connfd, "invalid_command", strlen("invalid_command"), 0);
    }
}


