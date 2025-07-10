#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_ETUDIANTS 100
#define MAX_MATIERES 50
#define MAX_NOTES 500
#define FICHIER_NOTES "notes.csv"
#define FICHIER_ETUDIANTS "etudiants.csv"
#define FICHIER_MATIERES "matieres.csv"

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
void sauvegarderDonnees();
void chargerDonnees();
void clearScreen();
void pause();
int estNombreValide(const char *str);
float saisirNote(const char *message);
void obtenirDateCourante(char *date);
void genererNumeroEtudiant(char *numero);
void genererReferenceMatiere(char *reference);
void ajouterEtudiantAuto();
void ajouterMatiereAuto();
void listerEtudiants();
void listerMatieres();
void listerNotes();
void ajouterNote();
void modifierNote();
void supprimerNote();
void rechercherNote();
void menuNotes();
void menuPrincipal();

/* Fonctions utilitaires */
void clearScreen() {
    system("cls || clear");
}

void pause() {
    printf("\nAppuyez sur Entree pour continuer...");
    while(getchar() != '\n');
    getchar();
}

int estNombreValide(const char *str) {
    int point_count = 0;
    
    if (*str == '-' || *str == '+') str++;
    if (*str == '\0') return 0;
    
    while (*str) {
        if (*str == '.') {
            point_count++;
            if (point_count > 1) return 0;
        } else if (!isdigit((unsigned char)*str)) {
            return 0;
        }
        str++;
    }
    return 1;
}

float saisirNote(const char *message) {
    char buffer[32];
    float valeur;
    
    while (1) {
        printf("%s (0-20): ", message);
        scanf("%s", buffer);
        
        if (!estNombreValide(buffer)) {
            printf("Saisie invalide ! Entrez uniquement des chiffres.\n");
            continue;
        }
        
        valeur = atof(buffer);
        if (valeur >= 0 && valeur <= 20) {
            return valeur;
        }
        printf("La note doit etre entre 0 et 20.\n");
    }
}

void obtenirDateCourante(char *date) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    sprintf(date, "%02d/%02d/%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
}

void genererNumeroEtudiant(char *numero) {
    static int compteur = 1;
    sprintf(numero, "ETU%04d", compteur++);
}

void genererReferenceMatiere(char *reference) {
    static int compteur = 1;
    sprintf(reference, "MAT%03d", compteur++);
}

void ajouterEtudiantAuto() {
    if (nb_etudiants >= MAX_ETUDIANTS) {
        printf("Nombre maximum d'etudiants atteint!\n");
        return;
    }
    
    genererNumeroEtudiant(etudiants[nb_etudiants].numero);
    printf("Nom de l'etudiant: ");
    scanf("%49s", etudiants[nb_etudiants].nom);
    printf("Classe de l'etudiant: ");
    scanf("%29s", etudiants[nb_etudiants].classe);
    nb_etudiants++;
    printf("Etudiant ajoute avec le numero: %s\n", etudiants[nb_etudiants-1].numero);
}

void ajouterMatiereAuto() {
    if (nb_matieres >= MAX_MATIERES) {
        printf("Nombre maximum de matieres atteint!\n");
        return;
    }
    
    genererReferenceMatiere(matieres[nb_matieres].reference);
    printf("Libelle de la matiere: ");
    scanf("%49s", matieres[nb_matieres].libelle);
    nb_matieres++;
    printf("Matiere ajoutee avec la reference: %s\n", matieres[nb_matieres-1].reference);
}

void sauvegarderDonnees() {
    FILE *f = fopen(FICHIER_ETUDIANTS, "w");
    if (f) {
        fprintf(f, "numero;nom;classe\n");
        for (int i = 0; i < nb_etudiants; i++) {
            fprintf(f, "%s;%s;%s\n", etudiants[i].numero, etudiants[i].nom, etudiants[i].classe);
        }
        fclose(f);
    }
    f = fopen(FICHIER_MATIERES, "w");
    if (f) {
        fprintf(f, "reference;libelle\n");
        for (int i = 0; i < nb_matieres; i++) {
            fprintf(f, "%s;%s\n", matieres[i].reference, matieres[i].libelle);
        }
        fclose(f);
    }
    f = fopen(FICHIER_NOTES, "w");
    if (f) {
        fprintf(f, "numero_etudiant;reference_matiere;note_cc;note_ds;date\n");
        for (int i = 0; i < nb_notes; i++) {
            fprintf(f, "%s;%s;%.2f;%.2f;%s\n",
                    notes[i].numero_etudiant,
                    notes[i].reference_matiere,
                    notes[i].note_cc,
                    notes[i].note_ds,
                    notes[i].date);
        }
        fclose(f);
    }
}

