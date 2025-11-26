#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "listeDouble.h"
#include "ligneBus.h"


//initListe ne fait pas de malloc, juste une initialisation à NULL du pointeur de liste
void initListe(T_liste *l){
*l=NULL;
}



bool listeVide( T_liste l){
    return (l==NULL);
}

void afficheListe( T_liste l){
    T_liste courant = l;
    while (courant!=NULL){
        afficheStation(courant->pdata);  //fourni par types.h
        courant=courant->suiv;
    }
}

T_liste ajoutEnTete(T_liste l, Tstation* mydata){
    T_liste nouv = (T_liste)malloc(sizeof(struct T_cell));

    //CHANGEMENT par rapport à la partie2 d'updago
    //nouv->pdata = (int*)malloc(sizeof(int)); ATTENTION PLUS DE RECOPIE de mydata, donc plus d'allocation mémoire sur ce champ

    //SIMPLE BRANCHEMENT EN MEMOIRE ENTRE POINTEURS (SUR LA STATION EXISTANTE)
    nouv->pdata=mydata;

    if (l==NULL) // on cree en fait la premiere cellule de la liste
    {
        nouv->suiv = NULL;  //sécurise la présence de NULL sur le champ suiv, au cas où le ptr l n'aurait pas été  initialisé correctement via initListe
        nouv->prec = NULL;
    }
    else  // la lste n'etait pas vide, on doit donc faire les branchements
    {
        nouv->suiv = l;
        nouv->prec = NULL;
        l->prec = nouv;
    }
    return nouv;
}

T_liste ajoutEnFin(T_liste l, Tstation* mydata){
    T_liste nouv, courant=l;  //remarque: pas de malloc si on appelle ajoutEnTete

    if (l==NULL) // on cree en fait la premiere cellule de la liste
    {
        return ajoutEnTete(l, mydata);
    }
    else  // la liste n'etant pas vide,déplacment sur la derniere cellule, malloc et branchements
    {
        while (courant->suiv != NULL){
            courant=courant->suiv;
        }
        nouv = (T_liste)malloc(sizeof(struct T_cell));
        nouv->pdata=mydata;

        nouv->suiv = NULL;
        nouv->prec = courant;
        courant->suiv = nouv;
    }
    return l;  //la tête d'origine, qui n'a pas changé
}

Tstation* getPtrData(T_liste l){
    if (l==NULL)
    {
        printf("\nERREUR ptr pdata non alloué");
        return NULL;
    }
    else
    {
        return l->pdata;
    }
}

T_liste getNextCell(T_liste l){
    if (l==NULL)
    {
        return NULL;  //convention
    }
    else{
        return l->suiv;
    }
}

T_liste getPrevCell(T_liste l){
    if (l==NULL)
    {
        return NULL;  //convention
    }
    else{
        return l->prec;
    }
}

//A vous la suite si besoin
void swapPtrData( TlisteStation source, TlisteStation destination ){
    Tstation* tmp = source->pdata;
    source->pdata=destination->pdata;
    destination->pdata=tmp;
}

TlisteStation getlastcell(TlisteStation mylistestation){
    TlisteStation current = mylistestation;
    while(getNextCell(current)!=NULL){
        current = getNextCell(current);
    }
    return current;
}


