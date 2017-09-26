#ifndef BOMBERMAN_CMAP_HPP
#define BOMBERMAN_CMAP_HPP

#include "ncurses.h"
#include "Constants.hpp"
#include <vector>
#include <fstream>
#include "CObjects.hpp"
#include "CPlayer.hpp"


class CPlayer;

class CObject;

class CBomb;

/**
 * Class containing whole map of game with CObjects.
 * Manipulates with map (deletes boxes, creates flames ...)
 */
class CMap {
public:

    /**
     * Allows to access an item in the map.
     * @param [in] y vertical coordinate
     * @param [in] x horizontal coordinate
     * @return reference to CObject in positions [y][x]
     */
    CObject &GetObj(int y, int x) const;

    /**
     * Check if the [y][x] position is in bomb range
     * @return true if the position is in bomb range, else return false
     */
    bool InBombRange(int y, int x) const;

    /**
     * Deletes all CObjects in map matrix
     */
    ~CMap();


    /**
     * Parse map file and creates matrix of objects according to the file
     * Also creates game window and initialize all players
     * @param [in] mapName name of map file to load
     * @param [in] playerCnt position of myPlayer in the map
     * @param [out] myPlayer pointer to controllable player
     * @param [out] Players vector of all other players
     * @param [out] game_win window of the main game
     * @param [in] GAMEMODE mode of game to laod map correctly
     * @return number of players loaded
     */
    int loadMap(const std::string &mapName, int playerCnt, CPlayer **myPlayer, std::vector<CPlayer *> *Players,
                WINDOW **game_win, int GAMEMODE);

    /**
     * Creates flame in specific position
     * @param [in] y vertical coordinate
     * @param [in] x horizontal coordinate
     * @param [in] FlameRange range of the flame (1 is default)
     */
    void CreateFlame(int y, int x, int FlameRange);

    /**
     * Draw all objects in their draw positions and tick all flames
     * @param [in] GameMode mode of the game to draw map correctly in 2PLAYERS mode
     */
    void Update(int GameMode);

    /**
     * Deletes CBonus object and replace it with empty CObject
     * @return type of the deleted bonus
     */
    int PickUpBonus(int y, int x);

    /**
     * Place bomb in specific position if it is possible
     * @param [in] y vertical coordinate
     * @param [in] x horizontal coordinate
     * @return pointer to the placed bomb or NULL if no bomb was placed
     */
    CBomb *PlaceBomb(int y, int x);

    /**
     * Returns the position of Camera
     * @param [out] y vertical coordinate
     * @param [out] x horizontal coordinate
     */
    void GetCamera(int *y, int *x) const;


private:
    /**Width and Height of the map*/
    int W = 0, H = 0;
    /**Pointer to the main game window*/
    WINDOW *game_win;
    /**Matrix of CObjects creating whole game map*/
    std::vector<std::vector<CObject *> > Map;
    /**Vertical coordinate of Camera center*/
    int CameraY = SCREEN_Y_CENTER;
    /**Horizontal coordinate of Camera center*/
    int CameraX = SCREEN_X_CENTER;
    /** pointer to main player to get camera position */
    CPlayer *MyPlayer;


};

#endif //BOMBERMAN_CMAP_HPP
