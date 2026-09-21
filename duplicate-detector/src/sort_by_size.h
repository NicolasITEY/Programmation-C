#ifndef SORTBYSIZE_H
#define SORTBYSIZE_H

#include "paths_fichiers.h"
#include "files_data.h"

typedef struct sameSizePath_arr {
	int    numPathArr;
	int    arraySize;
	filePathArray_t** sameSizePathsArray;
} sameSizePaths_t;

int addSize(sameSizePaths_t* array);
int addPathArr(sameSizePaths_t* array, filePathArray_t *paths);
int removePath(sameSizePaths_t* array, int position);
sameSizePaths_t* makeSameSizePathArray();
sameSizePaths_t *makeSameSizePathArrayFromPaths(filePathArray_t *paths);
filePathArray_t *getCopiedFiles(sameSizePaths_t *orderedPaths);
void freeSamePathArray(sameSizePaths_t* array);


#endif
