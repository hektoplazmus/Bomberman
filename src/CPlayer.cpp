#include "CPlayer.hpp"

#include <set>
#include <map>
#include <queue>
using namespace std;

void CPlayer::Draw(WINDOW *game_win, int Ypos, int Xpos, int Yoff, int Xoff) const{
    int draw_x = SCREEN_X_CENTER + pos_x - Xpos;
    int draw_y = SCREEN_Y_CENTER + pos_y - Ypos;

    if (Yoff == -100) {
        Yoff = 0;
        Xoff = 0;
    } else {
        Xoff -= HOffset;
        Yoff -= VOffset;

    }

    if (draw_x >= 0 && draw_y >= 0 && draw_x < SCREEN_WIDTH && draw_y < SCREEN_HEIGHT) {
        mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "  ___ ");
        mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, " (o_o) ");
        mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff,
                  string(" -|").append(string(to_string(PlayerNumber))).append("|- ").c_str());
        mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "  / \\ ");
    }

}

CPlayer::~CPlayer() {
    for (auto it = bombs.begin(); it != bombs.end(); it++) {
        (*it)->Detonate(Map, FlameRange);
        it = bombs.erase(it);
        if (it == bombs.end()) break;
    }
}

void CPlayer::TickBombs() {
    for (auto it = bombs.begin(); it != bombs.end(); it++) {
        if ((*it)->tick()) {
            (*it)->Detonate(Map, FlameRange);
            it = bombs.erase(it);
        }
        if (it == bombs.end()) break;
    }
}

bool CPlayer::Died() const {
    return Map->GetObj(pos_y, pos_x).ObjType() == OBJ_FLAME;
}


bool CPlayer::Move(const char &dir, bool CheckBorders) {

    if (!CheckBorders || CanMove) {
        MoveDirection = STOP;
        HOffset = 0;
        VOffset = 0;
        switch (dir) {
            case RIGHT :
                if (Map->GetObj(pos_y, pos_x + 1).Empty()) {
                    //MoveToPos(RIGHT);
                    pos_x++;
                    MoveDirection = RIGHT;
                    HOffset = -7;
                }
                break;
            case LEFT :
                if (Map->GetObj(pos_y, pos_x - 1).Empty()) {
                    //MoveToPos(LEFT);
                    pos_x--;
                    MoveDirection = LEFT;
                    HOffset = 7;
                }
                break;
            case UP :
                if (Map->GetObj(pos_y - 1, pos_x).Empty()) {
                    //MoveToPos(UP);
                    pos_y--;
                    MoveDirection = UP;
                    VOffset = 4;
                }
                break;
            case DOWN :
                if (Map->GetObj(pos_y + 1, pos_x).Empty()) {
                    // MoveToPos(DOWN);
                    pos_y++;
                    MoveDirection = DOWN;
                    VOffset = -4;
                }
                break;
        }
        CanMove = false;
        if (MoveDirection == STOP)
            return false;
        return true;
    }
    return false;

}

void CPlayer::CalculateAnimOffsets() {
    if (MoveDirection == RIGHT && HOffset != 0)
        HOffset++;
    else if (MoveDirection == LEFT && HOffset != 0)
        HOffset--;
    else if (MoveDirection == UP && VOffset != 0)
        VOffset--;
    else if (MoveDirection == DOWN && VOffset != 0)
        VOffset++;
}

void CPlayer::PlaceBomb() {
    if (ActiveBonus == BONUS_MANUAL_DETONATION && !bombs.empty()) {
        bombs[0]->Detonate(Map, FlameRange);
        bombs.clear();
    }
    else if ((int) bombs.size() < BombsCount) {
        CBomb *tmp;
        tmp = Map->PlaceBomb(pos_y, pos_x);
        if (tmp != NULL)
            bombs.push_back(tmp);
    }
}


void CPlayer::ApplyBonuses() {
    if (Map->GetObj(pos_y, pos_x).ObjType() == OBJ_BONUS) {
        ActiveBonus = Map->PickUpBonus(pos_y, pos_x);
    }

    SpeedDelay = MOVE_SPEED_DELAY;
    if (ActiveBonus == BONUS_SPEED)
        SpeedDelay = MOVE_SPEED_DELAY_BONUS;
    FlameRange = 1;
    if (ActiveBonus == BONUS_FLAME_RANGE)
        FlameRange = 2;
    BombsCount = 1;
    if (ActiveBonus == BONUS_MORE_BOMBS)
        BombsCount = BONUS_BOMBS_COUNT;

}

void Player_Not_AI::Process() {
    ApplyBonuses();
    CalculateAnimOffsets();
    if (ActiveBonus != BONUS_MANUAL_DETONATION)
        TickBombs();

    if (MoveTimer > SpeedDelay) {
        CanMove = true;
        MoveTimer = 0;
    }
    MoveTimer++;


}

