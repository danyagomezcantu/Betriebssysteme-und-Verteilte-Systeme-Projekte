#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include "keyValStore.h"

void process_command(char* buffer, int connfd) {

    char command[256], key[256], value[256];
    sscanf(buffer, "%s %s %s", command, key, value);

    //Check if the command is GET
    if (strcmp(command, "GET") == 0) {
        char res[256];
        //Retrieve the value assigned to key
        int ret = get(key, res);
        //If key exists, send the value to client
        if (ret == 0) {
            send(connfd, res, strlen(res), 0);
        } else {
            //If key does not exist, then inform
            send(connfd, "key_nonexistent\n", strlen("key_nonexistent\n"), 0);
        }
    } else if (strcmp(command, "PUT") == 0) {
        int ret = put(key, value);
        if (ret == 0) {
            char response[256];
            snprintf(response, sizeof(response), "PUT:%s:%s\n", key, value);
            send(connfd, response, strlen(response), 0);
        } else {
            send(connfd, "error_put", strlen("error_put"), 0);
        }
    } else if (strcmp(command, "DEL") == 0) {
        int ret = del(key);
        if (ret == 0) {
            char response[256];
            snprintf(response, sizeof(response), "DEL:%s:key_deleted\n", key);
            send(connfd, response, strlen(response), 0);
        } else {
            char response[256];
            snprintf(response, sizeof(response), "DEL:%s:key_nonexistent\n", key);
            send(connfd, response, strlen(response), 0);
        }
    } else if (strcmp(command, "QUIT") == 0) {
        //Close connection
        close(connfd);
    } else {
        send(connfd, "invalid_command", strlen("invalid_command"), 0);
    }
}