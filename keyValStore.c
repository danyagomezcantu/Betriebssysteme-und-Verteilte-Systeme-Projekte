#include "keyValStore.h"
#include <string.h>

typedef struct {
    char key[256];
    char value[256];
} KeyValue;
//Assume a maximum of 1024 key-value pairs
KeyValue store[1024];
int store_size = 0;

int put(char* key, char* value) {
    for (int i = 0; i < store_size; i++) {
        if (strcmp(store[i].key, key) == 0) {
            strcpy(store[i].value, value);
            //Key already exists, updated successfully
            return 0;
        }
    }

    if (store_size < 1024) {
        strcpy(store[store_size].key, key);
        strcpy(store[store_size].value, value);
        store_size++;
        //Key added successfully
        return 0;
    }
    //Store is full
    return -1;
}

int get(char* key, char* res) {
    for (int i = 0; i < store_size; i++) {
        if (strcmp(store[i].key, key) == 0) {
            strcpy(res, store[i].value);
            return 0;  // Key found
        }
    }
    //Key not found
    return -1;
}

int del(char* key) {
    for (int i = 0; i < store_size; i++) {
        if (strcmp(store[i].key, key) == 0) {
            for (int j = i; j < store_size - 1; j++) {
                store[j] = store[j + 1];
            }
            store_size--;
            //Key deleted successfully
            return 0;
        }
    }
    //Key not found
    return -1;
}