#ifndef PLAYER_H
#define PLAYER_H
#include "myStruct.h"

#include <vector>
using namespace std;
class Dealer;
class Player
{
    public:
        Player();
        card_t hole_card[2];
        Player *next;
        Player *prev;
        strength_t strength;
        unsigned int chip;
        unsigned int ID;
        void action(Dealer *dealer);
        void fold();
        void check();
        int raise(int amount,int min_raise_size);
        vector<int> bet[4];//pflop,flop,turn river
};
#endif
