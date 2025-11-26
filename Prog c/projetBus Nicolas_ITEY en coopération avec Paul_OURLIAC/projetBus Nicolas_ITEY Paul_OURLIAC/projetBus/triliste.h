#ifndef TRILISTE_H_INCLUDED
#define TRILISTE_H_INCLUDED

#include "types.h"
#include <stdbool.h>

bool comp_coutmaintenance(Tstation *a, Tstation *b);

bool compdatedemaintenance(Tstation *station1,Tstation *station2);

void tri_selection_liste(TlisteStation mylisteStation,bool (*fdiffcomp)(Tstation *a, Tstation *b));

TlisteStation getlistearret(TlisteStation mylistestation);

TlisteStation getlistetroncon(TlisteStation mylistestation);

#endif // TRILISTE_H_INCLUDED
