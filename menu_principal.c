// donne/menu_principal.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "menu_classe.h"
#include "affichage.h"

int confirmer_quitter() {
    char reponse[10];
    printf("\nVoulez-vous vraiment quitter ? (oui/non) \n ");
    scanf("%3s", reponse);
    vider_buffer();

    return (strcmp(reponse, "oui") == 0 || strcmp(reponse, "OUI") == 0 || strcmp(reponse, "o") == 0);
}

int lire_choix_menu(int *choix) {
    char buffer[10];
    printf("Votre choix : ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return 0;

    buffer[strcspn(buffer, "\n")] = '\0';

    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buffer[i])) {
            return 0;
        }
    }

    int valeur = atoi(buffer);
    if (valeur < 1 || valeur > 5)
        return 0;

    *choix = valeur;
    return 1;
}

void menu_principal() {
    int choix;
    int quitter = 0;

    while (!quitter) {
        efface_ecran();

        printf("========== MENU PRINCIPAL ==========\n");
        printf("1. Gestion des étudiants\n");
        printf("2. Gestion des matières\n");
        printf("3. Gestion des classes\n");
        printf("4. Gestion des notes\n");
        printf("5. Quitter\n");
        printf("====================================\n");

        if (!lire_choix_menu(&choix)) {
            printf("\nEntrée invalide. Veuillez entrer un choix entre 1 et 5.\n");
            printf("Appuyez sur Entrée pour recommencer...");
            vider_buffer();
            getchar();
            continue;
        }

        printf("\n");

        switch (choix) {
            case 1:
                printf(">>> Vous avez choisi Gestion des étudiants.\n");
                printf("Appuyez sur Entrée pour y acceder...");
                getchar();
                menu_etudiant();
                break;
            case 2:
                printf(">>> Vous avez choisi Gestion des matières.\n");
                printf("Appuyez sur Entrée pour y acceder...");
                getchar();
                menu_matiere();
                break;
            case 3:
                printf(">>> Vous avez choisi Gestion des classes.\n");
                printf("Appuyez sur Entrée pour y acceder...");
                getchar();
                menu_classe();
                break;
            case 4:
                printf(">>> Vous avez choisi Gestion des notes.\n");
                printf("Appuyez sur Entrée pour y acceder...");
                getchar();
                menu_notes();
                break;
            case 5:
                if (confirmer_quitter()) {
                    printf("Merci d'avoir utilisé le programme.\n");
                    quitter = 1;
                }
                break;
        }
    }
}
