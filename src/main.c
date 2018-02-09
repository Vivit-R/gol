#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "universe.h"

int numrows;
int numcols;

void create_universe(int rows, int cols);
void free_universe();
void panic(char *errstring);


struct cell **universe;

/**
  Execution begins here.
  @param argc the number of arguments given
  @param argv the arguments given
  */
int main(int argc, char **argv) {
    /* TODO: ASSIGN NUMROW AND NUMCOLS TO ARGUMENTS GIVEN IN 
       PARAMTERS */
    create_universe(40, 14);
    free_universe();
}

/**
  Allocates and initializes memory for the universe.
  @param rows The number of rows of cells in the universe.
  @param cols The number of columns of cells in the universe.
  */
void create_universe(int rows, int cols) {
    numrows = rows;
    numcols = cols;

    universe = malloc(numrows * (sizeof (void *)));
    for (int i = 0; i < numrows; i++) {
        universe[i] = malloc(numcols * (sizeof (struct cell)));
    }

    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            universe[i][j].is_alive = 0;
            universe[i][j].has_been_alive = 0;
            universe[i][j].liveneighbors = 0;
            universe[i][j].ycoord = i;
            universe[i][j].xcoord = j;
            
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


/**
  Increments the number of live neighbors.
  @param incremented A pointer to the cell to be incremented.
  */
void incr_liveneighbors(struct cell *incremented) {
    if (incremented->liveneighbors == 8) {
        panic("Trying bring number of living neighbors above eight");
    }

    incremented->liveneighbors++;
}

/**
  Decrements the number of live neighbors.
  @param decremented A pointer to the cell to be decremented.
  */
void decr_liveneighbors(struct cell *decremented) {
    if (decremented->liveneighbors == 0) {
        panic("Trying bring number of living neighbors below zero");
    }

    decremented->liveneighbors--;
}


/**
  Delet this
  */
void panic(char *errstring) {
    printf("ERROR: %s\n", errstring);
    int i = 1 / 0;
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
                    && (center->xcoord + x <= numcols)
                    && (center->ycoord + y >= 0)
                    && (center->ycoord + y <= numrows)) {
                f(&(universe[center->ycoord][center->xcoord]));
            }
        }
    }
}

/**
  A cell comes to life!
  @param *born a pointer to the cell that will come to life.
  */
void birth(struct cell *born) {
    born->is_alive = 1;
    born->has_been_alive++;
    check_neighbors(born, incr_liveneighbors);
}

/*
  A cell dies...
  @param *died a pointer to the cell that will come to life.
  */
void death(struct cell *died) {
    died->is_alive = 0;
    check_neighbors(died, decr_liveneighbors);
}
