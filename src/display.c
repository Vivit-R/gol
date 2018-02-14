#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "universe.h"
#include "command.h"

void help();
void printhelp();
void clearmsg();
WINDOW *helpwin;

int numhelpcols;
int totalnumhelpcols;
int numhelprows;

extern const int numrows;
extern const int numcols;

/* Defining the text for the help screen */
static const char helpstring[] = { 
    "Welcome to the Game of Life.\n"
    "Available commands:\n\n"

    "\'s\' - write current state to a text file\n"
//    "\'l\' - read a state from a text file\n"

    "\'.\' - run one step\n"
    "\'r\' - run multiple steps\n"
    "\'q\' - quit\n"
    "\'?\' - show this help screen\n\n"

    "Press any key to continue.\n"
};

/**
  Shows the help screen.
  */
void help() {
    numhelpcols = 0;
    totalnumhelpcols = 0;
    int numhelprows = 1;

    /* A bit of kludge to figure out how big to make the window: */
    for (int i = 0; helpstring[i] != '\0'; i++) {
        numhelpcols++;

        if (helpstring[i] == '\n') {
            numhelprows++;
            numhelpcols = 0;
        }

        if (numhelpcols > totalnumhelpcols) {
            totalnumhelpcols = numhelpcols;
        }
    }

    totalnumhelpcols += 4;  /* for padding */
    numhelprows += 2;  /* "   "       */

    helpwin = newwin(numhelprows, totalnumhelpcols+2, 1, 1);

    printhelp();
    wgetch(helpwin);
    wclear(helpwin);
    wrefresh(helpwin);
    delwin(helpwin);
}

/**
  Prints a row of #'s for window padding.
  */
void padline() {
    for (int i = 0; i < totalnumhelpcols; i++) {
        waddch(helpwin, '#');
    }
    waddch(helpwin, '\n');
}

/**
  Prints help information to the help window.
  */
void printhelp() {

    padline();
    int charssofar = 2;

    for (int i = 0; helpstring[i] != '\0'; i++) {
        if (charssofar == 2) {
            wprintw(helpwin, "# ");
        }

        if (helpstring[i] == '\n' && helpstring+i+1 != NULL) {
            for (int j = charssofar; j < totalnumhelpcols - 1; j++) {
                waddch(helpwin, ' ');
            }
            wprintw(helpwin, "#");
            charssofar = 2;
        } else {
            charssofar++;
        }
        waddch(helpwin, helpstring[i]);
    }

    padline();
}

/**
  Displays a message at the bottom of the window.
  @param *msg the message to display
  */
void bottom_screen_message(char *msg) {
    clearmsg();
    mvprintw(numrows, 0, msg);
    refresh();
}

/**
  Clears the message displayed.
  */
void clearmsg() {
    move(numrows, 0);
    clrtoeol();
}

/**
  Displays a message asking for a keystroke.
  @param *prompt_text the text to prompt the user with
  @return the key the user pressed
  */
char prompt(char *prompt_text) {
    bottom_screen_message(prompt_text);
    return getch();
}

/**
  Displays a message, and then allows the user to type something.
  @param *prompt_text the text to prompt the user with
  @return the key the user pressed
  */
char *stringprompt(char *prompt_text) {
    char *ret;
    ret = malloc(128 * sizeof (char));
    bottom_screen_message(prompt_text);
    mvprintw(numrows+1, 0, ">");
    echo();
    getnstr(ret, 32);
    clrtoeol();
    clearmsg();

    ret = realloc(ret, sizeof (char) * (strlen(ret) + 1));

    noecho();
    return ret;
}
