#ifndef PLAYER_H
#define PLAYER_H
#include "myStruct.h"

class Player
{
    public:
        Player();
        card_t card[2];
        Player *next;
        Player *prev;
        int ID;
};
#endif
