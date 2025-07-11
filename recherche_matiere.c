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
    char continuer[8] = "oui";
    int is_oui = 1;
    do {
        char libelle_recherche[15];
        printf("Veuillez saisir le libellé de la matière à rechercher : ");
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

        // Relecture du fichier à chaque recherche
        FILE *fichier2 = fopen("matiere.csv", "r");
        if (fichier2 == NULL) {
            perror("Erreur d'ouverture du fichier");
            return;
        }
        matiere chercher[1000];
        int i = 0;
        char ligne[100];
        while (fgets(ligne, sizeof(ligne), fichier2) && i < 1000) {
            if (sscanf(ligne, "%d,%14[^,],%hd", &chercher[i].reference, chercher[i].libelle, &chercher[i].coeficient) == 3) {
                i++;
            }
        }
        fclose(fichier2);

        // Affichage des matières trouvées avec ce libellé (insensible à la casse)
        int nb_trouvees = 0;
        printf("\nMatières trouvées avec le libellé '%s' :\n", libelle_recherche);
        printf("+-------------+----------------+-------------+\n");
        printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
        printf("+-------------+-----------------+------------+\n");
        for (int j = 0; j < i; j++) {
            int match = 1;
            for (int c = 0; libelle_recherche[c] != '\0' || chercher[j].libelle[c] != '\0'; c++) {
                char a = libelle_recherche[c];
                char b = chercher[j].libelle[c];
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
                strncpy(libelle_aff, chercher[j].libelle, 15);
                libelle_aff[15] = '\0';
                if (strlen(libelle_aff) > 15) {
                    libelle_aff[14] = '.';
                    libelle_aff[15] = '\0';
                }
                printf("|%-12d | %-15.15s | %-11hd|\n", chercher[j].reference, libelle_aff, chercher[j].coeficient);
                printf("+-------------+-----------------+------------+\n");
                nb_trouvees++;
            }
        }
        if (nb_trouvees == 0) 
        {   
            printf("Aucune matière trouvée avec ce libellé.\n");
        }
        printf("Voulez-vous rechercher une autre matière ? (oui/non) : ");
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