void chargerDonnees() {
    FILE *f = fopen(FICHIER_ETUDIANTS, "r");
    if (f) {
        char ligne[256];
        fgets(ligne, sizeof(ligne), f);
        while (fgets(ligne, sizeof(ligne), f) && nb_etudiants < MAX_ETUDIANTS) {
            if (sscanf(ligne, "%[^;];%[^;];%[^\n]",
                   etudiants[nb_etudiants].numero,
                   etudiants[nb_etudiants].nom,
                   etudiants[nb_etudiants].classe) == 3) {
                nb_etudiants++;
            }
        }
        fclose(f);
    }
    f = fopen(FICHIER_MATIERES, "r");
    if (f) {
        char ligne[256];
        fgets(ligne, sizeof(ligne), f);
        while (fgets(ligne, sizeof(ligne), f) && nb_matieres < MAX_MATIERES) {
            if (sscanf(ligne, "%[^;];%[^\n]",
                   matieres[nb_matieres].reference,
                   matieres[nb_matieres].libelle) == 2) {
                nb_matieres++;
            }
        }
        fclose(f);
    }
    f = fopen(FICHIER_NOTES, "r");
    if (f) {
        char ligne[256];
        fgets(ligne, sizeof(ligne), f);
        while (fgets(ligne, sizeof(ligne), f) && nb_notes < MAX_NOTES) {
            if (sscanf(ligne, "%[^;];%[^;];%f;%f;%[^\n]",
                   notes[nb_notes].numero_etudiant,
                   notes[nb_notes].reference_matiere,
                   &notes[nb_notes].note_cc,
                   &notes[nb_notes].note_ds,
                   notes[nb_notes].date) == 5) {
                nb_notes++;
            }
        }
        fclose(f);
    }
}

void ajouterNote() {
    char choix;
    do {
        if (nb_notes >= MAX_NOTES) {
            printf("Nombre maximum de notes atteint!\n");
            pause();
            return;
        }
        if (nb_etudiants == 0) {
            printf("Aucun etudiant enregistre. Voulez-vous en ajouter un ? (o/n) ");
            scanf(" %c", &choix);
            if (choix == 'o' || choix == 'O') ajouterEtudiantAuto();
            else return;
        }
        if (nb_matieres == 0) {
            printf("Aucune matiere enregistree. Voulez-vous en ajouter une ? (o/n) ");
            scanf(" %c", &choix);
            if (choix == 'o' || choix == 'O') ajouterMatiereAuto();
            else return;
        }
        listerEtudiants();
        listerMatieres();
        char type_note;
        printf("\nType de note a ajouter :\n1. Controle Continu (CC)\n2. Devoir Surveille (DS)\n3. Les deux\nChoix : ");
        scanf(" %c", &type_note);
        Note nouvelle_note;
        printf("\nNumero etudiant : ");
        scanf("%29s", nouvelle_note.numero_etudiant);
        printf("Reference matiere : ");
        scanf("%19s", nouvelle_note.reference_matiere);
        switch(type_note) {
            case '1':
                nouvelle_note.note_cc = saisirNote("Note de controle continu");
                nouvelle_note.note_ds = -1;
                break;
            case '2':
                nouvelle_note.note_cc = -1;
                nouvelle_note.note_ds = saisirNote("Note de devoir surveille");
                break;
            case '3':
                nouvelle_note.note_cc = saisirNote("Note de controle continu");
                nouvelle_note.note_ds = saisirNote("Note de devoir surveille");
                break;
            default:
                printf("Choix invalide!\n");
                return;
        }
        obtenirDateCourante(nouvelle_note.date);
        notes[nb_notes++] = nouvelle_note;
        printf("Note ajoutee avec succes!\n");
        printf("\nVoulez-vous ajouter une autre note ? (o/n) ");
        scanf(" %c", &choix);
    } while (choix == 'o' || choix == 'O');
}

void menuPrincipal() {
    char choix;
    do {
        clearScreen();
        printf("=== MENU PRINCIPAL ===\n");
        printf("1. Gestion des etudiants\n");
        printf("2. Gestion des matieres\n");
        printf("3. Gestion des notes\n");
        printf("4. Quitter\n");
        printf("Choix : ");
        scanf(" %c", &choix);
        switch(choix) {
            case '1': break; // A implementer
            case '2': break; // A implementer
            case '3': ajouterNote(); break;
            case '4': sauvegarderDonnees(); printf("Au revoir!\n"); return;
            default: printf("Choix invalide!\n"); pause();
        }
    } while (1);
}

int main() {
    chargerDonnees();
    menuPrincipal();
    return 0;
}