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
    char continuer[8] = "oui";
    int is_oui = 1;
    matiere modifier[1000];
    int nb_modifiees = 0;
    matiere modifiees[1000];
    int i = 0;
    char ligne[100];
    // Lecture du fichier initiale
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

    do {
        char libelle_recherche[15];
        printf("Veuillez saisir le libellé de la matière à modifier : ");
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
        for (int idx = 0; idx < i; idx++) {
            int match = 1;
            for (int c = 0; libelle_recherche[c] != '\0' || modifier[idx].libelle[c] != '\0'; c++) {
                char a = libelle_recherche[c];
                char b = modifier[idx].libelle[c];
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
                strncpy(libelle_aff, modifier[idx].libelle, 15);
                libelle_aff[15] = '\0';
                if (strlen(libelle_aff) > 15) {
                    libelle_aff[14] = '.';
                    libelle_aff[15] = '\0';
                }
                printf("|%-12d | %-15.15s | %-11hd|\n", modifier[idx].reference, libelle_aff, modifier[idx].coeficient);
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
                printf("Veuillez saisir la référence de la matière à modifier (entier uniquement) :  ");
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

            int pos = -1;
            for (int idx = 0; idx < i; idx++) {
                if (modifier[idx].reference == ref) {
                    pos = idx;
                    break;
                }
            }
            if (pos != -1) 
            {
                printf("Saisie des nouvelles informations pour la matière dont la référence est %d :\n", modifier[pos].reference);
                // Libellé : lettres, chiffres, tiret 6 (-) et tiret 8 (_), rien d'autre
                char libelle_tmp[15];
                int libelle_valide2 = 0;
                do {
                    printf("Nouveau libellé (lettres, chiffres, accents, - et _ uniquement) : ");
                    scanf("%14s", libelle_tmp);
                    libelle_valide2 = 1;
                    wchar_t wlibelle2[15];
                    mbstowcs(wlibelle2, libelle_tmp, 15);
                    for (int c = 0; wlibelle2[c] != L'\0'; c++) {
                        wchar_t ch = wlibelle2[c];
                        if (iswalpha(ch) || iswdigit(ch) || ch == L'-' || ch == L'_')
                            continue;
                        libelle_valide2 = 0;
                        printf("Caractère non autorisé dans le libellé : %lc\n", ch);
                        break;
                    }
                } while (!libelle_valide2);
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

                // Afficher les nouvelles données de la matière modifiée
                printf("\nNouvelle fiche de la matière modifiée :\n");
                printf("+-------------+----------------+-------------+\n");
                printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
                printf("+-------------+-----------------+------------+\n");
                char libelle_aff2[16];
                strncpy(libelle_aff2, modifier[pos].libelle, 15);
                libelle_aff2[15] = '\0';
                if (strlen(libelle_aff2) > 15) {
                    libelle_aff2[14] = '.';
                    libelle_aff2[15] = '\0';
                }
                printf("|%-12d | %-15.15s | %-11hd|\n", modifier[pos].reference, libelle_aff2, modifier[pos].coeficient);
                printf("+-------------+-----------------+------------+\n");
            } else 
            {
                printf("Référence %d non trouvée\n", ref);
            }
        }

        printf("Voulez-vous modifier une autre matière ? (oui/non) : ");
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
            char libelle_aff3[16];
            strncpy(libelle_aff3, modifiees[j].libelle, 15);
            libelle_aff3[15] = '\0';
            if (strlen(libelle_aff3) > 15) {
                libelle_aff3[14] = '.';
                libelle_aff3[15] = '\0';
            }
            printf("|%-12d | %-15.15s | %-11hd|\n", modifiees[j].reference, libelle_aff3, modifiees[j].coeficient);
            printf("+-------------+-----------------+------------+\n");
        }
    }
    wait_for_enter();
}