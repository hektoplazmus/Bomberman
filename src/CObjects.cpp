#include "CObjects.hpp"


using namespace std;

void CObject::Draw(WINDOW *game_win, int Ypos, int Xpos, int Yoff, int Xoff) const {
    int draw_x = SCREEN_X_CENTER + pos_x - Xpos;
    int draw_y = SCREEN_Y_CENTER + pos_y - Ypos;
    if (draw_x >= 0 && draw_y >= 0 && draw_x < SCREEN_WIDTH && draw_y < SCREEN_HEIGHT) {
        mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
        mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
        mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
        mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
    }
}

void CSolid::Draw(WINDOW *game_win, int Ypos, int Xpos, int Yoff, int Xoff) const {
    int draw_x = SCREEN_X_CENTER + pos_x - Xpos;
    int draw_y = SCREEN_Y_CENTER + pos_y - Ypos;
    if (draw_x >= 0 && draw_y >= 0 && draw_x < SCREEN_WIDTH && draw_y < SCREEN_HEIGHT) {
        mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "///////");
        mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "///////");
        mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "///////");
        mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "///////");
    }
}

void CBox::Draw(WINDOW *game_win, int Ypos, int Xpos, int Yoff, int Xoff) const {
    int draw_x = SCREEN_X_CENTER + pos_x - Xpos;
    int draw_y = SCREEN_Y_CENTER + pos_y - Ypos;
    if (draw_x >= 0 && draw_y >= 0 && draw_x < SCREEN_WIDTH && draw_y < SCREEN_HEIGHT) {
        mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, " _____ ");
        mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "|     |");
        mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "| /// |");
        mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "|_____|");
    }
}

void CFlame::Draw(WINDOW *game_win, int Ypos, int Xpos, int Yoff, int Xoff) const {
    int draw_x = SCREEN_X_CENTER + pos_x - Xpos;
    int draw_y = SCREEN_Y_CENTER + pos_y - Ypos;
    if (draw_x >= 0 && draw_y >= 0 && draw_x < SCREEN_WIDTH && draw_y < SCREEN_HEIGHT) {
        wattron(game_win, COLOR_PAIR(3));
        mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, " ***** ");
        mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "*******");
        mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "*******");
        mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, " ***** ");
        wattron(game_win, COLOR_PAIR(2));
    }

}

void CBonus::Draw(WINDOW *game_win, int Ypos, int Xpos, int Yoff, int Xoff) const {
    int draw_x = SCREEN_X_CENTER + pos_x - Xpos;
    int draw_y = SCREEN_Y_CENTER + pos_y - Ypos;
    if (draw_x >= 0 && draw_y >= 0 && draw_x < SCREEN_WIDTH && draw_y < SCREEN_HEIGHT) {
        wattron(game_win, COLOR_PAIR(1));

        if (BONUS == BONUS_SPEED) {
            mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
            mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, " SPEED ");
            mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "   UP  ");
            mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
        } else if (BONUS == BONUS_FLAME_RANGE) {
            mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
            mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, " FLAME ");
            mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, " RANGE ");
            mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "   UP  ");
        } else if (BONUS == BONUS_MORE_BOMBS) {
            mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
            mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, " MORE  ");
            mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, " BOMBS ");
            mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
        } else if (BONUS == BONUS_MANUAL_DETONATION) {
            mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
            mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "MANUAL ");
            mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "EXPLODE");
            mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
        }
        wattron(game_win, COLOR_PAIR(2));
    }

}

void CBomb::Draw(WINDOW *game_win, int Ypos, int Xpos, int Yoff, int Xoff) const {
    int draw_x = SCREEN_X_CENTER + pos_x - Xpos;
    int draw_y = SCREEN_Y_CENTER + pos_y - Ypos;;
    if (draw_x >= 0 && draw_y >= 0 && draw_x < SCREEN_WIDTH && draw_y < SCREEN_HEIGHT) {
        mvwprintw(game_win, 4 * draw_y + 1 - Yoff, 7 * draw_x + 1 - Xoff, "   _  ");
        mvwprintw(game_win, 4 * draw_y + 1 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "  / \\  ");
        mvwprintw(game_win, 4 * draw_y + 2 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "  \\_/  ");
        mvwprintw(game_win, 4 * draw_y + 3 + 1 - Yoff, 7 * draw_x + 1 - Xoff, "       ");
    }
}

int CFlame::FlameTick() {
    if (tickToDestroy) {
        tickToDestroy--;
        return 0;
    }
    else {
        return 1;
    }

}

void CBomb::Detonate(CMap *Map, int FlameRange) const {

    Map->CreateFlame(pos_y, pos_x, FlameRange);

}

