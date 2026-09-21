#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#define _XOPEN_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>

#include "myassert.h"
#include "common.h"
#include "master_client.h"

// fonctions éventuelles internes au fichier
//
// fonctions éventuelles proposées dans le .h


void my_mkfifo(const char* name){
    int res = mkfifo(name, 0641);
	myassert(res != -1, "erreur d'ouverture de creation du fichier FIFO_TO_MASTER");
	close(res);
}

int getReadPipe(const char* name){
    int res = open(name, O_RDONLY);
	myassert(res != -1, "erreur d'ouverture d'un pipe");
	return res;
}

int getWritePipe(const char* name){
    int res = open(name, O_WRONLY);
	myassert(res != -1, "erreur d'ouverture d'un pipe");
	return res;
}

void takeSem(int semaphore){
    struct sembuf operation = {0, -1, 0};
	int ret = semop(semaphore, &operation, 1);
	myassert(ret != -1, "semop raté");
}

void giveSem(int semaphore){
    struct sembuf operation = {0, 1, 0};
	int ret = semop(semaphore, &operation, 1);
	myassert(ret != -1, "semop raté");
}

int myMakeSem(const char* file,int id){
    int mutexKey = ftok(file, id);
	myassert(mutexKey != -1, "erreur file to key a rate");
	int res = semget(mutexKey, 1, IPC_CREAT | IPC_EXCL | 0641);
	myassert(res != -1, "erreur, mutex existe deja");
	return res;
}

void mySetVal(int semId,int val){
    int ret = semctl(semId, 0, SETVAL, val);
	myassert(ret != -1, "erreur set valeur du mutex à la valeur donné");
}

void makeNamedPipes(){
    if (access(FIFO_TO_MASTER, F_OK) != 0) {
		my_mkfifo(FIFO_TO_MASTER);
	}
	if (access(FIFO_FROM_MASTER, F_OK) != 0) {
		my_mkfifo(FIFO_FROM_MASTER);
	}
}
