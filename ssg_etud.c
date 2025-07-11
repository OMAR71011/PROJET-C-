#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "ssg_header.h"

void clear_screen() {
    system("cls || clear");
}

void centrer_texte(const char *texte, int largeur) {
    int longueur = strlen(texte);
    int espaces = (largeur - longueur) / 2;
    for (int i = 0; i < espaces; i++) printf(" ");
    printf("%s\n", texte);
}

void afficher_ligne(int longueur, char caractere) {
    for (int i = 0; i < longueur; i++) printf("%c", caractere);
    printf("\n");
}

void afficher_titre(const char *titre) {
    printf("\n");
    afficher_ligne(50, '=');
    centrer_texte(titre, 50);
    afficher_ligne(50, '=');
    printf("\n");
}

void pause() {
    printf("\nAppuyez sur Entree pour continuer...");
    while (getchar() != '\n');
    getchar();
}

int est_bissextile(int annee) {
    return (annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0);
}

int est_date_valide(const char *date) {
    if (strlen(date) != 10) return 0;
    if (date[2] != '/' || date[5] != '/') return 0;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(date[i])) return 0;
    }
    int jour = atoi(date);
    int mois = atoi(date + 3);
    int annee = atoi(date + 6);
    if (annee < 1900 || annee > 2100) return 0;
    if (mois < 1 || mois > 12) return 0;
    int jours_par_mois[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (est_bissextile(annee)) jours_par_mois[1] = 29;
    if (jour < 1 || jour > jours_par_mois[mois-1]) return 0;
    return 1;
}

int est_email_valide(const char *email) {
    int longueur = strlen(email);
    if (longueur < 3 || longueur >= MAX_EMAIL) return 0;
    
    int arobase_present = 0;
    int point_present = 0;
    
    for (int i = 0; i < longueur; i++) {
        if (email[i] == '@') {
            if (arobase_present) return 0;
            arobase_present = 1;
        }
        if (email[i] == '.' && i > 0 && i < longueur - 1) {
            point_present = 1;
        }
    }
    
    return arobase_present && point_present;
}



// Génère un code étudiant aléatoire à 4 chiffres
int generer_code_etudiant() {
    return 1000 + rand() % 9000;
}

int generer_numero_etudiant() {
    return rand() % 10000;
}

void saisir_etudiant(Etudiant *e) {
    printf("\n");
    e->numero = generer_numero_etudiant();
    e->code_classe = generer_code_etudiant();
    printf("Numéro étudiant : %04d\n", e->numero);
    printf("Code étudiant : %04d\n", e->code_classe);

    printf("Veuillez entrer le nom de l'étudiant : ");
    fgets(e->nom, MAX_NOM, stdin);
    e->nom[strcspn(e->nom, "\n")] = '\0';

    printf("Veuillez entrer le prénom de l'étudiant : ");
    fgets(e->prenom, MAX_PRENOM, stdin);
    e->prenom[strcspn(e->prenom, "\n")] = '\0';
    
    do {
        printf("Veulliez entrer l'émail de l'étudiant: ", MAX_EMAIL-1);
        fgets(e->email, MAX_EMAIL, stdin);
        e->email[strcspn(e->email, "\n")] = '\0';
        if (!est_email_valide(e->email)) {
            printf("Email invalide. Format attendu : exemple@domaine.com\n");
        }
    } while (!est_email_valide(e->email));
    
    do {
        printf("Date de naissance (JJ/MM/AAAA) : ");
        fgets(e->date_naissance, 11, stdin);
        e->date_naissance[strcspn(e->date_naissance, "\n")] = '\0';
        if (!est_date_valide(e->date_naissance)) {
            printf("Date invalide. Format attendu : JJ/MM/AAAA\n");
        }
    } while (!est_date_valide(e->date_naissance));
}

void afficher_etudiant(const Etudiant *e) {
    printf(" %04d   %04d   %-30s   %-20s   %-20s %12s  |\n", 
           e->numero, e->code_classe, e->nom, e->prenom, e->email, e->date_naissance);
}

