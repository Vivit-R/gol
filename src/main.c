#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "universe.h"
#include "command.h"

void create_universe();
void free_universe();
void soup();
void step();

void help();
void clearmsg();
char prompt();
char *stringprompt(char *prompt_text);

void write_save_game();
void read_saved_game();
void iterate();
void play();

extern struct cell **universe;
extern char **buffer;

int numrows;
int numcols;


/**
  Execution begins here.
  @param argc the number of arguments given
  @param argv the arguments given
  */
int main(int argc, const char **argv) {

    switch (argc) {
        case 3:
            numcols = atoi(argv[1]);
        case 2:
            numrows = atoi(argv[0]);
            break;

        default:
            numrows = DEFAULT_ROWS;
            numcols = DEFAULT_COLS;
            break;
    }

    create_universe(numrows, numcols);
    initscr();
    noecho();
    help();

    clear();
    soup();

    int should_continue = 1;

    while (should_continue) {
        refresh();
        char cmd = getch();

        switch (cmd) {
            case CMD_SAVE:
                write_save_game();
                break;

            case CMD_STEP:
                step();
                break;

/*            case CMD_LOAD:
                read_saved_game(); // fall through 
                */

            case CMD_ITER:
                iterate();
                break;

            case CMD_QUIT:
                switch (prompt("Save before quitting?" 
                            " ('y' - yes, 'n' - no, 'c' - cancel)")) {
                    case 'y':
                    case 'Y':
                        write_save_game(); /* fall through */

                    case 'n':
                    case 'N':
                        should_continue = 0;
                        clearmsg();
                        break;

                    case 'c':
                    case 'C':
                        clearmsg();
                        break;

                }
                break;

            case CMD_HELP:
                help();
                refresh();
                break;
        }
    }

    endwin();
    echo();
    free_universe();

    printf("Thank you for playing the Game of Life.\n");
}

/**
  Delet this
  */
void panic(char *errstring) {
    printf("ERROR: %s\n", errstring);
    exit(1);
}

/**
  Saves the game state to a text file.
  */
void write_save_game() {
    char *filename;
    FILE *written;

    do {
        filename = stringprompt("Please enter a filename.");
        written = fopen(filename, "a");
        if (written == NULL) {
            prompt("That file could not be opened for writing!");
        }
    } while (written == NULL);

    free(filename);
    
    for (int i = 0; i < numrows; i++) {
        fprintf(written, "%s%s", buffer[i], "\n");
    }
    fclose(written);
}

/**
  Loads and resumes a saved game.
  */
void read_saved_game() { /*
    clear();
    refresh();
    free_universe();

    char *filename;
    FILE *read;

    do {
        filename = stringprompt("Please enter a filename.");
        read = fopen(filename, "r");
        if (written == NULL) {
            prompt("That file could not be opened for reading!");
        }
    } while (written == NULL);

    do {
        filename = stringprompt(fgets()
    }
*/
    
}

void iterate() {
    int gens = atoi(stringprompt("How many generations would you like to run?"));
    for (int i = 0; i < gens; i++) {
        step();
    }
}

void play() {
}
