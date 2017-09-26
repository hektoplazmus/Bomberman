#include "CMap.hpp"
#include "CErrorHandler.hpp"


using namespace std;

CMap::~CMap() {
    for (auto it = Map.begin(); it != Map.end(); ++it) {
        for (auto it2 = (*it).begin(); it2 != (*it).end(); ++it2)
            delete *it2;
    }
}

void CMap::GetCamera(int *y, int *x) const {
    *y = CameraY;
    *x = CameraX;
}

CObject &CMap::GetObj(int y, int x) const {
    return *(Map[y][x]);
}

int CMap::loadMap(const string &mapName, int playerCnt, CPlayer **myPlayer, vector<CPlayer *> *Players, WINDOW **win,
                  int GAMEMODE) {

    string tmpName = string("./examples/").append(mapName);
    ifstream mapFile(tmpName.c_str(), ios::in);
    if (mapFile.fail()) { throw ErrorMessage("Could not open map file."); }

    vector<CObject *> row;
    int NumOfPlayers = 0;
    int rowNum = 0;
    string line;

    while (getline(mapFile, line)) {
        if (line == "" || line[0] == '#') continue;
        row.clear();
        int i = 0;
        for (char ch : line) {
            switch (ch) {
                case 'X' :
                    row.push_back(new CSolid(rowNum, i));
                    break;
                case 'O' :
                    row.push_back(new CBox(rowNum, i, BONUS_NONE));
                    break;
                case 'S' :
                    row.push_back(new CBox(rowNum, i, BONUS_SPEED));
                    break;
                case 'F' :
                    row.push_back(new CBox(rowNum, i, BONUS_FLAME_RANGE));
                    break;
                case 'B' :
                    row.push_back(new CBox(rowNum, i, BONUS_MORE_BOMBS));
                    break;
                case 'M' :
                    row.push_back(new CBox(rowNum, i, BONUS_MANUAL_DETONATION));
                    break;
                case ' ' :
                    row.push_back(new CObject(rowNum, i));
                    break;
                case 'P' : {
                    if (NumOfPlayers == 10) {
                        for (auto it = row.begin(); it != row.end(); it++)
                            delete *it;
                        throw ErrorMessage("Too Many Players on the map. Max is 10.");
                    }

                    if ((GAMEMODE == GM_SERVER || GAMEMODE == GM_CLIENT) && NumOfPlayers == 2) {
                        row.push_back(new CObject(rowNum, i));
                        break;
                    } else NumOfPlayers++;
                    if (NumOfPlayers == playerCnt)
                        *myPlayer = new Player_Not_AI(rowNum, i, NumOfPlayers, this);
                    else {
                        if (GAMEMODE == GM_CLASSIC_EASY)
                            (*Players).push_back(new Player_AI(rowNum, i, NumOfPlayers, this, 0));
                        else if (GAMEMODE == GM_CLASSIC_MEDIUM)
                            (*Players).push_back(new Player_AI(rowNum, i, NumOfPlayers, this, 1));
                        else if (GAMEMODE == GM_CLASSIC_HARD)
                            (*Players).push_back(new Player_AI(rowNum, i, NumOfPlayers, this, 2));
                        else if (GAMEMODE == GM_2PLAYERS && !(*Players).empty())
                            (*Players).push_back(new Player_AI(rowNum, i, NumOfPlayers, this, 1));
                        else
                            (*Players).push_back(new Player_Not_AI(rowNum, i, NumOfPlayers, this));
                    }
                    row.push_back(new CObject(rowNum, i));
                    break;
                }
                default : {
                    for (auto it = row.begin(); it != row.end(); it++)
                        delete *it;
                    throw ErrorMessage(string("Wrong map format: Unknown character ").append(string(&ch)));

                }
            }
            i++;
        }
        if (rowNum == 0) W = i;
        else if (W != i) {
            H = rowNum;
            Map.push_back(row);
            throw ErrorMessage("Wrong map format: Map is not a rectangle.");
        };
        rowNum++;
        Map.push_back(row);

    }
    if (rowNum == 0) throw ErrorMessage("Wrong map format: Map is empty.");

    if (NumOfPlayers < 2) throw ErrorMessage("Not enough players on this map. Min is 2.");
    if (H > 10 || W > 19) throw ErrorMessage("Map is too big.");
    H = rowNum;

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {
            if (i == 0 || i == H - 1 || j == 0 || j == W - 1) if (Map[i][j]->ObjType() != OBJ_SOLID)
                throw ErrorMessage("Wrong map format: Map is not bordered properly.");
        }
    if (GAMEMODE == GM_2PLAYERS && (H >= SCREEN_HEIGHT || W >= SCREEN_WIDTH))
        throw ErrorMessage("Map is too big for this Game Mode.");
    //*win = newwin(4 * H + 2, 7 * W + 2, 0, 0);
    *win = newwin(SCREEN_HEIGHT * 4 + 2, SCREEN_WIDTH * 7 + 2, 0, 0);
    //wrefresh(*win);
    game_win = *win;
    MyPlayer = *myPlayer;
    return NumOfPlayers;
}

