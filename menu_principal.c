#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Pour la fonction isdigit()
#include "matiere.h"

// Fonction utilisée pour vider le buffer clavier en cas de mauvaise saisie
void vider_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fonction qui efface l’écran selon l’OS utilisé 
// Cela permet un affichage plus propre du menu à chaque itération
void efface_ecran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void nathanJam(void) {
    // Vide le buffer d'entrée
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // Attend la nouvelle entrée
    efface_ecran();
}

// Cette fonction nous permet de demander confirmation à l’utilisateur
// avant de quitter le programme
int confirmer_quitter() {
    char reponse[10];
    printf("\nVoulez-vous vraiment quitter ? (oui/non) : ");
    scanf("%3s", reponse); // On lit un mot de 3 caractères maximum
    vider_buffer(); // On vide le buffer pour éviter des sauts de ligne restants

   
    return (strcmp(reponse, "oui") == 0 || strcmp(reponse, "OUI") == 0 || strcmp(reponse, "o") == 0);
}


int lire_choix_menu(int *choix) {
    char buffer[100]; // On utilise un buffer pour lire la ligne entière avec fgets()

    printf("Votre choix : ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
        return 0; // Problème de lecture

    // On enlève le saut de ligne '\n' ajouté par fgets()
    buffer[strcspn(buffer, "\n")] = '\0';

    // On vérifie que chaque caractère est un chiffre
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (!isdigit((unsigned char)buffer[i])) {
            // Si on trouve une lettre, un symbole, ou espace, on considère l’entrée invalide
            return 0;
        }
    }

    // Si la chaîne ne contient que des chiffres, on la convertit en entier
    int valeur = atoi(buffer);

    // On vérifie que le choix est bien entre 1 et 5, comme dans notre menu
    if (valeur < 1 || valeur > 5) {
        return 0;
    }

    *choix = valeur; // Si tout est bon  on retourne la valeur via pointeur
    return 1; // Choix valide
}

// Fonction principale du menu
void menu_principal() {
    int choix;
    int quitter = 0;

    
    while (!quitter) {
        efface_ecran(); 
        // Affichage du menu principal
        printf("========== MENU PRINCIPAL ==========\n");
        printf("1. Gestion des étudiants\n");
        printf("2. Gestion des matières\n");
        printf("3. Gestion des classes\n");
        printf("4. Gestion des notes\n");
        printf("5. Quitter\n");
        printf("====================================\n");

        // Ici au lieu d’utiliser scanf (qui peut être dangereux), on passe par notre propre fonction
        // Elle vérifie que l’entrée est un entier et rien d’autre
        if (!lire_choix_menu(&choix)) {
            
            printf("\nEntrée invalide. Veuillez entrer un choix {entre 1 et 5 }.\n");
            printf("Appuyez sur Entrée pour continuer...");
            getchar(); // On attend que l'utilisateur appuie sur Entrée
            continue; // On revient au début de la boucle
        }

        
        printf("\n");

        switch (choix) {
            case 1:
                printf(">>> Vous avez choisi Gestion des étudiants.\n");
                printf("Appuyez sur Entrée pour aller à la gestion des étudiants...");
                getchar();
                // Serigne Saliou GNINGUE : menu_etudiants();
                break;

            case 2:
                printf(">>> Vous avez choisi Gestion des matières.\n");
                printf("Appuyez sur Entrée pour aller à la gestion des matières...");
                nathanJam();
                Gestion_matieres();
                break;

            case 3:
                printf(">>> Vous avez choisi Gestion des classes.\n");
                printf("Appuyez sur Entrée pour aller à la gestion des classes...");
                getchar();
                // Omar Aminata Diop : menu_classes();
                break;

            case 4:
                printf(">>> Vous avez choisi Gestion des notes.\n");
                printf("Appuyez sur Entrée pour aller à la gestion des notes...");
                getchar();
                // Pape Moussa Gassama : menu_notes();
                break;

            case 5:
                // Demande confirmation pour quitter
                if (confirmer_quitter()) {
                    printf("\nMerci d’avoir utilisé le programme.\n");
                    quitter = 1; // On quitte la boucle
                } else {
                    printf("\nRetour au menu principal...\n");
                    printf("Appuyez sur Entrée pour continuer...");
                    getchar();
                }
                break;
        }
    }
}


int main() {
    menu_principal(); // Lancement du menu principal
    return 0;
}
