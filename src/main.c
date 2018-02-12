#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "universe.h"
#include "command.h"

void create_universe();
void free_universe();
void soup();
void generation();

void help();
void bottom_screen_message();
char prompt();
char *stringprompt(char *prompt_text);

void write_save_game();
void read_saved_game();
void iterate();
void play();

extern struct cell **universe;

int numrows;
int numcols;


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
    noecho();
    help();

    clear();
    soup();

    int should_continue = 1;

    while (should_continue) {
        refresh();

        switch (getch()) {
            case CMD_SAVE:
                write_save_game();
                break;

            case CMD_LOAD:
                read_saved_game(); /* fall through */

            case CMD_CLER: /* TODO */
                break;

            case CMD_ITER:
                iterate();
                break;

            case CMD_PLAY:
                play();
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
                        break;

                    case 'c':
                    case 'C':
                        break;

                }
                break;

            case CMD_HELP:
                help();
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
    char *readbuf;
    char *writebuf;
    char *filename;
    FILE *written;

    chtype *curschar;
    curschar = malloc(sizeof (chtype) * numcols);
    
    do {
        filename = stringprompt("Please enter a filename.");
        written = fopen(filename, "w");
        if (written == NULL) {
            prompt("That file could not be opened for writing!");
        }
    } while (written != NULL);

    free(filename);

    readbuf = NULL;
    writebuf = NULL;
    readbuf = malloc(sizeof (char) * numcols);

    for (int i = 0; i < numrows; i++) {
        writebuf = realloc(writebuf, sizeof (char) * numcols * (i + 1));
        move(i, 0);
        inchstr(curschar);
        int j = 0;

        do {
            readbuf[j] = (char) (curschar[j] & A_CHARTEXT);
            j++;
        } while (curschar[j] != '\0');
        readbuf[j] = '\0';

        strcat(writebuf, readbuf);
        strcat(writebuf, "\n");
    }
    free(readbuf);
    
    fwrite(writebuf, sizeof (char), strlen(writebuf), written);
    free(writebuf);
    fclose(written);
}

/**
  Loads and resumes a saved game.
  */
void read_saved_game() {
    
}

void iterate() {
}

void play() {
}
