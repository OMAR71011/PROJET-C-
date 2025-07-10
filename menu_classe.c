#include "menu_classe.h"
#include "affichage.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

int nom_valide(const char *nom);

static int charger_classes(Classe classes[]) {
    FILE *f = fopen(FICHIER_CLASSES, "r");
    if(!f) return 0;

    char ligne[100];
    int count = 0;
    fgets(ligne, sizeof(ligne), f);

    while(fgets(ligne, sizeof(ligne), f) && count < MAX_CLASSES) {
        sscanf(ligne, "%d,%[^,],%c",
               &classes[count].code,
               classes[count].nom,
               &classes[count].niveau);
        count++;
    }
    fclose(f);
    return count;
}

static void sauvegarder_classes(Classe classes[], int count) {
    FILE *f = fopen(FICHIER_CLASSES, "w");
    if(!f) {
        printf("Erreur de fichier!\n");
        return;
    }

    fprintf(f, "code,nom,niveau\n");
    for(int i = 0; i < count; i++) {
        fprintf(f, "%d,%s,%c\n",
                classes[i].code,
                classes[i].nom,
                classes[i].niveau);
    }
    fclose(f);
}

void ajouter_classe() {
    char reponse;
    do {
        Classe c;
        Classe classes[MAX_CLASSES];
        int count = charger_classes(classes);

        printf("\n--- NOUVELLE CLASSE ---\n");
        c.code = (count == 0) ? 1 : classes[count-1].code + 1;

        do {
            printf("Nom de la classe : ");
            fgets(c.nom, MAX_NAME, stdin);
            c.nom[strcspn(c.nom, "\n")] = '\0';
            if (!nom_valide(c.nom)) {
                printf("Nom invalide ! Veuillez entrer uniquement des lettres, chiffres et espaces, et au moins une lettre.\n");
            }
        } while (!nom_valide(c.nom));

        do {
            printf("Niveau (L/M) : ");
            c.niveau = toupper(getchar());
            vider_buffer();
            if (c.niveau != 'L' && c.niveau != 'M') {
                printf("Niveau invalide ! Entrez 'L' ou 'M'.\n");
            }
        } while (c.niveau != 'L' && c.niveau != 'M');

        classes[count] = c;
        sauvegarder_classes(classes, count + 1);
        
        printf("\nClasse ajoutée avec succès !\n");
        printf("+------+----------------------+--------+\n");
        printf("| %-4s | %-20s | %-6s |\n", "Code", "Nom de la classe", "Niveau");
        printf("+------+----------------------+--------+\n");
        printf("| %-4d | %-20s | %-6c |\n", c.code, c.nom, c.niveau);
        printf("+------+----------------------+--------+\n");

        printf("\nVoulez-vous ajouter une autre classe ? (o/n) : ");
        reponse = getchar();
        vider_buffer();
    } while (tolower(reponse) == 'o');
}

void afficher_classes() {
    Classe classes[MAX_CLASSES];
    int count = charger_classes(classes);

    printf("\n--- LISTE DES CLASSES ---\n");
    if (count == 0) {
        printf("Aucune classe enregistrée.\n");
        return;
    }

    printf("+------+----------------------+--------+\n");
    printf("| %-4s | %-20s | %-6s |\n", "Code", "Nom de la classe", "Niveau");
    printf("+------+----------------------+--------+\n");

    for (int i = 0; i < count; i++) {
        printf("| %-4d | %-20s | %-6c |\n",
               classes[i].code,
               classes[i].nom,
               classes[i].niveau);
    }

    printf("+------+----------------------+--------+\n");
}

