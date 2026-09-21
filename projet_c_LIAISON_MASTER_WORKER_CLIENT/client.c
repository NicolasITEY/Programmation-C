#if defined HAVE_CONFIG_H
#include "config.h"
#endif

#include "common.h"
#include "master_client.h"
#include "myassert.h"
#include <pthread.h>

#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <math.h>
// chaines possibles pour le premier paramètre de la ligne de commande
#define TK_STOP "stop"
#define TK_COMPUTE "compute"
#define TK_HOW_MANY "howmany"
#define TK_HIGHEST "highest"
#define TK_LOCAL "local"

/************************************************************************
 * Usage et analyse des arguments passés en ligne de commande
 ************************************************************************/

static void usage(const char *exeName, const char *message)
{
	fprintf(stderr, "usage : %s <ordre> [<nombre>]\n", exeName);
	fprintf(stderr, "   ordre \"" TK_STOP "\" : arrêt master\n");
	fprintf(stderr, "   ordre \"" TK_COMPUTE "\" : calcul de nombre premier\n");
	fprintf(stderr, "                       <nombre> doit être fourni\n");
	fprintf(stderr, "   ordre \"" TK_HOW_MANY "\" : combien de nombres premiers calculés\n");
	fprintf(stderr, "   ordre \"" TK_HIGHEST "\" : quel est le plus grand nombre premier calculé\n");
	fprintf(stderr, "   ordre \"" TK_LOCAL "\" : calcul de nombres premiers en local\n");
	if (message != NULL)
		fprintf(stderr, "message : %s\n", message);
	exit(EXIT_FAILURE);
}

static int parseArgs(int argc, char *argv[], int *number)
{
	int order = ORDER_NONE;

	if ((argc != 2) && (argc != 3))
		usage(argv[0], "Nombre d'arguments incorrect");

	if (strcmp(argv[1], TK_STOP) == 0)
		order = ORDER_STOP;
	else if (strcmp(argv[1], TK_COMPUTE) == 0)
		order = ORDER_COMPUTE_PRIME;
	else if (strcmp(argv[1], TK_HOW_MANY) == 0)
		order = ORDER_HOW_MANY_PRIME;
	else if (strcmp(argv[1], TK_HIGHEST) == 0)
		order = ORDER_HIGHEST_PRIME;
	else if (strcmp(argv[1], TK_LOCAL) == 0)
		order = ORDER_COMPUTE_PRIME_LOCAL;

	if (order == ORDER_NONE)
		usage(argv[0], "ordre incorrect");
	if ((order == ORDER_STOP) && (argc != 2))
		usage(argv[0], TK_STOP " : il ne faut pas de second argument");
	if ((order == ORDER_COMPUTE_PRIME) && (argc != 3))
		usage(argv[0], TK_COMPUTE " : il faut le second argument");
	if ((order == ORDER_HOW_MANY_PRIME) && (argc != 2))
		usage(argv[0], TK_HOW_MANY " : il ne faut pas de second argument");
	if ((order == ORDER_HIGHEST_PRIME) && (argc != 2))
		usage(argv[0], TK_HIGHEST " : il ne faut pas de second argument");
	if ((order == ORDER_COMPUTE_PRIME_LOCAL) && (argc != 3))
		usage(argv[0], TK_LOCAL " : il faut le second argument");
	if ((order == ORDER_COMPUTE_PRIME) || (order == ORDER_COMPUTE_PRIME_LOCAL))
	{
		*number = strtol(argv[2], NULL, 10);
		if (*number < 2)
			usage(argv[0], "le nombre doit être >= 2");
	}

	return order;
}

/************************************************************************
 * Fonction principale
 ************************************************************************/
typedef struct
{

	pthread_mutex_t *mutex; // mutex pour bloquer le chevauchement entre les tableau de Prime number
	int nombre_check;		// donc pointe sur une case du tableau défini dans le main
	int n;					// nombre maximal a verifier (le nombre donnée  pour le Order_COMPUTE_PRIME_LOCAL)
	bool *a;				// le pointeur vers le tableau de boolean
} ThreadData;

void *codeThread(void *number)
{

	ThreadData *p = (ThreadData *)number;

	if (p->a[p->nombre_check] == true)
	{
		//printf("the numer %d is prime, removing the rest\n",p->nombre_check);
		int ret = pthread_mutex_lock(p->mutex);
		myassert(ret == 0, "le thread n'est pas crée,il n'est pas lock");
		for (int i = p->nombre_check * p->nombre_check; i < p->n; i += p->nombre_check)
		{
			p->a[i] = false;
			
			//printf("\tle nombre %d n'est plus prime ? %s\n", i,p->a[i]? "false" : "true");
		}/*
		printf("on vérifie le résultat pour N = %d\n",p->nombre_check);
		for(int i=0;i<p->n;i++){
			printf("%d est premier? %s\n",i,p->a[i]? "true" : "false");
		}
		fflush(stdout);*/
		ret = pthread_mutex_unlock(p->mutex);
		myassert(ret == 0, "le threat n'est pas unlock");
	}
	return NULL;
}



