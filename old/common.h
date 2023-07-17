//
// Created by ich on 11.07.23.
//

#ifndef NIX_OPERATOR_COMMON_H
#define NIX_OPERATOR_COMMON_H
#include <ncurses.h>

int common_print_title(WINDOW* win)
{
    int maxWidth, maxHeight;
    getmaxyx(win, maxHeight, maxWidth);

    int y = 1;
    int x = (maxWidth - 127) / 2;

    if(maxWidth > 127 + 4)
    {
        mvwprintw(win, y++, x, " ,ggg, ,ggggggg,                          _,gggggg,_                                                                          ");
        mvwprintw(win, y++, x, "dP\"\"Y8,8P\"\"\"\"\"Y8b                       ,d8P\"\"d8P\"Y8b,                                              I8                        ");
        mvwprintw(win, y++, x, "Yb, `8dP'     `88                      ,d8'   Y8   \"8b,dP                                           I8                        ");
        mvwprintw(win, y++, x, " `\"  88'       88   gg                 d8'    `Ybaaad88P'                                        88888888                     ");
        mvwprintw(win, y++, x, "     88        88   \"\"                 8P       `\"\"\"\"Y8                                             I8                        ");
        mvwprintw(win, y++, x, "     88        88   gg      ,gg,   ,gg 8b            d8gg,gggg,     ,ggg,    ,gggggg,    ,gggg,gg   I8     ,ggggg,   ,gggggg, ");
        mvwprintw(win, y++, x, "     88        88   88     d8\"\"8b,dP\"  Y8,          ,8PI8P\"  \"Yb   i8\" \"8i   dP\"\"\"\"8I   dP\"  \"Y8I   I8    dP\"  \"Y8gggdP\"\"\"\"8I ");
        mvwprintw(win, y++, x, "     88        88   88    dP   ,88\"    `Y8,        ,8P'I8'    ,8i  I8, ,8I  ,8'    8I  i8'    ,8I  ,I8,  i8'    ,8I ,8'    8I ");
        mvwprintw(win, y++, x, "     88        Y8,_,88,_,dP  ,dP\"Y8,    `Y8b,,__,,d8P',I8 _  ,d8'  `YbadP' ,dP     Y8,,d8,   ,d8b,,d88b,,d8,   ,d8',dP     Y8,");
        mvwprintw(win, y++, x, "     88        `Y88P\"\"Y88\"  dP\"   \"Y8     `\"Y8888P\"'  PI8 YY88888P888P\"Y8888P      `Y8P\"Y8888P\"`Y88P\"\"Y8P\"Y8888P\"  8P      `Y8");
        mvwprintw(win, y++, x, "                                                       I8                                                                     ");
        mvwprintw(win, y++, x, "                                                       I8                                                                     ");
        mvwprintw(win, y++, x, "                                                       I8                                                                     ");
        mvwprintw(win, y++, x, "                                                       I8                                                                     ");
        mvwprintw(win, y++, x, "                                                       I8                                                                     ");
        mvwprintw(win, y++, x, "                                                       I8                                                                     ");
    }
    else
    {
        mvwprintw(win, y++, x, " _____ _        _____                 _           ");
        mvwprintw(win, y++, x, "|   | |_|_ _   |     |___ ___ ___ ___| |_ ___ ___ ");
        mvwprintw(win, y++, x, "| | | | |_'_|  |  |  | . | -_|  _| .'|  _| . |  _|");
        mvwprintw(win, y++, x, "|_|___|_|_,_|  |_____|  _|___|_| |__,|_| |___|_|  ");
        mvwprintw(win, y++, x, "                     |_|                          ");
    }

    return y;
}

#endif //NIX_OPERATOR_COMMON_H
