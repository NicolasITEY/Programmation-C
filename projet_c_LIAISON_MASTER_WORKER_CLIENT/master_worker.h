#ifndef MASTER_WORKER_H
#define MASTER_WORKER_H

// On peut mettre ici des éléments propres au couple master/worker :
//    - des constantes pour rendre plus lisible les comunications
//    - des fonctions communes (écriture dans un tube, ...)

#define WORKER_STOP -1
typedef struct worker_info {
	int primeN; // nombre premier donné a un worker
	int read;   // lit lordre du derner master (pour le premier) ou les worker precedants
	int writeM; // ecrire au master
	int writeW; // ecrire au prochain worker
	            // int readN;
} workerInfo;




int myFork();
void execWorker(int primeN,int readFD,int writeFD);
void makeNewWorker(int *pipes);
int makeNextWorker(int primeN,int writeFD);
#endif
