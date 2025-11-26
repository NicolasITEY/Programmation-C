#include <stdio.h>
#include <stdlib.h>

#include "triliste.h"
#include "types.h"
#include "ligneBus.h"
#include "listeDouble.h"








bool comp_coutmaintenance(Tstation *a, Tstation *b){
    return getcoutMaintenance(a) > getcoutMaintenance(b);
}

bool compdatedemaintenance(Tstation *station1,Tstation *station2) {

    if (station1->dateDerniereMaintenance.annee <= station2->dateDerniereMaintenance.annee){
        return false;
    } else {
        return true;
    }
    if (station1->dateDerniereMaintenance.mois <= station2->dateDerniereMaintenance.mois) {
        return false;
    } else {
        return true;
    }
     if (station1->dateDerniereMaintenance.jour <= station2->dateDerniereMaintenance.jour){
        return false;
    } else {
        return true;
    }

}



void tri_selection_liste(TlisteStation mylisteStation,bool (*fdiffcomp)(Tstation *a, Tstation *b)){
    TlisteStation en_cours, comparateur, j;
    for (en_cours = mylisteStation; !listeVide(en_cours); en_cours = getNextCell(en_cours)){
        comparateur = en_cours;
        for (j = en_cours; !listeVide(j); j=getNextCell(j)){
            if (fdiffcomp(getPtrData(j),getPtrData(comparateur))){
                comparateur = j;
            }
        }
        swapPtrData(en_cours,comparateur);

    }
}

TlisteStation getlistearret(TlisteStation mylistestation){
    TlisteStation current = mylistestation;
    TlisteStation listearret = NULL;
    while (!listeVide(current)){
        listearret = ajoutEnFin(listearret,getPtrData(current));
        current = getNextTroncon(current);
        current = getNextStation(current);

    }
    return listearret;
}

TlisteStation getlistetroncon(TlisteStation mylistestation){
    TlisteStation current = mylistestation;
    TlisteStation listetroncon = NULL;
    current = getNextTroncon(current);
    while (!listeVide(current)){
        listetroncon = ajoutEnFin(listetroncon,getPtrData(current));
        current = getNextStation(current);
        current = getNextTroncon(current);

    }
    return listetroncon;
}

