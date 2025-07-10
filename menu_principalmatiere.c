#include "matiere.h"

void Gestion_matieres()
{
    int choix;
    do {
        menu_principal_matiere();
        printf("\n\nVeuillez choisir le numéro de l'option que tu souhaites effectuer : ");
        char buffer[32];
        int valid = 0;
        while (!valid) {
            if (scanf("%31s", buffer) != 1) 
            {
                system("clear");
            menu_principal_matiere();
                printf("\nSaisie invalide\n");
                while (getchar() != '\n');
                printf("Veuillez choisir le numéro de l'option que tu souhaites effectuer : ");
                continue;
            }
            char *endptr;
            choix = (int)strtol(buffer, &endptr, 10);
            if (*endptr != '\0') 
            {
                system("clear");
                menu_principal_matiere();
                printf("\nSaisie invalide. Veuillez entrer un entier.\n");
                printf("Veuillez choisir le numéro de l'option que tu souhaites effectuer : ");
            } else {
                valid = 1;
            }
        }

        if (choix >= 0 && choix <= 5) {
            system("clear");
        }
        switch (choix) 
        {
            case 1:
                Ajout_matiere();
                break;
            case 2:
                modifie_matiere();
                break;
            case 3:
                recherche_matiere();
                break;
            case 4:
                supprime_matiere();
                break;
            case 5:
                affiche_matiere();
                break;
            case 0:
                printf("||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
                       "||*********************************************||\n"
                       "||-----------------AU REVOIR!------------------||\n"
                       "||*********************************************||\n"
                       "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n\n");
                break;
            default:
                system("clear");
                printf("Choix invalide. Veuillez choisir une nombre entre 1 et 5\n\n\n");
        }
    } while (choix != 0);

}

void menu_principal_matiere()
{
    printf("||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||----------LA GESTION DES MATIÈRES------------||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||         1. Ajouter une matière              ||\n"
           "||         2. Modifier une matière             ||\n"
           "||         3. Rechercher une matière           ||\n"
           "||         4. Supprimer une matière            ||\n"
           "||      5. Afficher toutes les matières        ||\n"
           "||         O. Retour au menu principal         ||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n");
}