void Player_AI::FindTrack() {
        std::stack<int> empty;
        std::swap( track, empty );
    queue<pair<int,int>> q;
    set<pair<int,int>> visited;
    q.push(make_pair(pos_y, pos_x));
    map<pair<int,int>,int> prev;
    bool found=false;
    prev[make_pair(pos_y, pos_x)]=STOP;
    pair<int,int>  cur;
    while(!q.empty()) {

        cur = q.front();
        q.pop();
        if (Map->GetObj(cur.first, cur.second).ObjType()==OBJ_BONUS) {
            found = true;
            break;
        }

        if (Map->GetObj(cur.first-1,cur.second).Empty() && visited.count(make_pair(cur.first-1,cur.second)) ==0) {
            q.push(make_pair(cur.first - 1, cur.second));
            prev[make_pair(cur.first - 1, cur.second)]=UP;
        }
        if (Map->GetObj(cur.first+1,cur.second).Empty() && visited.count(make_pair(cur.first+1,cur.second)) ==0) {
            q.push(make_pair(cur.first + 1, cur.second));
            prev[make_pair(cur.first +1, cur.second)]=DOWN;
        }
        if (Map->GetObj(cur.first,cur.second-1).Empty() && visited.count(make_pair(cur.first,cur.second-1)) ==0) {
            q.push(make_pair(cur.first, cur.second - 1));
            prev[ make_pair(cur.first, cur.second-1)] = LEFT;
        }
        if (Map->GetObj(cur.first,cur.second+1).Empty() && visited.count(make_pair(cur.first,cur.second+1)) ==0) {
            q.push(make_pair(cur.first, cur.second+1));
            prev[make_pair(cur.first , cur.second+1)] = RIGHT;
        }

        visited.insert(cur);
    }
    if (found) {
        int val = prev[make_pair(cur.first, cur.second)];
        while (val!=STOP) {
            track.push(val);
            switch(val) {
                case UP : {val = prev[make_pair(cur.first+1, cur.second)]; cur = make_pair(cur.first+1,cur.second); break;}
                case DOWN : {val = prev[make_pair(cur.first-1, cur.second)];  cur = make_pair(cur.first-1,cur.second); break;}
                case LEFT : {val = prev[make_pair(cur.first, cur.second+1)];  cur = make_pair(cur.first,cur.second+1); break;};
                case RIGHT : {val = prev[make_pair(cur.first, cur.second-1)];  cur = make_pair(cur.first,cur.second-1); break;}
            }
        }

    } else {
        std::stack<int> empty;
        std::swap( track, empty );
    }

}

