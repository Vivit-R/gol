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
void birth();
void bottom_screen_message();
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
            numcols = atoi(argv[2]);
        case 2:
            numrows = atoi(argv[1]);
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

            case CMD_LOAD:
                read_saved_game();
                break;

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

    int done = 0;

    filename = stringprompt("Please enter a filename.");
    written = fopen(filename, "r");
    if (written == NULL) {
        written = fopen(filename, "w");

        if (written == NULL) {
            bottom_screen_message("That file could not be "
                    "opened for writing!");
            return;
        }
    } else {
        fclose(written);
        while (!done) {
            switch (prompt("That file already exists. "
                       "Do you want to overwrite it? (y/n)")) {
                case 'n':
                    bottom_screen_message("Aborted.");
                    return;

                case 'y':
                    done = 1;

                default:
                    break;
            }
        }
        written = fopen(filename, "w");
        done = 0;

    }


    free(filename);
    
    for (int i = 0; i < numrows; i++) {
        fprintf(written, "%s%s", buffer[i], "\n");
    }
    fclose(written);
}

/**
  Loads and resumes a saved game.
  */
void read_saved_game() {
    char *filename;
    FILE *read;

    filename = stringprompt("Please enter a filename.");
    read = fopen(filename, "r");
    if (read == NULL) {
        bottom_screen_message("That file could not be opened for "
                "reading!");
        return;
    }

    free_universe();
    char readchar;

    int rowcount = 0;
    int colcount = 0;
    int done = 0;
    int rowlength = 0;

    int newline_previous = 0;

    /* Measuring the dimensions of the file */
    for (int i = 0; !done; i++) {
        readchar = fgetc(read);
        switch (readchar) {
            case '\n':
                if (rowlength > colcount) {
                    colcount = rowlength;
                }
                rowlength = 0;
                newline_previous = 1;
                rowcount++;
                break;

            case EOF:
                if (!newline_previous) {
                    colcount++;
                }
                done = 1;
                break;
            default:
                rowlength++;
                newline_previous = 0;
                break;
        }
    }

    numrows = rowcount;
    numcols = colcount;

    clear();
    refresh();

    create_universe();

    fseek(read, 0, SEEK_SET);

    done = 0;
    int currow = 0;
    int curcol = 0;
    int errchar = 0;
    
    for (int i = 0; !done; i++) {
        /* can't happen */
        if (currow > numrows || curcol > numcols) {
            bottom_screen_message("Dimension mismatch reading file.");
            break;
        }

        readchar = fgetc(read);

        switch (readchar) {
            case '\n':
                move(++currow, 0);
                curcol = 0;
                break;

            case LIVE_CHAR:
                birth(&(universe[currow][curcol]));
                curcol++;
                refresh();
                break;

            case DEAD_CHAR:
            case ' ':
                curcol++;
                refresh();
                break;

            case EOF:
                done = 1;
                break;

            default:
                errchar++;
                break;
        }
    }

    if (errchar) {
        bottom_screen_message("I found some characters in that file "
                "that I didn't understand, but here's what I did "
                "understand.");
    }
}

void iterate() {
    int gens = atoi(stringprompt("How many generations would you like to run?"));
    for (int i = 0; i < gens; i++) {
        step();
    }
}

void play() {
}
