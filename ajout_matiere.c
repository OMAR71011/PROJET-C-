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
        
    char continuer[8] = "oui";
    int is_oui = 1;
    matiere matiere_ajoutee;

    // Générer automatiquement la prochaine référence
    int derniere_ref = 0;
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
    // Parcourir le fichier pour trouver la dernière référence
    while (fgets(ligne, sizeof(ligne), fichier)) {
        int ref_tmp;
        if (sscanf(ligne, "%d", &ref_tmp) == 1) {
            if (ref_tmp > derniere_ref) {
                derniere_ref = ref_tmp;
            }
        }
    }

    int compteur = 0;
    // printf("Vous pouvez ajouter autant de matières que vous le souhaitez.\n");
    do {
        matiere_ajoutee.reference = derniere_ref + compteur + 1;
        printf("Saisie de la matière numéro %d (référence attribuée : %d)\n", compteur + 1, matiere_ajoutee.reference);
        // Libellé : lettres, chiffres, tiret 6 (-) et tiret 8 (_), rien d'autre
        char libelle_tmp[15];
        int libelle_valide = 0;
        setlocale(LC_ALL, "");
        do {
            printf("Veuillez saisir le libellé de la matière (lettres, chiffres, accents, - et _ uniquement) : ");
            scanf("%14s", libelle_tmp);
            libelle_valide = 1;
            wchar_t wlibelle[15];
            mbstowcs(wlibelle, libelle_tmp, 15);
            for (int c = 0; wlibelle[c] != L'\0'; c++) {
                wchar_t ch = wlibelle[c];
                if (iswalpha(ch) || iswdigit(ch) || ch == L'-' || ch == L'_')
                    continue;
                libelle_valide = 0;
                printf("Caractère non autorisé dans le libellé : %lc\n", ch);
                break;
            }
        } while (!libelle_valide);
        strcpy(matiere_ajoutee.libelle, libelle_tmp);


        // Coefficient : entier court uniquement
        short coef;
        int coef_valide = 0;
        do {
            printf("Veuillez saisir le coefficient de la matière (entier uniquement) : ");
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
        matiere_ajoutee.coeficient = coef;

        // Vérifier si le fichier est vide pour écrire l'en-tête
        fseek(fichier, 0, SEEK_END);
        if (ftell(fichier) == 0) {
            fprintf(fichier, "reference,libelle,coefficient\n");
        }
        fprintf(fichier, "%d,%s,%hd\n", matiere_ajoutee.reference, matiere_ajoutee.libelle, matiere_ajoutee.coeficient);
        
        printf("La matière a été bien ajoutée.\n");
        printf("+--------------------------------------------+\n");
        printf("|Voici la matière que vous venez d'ajouter   |\n");
        printf("+-------------+----------------+-------------+\n");
        printf("|%-12s | %-15s | %-11s|\n", "reference", "libelle", "coefficient");
        printf("+-------------+-----------------+------------+\n");
        // Gestion du débordement pour l'affichage
        char libelle_aff[16];
        strncpy(libelle_aff, matiere_ajoutee.libelle, 15);
        libelle_aff[15] = '\0';
        if (strlen(libelle_aff) > 15) {
            libelle_aff[14] = '.';
            libelle_aff[15] = '\0';
        }
        printf("|%-12d | %-15.15s | %-11hd|\n", matiere_ajoutee.reference, libelle_aff, matiere_ajoutee.coeficient);
        printf("+-------------+-----------------+------------+\n");

        compteur++;
        printf("Voulez-vous ajouter une autre matière ? (oui/non) : ");
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

    fclose(fichier);

    wait_for_enter();
}