void Player_AI::Process() {
    ApplyBonuses();
    CalculateAnimOffsets();
    if (ActiveBonus != BONUS_MANUAL_DETONATION)
        TickBombs();
    if (MoveTimer == MOVE_SPEED_DELAY) {
        CanMove = true;
        MoveTimer = 0;
    }
    MoveTimer++;


    if (CanMove) {
        if (AI_Mode == 0) {

            if (random() % 30 == 0)
                PlaceBomb();
            Move(random() % 4 + 1, true);

        } else if (AI_Mode == 1) {
            if (Map->InBombRange(pos_y, pos_x)) {
                if (Map->GetObj(pos_y - 1, pos_x).Empty() && Map->GetObj(pos_y - 1, pos_x).ObjType() != OBJ_FLAME)
                    Move(UP, true);
                else if (Map->GetObj(pos_y + 1, pos_x).Empty() && Map->GetObj(pos_y + 1, pos_x).ObjType() != OBJ_FLAME)
                    Move(DOWN, true);
                else if (Map->GetObj(pos_y, pos_x - 1).Empty() && Map->GetObj(pos_y, pos_x - 1).ObjType() != OBJ_FLAME)
                    Move(LEFT, true);
                else if (Map->GetObj(pos_y, pos_x + 1).Empty() && Map->GetObj(pos_y, pos_x + 1).ObjType() != OBJ_FLAME)
                    Move(RIGHT, true);
            } else if (random()%5 == 0){
                if (Map->GetObj(pos_y - 1, pos_x).Empty() && !Map->InBombRange(pos_y-1, pos_x) && Map->GetObj(pos_y - 1, pos_x).ObjType() != OBJ_FLAME)
                    Move(UP, true);
                else if (Map->GetObj(pos_y + 1, pos_x).Empty() && !Map->InBombRange(pos_y+1, pos_x)&& Map->GetObj(pos_y + 1, pos_x).ObjType() != OBJ_FLAME)
                    Move(DOWN, true);
                else if (Map->GetObj(pos_y, pos_x - 1).Empty() && !Map->InBombRange(pos_y, pos_x-1) && Map->GetObj(pos_y, pos_x - 1).ObjType() != OBJ_FLAME)
                    Move(LEFT, true);
                else if (Map->GetObj(pos_y, pos_x + 1).Empty() && !Map->InBombRange(pos_y, pos_x+1) && Map->GetObj(pos_y, pos_x + 1).ObjType() != OBJ_FLAME)
                    Move(RIGHT, true);
            }
            if (random() % 25 == 0)
                PlaceBomb();

        }
        else if (AI_Mode == 2) {
            if (track.empty() && random()%HARD_AI_BFS_CHANCE==0) {
                FindTrack();
            }
            if (Map->InBombRange(pos_y, pos_x)) {


                bool moved = false;
                for (int i = 0; i < 4; i++) {
                    int dir = random() % 4 + 1;
                    switch (dir) {
                        case UP :
                            if (Map->GetObj(pos_y - 1, pos_x).Empty() &&
                                Map->GetObj(pos_y - 1, pos_x).ObjType() != OBJ_FLAME)
                                Move(UP, true);
                            moved = true;
                            break;
                        case DOWN :
                            if (Map->GetObj(pos_y + 1, pos_x).Empty() &&
                                Map->GetObj(pos_y + 1, pos_x).ObjType() != OBJ_FLAME)
                                Move(DOWN, true);
                            moved = true;
                            break;
                        case RIGHT :
                            if (Map->GetObj(pos_y, pos_x + 1).Empty() &&
                                Map->GetObj(pos_y, pos_x + 1).ObjType() != OBJ_FLAME)
                                Move(RIGHT, true);
                            moved = true;
                            break;
                        case LEFT :
                            if (Map->GetObj(pos_y, pos_x - 1).Empty() &&
                                Map->GetObj(pos_y, pos_x - 1).ObjType() != OBJ_FLAME)
                                Move(LEFT, true);
                            moved = true;
                            break;
                    }
                    if (moved)break;
                }
            } else if (!track.empty()) {
             int dir = track.top();
                bool trackMove=false;
                switch(dir) {
                    case UP : if (Map->GetObj(pos_y-1, pos_x).ObjType()!=OBJ_FLAME) {
                            Move(dir, true);
                            trackMove = true;
                        }
                            break;
                    case DOWN : if (Map->GetObj(pos_y+1, pos_x).ObjType()!=OBJ_FLAME) {
                            Move(dir, true);
                        trackMove=true;
                        }
                        break;
                    case LEFT : if (Map->GetObj(pos_y, pos_x-1).ObjType()!=OBJ_FLAME) {
                            Move(dir, true);
                        trackMove=true;
                        }
                        break;
                    case RIGHT : if (Map->GetObj(pos_y, pos_x+1).ObjType()!=OBJ_FLAME) {
                            Move(dir, true);
                        trackMove=true;
                        }
                        break;
                }
             track.pop();
                if (!trackMove && random()%HARD_AI_BFS_CHANCE==0)
                    FindTrack();


            }else if (random() % 4 == 0) {
                bool moved = false;
                for (int i = 0; i < 4; i++) {
                    int dir = random() % 4 + 1;
                    switch (dir) {
                        case UP :
                            if (Map->GetObj(pos_y - 1, pos_x).Empty() && !Map->InBombRange(pos_y - 1, pos_x) &&
                                Map->GetObj(pos_y - 1, pos_x).ObjType() != OBJ_FLAME)
                                Move(UP, true);
                            moved = true;
                            break;
                        case DOWN :
                            if (Map->GetObj(pos_y + 1, pos_x).Empty() && !Map->InBombRange(pos_y + 1, pos_x) &&
                                Map->GetObj(pos_y + 1, pos_x).ObjType() != OBJ_FLAME)
                                Move(DOWN, true);
                            moved = true;
                            break;
                        case RIGHT :
                            if (Map->GetObj(pos_y, pos_x + 1).Empty() && !Map->InBombRange(pos_y, pos_x + 1) &&
                                Map->GetObj(pos_y, pos_x + 1).ObjType() != OBJ_FLAME)
                                Move(RIGHT, true);
                            moved = true;
                            break;
                        case LEFT :
                            if (Map->GetObj(pos_y, pos_x - 1).Empty() && !Map->InBombRange(pos_y, pos_x - 1) &&
                                Map->GetObj(pos_y, pos_x - 1).ObjType() != OBJ_FLAME)
                                Move(LEFT, true);
                            moved = true;
                            break;
                    }
                    if (moved)break;
                }
            }

            if (random() % 25 == 0)
                PlaceBomb();


        }
    }

}

