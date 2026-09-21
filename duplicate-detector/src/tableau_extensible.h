#ifndef TABLEAU_EXTENSIBLE_H
#define TABLEAU_EXTENSIBLE_H

#include "files_data.h"

typedef struct string_arr {
	int    numPath; //nombre 
	int    arraySize;
	char** pathArray;
	
} filePathArray_t;


int              addSizeArray(filePathArray_t* array);
int              addPath(filePathArray_t* array, char* path);
filePathArray_t* makePathArray();
void             freePathArray(filePathArray_t* array);
void             freePathInPosition(filePathArray_t* array,int j);
#endif
