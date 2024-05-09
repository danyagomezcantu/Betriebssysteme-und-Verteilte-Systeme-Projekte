//
// Created by b_sen on 09.05.2024.
//
#include "keyValStore.h"

#include <string.h>

#define MAX_KEYS 100
#define MAX_KEY_LENGTH 50
#define MAX_VALUE_LENGTH 100

typedef struct {

    char key[MAX_KEY_LENGTH];
    char value[MAX_VALUE_LENGTH];

} KeyValue; //this structure called as KeyValue

KeyValue dataStore[MAX_KEYS];
int numKeys = 0;

int put(char* key, char* value) {

    for (int i = 0; i < numKeys; ++i) {
        if (strcmp(dataStore[i].key, key) == 0) {
            strcpy(dataStore[i].value, value);
            return 1;
        }
    }

    if (numKeys < MAX_KEYS) {
        strcpy(dataStore[numKeys].key, key);
        strcpy(dataStore[numKeys].value, value);
        numKeys++;
        return 2;
    }
    else {
        return -1;
    }
}
