// donne/affichage.c
#include <stdio.h>
#include <stdlib.h>
#include "affichage.h"

#define FICHIER_CLASSES "classes.csv"

void efface_ecran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
