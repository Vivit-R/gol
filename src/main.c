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
  Execution begins here.
  @param argc the number of arguments given
  @param argv the arguments given
  */
int main(int argc, char **argv) {
    numrows = DEFAULT_ROWS;
    numcols = DEFAULT_COLS;

    switch (argc) {
        case 3:
            numcols = atoi(argv[1]);
        case 2:
            numrows = atoi(argv[0]);
            break;

        default:
            break;
    }

    create_universe(numrows, numcols);
    initscr();

    mvprintw(0, 0, "Press any key to progress the simulation by one "
            "generation, or press Q to quit.\n");
    refresh();
    char input = getch();

    clear();
    soup();

    refresh();

    do {
        input = getch();
        generation();
        refresh();
    } while (input != 'q' && input != 'Q');

    delscreen();
    endwin();
    free_universe();

    printf("Thank you for playing the Game of Life.\n");
}

/**
  Allocates and initializes memory for the universe.
  */
void create_universe() {
    universe = malloc(numrows * (sizeof (void *)));
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
  Runs a single generation.
  */
void generation() {
    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            /* If a dead cell has exactly 3 live neighbors, it comes to
               life.  If a live cell has fewer than 2 or more than 3 
               live neighbors, it dies.  This is the simplest 
               formulation of Conway's rules. */
            switch (universe[i][j].liveneighbors) {
                case 3:
                    if (!universe[i][j].is_alive) 
                        universe[i][j].fate = 1;
                case 2:
                    break;

                default:
                    if (universe[i][j].is_alive) 
                        universe[i][j].fate = -1;
                    break;
            }
        }
    }

    for (int i = 0; i < numrows; i++) {
        for (int j = 0; j < numcols; j++) {
            switch (universe[i][j].fate) {
                case 1:
                    if (!universe[i][j].is_alive)
                        birth(&universe[i][j]);
                    break;

                case -1:
                    if (universe[i][j].is_alive)
                        death(&universe[i][j]);
                    break;
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
                    && (center->xcoord + x < numcols)
                    && (center->ycoord + y >= 0)
                    && (center->ycoord + y < numrows)) {
                f(&(universe[center->ycoord + y][center->xcoord + x]));
            }
        }
    }
}

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

/*
  A cell dies...
  @param *died a pointer to the cell that will come to life.
  */
void death(struct cell *died) {
    died->is_alive = 0;
    check_neighbors(died, decr_liveneighbors);
    mvaddch(died->ycoord, died->xcoord, DEAD_CHAR);
    died->fate = 0;
}
