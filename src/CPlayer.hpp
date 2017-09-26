#ifndef BOMBERMAN_CPLAYER_HPP
#define BOMBERMAN_CPLAYER_HPP
#include "CMap.hpp"

#include <stack>
class CMap;
class CBomb;


/**
 * Main player class, contains all player values (position on the map, active bonuses ...)
 * Allow player to move and place bombs. Calculate player animations.
 */
class CPlayer {
public:
    /**
     * @param [in] y vertical position of player on map
     * @param [in] x horizontal position of player on map
     * @param [in] PlayerNumber number to be displayed on the player
     * @param [in] Map map where is player situated
     */
    CPlayer(int y, int x, int PlayerNumber, CMap * Map):pos_y(y),pos_x(x),PlayerNumber(PlayerNumber),Map(Map) {}

    /**
     * Destructor detonates all bombs which were placed by this player
     */
    virtual ~CPlayer();

    /**
     * Draw player on screen game_win
     * @param [in] game_win target screen to draw on
     * @param [in] Ypos vertical camera position
     * @param [in] Xpos horizontal camera position
     * @param [in] Yoff vertical animation offset
     * @param [in] Xoff horizontal animation offset
     */
    void Draw(WINDOW * game_win, int Ypos, int Xpos, int Yoff, int Xoff) const;

    /**
     * Check if bombs are going to explode and then create flame on the map.
     */
    void TickBombs();

    /**
     * Call all functions to update the player status
     */
    virtual void Process() = 0;

    /**
     * Check if player is in flame
     * @return true if player died
     */
    bool Died() const;

    /**
     * Move player to target direction. If CheckBorders is false, function do not check if move is possible.
     * @param [in] dir move direction
     * @param [in] CheckBorders
     * @return true if player changed position
     */
    bool Move (const char & dir, bool CheckBorders);
    /**
     * Place a bomb on the map
     */
    void PlaceBomb () ;


    /**
     * Recalculate player move animation offsets
     */
    void CalculateAnimOffsets();

    /**
     * Get player position to set up the camera
     * @param [out] y vertical coordinate
     * @param [out] x horizontal coordinate
     */
    void GetPosition(int * y, int * x) const {
        *y = pos_y;
        *x = pos_x;
    }

    /**
     * Get player animation offsets to draw screen in motion
     * @param [out] y vertical animation offset
     * @param [out] x horizontal horizontal animation offset
     */

    void GetAnimOffsets(int * y, int * x) const {
        *y = VOffset;
        *x = HOffset;
    }

    /**
     * @return current active bonus type
     */
    int UsingBonus() const {
        return ActiveBonus;
    }
protected:
    /**
     * Set upgrades to the player for current active bonus type
     */
    void ApplyBonuses();


    /**vertical coordinate of position on the map*/
    int pos_y;
    /**horizontal coordinate of position on the map*/
    int pos_x;
    /**number of this player to be displayed on him*/
    int PlayerNumber;
    /**pointer to game map for bomb manipulation and movement*/
    CMap * Map;
    /**range of the flame (1 is default)*/
    int FlameRange = 1;
    /**movement timer to change speed of movements*/
    int MoveTimer = 0;
    /**true if player can move, else false (default true)*/
    bool CanMove = true;
    /**horizontal offset of player draw animations*/
    int HOffset = 0;
    /**vertical offset of player draw animations*/
    int VOffset = 0;
    /**direction of the movement*/
    int MoveDirection=STOP;
    /**type of current active bonus*/
    int ActiveBonus = BONUS_NONE;
    /**delay between movements*/
    int SpeedDelay = MOVE_SPEED_DELAY;
    /**number of bombs which can player place in the same time*/
    int BombsCount = 1;
    /**pointers to bombs for exact detonations*/
    std::vector<CBomb*> bombs;
};

/**
 * Controllable player class.
 * Updates player state, call update functions.
 */
class Player_Not_AI : public CPlayer {
public:
    /**
     * Constructor of controllable player.
     * @param [in] y vertical coordinate of player position
     * @param [in] x horizontal coordinate of player position
     * @param [in] PlayerNumber number of player to be displayed
     * @param [in] Map game map pointer for movement and detonation functions
     */
    Player_Not_AI(int y, int x, int PlayerNumber, CMap * Map):CPlayer(y, x, PlayerNumber,Map) {}

    /**
     * Call functions to update player status.
     * Tick bombs, recalculate animation offsets.
     */
    void Process();
};

/**
 * AI player class.
 * Updates player state, calculate AI mechanics, call update functions.
 */
class Player_AI : public CPlayer {
public:
    /**
     * Constructor of controllable player
     * @param [in] y vertical coordinate of player position
     * @param [in] x horizontal coordinate of player position
     * @param [in] PlayerNumber number of player to be displayed
     * @param [in] Map game map pointer for movement and detonation functions
     * @param [in] AI_Mode type of AI difficulty (0 = easy, 1 = medium, 2 = hard)
     */
    Player_AI(int y, int x, int PlayerNumber, CMap * Map, int AI_Mode):CPlayer(y, x, PlayerNumber,Map),AI_Mode(AI_Mode) {}
    /**
     * Call functions to update player status.
     * calculate AI movement and placing bombs, tick bombs, recalculate animation offsets.
     */
    void Process();
    /**
     * BFS which finds track to nearest bonus and save the track to stack 'track'
     */
    void FindTrack();
private:
    /**type of AI (0 = easy, 1 = medium, 2 = hard)*/
int AI_Mode;
    /** sequence of movements which leads to nearest bonus */
    std::stack<int> track;

};



#endif //BOMBERMAN_CPLAYER_HPP
