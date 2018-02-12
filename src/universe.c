#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "universe.h"

int numrows;
int numcols;

void create_universe();
void free_universe();
void panic(char *errstring);
void soup();
void birth(struct cell *born);
void death(struct cell *died);
void generation();


struct cell **universe;


/**
  Allocates and initializes memory for the universe.
  */
void create_universe() {
    universe = malloc(numrows * sizeof (void *));

    for (int i = 0; i < numrows; i++) {
        universe[i] = malloc(numcols * (sizeof (struct cell)));
    }

    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            universe[i][j].is_alive = 0;
            universe[i][j].liveneighbors = 0;
            universe[i][j].ycoord = i;
            universe[i][j].xcoord = j;
            universe[i][j].fate = 0;
        }
    }
}

/**
 Iterates through the universe and births or kills cells randomly.
 */
void soup() {
    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            if (rand() % 2) {
                if (universe[i][j].is_alive)
                    death(&(universe[i][j]));
                else
                    birth(&(universe[i][j]));
            }
        }
    }
}

/**
  Frees all cells in universe.
  */
void free_universe() {
    for (int i = numrows - 1; i >= 0; i--) {
        free(universe[i]);
    }
    free(universe);
}
