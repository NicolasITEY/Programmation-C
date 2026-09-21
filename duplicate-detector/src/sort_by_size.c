#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "sort_by_size.h"

int addSize(sameSizePaths_t* array) {
	if (array->arraySize==0)
    {
        array->arraySize=1;
        array->sameSizePathsArray  = calloc(array->arraySize , sizeof(filePathArray_t *));
        if (array->sameSizePathsArray == NULL) {
		    return -1;
	    }
	    return 0;
    }
    else{
	    array->arraySize *= 2;
	    array->sameSizePathsArray  = realloc(array->sameSizePathsArray, array->arraySize * sizeof(filePathArray_t *));
	    if (array->sameSizePathsArray == NULL) {
		    return -1;
	    }
	    return 0;
	}
}

int addPathArr(sameSizePaths_t* array, filePathArray_t *paths) {
	int ret = 0;
	if (array->numPathArr == array->arraySize) {
		ret = addSize(array);
		if (ret == -1) {
			return ret;
		}
	}
	array->sameSizePathsArray[array->numPathArr]  = paths;
	array->numPathArr                            += 1;
	return ret;
}

int removePath(sameSizePaths_t* array, int position) {
	if (position > array->numPathArr || position < 0) {
		fprintf(stderr, "error, tried to remove path at position out of bounds");
		return -1;
	}


	return 0;
}
sameSizePaths_t* makeSameSizePathArray() {
	sameSizePaths_t* res = malloc(sizeof(sameSizePaths_t));
	if (res == NULL) {
		return res;
	}
	res->numPathArr   = 0;
	res->arraySize = 1;
	res->sameSizePathsArray = calloc(1,sizeof(filePathArray_t *));
	return res;
}

void freeSamePathArray(sameSizePaths_t* array) {
	for (int i = array->numPathArr-1; i >=0 ; i--) {
		freePathArray(array->sameSizePathsArray[i]);
	}
	free(array);
}



sameSizePaths_t *makeSameSizePathArrayFromPaths(filePathArray_t *paths) {
    sameSizePaths_t *array = makeSameSizePathArray();
    for (int i = 0;i<paths->numPath ;i++) {
        if(paths->pathArray[i]==NULL){
            continue;
        }
        int curSize = sizeOfFile(paths->pathArray[i]);
        filePathArray_t *sameSizePath_arr = makePathArray();
        addPath(sameSizePath_arr,paths->pathArray[i]);
        for (int j = i+1;j< paths->numPath;j++) {
            if(paths->pathArray[j]==NULL){
                continue;
            }
            int size = sizeOfFile(paths->pathArray[j]);
            if (curSize==size){
                addPath(sameSizePath_arr,paths->pathArray[j]);
                freePathInPosition(paths,j);
            }
            

        }
        if(sameSizePath_arr->numPath>1){
            addPathArr(array,sameSizePath_arr);
        }else{
            freePathArray(sameSizePath_arr);
        }
        freePathInPosition(paths,i);
    }
    
    return array;
}


filePathArray_t *getCopiedFiles(sameSizePaths_t *orderedPaths){
    filePathArray_t *res = makePathArray();
    for(int i=0;i<orderedPaths->numPathArr;i++){//for every paths of the same size
        filePathArray_t *paths = orderedPaths->sameSizePathsArray[i];
        for(int j=0;j<paths->numPath;j++){//for every paths in that array
            if(paths->pathArray[j]==NULL){
                continue;
            }
            hashOfFile(paths->pathArray[j]);
            char *curHash = recupHash();
            int numSimilar = 0;
            for(int k=j+1;k<paths->numPath;k++){
                if(paths->pathArray[k]==NULL){
                    continue;
                }
                hashOfFile(paths->pathArray[k]);
                char *newHash = recupHash();
                if(verifHash(curHash,newHash)){
                    numSimilar++;
                    addPath(res,paths->pathArray[k]);
                    freePathInPosition(paths,k);
                }
                free(newHash);
            }
            free(curHash);
            if(numSimilar>0){
                addPath(res,paths->pathArray[j]);
                freePathInPosition(paths,j);
            }
        }
    }
    return res;
}
