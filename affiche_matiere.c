#include "matiere.h"

void affiche_matiere()
{
    FILE *fichier = fopen("matiere.csv", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    matiere afficher[1000];
    int i = 0;
    char ligne[100];

    // Sauter l'en-tête
    if (fgets(ligne, sizeof(ligne), fichier)) {
        // Vérifie si c'est un en-tête
        int is_header = 0;
        for (int h = 0; ligne[h] != '\0'; h++) {
            if ((ligne[h] >= 'A' && ligne[h] <= 'Z') || (ligne[h] >= 'a' && ligne[h] <= 'z')) {
                is_header = 1;
                break;
            }
        }
        if (!is_header) {
            rewind(fichier);
        }
    }

    while (fgets(ligne, sizeof(ligne), fichier)) {
        if (sscanf(ligne, "%d,%14[^,],%hd", &afficher[i].reference, afficher[i].libelle, &afficher[i].coeficient) == 3) {
            i++;
        }
    }
    printf("||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||---------------------------------------------||\n"
           "||------------AFFICHAGE DES MATIERES-----------||\n"
           "||---------------------------------------------||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n\n\n");
    if (i == 0) {
        printf("Aucune matière enregistrée.\n");
    } else {
        printf("+-------------+-----------------+------------+\n");
        printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
        printf("+-------------+-----------------+------------+\n");
        for (int j = 0; j < i; j++) {
            char libelle_aff[16];
            strncpy(libelle_aff, afficher[j].libelle, 15);
            libelle_aff[15] = '\0';
            if (strlen(libelle_aff) > 15) {
                libelle_aff[14] = '.';
                libelle_aff[15] = '\0';
            }
            printf("|%-12d | %-15.15s | %-11hd|\n", afficher[j].reference, libelle_aff, afficher[j].coeficient);
            printf("+-------------+-----------------+------------+\n");
        }
    }
    fclose(fichier);
    wait_for_enter();
    
}