#include "CGame.hpp"
#include "CMap.hpp"
#include "CErrorHandler.hpp"

#include "CServer.hpp"
#include "CClient.hpp"


using namespace std;

CGame::CGame(int GAMEMODE, std::string MAP, std::string ipaddrname, int port) : GAMEMODE(GAMEMODE) {
    clear();
    refresh();
    myPlayer = NULL;
    PlayersAlive = 0;
    GameMap = new CMap();
    bool EnterGameLoop = true;

    if (GAMEMODE == GM_CLASSIC_EASY || GAMEMODE == GM_CLASSIC_MEDIUM || GAMEMODE == GM_CLASSIC_HARD ||
        GAMEMODE == GM_2PLAYERS) {
        try {
            PlayersAlive = GameMap->loadMap(MAP, 1, &myPlayer, &Players, &game_win, GAMEMODE);
        } catch (ErrorMessage &msg) {
            msg.Print();
            EnterGameLoop = false;
        }
    }
    else if (GAMEMODE == GM_SERVER) {
        try {
            PlayersAlive = GameMap->loadMap(MAP, 1, &myPlayer, &Players, &game_win, GM_SERVER);
        } catch (ErrorMessage &msg) {
            msg.Print();
            EnterGameLoop = false;
        }
        if (EnterGameLoop) {
            thread t2(&CGame::OpenServer, this, ipaddrname, port);
            t2.detach();
        }
    }
    else if (GAMEMODE == GM_CLIENT) {
        try {
            PlayersAlive = GameMap->loadMap(MAP, 2, &myPlayer, &Players, &game_win, GM_CLIENT);
        } catch (ErrorMessage &msg) {
            msg.Print();
            EnterGameLoop = false;
        }
        if (EnterGameLoop) {
            thread t3(&CGame::OpenClient, this, ipaddrname, port);
            t3.detach();
        }
    }

    if (EnterGameLoop) {

        box(game_win, 0, 0);
        keypad(game_win, TRUE);
        gameLoop();
    }
}

CGame::~CGame() {

    for (auto it3 = Players.begin(); it3 != Players.end(); ++it3)
        delete *it3;
    delete myPlayer;

    delete GameMap;

}


void CGame::KeyLoop() {
    int c;

    while (!gameEnd) {
        int DIR2 = -1;
        int CODE = -1;
        c = wgetch(game_win);
        if (gameEnd) break;
        switch (c) {
            case KEY_UP :
                DIR2 = UP;
                break;
            case KEY_RIGHT :
                DIR2 = RIGHT;
                break;
            case KEY_LEFT :
                DIR2 = LEFT;
                break;
            case KEY_DOWN :
                DIR2 = DOWN;
                break;
            case '0' :
                if (GAMEMODE == GM_2PLAYERS && Players[0]) Players[0]->PlaceBomb();
                break;
            case 'w' :
                CODE = UP;
                break;
            case 'd' :
                CODE = RIGHT;
                break;
            case 'a' :
                CODE = LEFT;
                break;
            case 's' :
                CODE = DOWN;
                break;
            case ' ' :
                CODE = BOMB;
                break;
            case 'q' :
                gameEnd = true;
                PlayersAlive = -1;
                break;
        }
        if (gameEnd) break;

        if (CODE != -1) {
            if ((GAMEMODE != GM_CLIENT && GAMEMODE != GM_SERVER) ||
                ((GAMEMODE == GM_CLIENT || GAMEMODE == GM_SERVER) && opponentCliSocket != -1)) if (myPlayer) {
                if (CODE == BOMB) myPlayer->PlaceBomb();
                else if (!myPlayer->Move(CODE, true)) CODE = -1;
            }

        }
        if (CODE != -1 && opponentCliSocket != -1) {
            SendCode = CODE;
            usleep((unsigned int) 50000);
            SendToClient(opponentCliSocket);
        }

        if (GAMEMODE == GM_2PLAYERS && DIR2 != -1 && Players[0])
            Players[0]->Move(DIR2, true);

    }
}


