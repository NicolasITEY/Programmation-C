#if defined HAVE_CONFIG_H
#include "config.h"
#endif

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
#include "master_worker.h"

// fonctions éventuelles internes au fichier
// fonctions éventuelles proposées dans le .h
void execWorker(int primeN,int readFD,int writeFD){
    char N[5]="";
	char R[5]="";
	char W[5]="";
	sprintf(N, "%d", primeN);
	sprintf(R, "%d", readFD);
	sprintf(W, "%d", writeFD);
	char* commands[] = {"./worker", N, R, W,NULL};
	execv("./worker", commands);
}




int myFork(){
    int ret = fork();
	myassert(ret != -1, "couldn't fork");
	return ret;
}
void makeNewWorker(int *pipes){
    int pipe_m[2];
	myPipe(pipe_m);
	int pipe_w[2];
	myPipe(pipe_w);

	int retfork = myFork();
	if (retfork > 0) {          // master
		myClose(pipe_m[0]);
	    pipes[1]=pipe_m[1];
	    pipes[0]=pipe_w[0];
		myClose(pipe_w[1]); // côté ecriture du master
	} else {                    // worker
	    myClose(pipe_m[1]);
	    myClose(pipe_w[0]);
		execWorker(2,pipe_m[0],pipe_w[1]);
	}
	bool testPrime = false;
	myRead(pipe_w[0], &testPrime, sizeof(bool));
	myassert(testPrime,"worker could not be initialized");
}


int makeNextWorker(int primeN,int writeFD){
    int fdW[2];
	myPipe(fdW);
	
	int ret = myFork();
	if (ret == 0) {
    	myClose(fdW[1]);
		execWorker(primeN,fdW[0],writeFD);
	}
	myClose(fdW[0]);
	return fdW[1];
}



