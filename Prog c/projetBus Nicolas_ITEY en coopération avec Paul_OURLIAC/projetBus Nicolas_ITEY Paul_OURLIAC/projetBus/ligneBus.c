#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "types.h"
#include "ligneBus.h"
#include "listeDouble.h"

Tbus creeBus( int idBus, TlisteStation start ){
    Tbus myBus = (Tbus)malloc(sizeof(Typebus));
    myBus->idBus = idBus;
    busSurStation( myBus, start, depart_vers_arrivee);
    busSurLigneX ( myBus, getIdLigneTroncon(getPtrData(getNextTroncon(start))));
    return myBus;
}


Tstation *creeArret( int posX, int posY, char* nomStation, int idStation){
    Tstation* newStation = (Tstation*)malloc(sizeof(Tstation));

    newStation->arret_ou_troncon = ARRET;

    //champs utiles si ARRET
    newStation->posX = posX;
    newStation->posY = posY;
    strcpy(newStation->nomStation, nomStation);
    newStation->idStation = idStation;
    setdatedernieremaintenance(newStation,setdate());
    setcoutmaintenance(newStation);


    //champs non utiles si ARRET
    newStation->depart = NULL;
    newStation->arrivee = NULL;
    newStation->coutTemps = 0;
    newStation->coutDistance = 0;
    newStation->idLigneBus= -1;  //id inexistant
    return newStation;
}

Tstation *creeTroncon(int idLigneBus, Tstation* depart, Tstation *arrivee, int coutTemps, int coutDistance){
    Tstation* newStation = (Tstation*)malloc(sizeof(Tstation));

    newStation->arret_ou_troncon = TRONCON;

    //champs non utiles si TRONCON
    newStation->posX = 0;
    newStation->posY = 0;
    strcpy(newStation->nomStation, "");
    newStation->idStation = -1;  //id inexistant

    //champs utiles si TRONCON
    newStation->idLigneBus=idLigneBus;
    newStation->depart = depart;
    newStation->arrivee = arrivee;
    newStation->coutTemps = coutTemps;
    newStation->coutDistance = coutDistance;
    setdatedernieremaintenance(newStation,setdate());
    setcoutmaintenance(newStation);

    return newStation;
}

void afficheConsoleLigneBus( TlisteStation l){
    afficheListe(l);
}

TlisteStation getNextStation( TlisteStation l){
    if (ligneBusVide(l)) return NULL;
    else {
            if (getTypeNoeud(getPtrData(l))==ARRET) {
                    if (ligneBusVide( getNextCell(l) )) return NULL;  //nous sommes sur un terminus
                    else return getNextCell( getNextCell( l ) );  //le prochain arret est dans 2 cellules dans la liste, la cellule suivaante �tant un TRONCON
            }
            else return getNextCell( l );  //si on est sur un TRONCON alors la prochaine cellule est la prochaine station (ARRET)
    }
}

TlisteStation getNextTroncon( TlisteStation l){
    if (ligneBusVide(l)) return NULL;
    else {
            if (getTypeNoeud(getPtrData(l))==ARRET) {
                return getNextCell(l);  //la prochaine cellule est un TRONCON, peut �tre �gale � NULL si on est sur un terminus
            }
            else{
                    printf("\n(getNextTroncon) erreur algo, on est deja sur un troncon");
                    return l;  //on retourne le troncon actuel
            }
    }
}

TlisteStation getPrevTroncon( TlisteStation l) {
	if (ligneBusVide(l)) return NULL;
	else {
		if (getTypeNoeud(getPtrData(l))==TRONCON) {
			if (ligneBusVide( getPrevCell(l) )|| ligneBusVide(getPrevCell(getPrevCell(l)))) return NULL;  //nous sommes sur un terminus
			return getPrevCell( getPrevCell( l ) );  //le prochain arret est dans 2 cellules dans la liste, la cellule suivaante �tant un TRONCON
		} else return getPrevCell( l ); //si on est sur un TRONCON alors la prochaine cellule est la prochaine station (ARRET)
	}
}

TlisteStation getPreviousStation( TlisteStation l){
    if (ligneBusVide(l)) return NULL;
    else {
            if (getTypeNoeud(getPtrData(l))==ARRET) {
                    if (ligneBusVide( getPrevCell(l) )) return NULL;  //nous sommes sur un terminus
                    return getPrevCell( getPrevCell( l ) );  //le prochain arret est dans 2 cellules dans la liste, la cellule suivaante �tant un TRONCON
            }
            else return getPrevCell( l );  //si on est sur un TRONCON alors la prochaine cellule est la prochaine station (ARRET)
    }
}

bool ligneBusVide( TlisteStation l){
    return listeVide( l );
}

int getPosXListeStation( TlisteStation myStationInListe ){
    return getPosXStation(getPtrData(myStationInListe));
}

int getPosYListeStation( TlisteStation myStationInListe ){
    return getPosYStation(getPtrData(myStationInListe));
}

