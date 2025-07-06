#include "matiere.h"

void modifie_matiere()
{
    FILE *fichier = fopen("matiere.csv", "r");
    if (fichier == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    printf("||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||-----------MODIFICATION DES MATIERES---------||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n\n\n");
    int nombre_matiere;
    printf("Combien de matières voulez-vous modifier? : ");
    if (scanf("%d", &nombre_matiere) != 1) {
        printf("Saisie invalide\n");
        fclose(fichier);
        return;
    }

    matiere matiere_modifie[nombre_matiere];
    matiere modifier[1000];
    printf("Collecte des informations sur les matières à modifier.\n\n");

    for (int i = 0; i < nombre_matiere; i++)
    {
        int ref;
        int valid = 0;
        do {
            printf("Veuillez saisir la référence de la matière numéro %d à modifier (entier uniquement) :  ", i + 1);
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
        matiere_modifie[i].reference = ref;
    }

    // Lecture du fichier
    int i = 0;
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier) && i < 1000) 
    {
        if (sscanf(ligne, "%d,%14[^,],%hd", &modifier[i].reference, modifier[i].libelle, &modifier[i].coeficient) == 3) 
        {
            i++;
        }else 
        {
            printf("Erreur format ligne %d\n", i + 1);
            while (fgetc(fichier) != '\n' && !feof(fichier))
            {
                continue;
            }
        }
    }
    fclose(fichier);

    // Modification et stockage des matières modifiées
    matiere modifiees[nombre_matiere];
    int nb_modifiees = 0;
    for (int k = 0; k < nombre_matiere; k++) 
    {
        matiere temp = {matiere_modifie[k].reference};
        int pos = recherche(modifier, &temp, i);
        if (pos != -1) 
        {
            printf("Saisie des nouvelles informations pour la matière dont la référence est %d :\n", modifier[pos].reference);
            // Libellé : lettres, chiffres, tiret 6 (-) et tiret 8 (_), rien d'autre
            char libelle_tmp[15];
            int libelle_valide = 0;
            do {
                printf("Nouveau libellé (lettres, chiffres, - et _ uniquement) : ");
                scanf("%14s", libelle_tmp);
                libelle_valide = 1;
                for (int c = 0; libelle_tmp[c] != '\0'; c++) {
                    char ch = libelle_tmp[c];
                    if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '-' || ch == '_')) {
                        libelle_valide = 0;
                        printf("Caractère non autorisé dans le libellé : %c\n", ch);
                        break;
                    }
                }
            } while (!libelle_valide);
            strcpy(modifier[pos].libelle, libelle_tmp);

            // Coefficient : entier court uniquement
            short coef;
            int coef_valide = 0;
            do {
                printf("Nouveau coefficient (entier court uniquement) : ");
                char coefbuf[16];
                if (scanf("%15s", coefbuf) != 1) {
                    printf("Erreur de saisie. Veuillez entrer un entier court : ");
                    while (getchar() != '\n');
                    continue;
                }
                char *endptr;
                long val = strtol(coefbuf, &endptr, 10);
                if (*endptr != '\0' || val < -32768 || val > 32767) {
                    printf("Entrée invalide. Veuillez entrer un entier court valide.\n");
                } else {
                    coef = (short)val;
                    coef_valide = 1;
                }
            } while (!coef_valide);
            modifier[pos].coeficient = coef;

            modifiees[nb_modifiees++] = modifier[pos];
        } else 
        {
            printf("Référence %d non trouvée\n", matiere_modifie[k].reference);
        }
    }

    // Réécriture
    fichier = fopen("matiere.csv", "w");
    if (fichier == NULL) 
    {
        perror("Erreur d'écriture");
        return;
    }
    fprintf(fichier, "reference,libelle,coefficient\n");
    for (int j = 0; j < i; j++) 
    {
        fprintf(fichier, "%d,%s,%hd\n", modifier[j].reference, modifier[j].libelle, modifier[j].coeficient);
    }
    fclose(fichier);
    printf("Modifications enregistrées.\n\n");


    // Affichage des matières modifiées
    if (nb_modifiees > 0) 
    {
        printf("+--------------------------------------------+\n");
        printf("|             Matières modifiées             |\n");
        printf("+--------------------------------------------+\n");
        printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
        printf("+-------------+-----------------+------------+\n");
        for (int j = 0; j < nb_modifiees; j++) 
        {
            printf("%-12d | %-15s | %-11hd\n", modifiees[j].reference, modifiees[j].libelle, modifiees[j].coeficient);
            printf("+-------------+-----------------+------------+\n");
        }
    }
    wait_for_enter();
}