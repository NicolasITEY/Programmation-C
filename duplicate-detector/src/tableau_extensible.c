#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "tableau_extensible.h"

int addSizeArray(filePathArray_t* array) {
    if (array->arraySize==0)
    {
        array->arraySize=1;
        array->pathArray  = calloc(array->arraySize , sizeof(char*));
        if (array->pathArray == NULL) {
		    return -1;
	    }
	    return 0;
    }
    else{
	    array->arraySize *= 2;
	    array->pathArray  = realloc(array->pathArray, array->arraySize * sizeof(char*));
	    if (array->pathArray == NULL) {
		    return -1;
	    }
	    return 0;
	}
}

int addPath(filePathArray_t* array, char* path) {
	int ret = 0;
	if (array->numPath == array->arraySize) {
		ret = addSizeArray(array);
		if (ret == -1) {
			return ret;
		}
	}
	array->pathArray[array->numPath]  = path;
	array->numPath                   += 1;
	return ret;
}


filePathArray_t* makePathArray() {
	filePathArray_t* res = malloc(sizeof(filePathArray_t));
	if (res == NULL) {
		return res;
	}
	res->numPath   = 0;
	res->arraySize = 0;
	res->pathArray = NULL;
	return res;
}

void freePathInPosition(filePathArray_t* array,int j){
    if(j<0||j>array->numPath){
        fprintf(stderr,"tried freeing path out of bounds");
    }
    assert(j>=0&&j<array->numPath);
    array->pathArray[j]=NULL;
}

void freePathArray(filePathArray_t* array) {
	for (int i = 0; i < array->numPath; i++) {
	    if(array->pathArray[i]!=NULL)
		    free(array->pathArray[i]);
	}
	free(array->pathArray);
	free(array);
}
