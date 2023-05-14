#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    int x; // coordonnées x du tableau
    int y; // coordonnées y du tablea
} Coords;

typedef struct {
    Coords taille; // taille x et y du tableau
    char** board; // tableau 2D qui caractérise le plateau de jeu
} Plate;

typedef struct {
    char type; // caractère qui représente l'identité du pion
    Coords coord; // coordonnées x et y du pion
} Pawn;

Plate GameBoard(int x, int y);
void displayBoard(Plate x);
bool checkWin(Plate x, char player);
void gravity(Plate x);
void pivotRight(Plate x, Coords y, int size);
void pivotLeft(Plate x, Coords y, int size);
void savePlate(Plate x, const char* file_name);
bool setPawn(Plate x, int col, char player);
