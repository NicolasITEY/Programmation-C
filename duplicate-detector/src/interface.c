#include "./paths_fichiers.h"

#define DEFAULT_PATH_SIZE 4096

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

filePathArray_t* affiche_rep(char* search_dir) {
	filePathArray_t* arr = makePathArray();

	char* cmd            = calloc(sizeof(char), (strlen("ls ") + strlen(search_dir) + strlen(" > ./paths.txt") + 1));
	strcat(cmd, "ls ");
	strcat(cmd, search_dir);
	strcat(cmd, " > ./paths.txt");
	system(cmd);
	free(cmd);

	int   buff_size = DEFAULT_PATH_SIZE;
	char* path      = calloc(buff_size, sizeof(char));
	strcpy(path, search_dir);
	int   baseLength = strlen(search_dir);
	int   length     = baseLength;
	if(path[length-1]!='/'){
	    path[length]='/';
	    length++;
	}
	FILE* paths_file = fopen("./paths.txt", "r");
	char  c          = getc(paths_file);

	while (c != EOF) {
		while (c != '\n' && c != EOF) {
			if (length == buff_size) { // haven't allocated enough characters for the path
				buff_size *= 2;
				path       = realloc(path, buff_size);
			}
			path[length]  = c;

			length       += 1;
			c             = getc(paths_file);
		}

		struct stat pathFound;
		if (stat(path, &pathFound) < 0) {
			fprintf(stderr,
			        "votre fichier n'a pas été trouvé, veuillez corriger le nom du fichier. le nom contient %s et se trouve "
			        "dans le répertoire %s\n",
			        path, search_dir);
			break;
		}
		if (S_ISDIR(pathFound.st_mode)) {
			if (length == buff_size) { // haven't allocated enough characters for the path
				buff_size *= 2;
				path       = realloc(path, buff_size);
			}
			path[length] += '/';
			addPath(arr, path);
		} else {
			addPath(arr, path);
		}


		c         = getc(paths_file);
		length    = baseLength;
		buff_size = DEFAULT_PATH_SIZE;
		path      = calloc(buff_size, sizeof(char));
		strcpy(path, search_dir);
	    if(path[length-1]!='/'){
	        path[length]='/';
	        length++;
	    }
	}
    free(path);
	fclose(paths_file);
	return arr;
}


int main() {
	bool              stayintheloop = true;
	int               reponse_5     = 0;
	char*             path          = calloc(DEFAULT_PATH_SIZE, sizeof(char));
	long unsigned int pathMaxLength = DEFAULT_PATH_SIZE;
	if (path == NULL) {
		perror("malloc");
		return 1;
	}
	filePathArray_t* display;
	do {
		char buffer[DEFAULT_PATH_SIZE];

		if (reponse_5 == 0) {
			printf("Bonjour utilisateur qu'elle répertoire vouler utiliser pour votre répértoriation de doublon ?\n");
			scanf("%s", buffer);
			realpath(buffer, path);
			strcat(path, "/");
		}
		display = affiche_rep(path);
		for (int i = 0; i < display->numPath; i++) {
			printf("%s\n", display->pathArray[i]);
		}
		printf("Voici tous les répertoires et sous fichiers basé sur le chemin donnée que voulais vous faire:\n");
		printf("1 : Regarder dans tout le pc parmis tout les fichiers où vous possedez une autorisation de lecture \n");
		printf("2 : Regarder dans le chemin que vous avez donnée\n");
		printf("3 : Aller dans le répertoire Parent\n");
		printf("4 : Revenir à l'écriture du chemin\n");
		printf("5 : Ce déplacer dans un répertoire que vous allez donner parmis les possibilité\n");
		printf("6 : Quiter le programme\n");

		int reponse = 0;
		scanf("%d", &reponse);
		if (reponse == 1) {
			getAllDuplicates("~");
			stayintheloop = false;
		} else if (reponse == 2) {
			getAllDuplicates(path);
			stayintheloop = false;
		} else if (reponse == 3) {
			char* newPath = getParentPath(path);
			for (long unsigned int i = 0; i <= strlen(path); i++) {
				path[i] = '\0';
			}
			if (strlen(newPath) + 1 > pathMaxLength) {
				pathMaxLength = strlen(newPath) + 1;
				path          = realloc(path, pathMaxLength);
			}
			strcpy(path, newPath);
			reponse_5 = 1;
			if (path[0] == '\0') {
				stayintheloop = false;
			}
			affiche_rep(path);
		} 
		else if(reponse == 4){
		    reponse_5 = 0;
		    free(path);
		    path          = calloc(DEFAULT_PATH_SIZE, sizeof(char));
		    freePathArray(display);
		    continue;
		}
		else if (reponse == 5) {
			printf("Dans qu'elle répertoire voullait vous aller ? entre 0 et %d ", display->numPath);
			int rep = 0;
			scanf("%d", &rep);
			filePathArray_t * tmp = affiche_rep(display->pathArray[rep]); // ls -r
			                                      // un scanf de la position dans le tableau du rep qu'on veut aller

			                                      // scanf("%d",i)
			//
			// sa modifie char *path = malloc(strlen(tab[i]) + 1);
			// en dehors du code fonction_5
			struct stat chosenFile;
			freePathArray(display);
			display = tmp;
			stat(display->pathArray[rep], &chosenFile);
			if (S_ISDIR(chosenFile.st_mode)) {
				for (long unsigned int i = 0; i <= strlen(path); i++) {
					path[i] = '\0';
				}
				if (strlen(display->pathArray[rep]) + 1 > pathMaxLength) {
					pathMaxLength = strlen(display->pathArray[rep]) + 1;
					path          = realloc(path, pathMaxLength);
				}
				strcpy(path, display->pathArray[rep]);
			} else {
				fprintf(stderr, "vous aviez mis un chemin qui n'est pas un repertoire, veuillez reesayer\n");
			}
			reponse_5 = 1;
		} else if (reponse == 6) {
			stayintheloop = false;
	       
		} else {
			fprintf(stderr,"error,commande invalide\n");
		}
	} while (stayintheloop);
    free(path);
    freePathArray(display);
	return 0;
}