void afficher_liste_etudiants(Etudiant etudiants[], int nb_etudiants) {
    if (nb_etudiants == 0) {
        printf("\nAucun etudiant enregistre.\n");
        return;
    }
    afficher_titre("LISTE DES ETUDIANTS");
    printf("+------+------+--------------------------------+----------------------+---------------------+-------------+\n");
    printf("| Num  | Code | Nom                            | Prenom               | Email               | Naissance   |\n");
    printf("+------+------+--------------------------------+----------------------+---------------------+-------------+\n");
    for (int i = 0; i < nb_etudiants; i++) {
        afficher_etudiant(&etudiants[i]);
    }
    printf("+------+------+--------------------------------+----------------------+---------------------+-------------+\n");
    printf("\nTotal etudiants : %d\n", nb_etudiants);
}

void ajouter_etudiant(Etudiant etudiants[], int *nb_etudiants) {
    int continuer = 1;
    while (continuer) {
        clear_screen();
        afficher_titre("AJOUTER UN ETUDIANT");
        if (*nb_etudiants >= MAX_ETUDIANTS) {
            printf("La capacite maximale d'etudiants est atteinte (%d).\n", MAX_ETUDIANTS);
            return;
        }
        saisir_etudiant(&etudiants[*nb_etudiants]);
        (*nb_etudiants)++;
        sauvegarder_etudiants(etudiants, *nb_etudiants);
        printf("\nEtudiant ajoute!\n");
        
        // Vider le buffer d'entrée avant de demander
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        continuer = demander_continuer("Voulez-vous ajouter un autre étudiant ?");
    }
}

void modifier_etudiant(Etudiant etudiants[], int nb_etudiants) {
    int continuer = 1;
    while (continuer) {
        clear_screen();
        afficher_titre("MODIFIER UN ETUDIANT");
        if (nb_etudiants == 0) {
            printf("Aucun etudiant a modifier.\n");
            return;
        }
        afficher_liste_etudiants(etudiants, nb_etudiants);
        int numero;
        printf("Entrez le numero de l'etudiant à modifier : ");
        scanf("%d", &numero);
        while (getchar() != '\n');
        int index = -1;
        for (int i = 0; i < nb_etudiants; i++) {
            if (etudiants[i].numero == numero) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            printf("Aucun etudiant trouve avec le numero %04d.\n", numero);
        } else {
            printf("\nEtudiant trouve :\n");
            afficher_etudiant(&etudiants[index]);
            printf("\nEntrez les nouvelles informations :\n");
            saisir_etudiant(&etudiants[index]);
            sauvegarder_etudiants(etudiants, nb_etudiants);
            printf("\nEtudiant modifie et sauvegarde dans le fichier CSV !\n");
        }
        continuer = demander_continuer("Voulez-vous modifier un autre étudiant ?");
    }
}

void supprimer_etudiant(Etudiant etudiants[], int *nb_etudiants) {
    int continuer = 1;
    while (continuer) {
        clear_screen();
        afficher_titre("SUPPRIMER UN ETUDIANT");
        if (*nb_etudiants == 0) {
            printf("Aucun etudiant a supprimer.\n");
            return;
        }
        char nom[MAX_NOM], prenom[MAX_PRENOM];
        printf("Entrez le nom de l'etudiant a supprimer : ");
        fgets(nom, MAX_NOM, stdin);
        nom[strcspn(nom, "\n")] = '\0';
        printf("Entrez le prenom de l'etudiant a supprimer : ");
        fgets(prenom, MAX_PRENOM, stdin);
        prenom[strcspn(prenom, "\n")] = '\0';
        // Recherche insensible à la casse
        int indices[MAX_ETUDIANTS];
        int nb_trouves = 0;
        for (int i = 0; i < *nb_etudiants; i++) {
            if (strcasecmp(etudiants[i].nom, nom) == 0 && strcasecmp(etudiants[i].prenom, prenom) == 0) {
                indices[nb_trouves++] = i;
            }
        }
        if (nb_trouves == 0) {
            printf("Aucun etudiant trouve avec ce nom et prenom.\n");
        } else {
            printf("\nEtudiants trouves :\n");
            for (int i = 0; i < nb_trouves; i++) {
                printf("[%d] ", i+1);
                afficher_etudiant(&etudiants[indices[i]]);
            }
            int choix = 1;
            if (nb_trouves > 1) {
                printf("Quel etudiant voulez-vous supprimer ? (1-%d) : ", nb_trouves);
                scanf("%d", &choix);
                while (getchar() != '\n');
                if (choix < 1 || choix > nb_trouves) {
                    printf("Choix invalide. Suppression annulee.\n");
                    continuer = demander_continuer("Voulez-vous supprimer un autre étudiant ?");
                    continue;
                }
            }
            int index = indices[choix-1];
            printf("\nEtudiant a supprimer :\n");
            afficher_etudiant(&etudiants[index]);
            char confirmation;
            printf("\nVoulez-vous vraiment supprimer cet etudiant ? (o/n) : ");
            scanf("%c", &confirmation);
            while (getchar() != '\n');
            if (confirmation == 'o' || confirmation == 'O') {
                for (int i = index; i < *nb_etudiants - 1; i++) {
                    etudiants[i] = etudiants[i + 1];
                }
                (*nb_etudiants)--;
                sauvegarder_etudiants(etudiants, *nb_etudiants);
                printf("Etudiant supprime et sauvegarde dans le fichier CSV.\n");
            } else {
                printf("Suppression annulee.\n");
            }
        }
        continuer = demander_continuer("Voulez-vous supprimer un autre étudiant ?");
    }
}

