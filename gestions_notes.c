#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "menu_principal.h"

#define MAX_ETUDIANTS 100
#define MAX_MATIERES 50
#define MAX_NOTES 500
#define FICHIER_NOTES "notes.csv"

/* Structures de donnees */
typedef struct {
    char numero[30];
    char nom[50];
    char classe[30];
} Etudiant;

typedef struct {
    char reference[20];
    char libelle[50];
} Matiere;

typedef struct {
    char numero_etudiant[30];
    char reference_matiere[20];
    float note_cc;
    float note_ds;
    char date[11];
} Note;

/* Variables globales */
Etudiant etudiants[MAX_ETUDIANTS];
Matiere matieres[MAX_MATIERES];
Note notes[MAX_NOTES];
int nb_etudiants = 0;
int nb_matieres = 0;
int nb_notes = 0;

/* Prototypes des fonctions */
void sauvegarderNotes();
void chargerNotes();
void clearScreen();
void pausse();
int estNombreValide(const char *str);
float saisirNote(const char *message);
void obtenirDateCourante(char *date);
void listerEtudiants();
void listerMatieres();
void listerNotes();
void ajouterNote();
int trouverNoteIndex(const char *num_etudiant, const char *ref_matiere);
void modifierNote();
void supprimerNote();
void menuNotes();
int numeroEtudiantExiste(const char *numero);
void rechercherNote();

/* Fonctions utilitaires */
void clearScreen() 
{
    system("cls || clear");
}

void pausse() 
{
    printf("\nAppuyez sur Entree pour continuer...");
    while(getchar() != '\n');
    getchar();
}

int estNombreValide(const char *str) 
{
    int point_count = 0;
    
    if (*str == '-' || *str == '+') str++;
    if (*str == '\0') return 0;
    
    while (*str) {
        if (*str == '.') 
        {
            point_count++;
            if (point_count > 1) return 0;
        } else if (!isdigit((unsigned char)*str)) 
        {
            return 0;
        }
        str++;
    }
    return 1;
}

float saisirNote(const char *message) 
{
    char buffer[32];
    float valeur;
    
    while (1) 
    {
        printf("%s (0-20): ", message);
        scanf("%s", buffer);
        
        if (!estNombreValide(buffer)) 
        {
            printf("Saisie invalide ! Entrez uniquement des chiffres.\n");
            continue;
        }
        
        valeur = atof(buffer);
        if (valeur >= 0 && valeur <= 20) 
        {
            return valeur;
        }
        printf("La note doit etre entre 0 et 20.\n");
    }
}

