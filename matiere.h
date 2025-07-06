#ifndef MATIERE_H
#define MATIERE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct matiere {
    int reference;
    char libelle[15];
    short coeficient;
} matiere;

void Gestion_matieres(void);
void menu_principal_matiere(void);
void Ajout_matiere(void);
void modifie_matiere(void);
void recherche_matiere(void);
void supprime_matiere(void);
void affiche_matiere(void);
void clear_screen(void);
void wait_for_enter(void);

int recherche(matiere *tableau, matiere *recherchee, int taille);

#endif

