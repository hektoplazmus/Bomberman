#ifndef SRC_CONSTANTS_HPP
#define SRC_CONSTANTS_HPP

/** player not moving code */
const int STOP = 0;
/** player move right code */
const int RIGHT = 1;
/** player move up code */
const int UP = 2;
/** player move left code */
const int LEFT = 3;
/** player move down code */
const int DOWN = 4;
/** player place bomb code */
const int BOMB = 5;

/** classic game mode with AI opponent with difficulty easy */
const int GM_CLASSIC_EASY = 0;
/** classic game mode with AI opponent with difficulty medium */
const int GM_CLASSIC_MEDIUM = 1;
/** classic game mode with AI opponent with difficulty hard */
const int GM_CLASSIC_HARD = 2;
/** server game mode */
const int GM_SERVER = 3;
/** client game mode */
const int GM_CLIENT = 4;
/** 2 players on the same computer game mode */
const int GM_2PLAYERS = 5;

/** empty object type */
const int OBJ_EMPTY = 0;
/** solid object type */
const int OBJ_SOLID = 1;
/** bos object type */
const int OBJ_BOX = 2;
/** flame object type */
const int OBJ_FLAME = 3;
/** bomb object type */
const int OBJ_BOMB = 4;
/** bonus box object type */
const int OBJ_BONUS = 5;

/** time between player moves, speed of a player without speed bonus */
const int MOVE_SPEED_DELAY = 8;
/** decreased time between player moves, speed of aplayer with speed bonus */
const int MOVE_SPEED_DELAY_BONUS = 5;
/** time until the bomb explode*/
const int BOMB_TIMETOEXPLODE = 20;
/** time until the flame disappears */
const int FLAME_TIMETODESTROY = 20;

/** width of game screen, number of object to draw in horizontal direction*/
const int SCREEN_WIDTH = 18;
/** height of game screen, number of object to draw in vertical direction */
const int SCREEN_HEIGHT = 8;
/** center of the screen, default cameraX position */
const int SCREEN_X_CENTER = SCREEN_WIDTH / 2;
/** center of the screen, default cameraY position */
const int SCREEN_Y_CENTER = SCREEN_HEIGHT / 2;

/** default no bonus type */
const int BONUS_NONE = 0;
/** movement speed bonus type */
const int BONUS_SPEED = 1;
/** big flame range bonus type */
const int BONUS_FLAME_RANGE = 2;
/** more bombs bonus type */
const int BONUS_MORE_BOMBS = 3;
/** manual detonation bonus type */
const int BONUS_MANUAL_DETONATION = 4;
/** number of bombs which can be placed with bonus more bombs */
const int BONUS_BOMBS_COUNT = 3;

/** chance of hard AI to find track to a bonus. 1 is 100%. */
const int HARD_AI_BFS_CHANCE = 35;
#endif //SRC_CONSTANTS_HPP
