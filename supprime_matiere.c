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
    char continuer[8] = "oui";
    int is_oui = 1;
    do {
        char libelle_recherche[15];
        printf("Veuillez saisir le libellé de la matière à supprimer : ");
        int libelle_valide = 0;
        setlocale(LC_ALL, "");
        do {
            scanf("%14s", libelle_recherche);
            libelle_valide = 1;
            wchar_t wlibelle[15];
            mbstowcs(wlibelle, libelle_recherche, 15);
            for (int c = 0; wlibelle[c] != L'\0'; c++) {
                wchar_t ch = wlibelle[c];
                if (iswalpha(ch) || iswdigit(ch) || ch == L'-' || ch == L'_')
                    continue;
                libelle_valide = 0;
                printf("Caractère non autorisé dans le libellé : %lc\n", ch);
                break;
            }
        } while (!libelle_valide);

        // Afficher toutes les matières correspondant à ce libellé (insensible à la casse)
        int trouve_libelle = 0;
        printf("\nMatières trouvées avec le libellé '%s' :\n", libelle_recherche);
        printf("+-------------+----------------+-------------+\n");
        printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
        printf("+-------------+-----------------+-----------+\n");
        for (int i = 0; i < n; i++) {
            int match = 1;
            for (int c = 0; libelle_recherche[c] != '\0' || matieres[i].libelle[c] != '\0'; c++) {
                char a = libelle_recherche[c];
                char b = matieres[i].libelle[c];
                if (a >= 'A' && a <= 'Z') a = a - 'A' + 'a';
                if (b >= 'A' && b <= 'Z') b = b - 'A' + 'a';
                if (a != b) {
                    match = 0;
                    break;
                }
                if (a == '\0' && b == '\0') break;
            }
            if (match) {
                char libelle_aff[16];
                strncpy(libelle_aff, matieres[i].libelle, 15);
                libelle_aff[15] = '\0';
                if (strlen(libelle_aff) > 15) {
                    libelle_aff[14] = '.';
                    libelle_aff[15] = '\0';
                }
                printf("|%-12d | %-15.15s | %-11hd|\n", matieres[i].reference, libelle_aff, matieres[i].coeficient);
                printf("+-------------+-----------------+------------+\n");
                trouve_libelle = 1;
            }
        }
        if (!trouve_libelle) {
            printf("Aucune matière trouvée avec ce libellé.\n");
        } else {
            int ref;
            int valid = 0;
            do {
                printf("Veuillez saisir la référence de la matière à supprimer (entier uniquement) : ");
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

            // Suppression
            int trouve = 0;
            // Réécriture du fichier sans la matière à supprimer
            FILE *fichier_w = fopen("matiere.csv", "w");
            if (fichier_w == NULL) {
                perror("Erreur d'écriture");
                return;
            }
            fprintf(fichier_w, "reference,libelle,coefficient\n");
            for (int i = 0; i < n; i++) {
                if (matieres[i].reference == ref) {
                    trouve = 1;
                    continue;
                }
                fprintf(fichier_w, "%d,%s,%hd\n", matieres[i].reference, matieres[i].libelle, matieres[i].coeficient);
            }
            fclose(fichier_w);

            if (!trouve) {
                printf("Aucune référence trouvée\n");
            } else {
                printf("Suppression effectuée\n");
                // Mettre à jour le tableau local pour les suppressions successives
                int new_n = 0;
                for (int i = 0; i < n; i++) {
                    if (matieres[i].reference != ref) {
                        matieres[new_n++] = matieres[i];
                    }
                }
                n = new_n;
            }
        }

        printf("Voulez-vous supprimer une autre matière ? (oui/non) : ");
        scanf("%7s", continuer);
        // Comparaison insensible à la casse uniquement pour 'oui'
        is_oui = 0;
        if (strlen(continuer) == 3) {
            char c0 = continuer[0], c1 = continuer[1], c2 = continuer[2];
            if ((c0 == 'o' || c0 == 'O') && (c1 == 'u' || c1 == 'U') && (c2 == 'i' || c2 == 'I')) {
                is_oui = 1;
            }
        }
    } while (is_oui);

    wait_for_enter();
}