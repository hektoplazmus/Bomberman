#include <thread>
#include "ncurses.h"
#include <vector>


class CPlayer;

class CMap;

/**
 * Main game class.
 * Initialize game values, creates thread for key detection function and for server/client communications.
 * Starts game loop and calls update functions.
 */
class CGame {
public:
    /**
     * Constructor to initialize the game
     * @param [in] GAMEMODE type of game (easy_AI, 2Players, server, client ...)
     * @param [in] MAP name of map file to be load
     * @param [in] ipaddrname ip address of server or client for connection
     * @param [in] port port of server or client for connection
     * ipaddrname and port use default values in games without internet communication
     */
    CGame(int GAMEMODE, std::string MAP, std::string ipaddrname, int port);


    /**
     * Destructor deletes players and GameMap
     */
    ~CGame();

private:

    /**
    * Main game loop.
    * Call draw functions, process player movements, AI decisions...
    * Checks for game end (main player die)
    */
    void gameLoop();

    /**
     * Thread which checks key presses and manipulate with players.
     * In game mode server or client, sends information about other players movements.
     */
    void KeyLoop();

    /**
     * Prints bottom bar with information about active bonuses of main player.
     */
    void DrawBottomBar() const;

    /**
     * Prints end game window with information who won.
     */
    void DrawEndGameMsg() const;

    /**
     * Open server socket
     * @param [in] srvName ip address of server
     * @param [in] srvPort port of the server
     * @return number of server socket or -1 if an error occurred
     */
    int openSrvSocket(const char *srvName, int srvPort);

    /**
     * serve the client and manipulates with opponent player.
     * @param [in] buffer array of received bytes to decode
     */
    void serveClient(char *buffer);

    /*
     * Thread that opens server and listens for clients.
     * @param [in] ipaddrname ip address of server
     * @param [in] port port of server
     * @return -1 if server was not opened, else 0
     */
    int OpenServer(std::string ipaddrname, int port);

    /**
     * Opens client socket.
     * @param [in] srvName ip address of server
     * @param [in] srvPort port of the server
     * @return number of client socket or -1 if an error occurred
     */
    int openCliSocket(const char *srvName, int srvPort);

    /**
     * Receives data from server and manipulates with an opponent player.
     * Sends data to server if main player changed position or placed a bomb.
     * @param [in] s socket to communicate with
     */
    void communicate(int s);

    /*
     * Thread that opens server and listens for clients.
     * @param [in] ipaddrname ip address of server
     * @param [in] port port of server
     * @return -1 if client was not opened, else 0
     */
    int OpenClient(std::string ipaddrname, int port);

    /**
     * Sends data to opponent.
     * @param [in] s number of the socket
     */
    void SendToClient(int s);

    /** mode of the game (2PLAYERS, SERVER, EASY_AI ...)*/
    int GAMEMODE;
    /** game window for drawing objects */
    WINDOW *game_win;
    /** bottom bar window to draw bonus information */
    WINDOW *bar_win;
    /** pointer to main controllable player */
    CPlayer *myPlayer;
    /** all other players including AI and NotAI players */
    std::vector<CPlayer *> Players;
    /** pointer to game map for updating game window */
    CMap *GameMap;
    /** true if game is going to end (main player died) */
    bool gameEnd = false;
    /** code for server client communication, code indicates movements and bomb placing */
    char SendCode = 0;
    /** number of opponent socket for server client communication */
    int opponentCliSocket = -1;
    /** number of players alive to indicate game end*/
    int PlayersAlive;


};