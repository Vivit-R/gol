#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "universe.h"

extern const int numrows;
extern const int numcols;

void create_universe();
void free_universe();
void panic(char *errstring);
void soup();
void birth(struct cell *born);
void death(struct cell *died);


struct cell **universe;
extern char **buffer;

/**
  Allocates and initializes memory for the universe.
  */
void create_universe() {
    universe = malloc(numrows * sizeof (void *));
    buffer = malloc(numrows * sizeof (void *));

    for (int i = 0; i < numrows; i++) {
        universe[i] = malloc(numcols * (sizeof (struct cell)));
        buffer[i] = malloc((numcols + 1) * (sizeof (char)));
        for (int j = 0; j < numcols; j++) {
            universe[i][j].is_alive = 0;
            universe[i][j].liveneighbors = 0;
            universe[i][j].ycoord = i;
            universe[i][j].xcoord = j;
            universe[i][j].fate = 0;
            buffer[i][j] = ' ';
        }
        buffer[i][numcols] = '\0';
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
    for (int i = 0; i < numrows; i++) {
        free(universe[i]);
        free(buffer[i]);
    }
    free(universe);
    free(buffer);
}

/**
  A simplified but equivalent version of the rules are used.
  */
void step() {
    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            switch (universe[i][j].liveneighbors) {
                case 2:
                    break;
                case 3:
                    if (!universe[i][j].is_alive) {
                        universe[i][j].fate = 1;
                    }
                    break;
               
                default:
                    if (universe[i][j].is_alive) {
                        universe[i][j].fate = -1;
                    }
            }
        }
    }

    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
                if (universe[i][j].fate == 1) {
                    birth(&(universe[i][j]));
                } else if (universe[i][j].fate == -1) {
                    death(&(universe[i][j]));
                }
                universe[i][j].fate = 0;
        }
    }

    refresh();
}
