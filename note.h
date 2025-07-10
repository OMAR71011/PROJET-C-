#ifndef NOTE_H
#define NOTE_H

typedef struct {
    char matricule[20];     // identifiant de l'étudiant
    char ref_matiere[20];   // identifiant de la matière
    float note_cc;          // note de contrôle continu
    float note_ds;          // note de devoir surveillé
} Note;

#endif
