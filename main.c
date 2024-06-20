#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>
#include "keyValStore.h"
#include "sub.h"
#include "main.h"

#define PORT 5678
#define SHM_SIZE sizeof(SharedMemoryStruct)

SharedMemoryStruct *shared_data;

void handle_client(int connfd) {
    char buffer[256];

    printf("Connection accepted from client...\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        read(connfd, buffer, sizeof(buffer) - 1);

        process_command(buffer, connfd);

        if (strcmp(buffer, "QUIT") == 0) {
            break;
        }
    }

    close(connfd);
}

void* client_handler_thread(void* arg) {
    int connfd = *((int *)arg);
    handle_client(connfd);
    close(connfd);
    free(arg);
    return NULL;
}

int main() {
    int sockfd, *connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;

    // Socket creation
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    // Set address for the server
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) < 0) {
        perror("listen failed");
        close(sockfd);
        exit(1);
    }

    // Initialize shared memory
    shared_data = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap failed");
        close(sockfd);
        exit(1);
    }
<<<<<<< HEAD
=======
    /* The arrow operator is used to access the member variable store_size within
     *          the structure or object pointed to by the pointer shared_data.
     */
    shared_data->store_size = 0;
>>>>>>> 545bce6fda2680b18427c0c940d9e9b386e46d77
    shared_data->subscription_count = 0;


    // Initialize semaphore
    if (sem_init(&shared_data->semaphore, 1, 1) < 0) {
        perror("sem_init failed");
        munmap(shared_data, SHM_SIZE);
        close(sockfd);
        exit(1);
    }

    printf("Server started. Waiting for incoming connections...\n");

    while (1) {
        clilen = sizeof(cliaddr);
        connfd = malloc(sizeof(int));
        *connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (*connfd < 0) {
            perror("accept failed");
            free(connfd);
            continue;
        }

        pthread_t tid;
        pthread_create(&tid, NULL, client_handler_thread, connfd);
        pthread_detach(tid);
    }

    // Clean up
    sem_destroy(&shared_data->semaphore);
    munmap(shared_data, SHM_SIZE);
    close(sockfd);
    return 0;
}