void CMap::Update(int GameMode) {


    int Xoff = 0, Yoff = 0;

    (MyPlayer)->GetPosition(&CameraY, &CameraX);
    (MyPlayer)->GetAnimOffsets(&Yoff, &Xoff);


    if (GameMode == GM_2PLAYERS) {
        CameraX = SCREEN_WIDTH / 2;
        CameraY = SCREEN_HEIGHT / 2;
        Yoff = 0;
        Xoff = 0;
    }
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++) {

            Map[i][j]->Draw(game_win, CameraY, CameraX, Yoff, Xoff);
            if (Map[i][j]->ObjType() == 3) if ((dynamic_cast<CFlame *>(Map[i][j]))->FlameTick()) {
                CObject *tmp = Map[i][j];
                int bonus = Map[i][j]->BonusType();
                if (bonus == BONUS_NONE)
                    Map[i][j] = new CObject(i, j);
                else Map[i][j] = new CBonus(i, j, bonus);
                delete tmp;
            }

        }
}

void CMap::CreateFlame(int y, int x, int FlameRange) {

    delete Map[y][x];
    Map[y][x] = new CFlame(y, x, BONUS_NONE);
    for (int i = y; i <= y + FlameRange; i++) {

        if (i >= H) break;
        if (Map[i][x]->ObjType() == OBJ_EMPTY || Map[i][x]->ObjType() == OBJ_BONUS) {
            delete Map[i][x];
            Map[i][x] = new CFlame(i, x, BONUS_NONE);
            continue;
        }
        if (Map[i][x]->ObjType() == OBJ_BOX) {
            CObject *tmp = Map[i][x];
            int bonus = Map[i][x]->BonusType();
            Map[i][x] = new CFlame(i, x, bonus);
            delete tmp;
            break;
        }
        if (Map[i][x]->ObjType() == OBJ_BOMB) {
            dynamic_cast<CBomb *>(Map[i][x])->ForceDetonation();
            break;
        }

    }

    for (int i = y; i >= y - FlameRange; i--) {
        if (i == 0) break;
        if (Map[i][x]->ObjType() == OBJ_EMPTY || Map[i][x]->ObjType() == OBJ_BONUS) {
            delete Map[i][x];
            Map[i][x] = new CFlame(i, x, BONUS_NONE);
            continue;
        }
        if (Map[i][x]->ObjType() == OBJ_BOX) {
            CObject *tmp = Map[i][x];
            int bonus = Map[i][x]->BonusType();
            Map[i][x] = new CFlame(i, x, bonus);
            delete tmp;
            break;
        }
        if (Map[i][x]->ObjType() == OBJ_BOMB) {
            dynamic_cast<CBomb *>(Map[i][x])->ForceDetonation();
            break;
        }
    }
    for (int i = x; i <= x + FlameRange; i++) {

        if (i >= W) break;
        if (Map[y][i]->ObjType() == OBJ_EMPTY || Map[y][i]->ObjType() == OBJ_BONUS) {
            delete Map[y][i];
            Map[y][i] = new CFlame(y, i, BONUS_NONE);
            continue;
        }
        if (Map[y][i]->ObjType() == OBJ_BOX) {
            CObject *tmp = Map[y][i];

            int bonus = Map[y][i]->BonusType();
            Map[y][i] = new CFlame(y, i, bonus);
            delete tmp;
            break;
        }
        if (Map[y][i]->ObjType() == OBJ_BOMB) {
            dynamic_cast<CBomb *>(Map[y][i])->ForceDetonation();
            break;
        }
    }

    for (int i = x; i >= x - FlameRange; i--) {
        if (i == 0) break;
        if (Map[y][i]->ObjType() == OBJ_EMPTY || Map[y][i]->ObjType() == OBJ_BONUS) {
            delete Map[y][i];
            Map[y][i] = new CFlame(y, i, BONUS_NONE);
            continue;
        }
        if (Map[y][i]->ObjType() == OBJ_BOX) {
            CObject *tmp = Map[y][i];
            int bonus = Map[y][i]->BonusType();
            Map[y][i] = new CFlame(y, i, bonus);
            delete tmp;
            break;
        }
        if (Map[y][i]->ObjType() == OBJ_BOMB) {
            dynamic_cast<CBomb *>(Map[y][i])->ForceDetonation();
            break;
        }
    }

}

CBomb *CMap::PlaceBomb(int y, int x) {
    if (((Map[y][x]))->Empty()) {
        delete Map[y][x];
        Map[y][x] = new CBomb(y, x);
        return dynamic_cast<CBomb *>(Map[y][x]);
    }
    return NULL;
}

bool CMap::InBombRange(int y, int x) const {
    if (Map[y][x]->ObjType() == OBJ_BOMB ||
        Map[y - 1][x]->ObjType() == OBJ_BOMB ||
        Map[y + 1][x]->ObjType() == OBJ_BOMB ||
        Map[y][x - 1]->ObjType() == OBJ_BOMB ||
        Map[y][x + 1]->ObjType() == OBJ_BOMB)
        return true;
    return false;
}

int CMap::PickUpBonus(int y, int x) {
    int bonus = Map[y][x]->BonusType();
    delete Map[y][x];
    Map[y][x] = new CObject(y, x);
    return bonus;

}