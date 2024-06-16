#ifndef KEYVALSTORE_H
#define KEYVALSTORE_H

int put(char* key, char* value, int connfd);
int get(char* key, char* res);
int del(char* key, int connfd);
int subscribe(char* key, int connfd);
void notify_subscribers(char* key, char* value, char* operation, int excluding_fd);

#endif  // KEYVALSTORE_H