void modifier_classe() {
    char reponse;
    do {
        Classe classes[MAX_CLASSES];
        int count = charger_classes(classes);
        int code, trouve = 0;

        if (count == 0) {
            printf("Aucune classe à modifier.\n");
            return;
        }

        afficher_classes();

        printf("\nEntrez le code de la classe à modifier : ");
        if (scanf("%d", &code) != 1) {
            printf("Code invalide !\n");
            vider_buffer();
            return;
        }
        vider_buffer();

        trouve = 0;
        for(int i = 0; i < count; i++) {
            if(classes[i].code == code) {
                trouve = 1;
                printf("Nouveau nom : ");
                fgets(classes[i].nom, MAX_NAME, stdin);
                classes[i].nom[strcspn(classes[i].nom, "\n")] = '\0';

                while (!nom_valide(classes[i].nom) || strlen(classes[i].nom) == 0) {
                    printf("Nom invalide ! Veuillez entrer uniquement des lettres, chiffres et espaces.\n");
                    printf("Nouveau nom : ");
                    fgets(classes[i].nom, MAX_NAME, stdin);
                    classes[i].nom[strcspn(classes[i].nom, "\n")] = '\0';
                }

                do {
                    printf("Nouveau niveau (L/M) : ");
                    classes[i].niveau = toupper(getchar());
                    vider_buffer();
                    if (classes[i].niveau != 'L' && classes[i].niveau != 'M') {
                        printf("Niveau invalide ! Entrez 'L' ou 'M'.\n");
                    }
                } while(classes[i].niveau != 'L' && classes[i].niveau != 'M');

                sauvegarder_classes(classes, count);
                printf("\nClasse modifiée avec succès !\n");
                printf("+------+----------------------+--------+\n");
                printf("| %-4s | %-20s | %-6s |\n", "Code", "Nom de la classe", "Niveau");
                printf("+------+----------------------+--------+\n");
                printf("| %-4d | %-20s | %-6c |\n", 
                       classes[i].code, 
                       classes[i].nom, 
                       classes[i].niveau);
                printf("+------+----------------------+--------+\n");
                break;
            }
        }
        if(!trouve) {
            printf("Classe non trouvée.\n");
        }

        printf("\nVoulez-vous modifier une autre classe ? (o/n) : ");
        reponse = getchar();
        vider_buffer();
    } while (tolower(reponse) == 'o');
}

void rechercher_classe() {
    char reponse;
    do {
        Classe classes[MAX_CLASSES];
        int count = charger_classes(classes);
        char nom[MAX_NAME];
        int trouve = 0;

        printf("Entrez le nom de la classe à rechercher : ");
        fgets(nom, MAX_NAME, stdin);
        nom[strcspn(nom, "\n")] = '\0';

        trouve = 0;
        printf("\nRésultats de la recherche :\n");
        printf("+------+----------------------+--------+\n");
        printf("| %-4s | %-20s | %-6s |\n", "Code", "Nom de la classe", "Niveau");
        printf("+------+----------------------+--------+\n");

        for(int i = 0; i < count; i++) {
            if(strcasecmp(classes[i].nom, nom) == 0) {
                printf("| %-4d | %-20s | %-6c |\n",
                       classes[i].code,
                       classes[i].nom,
                       classes[i].niveau);
                trouve = 1;
            }
        }

        printf("+------+----------------------+--------+\n");
        if(!trouve) {
            printf("Aucune classe trouvée avec ce nom.\n");
        }

        printf("\nVoulez-vous faire une autre recherche ? (o/n) : ");
        reponse = getchar();
        vider_buffer();
    } while (tolower(reponse) == 'o');
}