void rechercher_etudiant(Etudiant etudiants[], int nb_etudiants) {
    int continuer = 1;
    while (continuer) {
        clear_screen();
        afficher_titre("RECHERCHER UN ETUDIANT");
        if (nb_etudiants == 0) {
            printf("Aucun etudiant enregistre.\n");
            return;
        }
        printf("[1]. Rechercher par numero\n");
        printf("[2]. Rechercher par nom\n");
        printf("[3]. Rechercher par prenom\n");
        printf("[0]. Retour\n");
        printf("Choix : ");
        int choix;
        scanf("%d", &choix);
        while (getchar() != '\n');
        if (choix == 0) return;
        int trouve = 0;
        switch (choix) {
            case 1: {
                int numero;
                printf("Entrez le numero de l'etudiant : ");
                scanf("%d", &numero);
                while (getchar() != '\n');
                for (int i = 0; i < nb_etudiants; i++) {
                    if (etudiants[i].numero == numero) {
                        afficher_etudiant(&etudiants[i]);
                        trouve++;
                    }
                }
                break;
            }
            case 2: {
                char nom[MAX_NOM];
                printf("Entrez le nom (ou une partie) : ");
                fgets(nom, MAX_NOM, stdin);
                nom[strcspn(nom, "\n")] = '\0';
                for (int i = 0; i < nb_etudiants; i++) {
                    if (strcasestr(etudiants[i].nom, nom) != NULL) {
                        afficher_etudiant(&etudiants[i]);
                        trouve++;
                    }
                }
                break;
            }
            case 3: {
                char prenom[MAX_PRENOM];
                printf("Entrez le prenom (ou une partie) : ");
                fgets(prenom, MAX_PRENOM, stdin);
                prenom[strcspn(prenom, "\n")] = '\0';
                for (int i = 0; i < nb_etudiants; i++) {
                    if (strcasestr(etudiants[i].prenom, prenom) != NULL) {
                        afficher_etudiant(&etudiants[i]);
                        trouve++;
                    }
                }
                break;
            }
            default:
                printf("Choix invalide.\n");
                break;
        }
        if (trouve == 0) {
            printf("\nAucun etudiant trouve.\n");
        }
        continuer = demander_continuer("Voulez-vous continuer à rechercher un étudiant ?");
    }
}

void initialiser_fichier_csv() {
    FILE *fichier = fopen(FICHIER_ETUDIANTS, "r");
    if (fichier == NULL) {
        fichier = fopen(FICHIER_ETUDIANTS, "w");
        if (fichier != NULL) {
            fprintf(fichier, "numero,nom,prenom,email,date_naissance,code_classe\n");
            fclose(fichier);
        }
    } else {
        fclose(fichier);
    }
}

