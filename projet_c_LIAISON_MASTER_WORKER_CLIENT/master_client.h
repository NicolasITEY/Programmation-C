#ifndef CLIENT_CRIBLE
#define CLIENT_CRIBLE

// On peut mettre ici des éléments propres au couple master/client :
//    - des constantes pour rendre plus lisible les comunications
//    - des fonctions communes (création tubes, écriture dans un tube,
//      manipulation de sémaphores, ...)

// ordres possibles pour le master
#define ORDER_NONE                0
#define ORDER_STOP                -1
#define ORDER_COMPUTE_PRIME       1
#define ORDER_HOW_MANY_PRIME      2
#define ORDER_HIGHEST_PRIME       3
#define ORDER_COMPUTE_PRIME_LOCAL 4 // ne concerne pas le master

// Noms des tubes nommés
#define FIFO_TO_MASTER   "./fifo_to_master"
#define FIFO_FROM_MASTER "./fifo_from_master"

// Noms des sémaphores
#define MASTER_CLIENT_SEMAPHORE_FILE "./master_client.h"
#define SEM_MUTEX_ID 5
#define SEM_SYNC_ID 6


// bref n'hésitez à mettre nombre de fonctions avec des noms explicites
// pour masquer l'implémentation
void my_mkfifo(const char* name);
int getReadPipe(const char* name);
int getWritePipe(const char* name);
void takeSem(int semaphore);
void giveSem(int semaphore);
void myClose(int fd);
int myMakeSem(const char* file,int id);
void mySetVal(int semId,int val);

void myRead(int fd,void *to,long unsigned int bytes);
void myWrite(int fd,void *from,long unsigned int bytes);
void makeNamedPipes();

#endif
