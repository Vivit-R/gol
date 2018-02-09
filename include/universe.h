#ifndef ___UNIVERSE___
#define ___UNIVERSE___


/**
  A data structure representing a single cell in our implementation of
  the Game of Life
 */
struct cell {
    int is_alive;
    int has_been_alive;
    int liveneighbors;
    int xcoord;
    int ycoord;
};

#endif
