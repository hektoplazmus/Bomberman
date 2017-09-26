#ifndef SRC_CBOMBERMAN_HPP
#define SRC_CBOMBERMAN_HPP
#include <string>
#include "ncurses.h"
#include "dirent.h"
#include <vector>

/** height of menu screen */
#define HEIGHT 20
/** width of menu screen */
#define WIDTH 60

using namespace std;
class CGame;

/** Menu buttons text constants*/
const char static  *textR0[] = {"Single - Player", "Multi - Player", "Maps", "Exit game"};
const char static  *textR1[] = {"Easy", "Medium", "Hard", "Back"};
const char static  *textR2[] = {"2 Player on this computer", "Create server", "Connect to server", "Back"};
const char static  *textR3[] = {"Maps"};

/**
 * class for menu rooms like singleplayer, multiplayer ...
 * contains text for menu buttons
 */
class CRoom {
public:
    /**
     * Creates menu room with button choices from text array
     * @param [in] text array texts for buttons
     * @param [in] len number of buttons
     */
    CRoom(const char **text,int len) : len(len) {
        for (int i = 0; i < len; i++)
            choices.push_back(text[i]);
    }
    /**
     * Prints menu buttons highlighting the chosen one
     * @param [in] menu_win window to draw on
     * @param [in] highlight index of chosen button to highlight
     */
    void Print(WINDOW *menu_win, int &highlight) const;

    /**
        * @return number of buttons in room
        */
    const int &lenght() const {
        return len;
    }
    /**
     * Creates menu buttons for each map file in directory ./examples/
     */
    void setMapFileNames();

    /**
     * @param [in] i index of text to return
     * @return text of buttons in i position
     */
    string line(int i) const {
        return choices[i];
    }
private:
    /** button texts */
    std::vector<string> choices;
    /** number of buttons */
    int len;
};

/**
 * Main menu class, initialize all ncurses values, allow player to choose map and game mode
 * displays menu graphic and starts game in chosen game mode
 */
class CBomberman {
public:
    /**
     * Creates new application Bomberman
     * @param [in] ipaddr ip address of server or client for connection
     * @param [in] port port of server or client for connection
     * For playing without internet connection are used options 'localhost' '2666'
     */
    CBomberman(string ipaddr = "localhost", int port = 2666) : ipaddr(ipaddr), port(port) { }

    /**
     * Initialize menu window and all ncurses values
     */
    void Init();

    /**
     * Start the menu loop and allow player to interact with menu buttons
     * Player can chose map, start game in specific game mode or end the game
     */
    void Run();

private:
    /** ip address of server or client for connection */
    string ipaddr;
    /** port of server or client for connection */
    int port;
    /** menu window to print menu design and all buttons */
    WINDOW *menu_win;
    /** index of highlighted (chosen) button */
    int highlight = 1;
    /** number of current menu room */
    int room = 0;
    /** name of selected map (default map01)*/
    string mapName = "map01";
    /** list of all rooms in game menu */
    CRoom roomList[4] = {CRoom(textR0, sizeof(textR0)/sizeof(*textR0)),
                         CRoom(textR1, sizeof(textR1)/sizeof(*textR1)),
                         CRoom(textR2, sizeof(textR2)/sizeof(*textR2)), CRoom(textR3,0)};

};


#endif //SRC_CBOMBERMAN_HPP
