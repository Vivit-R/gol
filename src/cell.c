#include <stdlib.h>
#include <ncurses.h>
#include "universe.h"

void panic(char *errstring);
void check_neighbors();
void decr_liveneighbors(struct cell *decremented);
void incr_liveneighbors(struct cell *incremented);
void check_neighbors(struct cell *center, void (*f)(struct cell *));

extern int numrows;
extern int numcols;

extern struct cell **universe;

/* Whenever a cell dies or comes to life, the cells around it all have
   their liveneighbors fields updated accordingly. */

/**
  A cell comes to life!
  @param *born a pointer to the cell that will come to life.
  */
void birth(struct cell *born) {
    born->is_alive = 1;
    check_neighbors(born, incr_liveneighbors);
    mvaddch(born->ycoord, born->xcoord, LIVE_CHAR);
    born->fate = 0;
}

/**
  A cell dies...
  @param *died a pointer to the cell that will come to life.
  */
void death(struct cell *died) {
    died->is_alive = 0;
    check_neighbors(died, decr_liveneighbors);
    mvaddch(died->ycoord, died->xcoord, DEAD_CHAR);
    died->fate = 0;
}

/**
  Increments the number of live neighbors.
  @param incremented A pointer to the cell to be incremented.
  */
void incr_liveneighbors(struct cell *incremented) {
    if (incremented->liveneighbors >= 8)
        panic("Trying bring number of living neighbors above eight");

    incremented->liveneighbors++;
}

/**
  Decrements the number of live neighbors.
  @param decremented A pointer to the cell to be decremented.
  */
void decr_liveneighbors(struct cell *decremented) {
    if (decremented->liveneighbors <= 0)
        panic("Trying bring number of living neighbors below zero");

    decremented->liveneighbors--;
}

/**
  Checks the bordering cells and calls a given void function on them.
  It feels so weird to be paramtereizing functions in C, but also
  pretty cool.
  @param center the center cell whose neighbors we are looking at
  @param f a pointer to a function to call on those neighbors.
  */
void check_neighbors(struct cell *center, void (*f)(struct cell *)) {
    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            if (!(x == 0 && y == 0)
                    && (center->xcoord + x >= 0)
                    && (center->xcoord + x < numcols)
                    && (center->ycoord + y >= 0)
                    && (center->ycoord + y < numrows)) {
                f(&(universe[center->ycoord + y][center->xcoord + x]));
            }
        }
    }
}