void deplaceBus(Tbus myBus, TsensParcours sens_deplacement, int *incXSprite, int *incYSprite){
    TlisteStation dest;
    int xdep, ydep, xarr, yarr, pas;
    float ratio;
    pas = 2;  //�quivalent au parametre d'erreur dans le trac� de Segment de Bresenham
    //https://fr.wikipedia.org/wiki/Algorithme_de_trac%C3%A9_de_segment_de_Bresenham


    if (sens_deplacement == depart_vers_arrivee) dest = getNextStation(getActualStation(myBus));
        else dest = getPreviousStation(getActualStation(myBus));

    //par d�faut: pas de d�placement du bus
    *incXSprite = 0; *incYSprite = 0;

    //si un prochain arret existe?
    if (ligneBusVide(dest)){
        //printf("\nBus id%d est au terminus de la ligne %d sur la station id=%d %s", getIdBus(myBus), getIdLigneActuelleDuBus(myBus),getIdStation(getPtrData(getActualStation(myBus))),getNomStation(getPtrData(getActualStation(myBus))));
    }
    else //s'il existe:
    {
        //soit on est arriv�, soit on est en cours de trajet
        //r�cup�ration des coordonn�es
        xdep = getPosXBus(myBus);
        ydep = getPosYBus(myBus);
        xarr = getPosXListeStation(dest);
        yarr = getPosYListeStation(dest);

        //sommes-nous arriv�s?
        if ((abs(xdep-xarr)<=pas) && (abs(ydep-yarr)<=pas)){
            //alors mise � jour de la station actuelle du bus
            setActualStation( myBus, dest );
            printf("\nLe Bus id%d arrive sur la station id%d %s",getIdBus(myBus),getIdStation(getPtrData(getActualStation(myBus))),getNomStation(getPtrData(getActualStation(myBus))));
        }
        else{
            //sinon on calcule les offsets de d�placements pour la boucle main qui r�alise les affichages
            ratio = (float)(abs(yarr-ydep))/(float)(abs(xarr-xdep)); //div euclydienne

            //algo de Bresenham "simplifi� de fa�on ad hoc" et non optimis� (� cause des floats)
            if (xarr>xdep) *incXSprite = pas;
            else if (xarr<xdep) *incXSprite = -pas;

            if (yarr>ydep) *incYSprite = (int)((float)(pas) * ratio);             //pour un d�placement proportionnel en Y selon Y/X
            else if (yarr<ydep) *incYSprite = (int)((float)(-pas) * ratio);

            //remmarque: si xarr==ydep alors *incXSprite reste � 0, idem en Y

            //maj des coord du bus
            setPosXBus(myBus, getPosXBus(myBus) + *incXSprite);
            setPosYBus(myBus, getPosYBus(myBus) + *incYSprite);
        }
    }
}



void busSurLigneX( Tbus myBus, int idLigneX ){
    setIdLigneBus(myBus, idLigneX);
}

void busSurStation( Tbus myBus, TlisteStation myStation, TsensParcours sens){

    //Exemple encapsulation: on a pr�f�r� les fonctions setteurs � un acc�s direct aux champs

    setPositionSurLaLigneDeBus( myBus, myStation);
    //myBus->positionSurLaLigneDeBus = myStation;  //interdit

    setSensParcours( myBus, sens);
    //myBus->sensParcours = sens;

    setPosXBus( myBus, getPosXListeStation( myStation ) );
    //myBus->posXBus = getPosXListeStation( myStation );

    setPosYBus( myBus, getPosYListeStation( myStation ) );
    //myBus->posYBus = getPosYListeStation( myStation );

    busSurLigneX(myBus,getIdLigneTroncon(getPtrData(getNextTroncon(myStation))));

    //idem : on passe ci-dessous par les getteurs et non par un acc�s direct via les champs
    printf("\nBus id%d est en (x = %d, y = %d) sur la ligne %d, station %s", getIdBus(myBus), getPosXBus(myBus), getPosYBus(myBus), getIdLigneActuelleDuBus(myBus), getNomStation(getPtrData(myStation)));
}


TlisteStation stationSuivantePourLeBus(Tbus myBus){
    TlisteStation prochaineStation;
    if (getSensParcours(myBus)==depart_vers_arrivee) prochaineStation = getNextStation(getActualStation(myBus));
    else prochaineStation = getPreviousStation(getActualStation(myBus));
    return prochaineStation;
}

void afficheCoordonneesBus( Tbus myBus ){
    char *nomProchaineStation;
    TlisteStation prochaineStation = stationSuivantePourLeBus(myBus);

    if (ligneBusVide(prochaineStation)) printf("\nBus id: %d est en (x = %d, y = %d) ligne %d, derniere station %s (Terminus)", getIdBus(myBus), getPosXBus(myBus), getPosYBus(myBus), getIdLigneActuelleDuBus(myBus), getNomStation(getPtrData(getActualStation(myBus))));
    else {
            nomProchaineStation = getNomStation(getPtrData(prochaineStation));
            printf("\nBus id: %d est en (x = %d, y = %d) ligne %d, derniere station %s, prochaine station %s", getIdBus(myBus), getPosXBus(myBus), getPosYBus(myBus), getIdLigneActuelleDuBus(myBus), getNomStation(getPtrData(getActualStation(myBus))), nomProchaineStation);
    }
}



//Cr�er ci-dessous vos fonctions


void ligneboucle(TlisteStation myStation){
    Tstation *dep = getPtrData(getlastcell(myStation));
    Tstation *arr = getPtrData(myStation);
    double distancetemps = sqrt(pow(arr->posX - dep->posX, 2) + pow(arr->posY - dep->posY, 2));
    Tstation *troncon = creeTroncon(1, dep, arr, distancetemps,distancetemps );
    myStation = ajoutEnFin(myStation,troncon);
    TlisteStation lastcell = getlastcell(myStation);
    lastcell->suiv = myStation;
    myStation->prec = lastcell;
    //afficheStation(getPtrData(lastcell));

}
