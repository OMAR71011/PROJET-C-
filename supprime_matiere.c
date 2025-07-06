#include "matiere.h"

void supprime_matiere()
{
    FILE *fichier = fopen("matiere.csv", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    matiere matieres[1000];
    int n = 0;
    char ligne[100];

    // Lecture fichier
    while (fgets(ligne, sizeof(ligne), fichier) && n < 1000) {
        if (sscanf(ligne, "%d,%14[^,],%hd", &matieres[n].reference, matieres[n].libelle, &matieres[n].coeficient) == 3) {
            n++;
        }
    }
    fclose(fichier);
    printf("||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||------------SUPPRESSION DE MATIERES----------||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n\n\n");
    int nb;
    printf("Combien de matières à supprimer? : ");
    if (scanf("%d", &nb) != 1) {
        printf("Saisie invalide\n");
        return;
    }

    int refs[nb];
    for (int i = 0; i < nb; i++) {
        int ref;
        int valid = 0;
        do {
            printf("Référence %d (entier uniquement) : ", i + 1);
            char buffer[32];
            if (scanf("%31s", buffer) != 1) {
                printf("Erreur de saisie. Veuillez entrer un entier : ");
                while (getchar() != '\n');
                continue;
            }
            char *endptr;
            ref = (int)strtol(buffer, &endptr, 10);
            if (*endptr != '\0') {
                printf("Entrée invalide. Veuillez entrer un entier sans espace ni lettre.\n");
            } else {
                valid = 1;
            }
        } while (!valid);
        refs[i] = ref;
    }

    // Écriture nouveau fichier
    fichier = fopen("matiere.csv", "w");
    if (fichier == NULL) {
        perror("Erreur d'écriture");
        return;
    }

    fprintf(fichier, "reference,libelle,coefficient\n");
    int trouve = 0;
    for (int i = 0; i < n; i++) {
        int supprimer = 0;
        for (int j = 0; j < nb; j++) {
            if (matieres[i].reference == refs[j]) {
                supprimer = 1;
                trouve = 1;
                break;
            }
        }
        if (!supprimer) {
            fprintf(fichier, "%d,%s,%hd\n", matieres[i].reference, matieres[i].libelle, matieres[i].coeficient);
        }
    }
    fclose(fichier);

    if (!trouve) {
        printf("Aucune référence trouvée\n");
    } else {
        printf("Suppression effectuée\n");
    }
    wait_for_enter();
}