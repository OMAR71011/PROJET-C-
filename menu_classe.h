
#ifndef MENU_CLASSE_H
#define MENU_CLASSE_H

#define MAX_CLASSES 100
#define MAX_NAME 50
#define FICHIER_CLASSES "data/classes.csv"

typedef struct {
    int code;
    char nom[MAX_NAME];
    char niveau; 
} Classe;

void ajouter_classe();
void afficher_classes();
void modifier_classe();
void rechercher_classe();
void supprimer_classe();
void menu_classe();
void initialiser_fichiers(void);

#include "menu_classe.h"

#endif
