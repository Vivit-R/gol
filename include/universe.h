#ifndef ___UNIVERSE___
#define ___UNIVERSE___


/**
  A data structure representing a single cell in our implementation of
  the Game of Life
 */
struct cell {
    int fate; /* -1 means the cell will die next generation, 1 means the
                 cell will come alive */
    int is_alive;
    int liveneighbors;
    int xcoord;
    int ycoord;
};

#define LIVE_CHAR '#'
#define DEAD_CHAR '.'

#define DEFAULT_ROWS 20
#define DEFAULT_COLS 20

#endif
