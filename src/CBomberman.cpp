#include "CBomberman.hpp"
#include "CGame.hpp"


using namespace std;

#include "Constants.hpp"

void CRoom::Print(WINDOW *menu_win, int &highlight) const {
    int x, y, i;
    wclear(menu_win);
    x = 5;
    y = 3;
    wattron(menu_win, COLOR_PAIR(2));
    box(menu_win, 0, 0);
    mvwprintw(menu_win, 0, 3, "  BOMBERMAN in Terminal  ");
    mvwprintw(menu_win, 1, 10, "by Jan Pancir ");
    wattron(menu_win, COLOR_PAIR(1));
    for (i = 0; i < len; ++i) {
        if (highlight == i + 1)
        {
            wattron(menu_win, A_REVERSE);
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
            wattroff(menu_win, A_REVERSE);
        }
        else
            mvwprintw(menu_win, y, x, "%s", choices[i].c_str());
        ++y;
    }
    wattron(menu_win, COLOR_PAIR(2));
}

void CRoom::setMapFileNames() {
    choices.clear();
    DIR *d = NULL;
    struct dirent *dir = NULL;
    d = opendir("./examples/");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) choices.push_back(string(dir->d_name));

        }
    }
    closedir(d);
    choices.push_back("Back");
    len = choices.size();
}
void CBomberman::Init() {
    initscr();
    clear();
    noecho();
    curs_set(0);
    cbreak();
    start_color();
    //timeout(-1);
    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
    init_pair(4, COLOR_MAGENTA, COLOR_BLACK);

    menu_win = newwin(HEIGHT, WIDTH, 2, 2);
    keypad(menu_win, TRUE);


    roomList[room].Print(menu_win, highlight);
    mvwprintw(menu_win, 18, 2, string("Selected map: ").append(mapName).c_str());
    refresh();
}

void CBomberman::Run() {
    bool end_Game = false;
    while (1) {
        int choice = -1;
        int c = wgetch(menu_win);
        switch (c) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = roomList[room].lenght();
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == roomList[room].lenght())
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10:
                choice = highlight;
                break;
            default:
                break;
        }

        if (room == 0) {
            switch (choice) {
                case 1 :
                    room = 1;
                    break;
                case 2 :
                    room = 2;
                    highlight = 1;
                    break;
                case 3 : {
                    room = 3;
                    highlight = 1;
                    roomList[3].setMapFileNames();
                    break;
                }
                case 4 :
                    end_Game = true;
                    break;
            }
            if (end_Game) break;
        }
        else if (room == 1) {
            switch (choice) {
                case 1 : {
                    CGame(GM_CLASSIC_EASY, mapName, ipaddr, port);
                    break;
                }
                case 2 :
                    CGame(GM_CLASSIC_MEDIUM, mapName, ipaddr, port);
                    break;
                case 3 :

                    CGame(GM_CLASSIC_HARD, mapName, ipaddr, port);
                    break;
                case 4 :
                    room = 0;
                    highlight = 1;
                    break;
            }
        }
        else if (room == 2) {
            switch (choice) {
                case 1 : {
                    CGame(GM_2PLAYERS, mapName, ipaddr, port);
                    break;
                }
                case 2 : {
                    CGame(GM_SERVER, mapName, ipaddr, port);
                    break;
                }
                case 3 : {
                    CGame(GM_CLIENT, mapName, ipaddr, port);
                    break;
                }
                case 4 :
                    room = 0;
                    highlight = 1;
                    break;
            }
        }
        else if (room == 3) {

            if (choice == roomList[room].lenght()) {
                room = 0;
                highlight = 1;
            }
            else if (choice > 0) {
                mapName = roomList[room].line(choice - 1);
            }
        }

        roomList[room].Print(menu_win, highlight);
        mvwprintw(menu_win, 18, 2, string("Selected map: ").append(mapName).c_str());
        wrefresh(menu_win);
    }

    delwin(menu_win);
    endwin();
}