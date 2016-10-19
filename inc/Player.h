#ifndef PLAYER_H
#define PLAYER_H
#include "myStruct.h"

class Player
{
    public:
        Player();
        card_t hole_card[2];
        Player *next;
        Player *prev;
        strength_t strength;
        int ID;
};
#endif
