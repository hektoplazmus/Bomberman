/**
 * This part of code was inspired by https://edux.fit.cvut.cz/courses/BI-PA2/_media/net_2016.tgz
 */

#ifndef SRC_CCLIENT_HPP
#define SRC_CCLIENT_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

using namespace std;

//-------------------------------------------------------------------------------------------------
int CGame::openCliSocket(const char *srvName, int srvPort) {
    struct addrinfo *ai;
    char srvPortText[10];

    snprintf(srvPortText, sizeof(srvPortText), "%d", srvPort);

    if (getaddrinfo(srvName, srvPortText, NULL, &ai) != 0) {
        return -1;
    }

    int s = socket(ai->ai_family, SOCK_STREAM, 0);
    if (s == -1) {
        freeaddrinfo(ai);
        return -1;
    }

    if (connect(s, ai->ai_addr, ai->ai_addrlen) != 0) {
        close(s);
        freeaddrinfo(ai);
        return -1;
    }
    freeaddrinfo(ai);
    return s;
}

//-------------------------------------------------------------------------------------------------
void CGame::communicate(int s) {
    while (!gameEnd) {
        fd_set rd;
        FD_ZERO (&rd);
        FD_SET (s, &rd);
        FD_SET (STDIN_FILENO, &rd);

        timeval t;
        t.tv_sec = 0;
        t.tv_usec = 1;


        if (select(s + 1, &rd, NULL, NULL, &t) < 0) continue;
        if (FD_ISSET (s, &rd)) {
            char buffer[2];
            int l = recv(s, buffer, sizeof(buffer) - 1, 0);
            if (l <= 0) break;
            switch (buffer[0]) {
                case RIGHT :
                    Players[0]->Move(RIGHT, false);
                    break;
                case UP :
                    Players[0]->Move(UP, false);
                    break;
                case LEFT :
                    Players[0]->Move(LEFT, false);
                    break;
                case DOWN :
                    Players[0]->Move(DOWN, false);
                    break;
                case BOMB:
                    Players[0]->PlaceBomb();
                    break;
            }
        }

        if (SendCode != 0) {
            char buffer[2];
            buffer[0] = SendCode;
            buffer[1] = 0;
            send(s, buffer, 1, 0);
            SendCode = 0;

        }
    }
}

//-------------------------------------------------------------------------------------------------
int CGame::OpenClient(std::string ipaddrname, int port) {
    int s = openCliSocket(ipaddrname.c_str(), port);
    if (s == -1) {
        return 1;
    }
    opponentCliSocket = s;
    communicate(s);

    close(s);

    return 0;
}


#endif //SRC_CCLIENT_HPP