void charger_etudiants(Etudiant etudiants[], int *nb_etudiants) {
    initialiser_fichier_csv();
    
    FILE *fichier = fopen(FICHIER_ETUDIANTS, "r");
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", FICHIER_ETUDIANTS);
        return;
    }

    char ligne[256];
    fgets(ligne, sizeof(ligne), fichier);

    *nb_etudiants = 0;
    while (*nb_etudiants < MAX_ETUDIANTS && 
           fscanf(fichier, "%d,%[^,],%[^,],%[^,],%[^,],%d\n",
                  &etudiants[*nb_etudiants].numero,
                  etudiants[*nb_etudiants].nom,
                  etudiants[*nb_etudiants].prenom,
                  etudiants[*nb_etudiants].email,
                  etudiants[*nb_etudiants].date_naissance,
                  &etudiants[*nb_etudiants].code_classe) == 6) {
        (*nb_etudiants)++;
    }

    fclose(fichier);
}

void sauvegarder_etudiants(Etudiant etudiants[], int nb_etudiants) {
    FILE *fichier = fopen(FICHIER_ETUDIANTS, "w");
    if (fichier == NULL) {
        printf("Erreur: Impossible d'ouvrir le fichier %s\n", FICHIER_ETUDIANTS);
        return;
    }

    fprintf(fichier, "numero,nom,prenom,email,date_naissance,code_classe\n");

    for (int i = 0; i < nb_etudiants; i++) {
        fprintf(fichier, "%d,%s,%s,%s,%s,%d\n",
                etudiants[i].numero,
                etudiants[i].nom,
                etudiants[i].prenom,
                etudiants[i].email,
                etudiants[i].date_naissance,
                etudiants[i].code_classe);
    }

    fclose(fichier);
    printf("Donnees sauvegardees avec succes dans %s\n", FICHIER_ETUDIANTS);
}

// Fonction utilitaire pour demander à l'utilisateur s'il veut continuer (oui/non, o/n, insensible à la casse)
int demander_continuer(const char *message) {
    char reponse[10];
    printf("%s (oui/non) : ", message);
    
    // Lire toute la ligne et ignorer les entrées vides
    while (1) {
        if (fgets(reponse, sizeof(reponse), stdin) == NULL) {
            clearerr(stdin);
            continue;
        }
        
        // Supprimer le saut de ligne
        reponse[strcspn(reponse, "\n")] = '\0';
        
        // Ignorer les lignes vides
        if (reponse[0] == '\0') {
            continue;
        }
        
        if (strcasecmp(reponse, "oui") == 0 || strcasecmp(reponse, "o") == 0) return 1;
        if (strcasecmp(reponse, "non") == 0 || strcasecmp(reponse, "n") == 0) return 0;
        
        printf("Réponse invalide. Veuillez répondre 'oui' ou 'non' : ");
    }
}

// Recherche insensible à la casse (compatible Windows)
char *strcasestr(const char *haystack, const char *needle) {
    if (!*needle) return (char *)haystack;
    size_t len_needle = strlen(needle);
    for (; *haystack; haystack++) {
        if (strncasecmp(haystack, needle, len_needle) == 0) {
            return (char *)haystack;
        }
    }
    return NULL;
}

void menu_etudiant() {
    srand(time(NULL));
    Etudiant etudiants[MAX_ETUDIANTS];
    int nb_etudiants = 0;
    charger_etudiants(etudiants, &nb_etudiants);
    int choix;
    do {
        clear_screen();
        afficher_titre("GESTION DES ETUDIANTS");
        printf("[1]. Ajouter un etudiant\n");
        printf("[2]. Modifier un etudiant\n");
        printf("[3]. Supprimer un etudiant\n");
        printf("[4]. Rechercher un etudiant\n");
        printf("[5]. Afficher la liste des etudiants\n");
        printf("[0]. Retour au menu principal\n");
        printf("\nChoix : ");
        scanf("%d", &choix);
        while (getchar() != '\n');
        switch (choix) {
            case 1: 
                ajouter_etudiant(etudiants, &nb_etudiants); 
                break;
            case 2: 
                modifier_etudiant(etudiants, nb_etudiants); 
                break;
            case 3: 
                supprimer_etudiant(etudiants, &nb_etudiants); 
                break;
            case 4: 
                rechercher_etudiant(etudiants, nb_etudiants); 
                break;
            case 5: 
                afficher_liste_etudiants(etudiants, nb_etudiants); 
                break;
            case 0: 
                break;
            default: 
                printf("Choix invalide.\n"); 
                break;
        }
        if (choix != 0) pause();
    } while (choix != 0);
}