#include "matiere.h"

int recherche(matiere *tableau, matiere *recherchee, int taille) 
{
    for (int j = 0; j < taille; j++) {
        if (tableau[j].reference == recherchee->reference) {
            return j;
        }   
    }
    return -1;
}