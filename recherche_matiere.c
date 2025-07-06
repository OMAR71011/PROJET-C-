#include "matiere.h"

void recherche_matiere()
{
    FILE *fichier = fopen("matiere.csv", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    printf("||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||-------------RECHERCHE DE MATIERES-----------||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n\n\n");
    int nombre_matiere;
    printf("Combien de matières recherchez-vous? : ");
    if (scanf("%d", &nombre_matiere) != 1) {
        printf("Saisie invalide\n");
        fclose(fichier);
        return;
    }

    matiere matiere_recherche[nombre_matiere];
    matiere chercher[1000];
    
    for (int i = 0; i < nombre_matiere; i++) {
        int ref;
        int valid = 0;
        do {
            printf("Référence de la matière %d (entier uniquement) : ", i + 1);
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
        matiere_recherche[i].reference = ref;
    }

    // Lecture fichier
    int i = 0;
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) && i < 1000) {
        if (sscanf(ligne, "%d,%14[^,],%hd", &chercher[i].reference, chercher[i].libelle, &chercher[i].coeficient) == 3) {
            i++;
        }
    }
    fclose(fichier);

    // Recherche et stockage des matières trouvées
    matiere trouvees[nombre_matiere];
    int nb_trouvees = 0;
    for (int k = 0; k < nombre_matiere; k++) {
        matiere temp = {matiere_recherche[k].reference};
        int pos = recherche(chercher, &temp, i);
        if (pos != -1) {
            trouvees[nb_trouvees++] = chercher[pos];
        } else {
            printf("Référence %d non trouvée\n", matiere_recherche[k].reference);
        }
    }
    
    if (nb_trouvees > 0) 
    {
        printf("+--------------------------------------------+\n");
        printf("|              Matières trouvées             |\n");
        printf("+--------------------------------------------+\n");
        printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
        printf("+-------------+-----------------+------------+\n");
        for (int j = 0; j < nb_trouvees; j++) 
        {
            printf("|%-12d | %-15s | %-11hd|\n", trouvees[j].reference, trouvees[j].libelle, trouvees[j].coeficient);
            printf("+-------------+-----------------+------------+\n");

        }
    }
    wait_for_enter();
}
