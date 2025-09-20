#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>


char get_input_non_blocking(void) {
    char ch = 0;
    struct termios oldt, newt;

    // Sauvegarder l'état du terminal
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return 0;
    newt = oldt;

    // Désactiver l'entrée canonique et l'écho
    newt.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) return 0;

    // Rendre stdin non bloquant
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // Lire le caractère
    int bytes_read = read(STDIN_FILENO, &ch, 1);
    if (bytes_read <= 0) {
        ch = 0; // aucune touche pressée
    }

    // Restaurer stdin bloquant
    fcntl(STDIN_FILENO, F_SETFL, flags);

    // Restaurer le terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}