TlisteStation* createNewListeFromFusion(TlisteStation *tabTlisteStation,int *taille, int l1 , int l2){
    TlisteStation ligneA = tabTlisteStation[l1];
    TlisteStation ligneB = tabTlisteStation[l2];
    TlisteStation nouv = NULL;
    TlisteStation *ligneenposition =  (TlisteStation *)malloc(*taille + 1 * sizeof( TlisteStation));
    double distancetemps;

    while (getNextStation(ligneA)!=NULL){
        nouv=ajoutEnFin(nouv,getPtrData(ligneA));
        distancetemps = sqrt(pow(getPtrData(getlastcell(nouv))->posX - getPtrData(ligneA)->posX, 2) +
                       pow(getPtrData(getlastcell(nouv))->posY - getPtrData(ligneA)->posY, 2));
        nouv = ajoutEnFin(nouv,creeTroncon(*taille + 1 ,getPtrData(ligneA),getPtrData(getNextStation(ligneA)),distancetemps,distancetemps));
        ligneA=getNextStation(ligneA);
    }

    nouv = ajoutEnFin(nouv, getPtrData(ligneA));
    distancetemps = sqrt(pow(getPtrData(ligneB)->posX - getPtrData(ligneA)->posX, 2) +
                       pow(getPtrData(ligneB)->posY - getPtrData(ligneA)->posY, 2));
    nouv = ajoutEnFin(nouv,creeTroncon(*taille + 1 ,getPtrData(ligneA),getPtrData(ligneB),distancetemps,distancetemps));

    while (getNextStation(ligneB)!=NULL){
        nouv=ajoutEnFin(nouv,getPtrData(ligneB));
        distancetemps = sqrt(pow(getPtrData(getlastcell(nouv))->posX - getPtrData(ligneB)->posX, 2) +
                       pow(getPtrData(getlastcell(nouv))->posY - getPtrData(ligneB)->posY, 2));
        nouv = ajoutEnFin(nouv,creeTroncon(*taille + 1 ,getPtrData(ligneB),getPtrData(getNextStation(ligneB)),distancetemps,distancetemps));
        ligneB=getNextStation(ligneB);

    }
    nouv = ajoutEnFin(nouv, getPtrData(ligneB));

    for (int i = 0; i < *taille; i++){
        ligneenposition[i]=tabTlisteStation[i];
    }
    ligneenposition[*taille]=nouv;
    *taille = *taille + 1;
    free(tabTlisteStation);
    //printf("ok");
    //afficheListe(ligneenposition[0]);
    //afficheListe(ligneenposition[1]);
    //afficheListe(nouv);
    return  ligneenposition;
}

T_liste ajoutEnPosition(T_liste l, Tstation mydata, int pos) {
	if (pos ==0) {
		return ajoutEnTete(l,&mydata);
	}
	T_liste liste = (T_liste)malloc(sizeof(T_cellule));
	liste->pdata = (Tstation*)malloc(sizeof(Tstation));
	*(liste->pdata)=mydata;
	T_liste temp = l;

	for (int i=0 ; i < pos && (!listeVide(temp)); i++) {
		temp = temp->suiv;

	}
	if (temp == NULL ) {
		return l;

	}
	liste->suiv = temp->suiv;
	liste->prec = temp->prec;
	temp->suiv = liste;
	liste->prec = temp;

	return l;
}

TlisteStation Suppstation(TlisteStation ligne, int n) {

    TlisteStation current = ligne;
    int count = 0;
    while (current != NULL && count < n) {
        current = getNextStation(current);
        count++;
    }
    TlisteStation prevStation = getPreviousStation(current);
    TlisteStation nextStation = getNextStation(current);
    TlisteStation tronconAvant = getPrevTroncon(current);
    TlisteStation tronconApres  = getNextTroncon(current);


    if (prevStation != NULL) {
        prevStation->suiv = nextStation;
    } else {

        ligne = nextStation;
    }
    if (nextStation != NULL) {
        nextStation->prec = prevStation;
    }


    if (tronconAvant != NULL) {
        free(tronconAvant->pdata);
        free(tronconAvant);
    }
    if (tronconApres != NULL) {
        free(tronconApres->pdata);
        free(tronconApres);
    }


    if (prevStation != NULL && nextStation != NULL) {
    Tstation *nouveau_troncon_data = creeTroncon(
        getIdLigneTroncon(getPtrData(prevStation)),
        getPtrData(prevStation),
        getPtrData(nextStation),
        ligne->pdata->coutTemps,
        ligne->pdata->coutMaintenance
    );

    TlisteStation nouveauTroncon = (TlisteStation)malloc(sizeof(struct T_cell));
    nouveauTroncon->pdata = nouveau_troncon_data;

    nouveauTroncon->prec = prevStation;
    nouveauTroncon->suiv = nextStation;

    prevStation->suiv = nouveauTroncon;
    nextStation->prec = nouveauTroncon;
} else if (prevStation == NULL && nextStation != NULL) {
    ligne = nextStation;
}


    return ligne;
}