void CGame::gameLoop() {
    wclear(game_win);
    bar_win = newwin(3, SCREEN_WIDTH * 7 + 2, SCREEN_HEIGHT * 4 + 1, 0);
    wclear(bar_win);

    thread t1(&CGame::KeyLoop, this);
    t1.detach();
    int CameraY, CameraX;
    int endingTimer = 10;
    while (true) {
        if (gameEnd)
            endingTimer--;
        if (endingTimer == 0)
            break;

        int Xoff = 0, Yoff = 0;

        if (myPlayer != NULL) {
            myPlayer->GetAnimOffsets(&Yoff, &Xoff);
            myPlayer->Process();
            GameMap->Update(GAMEMODE);
            GameMap->GetCamera(&CameraY, &CameraX);
            if (GAMEMODE == GM_2PLAYERS) {
                CameraX = SCREEN_WIDTH / 2;
                CameraY = SCREEN_HEIGHT / 2;
                myPlayer->Draw(game_win, CameraY, CameraX, 0, 0);
            } else
                myPlayer->Draw(game_win, CameraY, CameraX, -100, 0);

            if (myPlayer->Died()) {
                delete myPlayer;
                myPlayer = NULL;
                gameEnd = true;
                PlayersAlive--;
            }

        }

        for (auto it = Players.begin(); it != Players.end(); it++) {
            if (*it) {
                (*it)->Process();
                if (GAMEMODE == GM_2PLAYERS)
                    (*it)->Draw(game_win, CameraY, CameraX, 0, 0);
                else
                    (*it)->Draw(game_win, CameraY, CameraX, Yoff, Xoff);
                if ((*it)->Died()) {
                    delete (*it);
                    (*it) = NULL;
                    if (PlayersAlive <= 2) {
                        gameEnd = true;
                    }
                    PlayersAlive--;
                }
            }
        }


        box(bar_win, 0, 0);
        box(game_win, 0, 0);

        DrawBottomBar();

        usleep((unsigned int) 50000);

        wrefresh(game_win);
        wrefresh(bar_win);

    }

    DrawEndGameMsg();

    wclear(game_win);
    wclear(bar_win);

    wrefresh(game_win);
    wrefresh(bar_win);
    delwin(game_win);
    delwin(bar_win);
}

void CGame::DrawBottomBar() const {

    string textBonus;
    if (myPlayer) {
        switch (myPlayer->UsingBonus()) {
            case BONUS_NONE :
                textBonus = "No active bonus       ";
                break;
            case BONUS_SPEED :
                textBonus = "faster move speed     ";
                break;
            case BONUS_FLAME_RANGE :
                textBonus = "Flame range up        ";
                break;
            case BONUS_MORE_BOMBS :
                textBonus = "More bombs            ";
                break;
            case BONUS_MANUAL_DETONATION :
                textBonus = "Manual detonation     ";
                break;
        }
        mvwprintw(bar_win, 1, 2, string("BONUS: ").append(textBonus).c_str());
    }
    else
        mvwprintw(bar_win, 1, 2, "You have died, wait for other players to finish the game");
    mvwprintw(bar_win, 1, SCREEN_WIDTH * 7 - 20, "Press 'Q' to exit");
}

void CGame::DrawEndGameMsg() const {
    WINDOW *msg_win = newwin(7, 50, 15, 5);
    box(msg_win, 0, 0);
    mvwprintw(msg_win, 0, 0, "Game Over");
    string msg;

    if (PlayersAlive == 0) msg = "Everybody lost!";
    else {
        if (myPlayer != NULL) msg = "You have won!";
        else msg = "You have lost!";
    }
    if (PlayersAlive == -1) msg = "Ending the game...";
    if (GAMEMODE == GM_2PLAYERS) msg = "Ending the game...";
    mvwprintw(msg_win, 2, 2, msg.c_str());
    wrefresh(msg_win);
    sleep(2);
}
