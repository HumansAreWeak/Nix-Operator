//
// Created by ich on 11.07.23.
//
#include "main_menu.h"
#include "err_codes.h"
#include "common.h"
#include <ncurses.h>
#define MAINMENU_SELECTION_SIZE 3

static const char* selection[MAINMENU_SELECTION_SIZE] = {
        "Analyze System Configuration",
        "Analyze Home-Manager Configuration",
        "Analyze current directory for Flakes",
};

static WINDOW *mainMenu = NULL;

static int highlight = 0;

int main_menu_prepare(int maxWidth, int maxHeight)
{
    mainMenu = newwin(maxHeight, maxWidth, 0, 0);
}

int main_menu_draw()
{
    if(mainMenu == NULL)
    {
        return ERR_WINDOW_NOT_INITIALIZED;
    }

    curs_set(0);
    keypad(mainMenu, 1);

    while(1)
    {
        int yOffset = common_print_title(mainMenu);

        for(int i = 0; i < MAINMENU_SELECTION_SIZE; i++)
        {
            if(i == highlight) {
                wattron(mainMenu, A_REVERSE);
            }
            mvwprintw(mainMenu, yOffset + i, 1, "%s", selection[i]);
            wattroff(mainMenu, A_REVERSE);
        }

        int key = wgetch(mainMenu);

        switch (key)
        {
            case KEY_UP:
                if(--highlight < 0) highlight = 0;
                break;
            case KEY_DOWN:
                if(++highlight >= MAINMENU_SELECTION_SIZE) highlight = MAINMENU_SELECTION_SIZE - 1;
                break;
            case KEY_ENTER:
                return highlight;
            default:
                break;
        }

        wrefresh(mainMenu);
    }
}