int getMCmutex(){
    int mutex_key = ftok(MASTER_CLIENT_SEMAPHORE_FILE, SEM_MUTEX_ID);
    int res = semget(mutex_key, 1, 0);
	myassert(res != -1, "sem_open mutex failed");
	return res;
}

int getMCsync(){
    int sync_key = ftok(MASTER_CLIENT_SEMAPHORE_FILE, SEM_SYNC_ID);
	int res = semget(sync_key, 1, 0);
	myassert(res != -1, "sem_open semaphore failed");
	return res;
}

int computePrime(int number){
	bool tab[number];
	tab[1] = false;
	tab[0] = false;
	for (int i = 2; i < number; i++)
	{
		tab[i] = true;
	}
	pthread_mutex_t monMutex = PTHREAD_MUTEX_INITIALIZER;

	ThreadData tab_prime[(int)sqrt(number) - 1];
	
	pthread_t tabId[(int)sqrt(number) - 1];
	for (int i = 0; i < (int)sqrt(number) - 1; i++)
	{
		tab_prime[i].mutex = &monMutex; 
		tab_prime[i].nombre_check = i+2;
		tab_prime[i].n = number;
		tab_prime[i].a = tab;
		int ret = pthread_create(&(tabId[i]), NULL, *codeThread, &tab_prime[i]);
		myassert(ret == 0, "threat creation failed");
	}
	for (int j = 0; j < (int)sqrt(number) - 1; j++)
	{
		int ret = pthread_join(tabId[j], NULL);
		myassert(ret == 0, "thread join failed");
	}
	printf("on affiche\n");
	for (int i = 0; i < number; i++)
	{
		if(tab[i])
			printf("%d est premier\n", i);
	}

	return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    // order peut valoir 5 valeurs (cf. master_client.h) :
	//      - ORDER_COMPUTE_PRIME_LOCAL
	//      - ORDER_STOP
	//      - ORDER_COMPUTE_PRIME
	//      - ORDER_HOW_MANY_PRIME
	//      - ORDER_HIGHEST_PRIME
	//
	int number = 0;
	int order = parseArgs(argc, argv, &number);
	printf("%d\n", order); // pour éviter le warning
	
	
	if (order == ORDER_COMPUTE_PRIME_LOCAL)
	{
	    // si c'est ORDER_COMPUTE_PRIME_LOCAL
	    //    alors c'est un code complètement à part multi-thread
		return computePrime(number);
	}
    // sinon
	int mutex = getMCmutex();
	int synch = getMCsync();
	//    - entrer en section critique :
	//           . pour empêcher que 2 clients communiquent simultanément
	//           . le mutex est déjà créé par le master
	takeSem(mutex);
	//    - ouvrir les tubes nommés (ils sont déjà créés par le master)
	//           . les ouvertures sont bloquantes, il faut s'assurer que
	//             le master ouvre les tubes dans le même ordre
	int fd_to_master = getWritePipe(FIFO_TO_MASTER);
	int fd_from_master = getReadPipe(FIFO_FROM_MASTER);
	
	myWrite(fd_to_master, &order, sizeof(order)); //    - envoyer l'ordre et les données éventuelles au master
	
	if (order == ORDER_COMPUTE_PRIME)
		myWrite(fd_to_master, &number, sizeof(int));

	giveSem(synch);
    
	int res;
	myRead(fd_from_master, &res, sizeof(int));    //    - attendre la réponse sur le second tube
	
	switch (order){
	    case ORDER_STOP:
	        printf("program stopped\n");
	        break;
	    case ORDER_COMPUTE_PRIME:
	        printf("%d %s\n",number,res==1?"is prime":"isn't prime");
	        break;
	    case ORDER_HOW_MANY_PRIME :
	        printf("calculated %d primes\n",res);
	        break;
        case ORDER_HIGHEST_PRIME :
            printf("highest prime calculated is %d\n",res);
	        
	}
	// Une fois que le master a envoyé la réponse au client, il se bloque
	// sur un sémaphore ; le dernier point permet donc au master de continuer
	//    - débloquer le master grâce à un second sémaphore (cf. ci-dessous)
	giveSem(synch);
	
	//    - sortir de la section critique
	giveSem(mutex);
	//    - libérer les ressources (fermeture des tubes, ...)
	myClose(fd_to_master);
	myClose(fd_from_master);

	return EXIT_SUCCESS;
}
