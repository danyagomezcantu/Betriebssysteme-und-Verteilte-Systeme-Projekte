#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "keyValStore.h"
#include "sub.h"
#include "main.h"

pthread_mutex_t lock;

void *client_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char buffer[256];
    int read_size;

    // Communicate with client
    while((read_size = read(sock, buffer, 256)) > 0) {
        pthread_mutex_lock(&lock);
        process_command(buffer, sock);
        pthread_mutex_unlock(&lock);

        if (strcmp(buffer, "QUIT") == 0) {
            break;
        }
    }

    close(sock);
    free(socket_desc);
    return 0;
}

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;

    pthread_mutex_init(&lock, NULL);

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    // Set address and port number for the server
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(5678);
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

    // Bind the socket to the address and port
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(sockfd, 3) < 0) {
        perror("listen failed");
        exit(1);
    }

    printf("Server started. Waiting for incoming connections...\n");

    while(1) {
        clilen = sizeof(cliaddr);
        connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
        if (connfd < 0) {
            perror("accept failed");
            continue;
        }

        pthread_t client_thread;
        int *new_sock = malloc(sizeof(int));
        *new_sock = connfd;

        if(pthread_create(&client_thread, NULL, client_handler, (void*)new_sock) < 0) {
            perror("could not create thread");
            return 1;
        }

        printf("Handler assigned\n");
    }

    pthread_mutex_destroy(&lock);
    close(sockfd);
    return 0;
}

//int main() {
//    int sockfd, connfd;
//    struct sockaddr_in servaddr, cliaddr;
//    socklen_t clilen;
//    char buffer[256];
//
//    // Create socket
//    sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    if (sockfd < 0) {
//        perror("socket creation failed");
//        exit(1);
//    }
//
//    // Set address and port number for the server
//    servaddr.sin_family = AF_INET;
//    servaddr.sin_port = htons(5678);
//    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
//
//    // Bind the socket to the address and port
//    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
//        perror("bind failed");
//        exit(1);
//    }
//
//    // Listen for incoming connections
//    if (listen(sockfd, 3) < 0) {
//        perror("listen failed");
//        exit(1);
//    }
//
//    printf("Server started. Waiting for incoming connections...\n");
//
//    while (1) {
//        clilen = sizeof(cliaddr);
//        connfd = accept(sockfd, (struct sockaddr *)&cliaddr, &clilen);
//        if (connfd < 0) {
//            perror("accept failed");
//            continue;
//        }
//
//        printf("Connection accepted from client...\n");
//
//        while (1) {
//            read(connfd, buffer, 256);
//            process_command(buffer, connfd);
//            if (strcmp(buffer, "QUIT") == 0) {
//                break;
//            }
//        }
//
//        close(connfd);
//    }
//
//    close(sockfd);
//    return 0;
//}