#include <stdio.h>
#include <stdlib.h>
#include "files_data.h"


int sizeOfFile(char * thePath){

    struct stat theState;

    fileInfo info= {0};

    stat(thePath, &theState);

    info.size=(int)theState.st_size;

    return info.size;
}

void hashOfFile(char * path){

    char * commandeSum = "md5sum ";
    char * renvoieVersFichier = " > hash.txt";
    char * commande = calloc(strlen(commandeSum)+strlen(path)+strlen(renvoieVersFichier)+1,sizeof(char));

    strcat(commande,commandeSum);
    strcat(commande,path);
    strcat(commande,renvoieVersFichier);

    system(commande);

    free(commande);

}

char * recupHash(){

    char * hash = calloc(33,sizeof(char));

    FILE * fd = fopen("hash.txt","r+");

    fread(hash,sizeof(char),32,fd);
    fclose(fd);
    //remove("hash.txt");
    return hash;
}

bool verifHash(const char * hash1,const char * hash2){
    
    return strcmp(hash1,hash2)==0;

}
/*
//Juste pour tester 
int main() {
    

    hashOfFile("kamardine.txt");
    char * hashkama = recupHash();


    hashOfFile("kilo.txt");
    char * hashkilo = recupHash();

    if(verifHash(hashkama,hashkilo)){
        printf("trueeeeeeeeeeeeeeeeeeeeee\n");
    }else printf("falssssssssssssssssseeeeeeeeeeeeeeeee\n");
    free(hashkama);
    free(hashkilo);

    remove("hash.txt");

    return 0;
}*/