void supprimer_classe() {
    char reponse;
    do {
        Classe classes[MAX_CLASSES];
        int count = charger_classes(classes);
        int code, trouve = 0;

        if (count == 0) {
            printf("Aucune classe à supprimer.\n");
            return;
        }

        printf("\n--- LISTE DES CLASSES ---\n");
        printf("+------+----------------------+--------+\n");
        printf("| %-4s | %-20s | %-6s |\n", "Code", "Nom de la classe", "Niveau");
        printf("+------+----------------------+--------+\n");

        for (int i = 0; i < count; i++) {
            printf("| %-4d | %-20s | %-6c |\n",
                   classes[i].code,
                   classes[i].nom,
                   classes[i].niveau);
        }

        printf("+------+----------------------+--------+\n");

        printf("\nEntrez le code de la classe à supprimer : ");
        if (scanf("%d", &code) != 1) {
            printf("Code invalide !\n");
            vider_buffer();
            return;
        }
        vider_buffer();

        for(int i = 0; i < count; i++) {
            if(classes[i].code == code) {
                printf("\nClasse à supprimer :\n");
                printf("+------+----------------------+--------+\n");
                printf("| %-4s | %-20s | %-6s |\n", "Code", "Nom de la classe", "Niveau");
                printf("+------+----------------------+--------+\n");
                printf("| %-4d | %-20s | %-6c |\n", 
                       classes[i].code, 
                       classes[i].nom, 
                       classes[i].niveau);
                printf("+------+----------------------+--------+\n");

                printf("Confirmez la suppression (o/n) ? ");
                char confirmation = getchar();
                vider_buffer();

                if (tolower(confirmation) == 'o') {
                    for(int j = i; j < count - 1; j++) {
                        classes[j] = classes[j+1];
                    }
                    sauvegarder_classes(classes, count - 1);
                    printf("\nClasse supprimée avec succès !\n");
                    
                    printf("\n--- NOUVELLE LISTE DES CLASSES ---\n");
                    printf("+------+----------------------+--------+\n");
                    printf("| %-4s | %-20s | %-6s |\n", "Code", "Nom de la classe", "Niveau");
                    printf("+------+----------------------+--------+\n");

                    for (int k = 0; k < count - 1; k++) {
                        printf("| %-4d | %-20s | %-6c |\n",
                               classes[k].code,
                               classes[k].nom,
                               classes[k].niveau);
                    }

                    printf("+------+----------------------+--------+\n");
                } else {
                    printf("Suppression annulée.\n");
                }
                trouve = 1;
                break;
            }
        }
        if(!trouve) {
            printf("Classe non trouvée.\n");
        }

        printf("\nVoulez-vous supprimer une autre classe ? (o/n) : ");
        reponse = getchar();
        vider_buffer();
    } while (tolower(reponse) == 'o');
}

void menu_classe() {
    int choix;
    do {
        efface_ecran();
        printf("\n=== GESTION DES CLASSES ===");
        printf("\n1. Ajouter une classe");
        printf("\n2. Lister les classes");
        printf("\n3. Modifier une classe");
        printf("\n4. Rechercher une classe");
        printf("\n5. Supprimer une classe");
        printf("\n6. Retour");
        printf("\n===========================");
        printf("\n Renseigner votre Choix : ");

        if(scanf("%d", &choix) != 1) {
            printf("Saisie invalide!\n");
            vider_buffer();
            continue;
        }
        vider_buffer();

        switch (choix) {
            case 1:
                ajouter_classe();
                break;
            case 2:
                afficher_classes();
                break;
            case 3:
                modifier_classe();
                break;
            case 4:
                rechercher_classe();
                break;
            case 5:
                supprimer_classe();
                break;
        }
        if (choix != 6) {
            printf("Appuyez sur Entrée pour continuer...");
            getchar();
        }
    } while (choix != 6);
}

void initialiser_fichiers(void) {
    FILE *f = fopen(FICHIER_CLASSES, "r");
    if (!f) {
        f = fopen(FICHIER_CLASSES, "w");
        if (f) {
            fprintf(f, "code,nom,niveau\n");
            fclose(f);
        }
    } else {
        fclose(f);
    }
}

int nom_valide(const char *nom) {
    int lettre_trouvee = 0;
    for (int i = 0; nom[i] != '\0'; i++) {
        if (!isalnum(nom[i]) && nom[i] != ' ') {
            return 0;
        }
        if (isalpha(nom[i])) {
            lettre_trouvee = 1;
        }
    }
    return lettre_trouvee;
}