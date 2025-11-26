#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "types.h"
#include "ligneBus.h"
#include "listeDouble.h"


//700 par 700

//fonction qui fait les fscan
//check le nombre de ligne qui verifie si le prochain  print  \n  est vide

TlisteStation *creedeslignesdebus(int *nbrdeligne) {



    int Totalnumberofstation;
    int taille = *nbrdeligne;

    //mettre le nbr of lignes en parametre pas en varaible locale
    //le nombre de station par ligne  CHECK
    //le nombre de ligne total CHECK
    FILE *lignedebus = fopen("listelignedeBus.txt", "r");
    if (!lignedebus) {
        perror("Erreur d'ouverture du fichier");
        return NULL;
    }


    if (fscanf(lignedebus,"%d",&taille) < 1 || taille <= 0 ) {
        printf("Erreur : nombre de ligne invalide\n");
        fclose(lignedebus);
        return NULL;

    }
    //printf("%d\n",taille);
    int nbstationsParLigne[taille];

    for (int i=0; i<taille;i++ ) {
        if ( fscanf(lignedebus,"%d",&nbstationsParLigne[i]) < 1 || nbstationsParLigne[i] <= 0) {
            perror("Aucune station n'a de ligne d'affecter");
            return NULL;
        }
    }


    if (fscanf(lignedebus, "%d", &Totalnumberofstation) <1 || Totalnumberofstation <= 0) {
        printf("Erreur : nombre de stations invalide\n");
        fclose(lignedebus);
        return NULL;
    }

    // Allocation dynamique après avoir lu Totalnumberofstation


    Tstation **stations = (Tstation**)malloc(sizeof(Tstation*) * Totalnumberofstation);

    if (stations == NULL) {
        printf("Erreur : allocation mémoire échouée\n");
        fclose(lignedebus);
        return NULL;
    }

    TlisteStation newLigne = NULL;
    TlisteStation *ligneenposition =  (TlisteStation *)malloc(taille * sizeof( TlisteStation));



    int count = 0;
    double distancetemps;
    for (int j = 0; j<taille ;j++) {

        for (int i = count ; i <  count + nbstationsParLigne[j];i++){
            int posX, posY, id;
            char nomStation[30];
            fscanf(lignedebus, "%d %d %d %s\n ", &id, &posX, &posY, nomStation);
            printf("%d %d %d %s\n",id,posX,posY,nomStation);
            if (posX > 700 || posY > 700) {
                printf("Format invalide : station hors limites\n");
                free(stations);
                fclose(lignedebus);
                return NULL;
            }
            stations[i] = creeArret(posX, posY, nomStation, id);



        }
        newLigne = ajoutEnFin(newLigne, stations[count]);

        for (int i = count + 1; i< count + nbstationsParLigne[j];i++){
            //newLigne = ajoutEnFin(newLigne, stations[i]);
            distancetemps = sqrt(pow(stations[i]->posX - stations[i-1]->posX, 2) +
                       pow(stations[i]->posY - stations[i-1]->posY, 2));
            Tstation *troncon = creeTroncon(j+1, stations[i-1], stations[i], distancetemps, distancetemps);
            // Ajout du tronçon à la liste
            newLigne = ajoutEnFin(newLigne, troncon);


            // Ajout de la station à la liste
            newLigne = ajoutEnFin(newLigne, stations[i]);
        }

        ligneenposition[j] = newLigne;
        count = count + nbstationsParLigne[j];
        //afficheListe(newLigne);
        newLigne =NULL;
 // Libérez le tableau si vous n'avez plus besoin de ce pointeur


    }
    afficheListe(ligneenposition[2]);
    printf("fin de fonction\n");
    fclose(lignedebus);
    *nbrdeligne = taille;
    return ligneenposition;
}


TlisteStation* convertV2() {

	FILE *file = fopen("listelignedeBus.txt", "r");
	if (!file) {
		perror("Erreur d'ouverture du fichier");
		return NULL;
	}

	int nbLignes;
	fscanf(file, "%d", &nbLignes);

	int *nbStationsParLigne = malloc(nbLignes * sizeof(int));
	for (int i = 0; i < nbLignes; i++) {
		fscanf(file, "%d", &nbStationsParLigne[i]);
	}

	int nbStationsUniques;
	fscanf(file, "%d", &nbStationsUniques);

	// Lire les stations uniques
	Tstation **stationsUniques = malloc(nbStationsUniques * sizeof(Tstation*));
	for (int i = 0; i < nbStationsUniques; i++) {
		int posX, posY, id;
		char nomStation[30];
		fscanf(file, "%d %d %d %s", &id, &posX, &posY, nomStation);

		// Vérifier que les coordonnées sont dans les limites
		if (posX > 700 || posY > 700) {
			printf("Format invalide : station hors limites\n");
				free(stationsUniques);
				fclose(file);
				return NULL;
		}

		stationsUniques[i] = creeArret(posX, posY, nomStation, id);
	}

	// Lire les lignes et les créer
	TlisteStation *toutesLesLignes = malloc(nbLignes * sizeof(TlisteStation));
	for (int i = 0; i < nbLignes; i++) {
		toutesLesLignes[i] = NULL;
		int precedentId = -1;

		for (int j = 0; j < nbStationsParLigne[i]; j++) {
			int id;
			fscanf(file, "%d", &id);

			// Rechercher la station avec cet ID
			Tstation *station = NULL;
			for (int k = 0; k < nbStationsUniques; k++) {
				if (stationsUniques[k]->idStation == id) {
					station = stationsUniques[k];
					break;
				}
			}

			if (station == NULL) {
				printf("Erreur : station ID %d introuvable\n", id);
				continue;
			}

			if (precedentId != -1) {
				// Créer un tronçon entre l'ancienne et la nouvelle station
				Tstation *dep = NULL, *arr = station;

				for (int k = 0; k < nbStationsUniques; k++) {
					if (stationsUniques[k]->idStation == precedentId) {
						dep = stationsUniques[k];
						break;
					}
				}
				double distancetemps = sqrt(pow(arr->posX - dep->posX, 2) + pow(arr->posY - dep->posY, 2));
				if (dep != NULL) {
					Tstation *troncon = creeTroncon(i + 1, dep, arr, distancetemps,distancetemps );

					toutesLesLignes[i] = ajoutEnFin(toutesLesLignes[i], troncon);
				}
			}

			// Ajouter la station à la ligne
			toutesLesLignes[i] = ajoutEnFin(toutesLesLignes[i], station);
			precedentId = id;
		}
	}

	fclose(file);
	free(nbStationsParLigne);

	return toutesLesLignes;
}