void obtenirDateCourante(char *date) 
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(date, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

/* Fonctions de sauvegarde/chargement */
void sauvegarderNotes() 
{
    FILE *fichier = fopen(FICHIER_NOTES, "w");
    if (fichier == NULL) 
    {
        printf("Erreur lors de l'ouverture du fichier %s\n", FICHIER_NOTES);
        return;
    }

    // En-tête du fichier CSV
    fprintf(fichier, "NumeroEtudiant;ReferenceMatiere;NoteCC;NoteDS;Date\n");

    // Écriture des notes
    for (int i = 0; i < nb_notes; i++) 
    {
        fprintf(fichier, "%s;%s;%.2f;%.2f;%s\n",
                notes[i].numero_etudiant,
                notes[i].reference_matiere,
                notes[i].note_cc,
                notes[i].note_ds,
                notes[i].date);
    }

    fclose(fichier);
    printf("Notes sauvegardees dans %s\n", FICHIER_NOTES);
}

void chargerNotes() 
{
    FILE *fichier = fopen(FICHIER_NOTES, "r");
    if (fichier == NULL) 
    {
        printf("Aucun fichier de notes existant. Un nouveau sera cree.\n");
        return;
    }

    // Lire l'en-tête
    char ligne[256];
    fgets(ligne, sizeof(ligne), fichier);

    nb_notes = 0;
    while (fgets(ligne, sizeof(ligne), fichier) && nb_notes < MAX_NOTES) 
    {
        // Format: NumeroEtudiant;ReferenceMatiere;NoteCC;NoteDS;Date
        if (sscanf(ligne, "%[^;];%[^;];%f;%f;%[^\n]",
               notes[nb_notes].numero_etudiant,
               notes[nb_notes].reference_matiere,
               &notes[nb_notes].note_cc,
               &notes[nb_notes].note_ds,
               notes[nb_notes].date) == 5) 
            {
                nb_notes++;
            }
    }

    fclose(fichier);
    printf("%d notes chargees depuis %s\n", nb_notes, FICHIER_NOTES);
}

/* Fonctions d'affichage */
void listerEtudiants() 
{
    printf("\n--- Liste des etudiants (%d) ---\n", nb_etudiants);
    for (int i = 0; i < nb_etudiants; i++) 
    {
        printf("%d. %s - %s - %s\n", i+1, etudiants[i].numero, etudiants[i].nom, etudiants[i].classe);
    }
}

void listerMatieres() 
{
    printf("\n--- Liste des matieres (%d) ---\n", nb_matieres);
    for (int i = 0; i < nb_matieres; i++) 
    {
        printf("%d. %s - %s\n", i+1, matieres[i].reference, matieres[i].libelle);
    }
}

void listerNotes() 
{
    clearScreen();
    printf("\n--- Liste des notes (%d) ---\n", nb_notes);
    if (nb_notes == 0) 
    {
        printf("Aucune note enregistree.\n");
        return;
    }
    
    printf("+-----+----------------------+-------------------+---------+---------+------------+\n");
    printf("| Num | Num_Etudiant         | Ref_Matiere       | CC (/20)| DS (/20)| Date       |\n");
    printf("+-----+----------------------+-------------------+---------+---------+------------+\n");
    for (int i = 0; i < nb_notes; i++) 
    {
        printf("| %-3d | %-20s | %-17s | %-7.2f | %-7.2f | %-10s |\n",
            i+1,
            notes[i].numero_etudiant,
            notes[i].reference_matiere,
            notes[i].note_cc,
            notes[i].note_ds,
            notes[i].date
        );
    }
    printf("+-----+----------------------+-------------------+---------+---------+------------+\n");
}

/* Fonctions de gestion des notes */
void ajouterNote() 
{
    char buffer[8];
    do {
        if (nb_notes >= MAX_NOTES) 
        {
            printf("Nombre maximum de notes atteint!\n");
            pausse();
            return;
        }
        
        Note nouvelle_note;
        clearScreen();
        printf("\n--- Ajout d'une nouvelle note ---\n");

        printf("Numero etudiant: ");
        scanf("%29s", nouvelle_note.numero_etudiant);

        // Vérifie si le numéro d'étudiant existe déjà
        if (numeroEtudiantExiste(nouvelle_note.numero_etudiant)) 
        {
            printf("Erreur: Le numero d'etudiant %s existe deja!\n", nouvelle_note.numero_etudiant);
            pausse();
        } else {
            printf("Reference matiere: ");
            scanf("%19s", nouvelle_note.reference_matiere);

            nouvelle_note.note_cc = saisirNote("Note controle continu");
            nouvelle_note.note_ds = saisirNote("Note devoir surveille");

            obtenirDateCourante(nouvelle_note.date);

            notes[nb_notes++] = nouvelle_note;
            printf("Note ajoutee avec succes!\n");
            pausse();
        }

        printf("Voulez-vous ajouter une autre note ? (o/n): ");
        scanf("%7s", buffer);
    } while (
        strcasecmp(buffer, "o") == 0 ||
        strcasecmp(buffer, "oui") == 0
    );
}

int trouverNoteIndex(const char *num_etudiant, const char *ref_matiere) 
{
    for (int i = 0; i < nb_notes; i++) 
    {
        if (strcmp(notes[i].numero_etudiant, num_etudiant) == 0 && 
            strcmp(notes[i].reference_matiere, ref_matiere) == 0) 
            {
                return i;
            }
    }
    return -1;
}

void modifierNote() 
{
    char buffer[8];
    do {
        clearScreen();
        if (nb_notes == 0) 
        {
            printf("Aucune note a modifier!\n");
            pausse();
            return;
        }
        
        char num_etudiant[30], ref_matiere[20];
        printf("\n--- Modification d'une note ---\n");
        listerNotes();
        
        printf("Numero etudiant de la note a modifier: ");
        scanf("%29s", num_etudiant);
        printf("Reference matiere de la note a modifier: ");
        scanf("%19s", ref_matiere);
        
        int index = trouverNoteIndex(num_etudiant, ref_matiere);
        if (index == -1) 
        {
            printf("Note non trouvee!\n");
            pausse();
        }
        else
        {
            printf("Ancienne note CC: %.2f, Ancienne note DS: %.2f\n", 
                notes[index].note_cc, notes[index].note_ds);
            
            notes[index].note_cc = saisirNote("Nouvelle note de controle continu");
            notes[index].note_ds = saisirNote("Nouvelle note de devoir surveille");
            
            obtenirDateCourante(notes[index].date);
            printf("Note modifiee avec succes!\n");
            pausse();
        }

        printf("Voulez-vous modifier une autre note ? (o/n): ");
        scanf("%7s", buffer);
    } while (
        strcasecmp(buffer, "o") == 0 ||
        strcasecmp(buffer, "oui") == 0
    );
}

void supprimerNote() 
{
    char buffer[8];
    do {
        clearScreen();
        if (nb_notes == 0) 
        {
            printf("Aucune note a supprimer!\n");
            pausse();
            return;
        }
        
        char num_etudiant[30], ref_matiere[20];
        printf("\n--- Suppression d'une note ---\n");
        listerNotes();
        
        printf("Numero etudiant de la note a supprimer: ");
        scanf("%29s", num_etudiant);
        printf("Reference matiere de la note a supprimer: ");
        scanf("%19s", ref_matiere);
        
        int index = trouverNoteIndex(num_etudiant, ref_matiere);
        if (index == -1) 
        {
            printf("Note non trouvee!\n");
            pausse();
        }
        else
        {
            for (int i = index; i < nb_notes - 1; i++) 
            {
                notes[i] = notes[i + 1];
            }
            nb_notes--;
            printf("Note supprimee avec succes!\n");
            pausse();
        }

        printf("Voulez-vous supprimer une autre note ? (o/n): ");
        scanf("%7s", buffer);
    } while (
        strcasecmp(buffer, "o") == 0 ||
        strcasecmp(buffer, "oui") == 0
    );
}

void rechercherNote() 
{
    clearScreen();
    if (nb_notes == 0) 
    {
        printf("Aucune note enregistree.\n");
        pausse();
        return;
    }

    char num_etudiant[30], ref_matiere[20];
    printf("\n--- Recherche d'une note ---\n");
    printf("Numero etudiant : ");
    scanf("%29s", num_etudiant);
    printf("Reference matiere : ");
    scanf("%19s", ref_matiere);

    int index = trouverNoteIndex(num_etudiant, ref_matiere);
    if (index == -1) 
    {
        printf("Note non trouvee !\n");
    } 
    else 
    {
        printf("\nNote trouvee :\n");
        printf("Etudiant : %s\n", notes[index].numero_etudiant);
        printf("Matiere  : %s\n", notes[index].reference_matiere);
        printf("CC (/20) : %.2f\n", notes[index].note_cc);
        printf("DS (/20) : %.2f\n", notes[index].note_ds);
        printf("Date     : %s\n", notes[index].date);
    }
    pausse();
}

/* Menu de gestion des notes */
void menuNotes() 
{
    int choix;
    do {
        clearScreen();
        printf("=== GESTION DES NOTES ===\n");
        printf("1. Ajouter une note\n");
        printf("2. Lister les notes\n");
        printf("3. Modifier une note\n");
        printf("4. Supprimer une note\n");
        printf("5. Rechercher une note\n"); // Ajout de l'option
        printf("6. Sauvegarder les notes\n");
        printf("7. Retour au menu principal\n");
        printf("Choix: ");
        scanf("%d", &choix);
        
        switch(choix) 
        {
            case 1: ajouterNote(); break;
            case 2: listerNotes(); break;
            case 3: modifierNote(); break;
            case 4: supprimerNote(); break;
            case 5: rechercherNote(); break; // Ajout de la fonction
            case 6: sauvegarderNotes(); pausse(); break;
            case 7: return;
            default: printf("Choix invalide!\n");
        }
        pausse();
    } while (1);
}

/* Programme principal */
void gestion_note()  
{
    chargerNotes(); // Charge les notes au démarrage
    
    int choix;
    do {
        clearScreen();
        printf("=== SYSTEME DE GESTION DES NOTES ===\n");
        printf("1. Gestion des notes\n");
        printf("2. Quitter\n");
        printf("Choix: ");
        scanf("%d", &choix);
        
        switch(choix) 
        {
            case 1: menuNotes(); break;
            case 2: 
                sauvegarderNotes(); // Sauvegarde avant de quitter
                printf("Au revoir!\n"); 
                break;
            default: printf("Choix invalide!\n"); pausse();
        }
    } while (choix != 2);

}

// Vérifie si un numéro d'étudiant existe déjà (insensible à la casse)
int numeroEtudiantExiste(const char *numero) 
{
    for (int i = 0; i < nb_etudiants; i++) 
    {
        if (strcasecmp(etudiants[i].numero, numero) == 0) 
        {
            return 1; // Existe déjà
        }
    }
    return 0;
}