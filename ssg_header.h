#ifndef ETUDIANT_H
#define ETUDIANT_H



#define MAX_ETUDIANTS 1000
#define MAX_NOM 31
#define MAX_PRENOM 21
#define MAX_EMAIL 21
#define FICHIER_ETUDIANTS "Etudiant.csv"

typedef struct {
    int numero;
    char nom[MAX_NOM];
    char prenom[MAX_PRENOM];
    char email[MAX_EMAIL];
    char date_naissance[11];
    int code_classe;
} Etudiant;

// Fonctions utilitaires
void clear_screen();
void centrer_texte(const char *texte, int largeur);
void afficher_ligne(int longueur, char caractere);
void afficher_titre(const char *titre);
void pause();

// Contrôles de saisie
int est_date_valide(const char *date);
int est_email_valide(const char *email);

// Fonctions de gestion des étudiants
int generer_numero_etudiant();
int generer_code_etudiant();
void saisir_etudiant(Etudiant *e);
void afficher_etudiant(const Etudiant *e);
void afficher_liste_etudiants(Etudiant etudiants[], int nb_etudiants);
void ajouter_etudiant(Etudiant etudiants[], int *nb_etudiants);
void modifier_etudiant(Etudiant etudiants[], int nb_etudiants);
void supprimer_etudiant(Etudiant etudiants[], int *nb_etudiants);
void rechercher_etudiant(Etudiant etudiants[], int nb_etudiants);
int demander_continuer(const char *message);
void sauvegarder_etudiants(Etudiant etudiants[], int nb_etudiants);

// Gestion des fichiers
void initialiser_fichier_csv();
void charger_etudiants(Etudiant etudiants[], int *nb_etudiants);

// Menu étudiant
void menu_etudiant();
char *strcasestr(const char *haystack, const char *needle);
#endif