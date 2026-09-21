#ifndef FILES_DATA_H
#define FILES_DATA_H

#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
    char * name;
    int size;
    char * path;
} fileInfo;

int sizeOfFile(char * path);

void hashOfFile(char * path);
bool verifHash(const char * hash1,const char * hash2);
char * recupHash();

#endif
