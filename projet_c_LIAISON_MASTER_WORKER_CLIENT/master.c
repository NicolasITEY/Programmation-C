#if defined HAVE_CONFIG_H
	#include "config.h"
#endif

#include "common.h"
#include "master_client.h"
#include "master_worker.h"
#include "myassert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>

/************************************************************************
 * Données persistantes d'un master
 ************************************************************************/

int num_primes    = 0;
int biggest_prime = 2;

/************************************************************************
 * Usage et analyse des arguments passés en ligne de commande
 ************************************************************************/

static void usage(const char* exeName, const char* message) {
	fprintf(stderr, "usage : %s\n", exeName);
	if (message != NULL)
		fprintf(stderr, "message : %s\n", message);
	exit(EXIT_FAILURE);
}


void rmSem(int id){
    int ret = semctl(id, IPC_RMID, 0);
	myassert(ret != -1, "erreur de fermeture du semaphore");
}

void rmPipe(const char* name){
    int res = unlink(name);
    myassert(res !=-1,"pipe got removed");
}

/************************************************************************
 * boucle principale de communication avec le client
 ************************************************************************/
void loop(int sem_sync, int writeW, int readW) // rajouter pipe_m dans void loop
{
	int fd_from_client = getReadPipe(FIFO_TO_MASTER);
	int fd_to_client = getWritePipe(FIFO_FROM_MASTER);
	int order = 0;
	int res   = 123456;
	// boucle infinie :
	do {
		takeSem(sem_sync);
		myRead(fd_from_client, &order, sizeof(int));
		if (order == ORDER_COMPUTE_PRIME) {
			// - si ORDER_COMPUTE_PRIME
			int prime_to_calculate = 0;
			myRead(fd_from_client, &prime_to_calculate, sizeof(int));
			//       . récupérer le nombre N à tester provenant du client
			bool isPrime = false;
			// printf("yep totally computing a prime dude bro\n");
			//       . construire le pipeline jusqu'au nombre N-1 (si non encore fait) :
			//             il faut connaître le plus grand nombre (M) déjà enovoyé aux workers
			for (int i = biggest_prime + 1; i < prime_to_calculate; i++) {
				//             on leur envoie tous les nombres entre M+1 et N-1
				myWrite(writeW, &i, sizeof(int));
				myRead(readW, &isPrime, sizeof(bool));
				if (isPrime) {
					biggest_prime = i;
				}
				//             note : chaque envoie déclenche une réponse des workers
			}
			myWrite(writeW, &prime_to_calculate, sizeof(int));
			myRead(readW, &isPrime, sizeof(bool));
			//       . envoyer N dans le pipeline
			if (isPrime) {
				num_primes++;
				if (prime_to_calculate > biggest_prime) {
					biggest_prime = prime_to_calculate;
				}
				res = 1;
			} else {
				res = 0;
			}
			myWrite(fd_to_client, &res, sizeof(int));
			//      transmettre la reponse au client

			// on envoie un nbr : on construit une pipline anonyme m->w, a chaque fork de fait le worker
			// cree une pipeline ano w->m pour envoier le plus grand nbr puis la supprime
		}
		if (order == ORDER_HOW_MANY_PRIME) {
			res = num_primes;
			myWrite(fd_to_client, &res, sizeof(int));
			// - si ORDER_HOW_MANY_PRIME
			//       . transmettre la réponse au client (le plus simple est que cette
			//         information soit stockée en local dans le master)
		}
		if (order == ORDER_HIGHEST_PRIME) {
			res = biggest_prime;
			// printf("definatly 1 dude\n");
			myWrite(fd_to_client, &res, sizeof(int));
			// - si ORDER_HIGHEST_PRIME
			//       . transmettre la réponse au client (le plus simple est que cette
			//         information soit stockée en local dans le master)
		}
		if (order == ORDER_STOP) {
			res = -1;
			myWrite(writeW, &res, sizeof(int));
			myWrite(fd_to_client, &res, sizeof(int));
			// - si ORDER_STOP
			//       . envoyer ordre de fin au premier worker et attendre sa fin
			//       . envoyer un accusé de réception au client
		}
		takeSem(sem_sync);
		// - revenir en début de boucle
	} while (order != ORDER_STOP);
	// - fermer les tubes nommés
	myClose(fd_from_client);
	myClose(fd_to_client);
}

/************************************************************************
 * Fonction principale
 ************************************************************************/

int main(int argc, char* argv[]) {
	if (argc != 1)
		usage(argv[0], NULL);

	// - création des sémaphores
	// creation mutex entre clients
	
	int mutex_master_worker = myMakeSem(MASTER_CLIENT_SEMAPHORE_FILE,SEM_MUTEX_ID);
	mySetVal(mutex_master_worker,1);

	// creation semaphore sync client master
	int sem_sync_m_c = myMakeSem(MASTER_CLIENT_SEMAPHORE_FILE, SEM_SYNC_ID);
	mySetVal(sem_sync_m_c,0);

	// - création des tubes nommés
	makeNamedPipes();

	// - création du premier worker
	int pipes[2];
	makeNewWorker(pipes);
	
	// boucle infinie
	loop(sem_sync_m_c, pipes[1], pipes[0]);

	// destruction des tubes nommés, des sémaphores, ...
	rmSem(mutex_master_worker);
	rmSem(sem_sync_m_c);
	rmPipe(FIFO_TO_MASTER);
	rmPipe(FIFO_FROM_MASTER);
	
	return EXIT_SUCCESS;
}

// N'hésitez pas à faire des fonctions annexes ; si les fonctions main
// et loop pouvaient être "courtes", ce serait bien
