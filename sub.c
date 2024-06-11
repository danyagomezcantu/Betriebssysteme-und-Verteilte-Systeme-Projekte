#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "keyValStore.h"


void process_command(char* command, int connfd) {
    char key[256];
    char value[256];
    char operation[256];

    if (sscanf(command, "PUT %s %s", key, value) == 2) {
        if (put(key, value) == 0) {
            write(connfd, "PUT successful\n", 15);
        } else {
            write(connfd, "PUT failed\n", 11);
        }
    } else if (sscanf(command, "GET %s", key) == 1) {
        if (get(key, value) == 0) {
            write(connfd, value, strlen(value));
            write(connfd, "\n", 1);
        } else {
            write(connfd, "GET failed\n", 11);
        }
    } else if (sscanf(command, "DEL %s", key) == 1) {
        if (del(key) == 0) {
            write(connfd, "DEL successful\n", 15);
        } else {
            write(connfd, "DEL failed\n", 11);
        }
    } else if (sscanf(command, "SUB %s", key) == 1) {
        if (subscribe(key, connfd) == 0) {
            write(connfd, "SUB successful\n", 15);
        } else {
            write(connfd, "SUB failed\n", 11);
        }
    } else {
        write(connfd, "Unknown command\n", 16);
    }
}
