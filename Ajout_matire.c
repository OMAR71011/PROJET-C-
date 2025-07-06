
#include "matiere.h"

void Ajout_matiere()
{
    FILE *fichier = fopen("matiere.csv", "a+"); // Ouvre en mode ajout pour ne pas écraser le fichier
    if (fichier == NULL) 
    {
        perror("Erreur d'ouverture du fichier");
        return;
    }
    printf("||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||--------------AJOUT DES MATIRES--------------||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n"
           "||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||\n\n\n");
        
    int taille;
    printf("Combien de matières voulez-vous ajouter : ");
    if (scanf("%d", &taille) != 1) {
        printf("Saisie invalide\n");
        fclose(fichier);
        return;
    }

    matiere matieres[taille];

    // Charger les références existantes
    int refs_existantes[1000];
    int nb_existantes = 0;
    rewind(fichier);
    char ligne[100];
    // Sauter l'en-tête si présent
    if (fgets(ligne, sizeof(ligne), fichier)) {
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
        int ref_tmp;
        if (sscanf(ligne, "%d", &ref_tmp) == 1) {
            refs_existantes[nb_existantes++] = ref_tmp;
        }
    }

    for (int i = 0; i < taille; i++)
    {
        int ref;
        int unique = 0;
        do {
            unique = 1;
            printf("Saisie de la matière numéro %d \n", i + 1);
            printf("Veuillez saisir la référence de la matière (entier uniquement) : ");
            char buffer[32];
            int valid = 0;
            while (!valid) {
                if (scanf("%31s", buffer) != 1) {
                    printf("Erreur de saisie. Veuillez entrer un entier : ");
                    while (getchar() != '\n');
                    continue;
                }
                char *endptr;
                ref = (int)strtol(buffer, &endptr, 10);
                if (*endptr != '\0') {
                    printf("Entrée invalide. Veuillez entrer un entier sans espace ni lettre : ");
                } else {
                    valid = 1;
                }
            }
            // Vérifier unicité
            for (int j = 0; j < nb_existantes; j++) {
                if (ref == refs_existantes[j]) {
                    printf("Cette référence existe déjà. Veuillez en saisir une autre.\n");
                    unique = 0;
                    valid = 0;
                    break;
                }
            }
            // Vérifier aussi dans les nouvelles saisies
            for (int k = 0; k < i; k++) {
                if (ref == matieres[k].reference) {
                    printf("Vous avez déjà saisi cette référence pour une nouvelle matière.\n");
                    unique = 0;
                    valid = 0;
                    break;
                }
            }
        } while (!unique);
        matieres[i].reference = ref;
        // Libellé : lettres, chiffres, tiret 6 (-) et tiret 8 (_), rien d'autre
        char libelle_tmp[15];
        int libelle_valide = 0;
        do {
            printf("Veuillez saisir le libellé de la matière (lettres, chiffres, - et _ uniquement) : ");
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
        strcpy(matieres[i].libelle, libelle_tmp);

        // Coefficient : entier court uniquement
        short coef;
        int coef_valide = 0;
        do {
            printf("Veuillez saisir le coéficient de la matière (entier court uniquement) : ");
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
        matieres[i].coeficient = coef;
    }

    // Vérifier si le fichier est vide pour écrire l'en-tête
    fseek(fichier, 0, SEEK_END);
    if (ftell(fichier) == 0) {
        fprintf(fichier, "reference,libelle,coefficient\n");
    }
    for (int i = 0; i < taille; i++)
    {
        fprintf(fichier, "%d,%s,%hd\n", matieres[i].reference, matieres[i].libelle, matieres[i].coeficient);
    }

    printf(taille == 1 ? "La matière a été bien ajoutée.\n" : "Les matières ont été bien ajoutées.\n");

    // Afficher les matières ajoutées
    printf("+--------------------------------------------+\n");
    printf("|Voici les matières que vous venez d'ajouter |\n");
    printf("+-------------+----------------+-------------+\n");
    printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
    printf("+-------------+-----------------+-----------+\n");
    for (int i = 0; i < taille; i++) {
        printf("|%-12d | %-15s | %-11hd|\n", matieres[i].reference, matieres[i].libelle, matieres[i].coeficient);
        printf("+-------------+-----------------+------------+\n");

    }

    fclose(fichier);

    wait_for_enter();
}