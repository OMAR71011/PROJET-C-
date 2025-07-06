#include "matiere.h"
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void clear_screen(void) {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void wait_for_enter(void) {
    printf("\nAppuyez sur Entrée pour retourner au menu précédent...");
    // Vide le buffer d'entrée
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // Attend la nouvelle entrée
    getchar();
    clear_screen();
}