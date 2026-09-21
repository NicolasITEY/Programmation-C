#if defined HAVE_CONFIG_H
	#include "config.h"
#endif

#include "master_worker.h"
#include "common.h"
#include "myassert.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/************************************************************************
 * Données persistantes d'un worker
 ************************************************************************/

// on peut ici définir une structure stockant tout ce dont le worker
// a besoin : le nombre premier dont il a la charge, ...

/************************************************************************
 * Usage et analyse des arguments passés en ligne de commande
 ************************************************************************/

static void usage(const char* exeName, const char* message) {
	fprintf(stderr, "usage : %s <n> <fdIn> <fdToMaster>\n", exeName);
	fprintf(stderr, "   <n> : nombre premier géré par le worker\n");
	fprintf(stderr, "   <fdIn> : canal d'entrée pour tester un nombre\n");
	fprintf(stderr, "   <fdToMaster> : canal de sortie pour indiquer si un nombre est premier ou non\n");
	if (message != NULL)
		fprintf(stderr, "message : %s\n", message);
	exit(EXIT_FAILURE);
}

static void parseArgs(int argc, char* argv[], workerInfo* info) {
	if (argc != 4)
		usage(argv[0], "Nombre d'arguments incorrect");

	info->primeN = atoi(argv[1]);
	info->read   = atoi(argv[2]);
	info->writeM = atoi(argv[3]);
	info->writeW = -1;
}

/************************************************************************
 * Boucle principale de traitement
 ************************************************************************/

void loop(workerInfo* info) {
	int  order   = 0;
	bool isPrime = true;
	do {
		//    attendre l'arrivée d'un nombre à tester
		myRead(info->read, &order, sizeof(int));
		if (order == -1) {
			if (info->writeW != -1) {
				myWrite(info->writeW, &order, sizeof(int));
			}
			// si ordre d'arrêt
			//        si il y a un worker suivant, transmettre l'ordre et attendre sa fin
			//        sortir de la boucle
		} else {
			//    sinon c'est un nombre à tester, 4 possibilités :
			isPrime = (order == info->primeN); //           - le nombre est premier
			if (isPrime) {
				myWrite(info->writeM, &isPrime, sizeof(bool));
			} else {
				//           - s'il y a un worker suivant lui transmettre le nombre
				bool isMult = (order % info->primeN == 0);
				if (isMult) {
					isMult = false;
					//           - le nombre n'est pas premier
					myWrite(info->writeM, &isMult, sizeof(bool));
				} else if (info->writeW == -1) {
					//           - s'il n'y a pas de worker suivant, le créer
					

					// info->readN  = fdW[0];
					info->writeW = makeNextWorker(order,info->writeM);

				} else {
					myWrite(info->writeW, &order, sizeof(int));
				}
			}
		}

	} while (order != WORKER_STOP);
}

/************************************************************************
 * Programme principal
 ************************************************************************/

int main(int argc, char* argv[]) {
	workerInfo info = {0};
	parseArgs(argc, argv, &info);
	bool isPrime = true;
	// Si on est créé c'est qu'on est un nombre premier
	// Envoyer au master un message positif pour dire
	// que le nombre testé est bien premier
	myWrite(info.writeM, &isPrime, sizeof(bool));

	loop(&info);

	// libérer les ressources : fermeture des files descriptors par exemple
	myClose(info.read);
	myClose(info.writeM);
	if(info.writeW!=-1)
	    myClose(info.writeW);
	return EXIT_SUCCESS;
}
