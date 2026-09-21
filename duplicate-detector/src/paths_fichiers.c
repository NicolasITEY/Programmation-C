#include "paths_fichiers.h"


filePathArray_t* getFilePaths(char* search_dir) {
	filePathArray_t* arr = makePathArray();
	char* cmd = calloc(sizeof(char), (strlen("ls -R -l ") + strlen(search_dir) + strlen(" > ./paths.txt") + 1));
	strcat(cmd, "ls -R -l ");
	strcat(cmd, search_dir);
	strcat(cmd, " > ./paths.txt");
	system(cmd);
	system("echo "
	       " >> ./paths.txt");
	free(cmd);


	int   buff_size  = 4096;
	char* path       = calloc(buff_size, sizeof(char));
	int   length     = 0;
	FILE* paths_file = fopen("./paths.txt", "r");
	if (paths_file != NULL) {
		char c = getc(paths_file);
		while (c != EOF) {
			while (c != ':') {
				if (length == buff_size) { // haven't allocated enough characters for the path
					buff_size *= 2;
					path       = realloc(path, buff_size);
				}
				path[length]  = c;
				length       += 1;
				c             = getc(paths_file);
			} // if it's the end of the path we move on to the files
			// printf("[PATH FOUND = %s]\n", path);
			getc(paths_file);
			c = getc(paths_file);
			while (c != '\n') {
				if (c == 'd' || c == 't' || c == 'l'
				    || c == 'p') { // if it's a dirrectory,the total number of blocks or a symbolic link

					// printf("%c", c);
					while (c != '\n') {
						c = getc(paths_file);
						// printf("%c", c);
					}

					c = getc(paths_file);
					continue;
				} else {
					long pos = ftell(paths_file);
					// printf("position before space finding is %ld\n", pos);
					while (c != '\n') {
						if (c == ' ') {
							pos = ftell(paths_file);
							// printf("\nspace found at position %ld \n", pos);
						}
						// printf("%c", c);
						c = getc(paths_file);
					}
					// printf("%c", c);
					// printf("previous space position = %ld\n", pos);
					long endPos = ftell(paths_file);
					// printf("current position = %ld\n", endPos);

					fseek(paths_file, pos, SEEK_SET);
					// printf("position is now %ld,difference = %ld\n", ftell(paths_file), endPos - pos);
					char* name       = calloc((endPos - pos), sizeof(char));
					int   nameLength = 0;
					c                = getc(paths_file);
					while (c != '\n') {
						name[nameLength] = c;
						nameLength++;
						c = getc(paths_file);
					}
					// printf("length of name = %d, length alocated = %ld\n", nameLength, endPos - pos);
					// printf("[NAME FOUND = %s]\n", name);

					char* fullPath = calloc(sizeof(char), length + nameLength + 2);
					strcat(fullPath, path);
					// printf("char at %d is %c\n", length - 1, fullPath[length - 1]);
					if (fullPath[length - 1] != '/')
						strcat(fullPath, "/");
					strcat(fullPath, name);

					// printf("\npath result = %s\n\n", fullPath);
					if (access(fullPath, F_OK)) {
						fprintf(stderr,
						        "ERROR, file not found in path %s, has %s in name, please remove spaces in "
						        "file\ncontinuing\n",
						        path, name);
						free(fullPath);

					} else {
						addPath(arr, fullPath);
					}
					free(name);
				}


				c = getc(paths_file);
			}
			free(path);
			length    = 0;
			buff_size = 4096;
			path      = calloc(buff_size, sizeof(char));
			c         = getc(paths_file);
			// unallocate memory and reset values to default
		}
		free(path);
		fclose(paths_file);
        //remove("./paths.txt");
        
		return arr;

	} else {
		fprintf(stderr, "paths file couldn't be opened\n");
		freePathArray(arr);
		return NULL;
	}
}



void getAllDuplicates(char* search_dir) {
	filePathArray_t* display   = getFilePaths(search_dir);
	sameSizePaths_t* sameSizes = makeSameSizePathArrayFromPaths(display);
	filePathArray_t* copies    = getCopiedFiles(sameSizes);

	printf("résultat : \n");
	for (int i = 0; i < copies->numPath; i++) {
		int sizeFile = sizeOfFile(copies->pathArray[i]);
		printf("%s size : %d\n ", copies->pathArray[i], sizeFile);
	}
	freePathArray(copies);
	freeSamePathArray(sameSizes);
	freePathArray(display);
	// on prend ~ pour le path
	// on fait la recherche de doublon dans le ~
	// on lecrit dans notre document comme voulu
}

char* getParentPath (char* curPath) {
	// transformer path en ce qu'affiche PWD
	// on prend tout sauf ce qui a apres l'avent dernier /
	if (access(curPath, F_OK) == 0) {
		if (curPath[strlen(curPath) - 1] == '/') {
			char* newPath = calloc(strlen(curPath) + strlen("../") + 1, sizeof(char));
			strcpy(newPath, curPath);
			strcat(newPath, "../");
			char* ptr = (realpath(newPath, NULL));
			return ptr;
		} else {
			char* newPath = calloc(strlen(curPath) + strlen("/../") + 1, sizeof(char));
			strcpy(newPath, curPath);
			strcat(newPath, "/../");
			char* ptr = (realpath(newPath, NULL));
			return ptr;
		}
	} else {
		fprintf(stderr, "le chemin %s n'existe pas\n", curPath);
		char* errPath = "\0";
		return errPath;
	}
}

