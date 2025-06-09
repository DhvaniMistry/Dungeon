#include "Game.h"
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(0));
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    start_color();
    init_pair(COLOR_YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
    init_pair(COLOR_MAGENTA_ON_BLACK, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(COLOR_GREEN_ON_BLACK, COLOR_GREEN, COLOR_BLACK);

    int mode = 0;
    do {
        clear();
        attron(COLOR_PAIR(COLOR_GREEN_ON_BLACK));
        mvprintw(0, 0, "Use keys: W,E,R,S,D,F,X,C,V to play\n");
        attroff(COLOR_PAIR(COLOR_GREEN_ON_BLACK));
        
        mvprintw(1, 0, "Choose mode: 1. Human vs Human  2. Human vs Computer > ");
        refresh();
        int ch = getch();
        if (ch == '1' || ch == '2') {
            mode = ch - '0';
            break;
        }
    } while (true);

    do {
        Game game(mode);
        game.play();
    } while (Game(mode).playAgain());

    endwin();
    return 0;
}
