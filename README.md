# Threadmaze

**Threadmaze** est un projet personnel développé en C dont le but est de créer un **jeu de labyrinthe jouable directement dans le terminal**.
Il permet de **générer des labyrinthes aléatoires**, de **naviguer dedans en temps réel**, et d’utiliser un **solveur intelligent** pour trouver le chemin vers la sortie.


## Captures d'écran

### Exemple de labyrinthe
![Texte alternatif](captures/labyrinthe.png "Exemple de labyrinthe")

### Solveur A*
![Texte alternatif](captures/solveur.png "Solveur")


## Technologies utilisées

- **Langage :** C    
- **IDE :** Visual Studio Code 


## Équipe

- Nombre de développeurs : **1**  
- Statut : **en cours**


## Installation et exécution

1. **Cloner le dépôt :**
    ```bash
    git clone https://github.com/D-l-E-G-O/threadmaze.git
    cd threadmaze
    ```
2. **Compilation :**
    ```bash
    make
    ```
2. **Lancer le jeu :**
    ```bash
    ./bin/maze-game
    ```
4. **Lancer le jeu avec différents paramètres :**
    - **-w** : Défnit la **largeur**
    - **-h** : Définit la **hauteur**
    - **-t** : Définit la **durée du timer**

    *Exemple : Générer un labyrinthe de taille 10x9 avec un temps limité de 5 secondes :*
    ```bash
    ./bin/maze-game -w 10 -h 9 -t 5
    ```

> [!IMPORTANT]
> La taille personnalisée du labyrinthe ne sera appliquée que si elle peut tenir dans la fenêtre du terminal. Si la taille demandée dépasse la largeur ou la hauteur de la fenêtre, le labyrinthe sera automatiquement ajusté pour remplir l'espace disponible dans cet axe.
    

## Fonctionnalités principales

- Génération aléatoire de labyrinthes à probabilité égale basée sur l’algorithme de Wilson
- La taille du labyrinthe généré s'adapte automatiquement pour remplir tout l'espace disponible dans la fenêtre du terminal
- Déplacement du joueur en temps réel dans le terminal
- Solveur intelligent basé sur l’algorithme A* pour trouver le chemin optimal
- Timer optionnel pour ajouter de la difficulté
- Système d'indice “hint” dynamique : affiche temporairement le chemin optimal entre la position du joueur et la sortie
- Interface ligne de commande flexible avec options -w, -h, -t
