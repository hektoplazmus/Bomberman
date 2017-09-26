#include "CBomberman.hpp"
/**
 * @mainpage Bomberman
 *
 *Game bomberman is an action game with many different
 * game modes
 *      :
 *      - mode against AI bots with 3 different options of difficulty (easy, medium, hard)
 *      - 2 players on the same computer (second player uses arrow keys to move and zero for bomb placing)
 *      - create server (default ip is localhost, default port is 2666)
 *      - connect to server (default ip is localhost, default port is 2666)
 *
 *
 *  MAPS:
 *  in menu you can choose one of the maps from directory ./examples/
 *
 *  CREATING MAPS:
 *  Map is text file with specific format,
 *  line beginning with '#' is a comment and is skipped
 *  - ' ' space is empty place, player can move to this place
 *  - 'X' is a solid block (map has to be bordered with this symbol!)
 *  - 'O' is a box which can be destroyed by bombs
 *  - 'S' is a movement speed bonus in box
 *  - 'M' is a manual detonation bonus in box
 *  - 'F' is a flame range bonus in box
 *  - 'B' is a more bombs bonus
 *  - 'P' is a player (map has to have minimum 2 players)
 *  - map has to be rectangle
 *
 *  CONTROLS:
 *  In menu use Arrow keys and enter, in game
 *  player move keys are W A S D and bomb place with SPACE. Press Q in game to return to menu.
 *
 *  HOW TO RUN THE PROGRAM:
 *  You can run program without parameters for playing without internet connection
 *  or using localhost ip address with port 2666.
 *  Or you can run program with 2 parameters (in this order./pancijan ipaddress port)
 *  then it will use these values for server and client connection.
 *
 */
using namespace std;

int main(int argc, char *argv[]) {
    /**
     * Parse main function arguments.
     * First is ip address of server or client to connect.
     * Second is port of server or client to connect.
     * Running program without arguments uses values 'localhost' '2666'.
     */
    string ipaddrname = "localhost";
    int port = 2666;
    char *p;
    if (argc > 2) {
        ipaddrname = argv[1];
        port = strtol(argv[2], &p, 10);
    }
    if (port == 0) port = 2666;

    /** creates application object */
    CBomberman GAME = CBomberman(ipaddrname, port);
    /** creates menu window and initialize ncurses values */
    GAME.Init();
    /** start menu drawing loop */
    GAME.Run();


    return 0;
}
