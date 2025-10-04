#include "hint.h"


// static int heuristic(CellCoord a, CellCoord b) {
//     return abs(a.row - b.row) + abs(a.col - b.col);
// }

static bool calculate_path_astar(Maze* maze, Player* player, Path* path) {
    // Implémentation classique d'A*
    // Tu peux utiliser une matrice de bool visited, une min-heap ou un tableau simple
    // Chaque CellCoord doit inclure r et c (row, col)
    // Remplir path->cells avec la séquence de cellules du joueur vers la sortie
    // Retourner true si chemin trouvé, false sinon
    maze = maze;
    player = player;
    path = path;
    return true; // placeholder, tu implémentes A*
}

bool init_hint(Hint* hint, Maze* maze, Player* player) {
    if (!hint || !maze || !player) return false;

    hint->active = false;
    hint->duration = 0;

    int max_capacity = maze->width * maze->height;
    hint->path.cells = malloc(sizeof(CellCoord) * max_capacity);
    if (!hint->path.cells) return false;
    hint->path.size = 0;
    hint->path.capacity = max_capacity;

    calculate_path_astar(maze, player, &hint->path);

    return true;
}

void toggle_hint(Hint* hint, int duration) {
    if (!hint) return;
    if (hint->active) {
        hint->active = false;
        hint->duration = 0;
    } else {
        hint->active = true;
        hint->duration = duration;
    }
}

void update_hint(Hint* hint, Maze* maze, Player* player) {
    if (!hint || !hint->active) return;

    hint->duration--;
    if (hint->duration <= 0) {
        hint->active = false;
        hint->duration = 0;
        return;
    }

    // Recalculate path each time the player moves
    hint->path.size = 0;
    calculate_path_astar(maze, player, &hint->path);
}

void free_hint(Hint* hint) {
    if (!hint) return;
    free(hint->path.cells);
    hint->path.cells = NULL;
    hint->path.size = 0;
    hint->path.capacity = 0;
}