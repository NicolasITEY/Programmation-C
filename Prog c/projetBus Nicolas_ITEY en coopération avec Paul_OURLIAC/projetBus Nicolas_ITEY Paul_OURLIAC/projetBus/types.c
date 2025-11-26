#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include <time.h>

void afficheStation( Tstation *station){
    if (station == NULL){
        printf("\nerreur station non allouée!\n");
    }
    else{
        if (station->arret_ou_troncon == ARRET){
            t_date datemaint = getdatedernieremaintenance(station);
            printf("\nSTATION idStation: %d, Station : %s, posX = %d, posY = %d, cout maintenance = %d, date maintenance : %d/%d/%d",
                   station->idStation, station->nomStation, station->posX, station->posY, getcoutMaintenance(station),datemaint.jour,datemaint.mois,datemaint.annee);
        }
        else{ //alors le noeud est un tronçon
            t_date datemaint = getdatedernieremaintenance(station);
            printf("\n\nTRONCON idLigneBus: %d, temps: %d sec, distance à parcourir: %d mètres, cout maintenance = %d, date maintenance : %d/%d/%d",
                   station->idLigneBus, station->coutTemps, station->coutDistance, getcoutMaintenance(station),datemaint.jour,datemaint.mois,datemaint.annee);
            afficheStation(station->depart);
            afficheStation(station->arrivee);
            printf("\nfin tronçon\n");
        }
    }
}




// Getteurs

int getIdStation( Tstation *myStation){
    return myStation->idStation;
}

char *getNomStation( Tstation *myStation){
    return myStation->nomStation;
}

int getPosXStation( Tstation *myStation ){
    return myStation->posX;
}
int getPosYStation( Tstation *myStation ){
    return myStation->posY;
}

int getIdLigneTroncon(Tstation *myStation){
    //une station n'est pas liée à une ligne, seulement le troncon
    if (getTypeNoeud(myStation)==TRONCON)
        return myStation->idLigneBus;
    else{
        printf("\n(getIdLigneTroncon) Erreur algo, vous n etes pas sur un troncon");
        return -1;
    }
}

TypeNoeud getTypeNoeud(Tstation *myStation){
    return myStation->arret_ou_troncon;
}

int getcoutMaintenance(Tstation *myStation){
    return myStation->coutMaintenance;
}


int getPosXBus( Tbus myBus ){
    return myBus->posXBus;
}
int getPosYBus( Tbus myBus ){
    return myBus->posYBus;
}
int getIdBus( Tbus myBus ){
    return myBus->idBus;
}
int getIdLigneActuelleDuBus( Tbus myBus ){
    return myBus->idLigneBusActuelle;
}

TsensParcours getSensParcours(Tbus myBus){
    return myBus->sensParcours;
}

TlisteStation getActualStation( Tbus myBus ){
    return myBus->positionSurLaLigneDeBus;
}

int getIdLigneBus(Tbus myBus){
    return myBus->idLigneBusActuelle;
}

t_date getdatedernieremaintenance (Tstation *mystation) {
    return mystation->dateDerniereMaintenance;
}

// Setteurs

void setActualStation( Tbus myBus, TlisteStation arrivalStation ){
    myBus->positionSurLaLigneDeBus = arrivalStation;
}

void setPosXBus(Tbus myBus, int newX){
    myBus->posXBus = newX;
}

void setPosYBus(Tbus myBus, int newY){
    myBus->posYBus = newY;
}

void setIdLigneBus(Tbus myBus, int idLigne){
    myBus->idLigneBusActuelle = idLigne;
}

void setSensParcours(Tbus myBus, TsensParcours sens ){
    myBus->sensParcours = sens;
}

void setPositionSurLaLigneDeBus( Tbus myBus, TlisteStation myStation){
    myBus->positionSurLaLigneDeBus = myStation;
}

void setcoutmaintenance(Tstation *myStation){
    myStation->coutMaintenance = (rand() % 90 )+10;
}

t_date setdate () {
    t_date date;
    date.jour = rand()%31+1;
    date.mois = rand()%12+1;
    int nbr_annee = rand()%100;
    date.annee = 2025 + nbr_annee;
    if (date.jour > 30 && ((date.mois == 2) || (date.mois == 4) || (date.mois == 6) || (date.mois == 9) || (date.mois == 11) )) {
        date.jour -=3;
    }
    return date;
}

void setdatedernieremaintenance (Tstation *mystation,t_date date ) {
    mystation->dateDerniereMaintenance = date;

}

