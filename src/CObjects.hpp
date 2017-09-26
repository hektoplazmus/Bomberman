#ifndef BOMBERMAN_OBJECTS_HPP
#define BOMBERMAN_OBJECTS_HPP
#include "CMap.hpp"

class CMap;

/**
 * Class of objects on the map.
 * Contains draw functions and functions for interacting with player movement.
 */
class CObject {
public:
    /**
     * Creates CObject in y x position
     * @param [in] y vertical coordinate of position on the map
     * @param [in] x horizontal coordinate of position on the map
     */
    CObject(int y,int x):pos_y(y),pos_x(x) {}
    /**
     * Empty destructor to call destructor of inherited classes
     */
    virtual ~CObject() {};
    /**
     * Prints object picture on the screen
     * @param [in] game_win window to print on
     * @param [in] Ypos vertical position of the camera
     * @param [in] Xpos horizontal position of the camera
     * @param [in] Yoff vertical animation offset
     * @param [in] Xoff horizontal animation offset
     */
    virtual void Draw(WINDOW * game_win, int Ypos, int Xpos, int Yoff, int Xoff) const;

    /**
     * @return type of object like bomb, box, solid,...
     */
    virtual int ObjType() const{
        return OBJ_EMPTY;
    }
    /**
     * @return type of bonus in this object
     */
    virtual int BonusType() const {
        return BONUS_NONE;
    }
    /**
     * @return true if player can move to this place, else false
     */
    virtual bool Empty() const {
        return true;
    }

protected:
    /**vertical coordinate of position on the map*/
    int pos_y;
    /**horizontal coordinate of position on the map*/
    int pos_x;
};

/**
 * Solid object, player can not move on it.
 */
class CSolid : public CObject {
public:
    CSolid(int y, int x):CObject(y,x) {}

    virtual int ObjType() const {
        return OBJ_SOLID;
    }
    virtual int BonusType() const {
        return BONUS_NONE;
    }
    void Draw(WINDOW * game_win, int Ypos, int Xpos, int Yoff, int Xoff) const;
    virtual bool Empty() const {
        return false;
    }
private:

};
/**
 * Box object which can be destroyed by bombs.
 */
class CBox : public CObject {
public:
    CBox(int y, int x, int bonus):CObject(y,x),BONUS(bonus) {}
    virtual int ObjType() const{
        return OBJ_BOX;
    }
    virtual int BonusType() const {
        return BONUS;
    }
    void Draw(WINDOW * game_win, int Ypos, int Xpos, int Yoff, int Xoff) const;
    virtual bool Empty() const {
        return false;
    }

private:
    /**type of bonus in the box*/
    int BONUS;
};

/**
 * Flame object which can kill players.
 */
class CFlame : public CObject {
public:
    CFlame(int y, int x, int bonus):CObject(y,x),BONUS(bonus) {}

    virtual int ObjType() const{
        return OBJ_FLAME;
    }
    virtual int BonusType() const {
        return BONUS;
    }
    /**
     * Tick the flame timer
     * @return true if flame should be destroyed, else false
     */
    int FlameTick();
    void Draw(WINDOW * game_win, int Ypos, int Xpos, int Yoff, int Xoff) const;
    virtual bool Empty() const {
        return true;
    }

private:
    /**type of bonus to create after the flame disapears*/
    int BONUS;
    /**time to destroy the flame*/
    int tickToDestroy=FLAME_TIMETODESTROY;


};
/**
 * Bomb object which explodes after time.
 */
class CBomb : public CObject{
public:
    CBomb(int y, int x):CObject(y, x) {}

    virtual int ObjType() const{
        return OBJ_BOMB;
    }
    virtual int BonusType() const {
        return BONUS_NONE;
    }
    /**
     * Detonate the bomb and create flame on the map
     * @param [in] Map game map to call CreateFlame function
     * @param [in] FlameRange range of the explosion
     */
    void Detonate( CMap * Map,int FlameRange) const;
    /**
     * Tick the bomb timer
     * @return true if bomb is going to explode, else false
     */
    bool tick()  {
        if (tickToDetonate) {
            tickToDetonate--;
            return false;
        }
        return true;
    }
    /**
     * Reduce the bomb timer to speed up the detonation
     */
    void ForceDetonation() {
            tickToDetonate=0;
    };

    void Draw(WINDOW * game_win, int Ypos, int Xpos, int Yoff, int Xoff) const;
    virtual bool Empty() const {
        return false;
    }


private:
    /**time to detonate the bomb*/
    int tickToDetonate = BOMB_TIMETOEXPLODE;
};
/**
 * Bonus object which can be picked up by a player.
 */
class CBonus : public CObject {
public:
    CBonus(int y, int x, int bonus):CObject(y,x),BONUS(bonus) {}

    virtual int ObjType() const{
        return OBJ_BONUS;
    }
    virtual int BonusType() const {
        return BONUS;
    }

    void Draw(WINDOW * game_win, int Ypos, int Xpos, int Yoff, int Xoff) const;
    virtual bool Empty() const {
        return true;
    }

private:
    /**type of the bonus*/
    int BONUS;

};


#endif //BOMBERMAN_OBJECTS_HPP
