#ifndef PATHS_FICHIERS_H
#define PATHS_FICHIERS_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./tableau_extensible.h"
#include "./sort_by_size.h"
#include "./files_data.h"

filePathArray_t* getFilePaths(char* search_dir); //renvoie un tableau qui contient tous les paths d'un repertoire données.
void getAllDuplicates(char* search_dir) ;
char* getParentPath (char* curPath);

#endif
