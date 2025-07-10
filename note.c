void afficher_notes_etudiant(const char* matricule) {
    FILE *f = fopen("note.csv", "r");
    if (!f) {
        printf("Fichier note.csv introuvable.\n");
        return;
    }

    Note note;
    int trouve = 0;

    printf("\n+--------------------+--------------------+----------+----------+\n");
    printf("| Référence Matière  | Nom Étudiant       |   CC     |   DS     |\n");
    printf("+--------------------+--------------------+----------+----------+\n");

    while (fscanf(f, "%[^,],%[^,],%f,%f\n",
                  note.numero_etudiant,
                  note.reference_matiere,
                  &note.note_cc,
                  &note.note_ds) == 4) {

        if (strcmp(note.numero_etudiant, matricule) == 0) {
            printf("| %-18s | %-18s | %8.2f | %8.2f |\n",
                   note.reference_matiere,
                   note.numero_etudiant,
                   note.note_cc,
                   note.note_ds);
            trouve = 1;
        }
    }

    printf("+--------------------+--------------------+----------+----------+\n");

    if (!trouve) {
        printf("Aucune note trouvée pour l'étudiant : %s\n", matricule);
    }

    fclose(f);
}
